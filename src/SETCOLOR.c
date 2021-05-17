#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

int SETCOLOR(struct Screen* SC_Screen, char* FName)
{
    uint32  AddrX, AddrEnd, l;
    LONG    ISize;
    int     i;
    uint32* ColorID;
    r_Col_t*  Col;
    BPTR    FHandle;
    int     _SETCOLOR = 0;

    FHandle = OPENSMOOTH(FName, MODE_OLDFILE, &ISize);
    if (0 != FHandle)
    {
        (void) Read(FHandle, IMemA[0], ISize);
        Close(FHandle);
        AddrX   = (uint32) IMemA[0];
        AddrEnd = AddrX + ISize;
        do
        {
            ColorID = (uint32*) AddrX;
            AddrX = AddrX+4;
        }
        while ((AddrX < AddrEnd) && (_COLOR_CMAP_TEXT_ != (*ColorID)));

        if (_COLOR_CMAP_TEXT_ == (*ColorID))
        {
            AddrX += 4;
            i = 0;
            do
            {
                Col = (r_Col_t*) AddrX;
                AddrX += sizeof(r_Col_t);
                SetRGB32(&(SC_Screen->ViewPort), i, (Col->r)<<24, (Col->g)<<24, (Col->b)<<24);
                i++;
            }
            while (AddrX < AddrEnd);
            l = (uint32) ((ISize-8) / 3);
            i = 0;
            do
            {
                l = l>>1;
                i++;
            }
            while (l>1);
            _SETCOLOR = i;
        }
    }

    return _SETCOLOR;
}
