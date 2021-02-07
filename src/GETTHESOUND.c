#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

APTR GETTHESOUND(const int MID)
{
    APTR modptr = NULL;
    if (NULL != ModMemA[MID])
    {
        modptr = AllocMem(ModMemL[MID], MEMF_CHIP);
        if (NULL != modptr)
        {
            CopyMem(ModMemA[MID], modptr, ModMemL[MID]);
            StopPlayer();
            RelocModule((struct MMD0*)modptr);
            if (GETMIDIPLAYER((struct MMD0*)modptr))
            {
                PlayModule((struct MMD0*)modptr);
            }
        }
    }
    return modptr;
}
