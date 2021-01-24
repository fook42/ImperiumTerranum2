#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLAYSOUND(const int SoundID, const int SoundRate)
{
    uint8   Volume = 32;

    if (Audio_enable)
    {
        custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1; // 0x0003 .. stop audio on channel 0+1
    }
    WaitTOF();
    if (0 == SoundID)
    {
        if (Audio_enable)
        {
            SPAddrA = SoundMemA[0]; SPFreqA = (UWORD) SoundRate; SPLengthA = SoundSize[0]; SPVolA = (UWORD) (64-Volume);
            SPAddrB = SoundMemA[0]; SPFreqB = (UWORD) SoundRate; SPLengthB = SoundSize[0]; SPVolB = (UWORD) (Volume);

            custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003 .. playback audio on channel 0+1
            
            WaitTOF();

            SPAddrA = ZeroSound; SPLengthA = 1;
            SPAddrB = ZeroSound; SPLengthB = 1;
        } else {
            WaitTOF();
        }
        
        do
        {
            Delay(RDELAY);
        }
        while ((LMB_PRESSED || RMB_PRESSED) && (!Save.PlayMySelf));

        WaitTOF();
        if (Audio_enable)
        {
            custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1; // 0x0003 .. stop audio on channel 0+1
        }
        WaitTOF();
    } else {
        if (Audio_enable)
        {
            SPAddrA = SoundMemA[SoundID];
            SPFreqA = (UWORD) SoundRate; SPVolA = 64;
            SPFreqB = (UWORD) SoundRate; SPVolB = 64;
            if (3 == SoundID)
            {
                SPAddrB   = SoundMemA[SoundID];
                SPLengthA = SoundSize[SoundID];
                SPLengthB = SoundSize[SoundID];
            } else {
                // play the sound on 2 different channels :
                //  1 half on channel 0.. 2nd half on channel 1
                SPAddrB   = SoundMemA[SoundID]+SoundSize[SoundID];
                SPLengthA = SoundSize[SoundID] >> 1;
                SPLengthB = SoundSize[SoundID] >> 1;
            }

            custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003 .. playback audio on channel 0+1

            WaitTOF();

            SPAddrA = ZeroSound; SPLengthA = 1;
            SPAddrB = ZeroSound; SPLengthB = 1;
        } else {
            WaitTOF();
        }
    }
}
