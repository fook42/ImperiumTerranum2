#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 SETCOLOR(struct Screen* XScreen, char* FName)
{
    uint32  AddrX, l, ISize;
    int     i;
    uint32* ColorID;
    r_Col*  Col;
    BPTR    FHandle;
    uint8   _SETCOLOR = 0;

    FHandle = OPENSMOOTH(FName, MODE_OLDFILE);
    if (0 != FHandle)
    {
        (void)  Seek(FHandle, 0, OFFSET_END);
        ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
        (void) Read(FHandle, IMemA[0], ISize);
        Close(FHandle);
        AddrX = (uint32) IMemA[0];
        do
        {
            ColorID = (uint32*) AddrX;
            AddrX = AddrX+4;
        }
        while ((AddrX < ((uint32) IMemA[0]+ISize)) && (_COLOR_CMAP_TEXT_ != *ColorID));
        if (_COLOR_CMAP_TEXT_ == *ColorID)
        {
            AddrX = AddrX+4;
            i = 0;
            do
            {
                Col = (r_Col*) AddrX;
                AddrX = AddrX+3;
                SetRGB32(&(XScreen->ViewPort), i, (Col->r)<<24, (Col->g)<<24, (Col->b)<<24);
                i++;
            }
            while (AddrX<((uint32) IMemA[0]+ISize));
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
