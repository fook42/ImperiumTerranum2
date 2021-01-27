#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool READPATHS()
{
    BPTR    FHandle;
    bool    status = false;

    FHandle = Open((CONST_STRPTR) "Paths.txt", MODE_OLDFILE);
    if (0 != FHandle)
    {
        (void)     Seek(FHandle, 0, OFFSET_END);
        PathMemL = Seek(FHandle, 0, OFFSET_BEGINNING);
        PathMemA = (uint8*) AllocMem(PathMemL, MEMF_CLEAR);
        if (NULL != PathMemA)
        {
            (void) Read(FHandle, PathMemA, PathMemL);
            status = true;
        } else {
            puts("Nicht genug Speicher vorhanden!\n");
        }
        Close(FHandle);
    } else {
        puts("Kann Datei \"Paths.txt\" nicht finden!\n");
    }
    return status;
}


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
