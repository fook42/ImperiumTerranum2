#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CLOCK()
{
    struct DateStamp datestamp = {0,0,0};
    struct DateStamp* ds;
    uint8   Stunde, Minute;
    char    s[8];
    char*   pos;

    ds = DateStamp(&datestamp);

    Stunde = (uint8) (ds->ds_Minute/60);
    Minute = (uint8) (ds->ds_Minute%60);

    if (((Stunde<9) || (Stunde>21)) && (!DoClock))
    {
        DoClock = true;
        MAKEWINBORDER(MyRPort_PTR[0],537,350,614,372,29,12,0);
    }
    if (DoClock)
    {
        pos = dez2out(Stunde, 2, s);
        *pos++=':';
        (void) dez2out(Minute, 2, pos);
        WRITE(541,354,8,1,MyRPort_PTR[0],1,s);
    }
}
