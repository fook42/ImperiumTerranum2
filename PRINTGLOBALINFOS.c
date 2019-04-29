#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PRINTGLOBALINFOS(uint8 PGI_ActPlayer)
{
    char    s[12];

    (void) dez2out(Year, 0, s);
    // clean the area before...
    RECTWIN(MyRPort_PTR[0],0,519,132,632,149);
    WRITE(575,134,109,(1|WRITE_Center),MyRPort_PTR[0],2,s);

    if ((true == Valid) && ((0 != Save.CivPlayer[PGI_ActPlayer]) || (!Save.PlayMySelf)))
    {
        if ((0 != Save.CivPlayer[PGI_ActPlayer]) && (Informed))
        {
            (void) dez2out(Save.Staatstopf[PGI_ActPlayer], 0, s);
        } else {
            s[0]='0';
            s[1]=0;
        }
        // clean the area before...
        RECTWIN(MyRPort_PTR[0],0,519,183,632,200);
        WRITE(640,185,109,(1|WRITE_Right),MyRPort_PTR[0],2,s);

        if ((0 != Save.CivPlayer[PGI_ActPlayer]) && (Informed))
        {
            (void) dez2out(Save.Bevoelkerung[PGI_ActPlayer], 0, s);
        } else {
            s[0]='0';
            s[1]=0;
        }
        // clean the area before...
        RECTWIN(MyRPort_PTR[0],0,519,234,632,251);
        WRITE(640,236,109,(1|WRITE_Right),MyRPort_PTR[0],2,s);
    }
}
