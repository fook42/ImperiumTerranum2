#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REFRESHDISPLAY()
{
    uint8   Mode = MODE_REFRESH;
    if (0 != Display)
    {
        if ((OldX != OffsetX) || (OldY != OffsetY))
        {
            Mode = MODE_REDRAW;
        }
        DRAWSYSTEM(Mode,Display,NULL);
    } else {
        DRAWSTARS(Mode);
    }
}
