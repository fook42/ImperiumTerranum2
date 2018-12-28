#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PLAYSOUND(uint8 SID, uint16 SRate)
{
    uint8   Vol = 32;

    if (Audio_enable)
    {
        custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1; // 0x0003
    }
    WaitTOF();
    SID--;
    if (0 == SID)
    {
        // Vol = IBase->MouseX / 10;

        if (Audio_enable)
        {
            SPAddrA = SoundMemA[SID]; SPFreqA = SRate; SPLengthA = SoundSize[SID]; SPVolA = 64-Vol;
            SPAddrB = SoundMemA[SID]; SPFreqB = SRate; SPLengthB = SoundSize[SID]; SPVolB = Vol;

            custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003
        }

        WaitTOF();

        if (Audio_enable)
        {
            SPAddrA = ZeroSound; SPLengthA = 1;
            SPAddrB = ZeroSound; SPLengthB = 1;
        }
        do
        {
            delay(RDELAY);
        }
        while ((LMB_PRESSED || RMB_PRESSED) && (!Save.PlayMySelf));

        WaitTOF();

        if (Audio_enable)
        {
            custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1; // 0x0003
            WaitTOF();
        }
    } else {
        if (Audio_enable)
        {
            SPAddrA = SoundMemA[SID];                SPFreqA = SRate; SPLengthA = SoundSize[SID] >> 1; SPVolA = 64;
            SPAddrB = SoundMemA[SID]+SoundSize[SID]; SPFreqB = SRate; SPLengthB = SoundSize[SID] >> 1; SPVolB = 64;

            if (3 == SID)
            {
                SPAddrA = SoundMemA[SID]; SPFreqA = SRate; SPLengthA = SoundSize[SID]; SPVolA = 64;
                SPAddrB = SoundMemA[SID]; SPFreqB = SRate; SPLengthB = SoundSize[SID]; SPVolB = 64;
            }

            custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003
        }
        WaitTOF();

        if (Audio_enable)
        {
            SPAddrA = ZeroSound; SPLengthA = 1;
            SPAddrB = ZeroSound; SPLengthB = 1;
        }
    }
}
