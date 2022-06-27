#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CLOCK()
{
    struct DateStamp datestamp = {0,0,0};
    struct DateStamp* ds;
    uint8   Stunde, Minute;
    char    clockstring[6];
    char*   pos;

    ds = DateStamp(&datestamp);

    Stunde = (uint8) (ds->ds_Minute/60);
    Minute = (uint8) (ds->ds_Minute%60);

    if (((9 > Stunde) || (21 < Stunde)) && (!DoClock))
    {
        DoClock = true;
        MAKEWINBORDER(MyRPort_PTR[0],HighRes_Width-103,350,HighRes_Width-26,372,29,12,0);
    }
    if (DoClock)
    {
        pos = dez2out(Stunde, 2, clockstring);
        *pos++=':';
        (void) dez2out(Minute, 2, pos);
        WRITE_RP0(HighRes_Width-99,354,8,1,1,clockstring);
    }
}
