#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INFORMUSER()
{
    static uint8 LastPlayer = 0;
    char s[] = {"Player 0\0"};

    if ((Save.CivPlayer[ActPlayer-1] == 0) || (Informed)
     || (Save.WarState[ActPlayer-1][ActPlayer-1] == LEVEL_DIED))
    {
        return;
    }
    Informed = true;
    if (LastPlayer == ActPlayer)
    {
        return;
    }
    LastPlayer = ActPlayer;
    Display = LastDisplay[ActPlayer];
    RECTWIN(MyRPort_PTR[0],0,523,10,630,117);
    if (MultiPlayer)
    {
        if (!Save.PlayMySelf)
        {
            RECTWIN(MyRPort_PTR[0],0,0,0,511,511);
        }
        s[strlen(s)-1]=Save.CivPlayer[ActPlayer-1]+'0';
        PLAYERJINGLE(ActPlayer);

        MAKEWINBORDER(MyRPort_PTR[0],35,80,475,290,12,6,0);
        WRITE(256,100,12,WRITE_Center,MyRPort_PTR[0],3,PText[207]);
        WRITE(256,125,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,s);
        DISPLAYLOGO(ActPlayer,192,150);
        PRINTGLOBALINFOS(ActPlayer-1);
        if (Save.PlayMySelf)
        {
            Delay(PAUSE);
        }
        WAITLOOP(Save.PlayMySelf);
        if (0 == Display)
        {
            DRAWSTARS(MODE_REDRAW,ActPlayer);
        } else {
            DRAWSYSTEM(MODE_REDRAW,Display,NULL);
        }
    }
}
