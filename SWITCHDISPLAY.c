#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SWITCHDISPLAY()
{
    custom.dmacon = BITCLR | DMAF_RASTER;
}
