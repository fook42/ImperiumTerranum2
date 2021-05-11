#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const worldcolors_t worldcolors[] = {   {0x00000000,0xFFFFFFFF,0xFFFFFFFF}, // cyan
                                        {0x66666666,0x66666666,0xF7F7F7F7}, // light blue
                                        {0xFFFFFFFF,0x00000000,0x00000000}, // red
                                        {0x00000000,0xFFFFFFFF,0x12121212}, // green
                                        {0xFFFFFFFF,0xFFFFFFFF,0x00000000}, // yellow
                                        {0xBABABABA,0x8B8B8B8B,0x48484848}, // brown
                                        {0xFFFFFFFF,0x00000000,0xB0B0B0B0}, // purple
                                        {0x77777777,0x77777777,0x77777777}  // grey
                                    };

void SETWORLDCOLORS()
{
    uint8 col;
    col = GETCIVVAR(Save.WorldFlag);
    if (7 < col)
    {
        col = 0;
    }

    SetRGB32(MyVPort_PTR[0], 1, worldcolors[col].r, worldcolors[col].g, worldcolors[col].b);
}
