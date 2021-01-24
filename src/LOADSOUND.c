#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void LOADSOUND(char* FName, const int SoundID)
{
    int     Filesize;
    BPTR    FHandle;

    FHandle = OPENSMOOTH( FName, MODE_OLDFILE );
    if (0 == FHandle) { return; }

    (void) Seek(FHandle, 0, OFFSET_END);
    Filesize = Seek(FHandle, 0, OFFSET_BEGINNING);
    SoundSize[SoundID] = (UWORD) (Filesize >> 1);
    SoundMemA[SoundID] = (UWORD*) AllocMem(Filesize, MEMF_CHIP);
    if (NULL == SoundMemA[SoundID])
    {
        Close( FHandle );
        return;
    }
    (void) Read(FHandle, SoundMemA[SoundID], Filesize);
    Close(FHandle);
}
