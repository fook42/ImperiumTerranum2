#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool DISPLAYIMAGE(char* Fn, int LEdge, int TEdge, int Width, int Height, int Depth, struct Screen* DI_Screen, uint8 CacheNum)
{
    char    FName[80];
    BPTR    FHandle;
    uint8   realCacheNum=0;
    uint16  CNum, i;
    uint16  CNum3;
    uint32  ISize, l=0, Addr;
    uint32* Size;
    uint16* Colors;
    r_Col_t*  RGB;
    bool    ImageIsValid = false;

    if (0 != CacheNum)
    {
        realCacheNum = CacheNum-1;
        if (NULL != CacheMemA[realCacheNum])
        {
            ImageIsValid = true;
        }
    }
    if (false == ImageIsValid)
    {
        FHandle = OPENSMOOTH(Fn, MODE_OLDFILE);
        if (0 == FHandle)
        {
            return false;
        }
        (void)  Seek(FHandle, 0, OFFSET_END);
        ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
        (void) Read(FHandle, (IMemA[0]+IMemL[0]-ISize-250), ISize);
        l = (Width*Height*Depth)>>3;
        UNPACK(IMemA[0], IMemA[0]+IMemL[0]-ISize-250, l, 0);
        Close(FHandle);
    }
    CNum =1<<Depth;     // 2 -> 4, 3 -> 8 ... 8 -> 256
    CNum3=3*CNum;       // 2 -> 12,3 -> 24... 8 -> 768

    if (true == ImageIsValid)
    {
        CopyMem((APTR) (CacheMemA[realCacheNum]+CNum3+8), (APTR) IMemA[0], (ULONG) (CacheMemL[realCacheNum]-CNum3-8));
    }
    struct Image DI_Img = {0, 0, Width, Height, Depth, (UWORD*) IMemA[0], CNum-1, 0, NULL};
    DrawImage(&(DI_Screen->RastPort), &DI_Img, LEdge, TEdge);

    if ((0 != CacheNum) && (false == ImageIsValid))
    {
        CacheMemL[realCacheNum] = l+CNum3+8;
        CacheMemA[realCacheNum] = AllocMem(CacheMemL[realCacheNum], MEMF_FAST);

        if (NULL != CacheMemA[realCacheNum])
        {
            Addr = (uint32) CacheMemA[realCacheNum];
            Size = (uint32*) Addr;
            *Size= l;
            Addr += 4;
            Colors = (uint16*) Addr;
            *Colors= CNum;
            Addr += 4;
            l = strlen(Fn)-3;
            memcpy(FName, Fn, l);
            strcpy(FName+l, "pal");
            FHandle = OPENSMOOTH(FName, MODE_OLDFILE);
            if (0 == FHandle)
            {
                return false;
            }
            (void) Seek(FHandle, 8, OFFSET_BEGINNING);
            (void) Read(FHandle, (uint8*) Addr, CNum3);
            Close(FHandle);
            Addr += CNum3;
            CopyMem((APTR) IMemA[0], (APTR) Addr, (ULONG) (*Size));
            ImageIsValid = true;
        } else {
            (void) SETCOLOR(DI_Screen, FName);
        }
    }
    if (true == ImageIsValid)
    {
        Addr = (uint32) CacheMemA[realCacheNum];
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
