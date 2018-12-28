#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEPATHS()
{
    uint8*  btx;
    uint8   i = 0;
    uint32  l, EndofPaths;

    l = (uint32) PathMemA;
    EndofPaths = l+PathMemL;
    do
    {
        PathStr[i] = (char*) l;
        do
        {
            btx = (uint8*) l;
            l++;
            if (*btx == 10)
            {
                *btx = 0;
            }
        }
        while ((*btx != 0) && (l <= EndofPaths));
        i++;
    }
    while ((i < PATHS) && (l < EndofPaths));
}
