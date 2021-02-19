#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void LOADMOD(char* FName, int MID)
{
    BPTR    FHandle;

    FHandle = OPENSMOOTH( FName, MODE_OLDFILE );
    if (0 != FHandle)
    {
        (void)         Seek(FHandle, 0, OFFSET_END);
        ModMemL[MID] = Seek(FHandle, 0, OFFSET_BEGINNING);
        ModMemA[MID] = (uint8*) my_AllocMem(ModMemL[MID], MEMF_ANY);
        if (NULL != ModMemA[MID])
        {
            (void) Read(FHandle, (APTR) ModMemA[MID], ModMemL[MID]);
        }
        Close( FHandle );
    }
}

LONG GETMIDIPLAYER(struct MMD0 *module)
{
    /* Check if it's a MIDI song. We check the MIDI channel of
    each instrument. */
    int count;
    UWORD midi = 0;

    for(count = 0; count < 63; count++)
    {
        if(module->song->sample[count].midich)
        {
            midi = 1;
            break;
        }
    }
    return !(GetPlayer(midi));
}
