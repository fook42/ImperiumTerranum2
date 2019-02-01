#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool DISPLAYIMAGE(char* Fn, int LEdge, int TEdge, int Width, int Height, int Depth, struct Screen* DI_Screen, uint8 CacheNum)
{
    char    FName[80];
    BPTR    FHandle;
    uint16  CNum, i;
    uint16  CNum3;
    uint32  ISize, l=0, Addr;
    uint32* Size;
    uint16* Colors;
    r_Col*  RGB;
    bool    ImageIsValid = false;

    if (0 < CacheNum)
    {
        if (NULL != CacheMemA[CacheNum])
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
        memcpy(IMemA[0], (CacheMemA[CacheNum]+CNum3+8), (CacheMemL[CacheNum]-CNum3-8));
    }
    struct Image DI_Img = {0, 0, Width, Height, Depth, IMemA[0], CNum-1, 0, NULL};
    DrawImage(&(DI_Screen->RastPort), &DI_Img, LEdge, TEdge);

    if ((0 < CacheNum) && (false == ImageIsValid))
    {
        CacheMemL[CacheNum] = l+CNum3+8;
        CacheMemA[CacheNum] = AllocMem(CacheMemL[CacheNum], MEMF_FAST);

        if (NULL != CacheMemA[CacheNum])
        {
            Addr = (uint32) CacheMemA[CacheNum];
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
            memcpy((void*)Addr, IMemA[0], *Size);
            ImageIsValid = true;
        } else {
            (void) SETCOLOR(DI_Screen, FName);
        }
    }
    if (true == ImageIsValid)
    {
        Addr = (uint32) CacheMemA[CacheNum];
        Addr += 4;
        Colors = (uint16*) Addr;
        Addr += 4;
        for (i = 0; i < (*Colors); ++i)
        {
            RGB  = (r_Col*) Addr;
            Addr += 3;
            SetRGB32(&(DI_Screen->ViewPort), i, (RGB->r)<<24, (RGB->g)<<24, (RGB->b)<<24);
        }
    }
    return true;
}
