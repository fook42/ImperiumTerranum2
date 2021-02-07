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
        PathMemL = Seek(FHandle, 0, OFFSET_BEGINNING); // added 1 for last path-delimiter!
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
    uint8*  EndofPaths;

    EndofPaths = PathMemA+PathMemL;
    btx = PathMemA;
    do
    {
        /* set PathStr[i] to current PathAddr. */
        PathStr[i] = (char*) btx;

        /* now search for $0A-byte in current Path or End and select next PathStr */
        while ((0 != *btx) && (10 != *btx) && (EndofPaths > btx))
        {
            ++btx;
        }
        *btx++ = 0;
        ++i;
    }
    while ((i < PATHS) && (btx < EndofPaths));
}
