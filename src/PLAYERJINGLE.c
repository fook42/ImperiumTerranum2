#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLAYERJINGLE(const int JingleID)
{
    char    FName[60];
    char*   _s;
    BPTR    FHandle;

    // load player-sound if not already in memory
    if (NULL == LogoSMemA[JingleID])
    {
        _s=my_strcpy(FName, PathStr[9]);
        *_s++ = '1'+JingleID;
        (void) my_strcpy(_s, ".RAW");
        FHandle = OPENSMOOTH(FName,MODE_OLDFILE);
        if (0 == FHandle) { return; }
        (void) Seek(FHandle, 0, OFFSET_END);
        LogoSMemL[JingleID] = Seek(FHandle, 0, OFFSET_BEGINNING);
        LogoSMemA[JingleID] = (uint8*) AllocMem(LogoSMemL[JingleID], MEMF_ANY );
        if (NULL == LogoSMemA[JingleID])
        {
            Close(FHandle);
            return;
        }
        (void) Read(FHandle, LogoSMemA[JingleID], LogoSMemL[JingleID]);
        Close(FHandle);
    }
    if ((NULL == SoundMemA[3]) || ((SoundSize[3]<<1) != LogoSMemL[JingleID]))
    {
        if (0 != SoundSize[3]) { FreeMem(SoundMemA[3],SoundSize[3]<<1); }
        SoundSize[3] = (UWORD) (LogoSMemL[JingleID] >> 1);
        SoundMemA[3] = (UWORD*) AllocMem(LogoSMemL[JingleID], MEMF_CHIP );
        if (NULL == SoundMemA[3]) { return; }
    }
    CopyMemQuick(LogoSMemA[JingleID], SoundMemA[3], LogoSMemL[JingleID]);
    PLAYSOUND(3, 300);
}
