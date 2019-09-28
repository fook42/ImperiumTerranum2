#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void WAITLOOP(bool no_wait)
{
    uint8 RawCode;
    do
    {
        delay(RDELAY);
        RawCode = GETRAWCODE();
    }
    while ((64 != RawCode) && (67 != RawCode) && (68 != RawCode) && LMB_NOTPRESSED && RMB_NOTPRESSED && (!no_wait));

    PLAYSOUND(1,300);

    do
    {
        delay(RDELAY);
    }
    while ((LMB_PRESSED || RMB_PRESSED) && (!no_wait));
    CLEARINTUITION();
}
