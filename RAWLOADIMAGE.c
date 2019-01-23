#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool RAWLOADIMAGE(char* Fn, int LEdge, int TEdge, int Width, int Height, int Depth, ITBitMap* DestBitMap)
{
    BPTR            FHandle;
    uint32          ISize, piclen;
    PLANEPTR        plane_PTR;
    int             i;
    bool            _RAWLOADIMAGE = false;
    struct BitMap   MyBitMap;

    FHandle = OPENSMOOTH(Fn, MODE_OLDFILE);
    if (0 != FHandle)
    {
        (void)  Seek(FHandle, 0, OFFSET_END);
        ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
        if (0 != ISize)
        {
            (void) Read(FHandle, (IMemA[0]+IMemL[0]-ISize-250), ISize);
            piclen = (Width*Height*Depth)>>3;

            UNPACK(IMemA[0], IMemA[0]+IMemL[0]-ISize-250, piclen, 0);
            if ((piclen == (DestBitMap->MemL)) && (4 == Depth))
            {
                CopyMemQuick(IMemA[0], DestBitMap->MemA, piclen);
                _RAWLOADIMAGE = true;
            }
            else
            {
                ISize = (Width*Height)>>3;
                InitBitMap(&MyBitMap, Depth, Width, Height);
                plane_PTR = (PLANEPTR) IMemA[0];
                for (i = 0; i < Depth; ++i)
                {
                    MyBitMap.Planes[i] = plane_PTR;
                    plane_PTR += ISize;
                }
                for (i = Depth; i < 8; ++i)
                {
                    MyBitMap.Planes[i] = NULL;
                }

                if (Depth == BltBitMap( &MyBitMap, 0, 0, (struct BitMap*) DestBitMap, LEdge, TEdge, Width, Height, 192, 255, NULL ))
                {
                    _RAWLOADIMAGE = true;
                }
            }
        }
        Close(FHandle);
    }
    return _RAWLOADIMAGE;
}
