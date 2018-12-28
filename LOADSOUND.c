#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void LOADSOUND(char* FName, uint8 SID)
{
    uint32  l;
    BPTR    FHandle;

    FHandle = OPENSMOOTH( FName, MODE_OLDFILE );
    if (0 == FHandle) { return; }

    l = Seek(FHandle, 0, OFFSET_END);
    l = Seek(FHandle, 0, OFFSET_BEGINNING);
    SoundSize[SID] = l / 2;
    SoundMemA[SID] = AllocMem(l, MEMF_CHIP+MEMF_CLEAR);
    if (NULL == SoundMemA[SID])
    {
        Close( FHandle );
        return;
    }
    (void) Read(FHandle, SoundMemA[SID], l);
    Close(FHandle);
}
