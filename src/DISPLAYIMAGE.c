#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool DISPLAYIMAGE(char* Fn, const int LEdge, const int TEdge, const int Width, const int Height, const int Depth, struct Screen* DI_Screen, const int CacheNum)
{
    char    FName[80];
    char*   _s;
    BPTR    FHandle;
    uint8   realCacheNum=0;
    uint16  CNum, i;
    uint16  CNum3;
    uint32  ImgSize_packed, ImgSize=0; // , Addr;
    APTR    Addr;
    uint16* Colors;
    r_Col_t*  RGB;
    bool    ImageIsValid = false;

    if (1==Depth)
    {
        CNum = 1;
    } else {
        CNum = 1<<Depth;     // 2 -> 4, 3 -> 8 ... 8 -> 256
    }
    CNum3=3*CNum;       // 2 -> 12,3 -> 24... 8 -> 768

    if (0 != CacheNum)
    {
        realCacheNum = CacheNum-1;
        if (NULL != CacheMemA[realCacheNum])
        {
            if (0 != *CacheMemA[realCacheNum])
            {
                ImageIsValid = true;
            }
        }
    }
    // fill IMemA[0] with image data .. either load from file or copy from cache
    if (false == ImageIsValid)
    {
        FHandle = OPENSMOOTH(Fn, MODE_OLDFILE);
        if (0 == FHandle)
        {
            return false;
        }
        (void)  Seek(FHandle, 0, OFFSET_END);
        ImgSize_packed = Seek(FHandle, 0, OFFSET_BEGINNING);
        Addr = (APTR) (IMemA[0]+IMemL[0]-ImgSize_packed-250);   // start at 250+FileSize Bytes from the end (!) of IMem-Area
        (void) Read(FHandle, Addr, ImgSize_packed);
        ImgSize = (Width*Height*Depth)>>3;    // w*h*depth/8 = num of bytes for uncompressed imagedata
        UNPACK(IMemA[0], (uint8*) Addr, ImgSize, 0);
        Close(FHandle);
    } else {

        memcpy(IMemA[0], (CacheMemA[realCacheNum]+CNum3+8), *((uint32*)CacheMemA[realCacheNum]));
    }

    struct Image DI_Img = {0, 0, (WORD) Width, (WORD) Height, (WORD) Depth, (UWORD*) IMemA[0], CNum-1, 0, NULL};
    DrawImage(&(DI_Screen->RastPort), &DI_Img,(WORD) LEdge,(WORD) TEdge);

    // if Image was not in cache, store it there, together with colorpalette-data
    if ((0 != CacheNum) && (false == ImageIsValid))
    {
        CacheMemL[realCacheNum] = ImgSize+CNum3+8;
        CacheMemA[realCacheNum] = AllocMem(CacheMemL[realCacheNum], MEMF_FAST);

        if (NULL != CacheMemA[realCacheNum])
        {
            Addr = (APTR) CacheMemA[realCacheNum];
            *((uint32*) Addr) = ImgSize;
            Addr += 4;
            *((uint16*) Addr) = CNum;
            Addr += 4;
            _s=my_strcpy(FName, Fn);
            (void) my_strcpy(_s-3, "pal");
            FHandle = OPENSMOOTH(FName, MODE_OLDFILE);
            if (0 == FHandle)
            {
                return false;
            }
            (void) Seek(FHandle, 8, OFFSET_BEGINNING);
            (void) Read(FHandle, Addr, CNum3);
            Close(FHandle);
            Addr += CNum3;
            memcpy((void*) Addr, IMemA[0], ImgSize);

            ImageIsValid = true;
        } else {
            (void) SETCOLOR(DI_Screen, FName);
        }
    }
    if (true == ImageIsValid)
    {
        Addr = (APTR) CacheMemA[realCacheNum];
        Addr += 4;
        Colors = (uint16*) Addr;
        Addr += 4;
        for (i = 0; i < (*Colors); ++i)
        {
            RGB  = (r_Col_t*) Addr;
            Addr += sizeof(r_Col_t);
            SetRGB32(&(DI_Screen->ViewPort), i, (RGB->r)<<24, (RGB->g)<<24, (RGB->b)<<24);
        }
    }
    return true;
}
