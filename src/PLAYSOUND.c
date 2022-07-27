#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLAYSOUND(const int SoundID, const int SoundRate)
{
    UWORD LRFader;
    if (true == Audio_enable)
    {
        LRFader = it_round((double) IntuitionBase->MouseX / 10.0);

        custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1; // 0x0003 .. stop audio on channel 0+1
        WaitTOF();
        if (0 == SoundID)
        {
            SPAddr(0) = SoundMemA[0]; SPFreq(0) = (UWORD) SoundRate; SPLength(0) = SoundSize[0]; SPVol(0) = (UWORD) (64-LRFader);
            SPAddr(1) = SoundMemA[0]; SPFreq(1) = (UWORD) SoundRate; SPLength(1) = SoundSize[0]; SPVol(1) = (UWORD) (LRFader);

            custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003 .. playback audio on channel 0+1
            WaitTOF();

            SPAddr(0) = ZeroSound; SPLength(0) = 1;
            SPAddr(1) = ZeroSound; SPLength(1) = 1;
            
            do
            {
                Delay(RDELAY);
            }
            while ((LMB_PRESSED || RMB_PRESSED) && (!Save.PlayMySelf));

            WaitTOF();
            custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1; // 0x0003 .. stop audio on channel 0+1
            WaitTOF();
        } else {
            SPAddr(0) = SoundMemA[SoundID];
            SPFreq(0) = (UWORD) SoundRate; SPVol(0) = 64;
            SPFreq(1) = (UWORD) SoundRate; SPVol(1) = 64;
            if (3 == SoundID)
            {
                SPAddr(1)   = SoundMemA[SoundID];
                SPLength(0) = SoundSize[SoundID];
                SPLength(1) = SoundSize[SoundID];
            } else {
                // play the sound on 2 different channels :
                //  1 half on channel 0.. 2nd half on channel 1
                SPAddr(1)   = SoundMemA[SoundID]+SoundSize[SoundID];
                SPLength(0) = SoundSize[SoundID] >> 1;
                SPLength(1) = SoundSize[SoundID] >> 1;
            }

            custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003 .. playback audio on channel 0+1
            WaitTOF();

            SPAddr(0) = ZeroSound; SPLength(0) = 1;
            SPAddr(1) = ZeroSound; SPLength(1) = 1;
        }
    } else {
        WaitTOF();
        if (0 == SoundID)
        {
            do
            {
                Delay(RDELAY);
            }
            while ((LMB_PRESSED || RMB_PRESSED) && (!Save.PlayMySelf));
        }
    }
}
