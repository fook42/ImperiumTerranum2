#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLAYERJINGLE(uint8 JingleID)
{
    char    FName[60];
    char*   _s;
    BPTR    FHandle;

    if (NULL == LogoSMemA[JingleID])
    {
        _s=my_strcpy(FName, PathStr[9]);
        _s = dez2out(JingleID, 0, _s);
        (void) my_strcpy(_s, ".RAW");
        FHandle = OPENSMOOTH(FName,MODE_OLDFILE);
        if (0 == FHandle) { return; }
        (void) Seek(FHandle, 0, OFFSET_END);
        LogoSMemL[JingleID] = Seek(FHandle, 0, OFFSET_BEGINNING);
        LogoSMemA[JingleID] = (uint8*) AllocMem(LogoSMemL[JingleID], MEMF_FAST | MEMF_CLEAR);
        if (NULL == LogoSMemA[JingleID])
        {
            Close(FHandle);
            return;
        }
        (void) Read(FHandle, LogoSMemA[JingleID], LogoSMemL[JingleID]);
        Close(FHandle);
    }
    if ((NULL == SoundMemA[3]) || ((SoundSize[3]*2) != LogoSMemL[JingleID]))
    {
        if (0 != SoundSize[3]) { FreeMem(SoundMemA[3],SoundSize[3]*2); }
        SoundSize[3] = LogoSMemL[JingleID] / 2;
        SoundMemA[3] = (UWORD*) AllocMem(LogoSMemL[JingleID], MEMF_CHIP | MEMF_CLEAR);
        if (NULL == SoundMemA[3]) { return; }
    }
    CopyMemQuick(LogoSMemA[JingleID], SoundMemA[3], LogoSMemL[JingleID]);
    PLAYSOUND(3, 300);
}
