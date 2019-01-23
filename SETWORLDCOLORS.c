#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const struct worldcolors_t const worldcolors[] = {  {0x00000000,0xFF000000,0xFF000000}, // cyan
                                                    {0x66000000,0x66000000,0xF7000000}, // light blue
                                                    {0xFF000000,0x00000000,0x00000000}, // red
                                                    {0x00000000,0xFF000000,0x12000000}, // green
                                                    {0xFF000000,0xFF000000,0x00000000}, // yellow
                                                    {0xBA000000,0x8B000000,0x48000000}, // brown
                                                    {0xFF000000,0x00000000,0xB0000000}, // purple
                                                    {0x77000000,0x77000000,0x77000000} // grey
                                                };

void SETWORLDCOLORS()
{
    uint8 col;
    col = GETCIVVAR(Save.WorldFlag);
    if (8 <= col)
    {
        col = 0;
    }

    SetRGB32(MyVPort_PTR[0], 1, worldcolors[col].r, worldcolors[col].g, worldcolors[col].b);
}
