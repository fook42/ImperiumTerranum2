#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SETWORLDCOLORS()
{
    uint8 r, g, b;
    switch (GETCIVVAR(Save.WorldFlag))
    {
        case 1: { r = 0x66; g = 0x66; b = 0xF7; break; } /* light blue */
        case 2: { r = 0xFF; g = 0x00; b = 0x00; break; } /* red */
        case 3: { r = 0x00; g = 0xFF; b = 0x12; break; } /* green */
        case 4: { r = 0xFF; g = 0xFF; b = 0x00; break; } /* yellow */
        case 5: { r = 0xBA; g = 0x8B; b = 0x48; break; } /* brown */
        case 6: { r = 0xFF; g = 0x00; b = 0xB0; break; } /* purple */
        case 7: { r = 0x77; g = 0x77; b = 0x77; break; } /* grey */
        default:{ r = 0x00; g = 0xFF; b = 0xFF; }        /* cyan */
    }
    SetRGB32(&(MyScreen[0]->ViewPort),1,((uint32) r)<<24,((uint32) g)<<24,((uint32) b)<<24);
}
