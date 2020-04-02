#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEPATHS()
{
    uint8*  btx;
    uint8   i = 0;
    uint32  PathPos, EndofPaths;

    PathPos = (uint32) PathMemA;
    EndofPaths = PathPos+PathMemL;
    do
    {
        /* set PathStr[i] to current PathAddr. */
        PathStr[i] = (char*) PathPos;
        /* now search for 0-byte in current Path or End and select next PathStr */
        do
        {
            btx = (uint8*) PathPos;
            ++PathPos;
            if (10 == *btx)
            {
                *btx = 0;
            }
        }
        while ((0 != *btx) && (PathPos <= EndofPaths));
        ++i;
    }
    while ((i < PATHS) && (PathPos < EndofPaths));
}
