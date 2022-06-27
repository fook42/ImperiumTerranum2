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
    RECT_RP0_C0((HighRes_Width-118),9,(HighRes_Width-11),117);      // clear Minimap
    if (MultiPlayer)
    {
        if (!Save.PlayMySelf)
        {
            RECT_RP0_C0(0,0,Area_Width-1,Area_Height-1);   // clear Main Star-area
        }
        s[7]=Save.CivPlayer[ActPlayer-1]+'0';
        PLAYERJINGLE(ActPlayer-1);

        MAKEWINBORDER(MyRPort_PTR[0],30,80,Area_Width-30,290,12,6,0);
        WRITE_RP0(Area_CenterX,100,           12,WRITE_Center,3,PText[207]);
        WRITE_RP0(Area_CenterX,125,ActPlayerFlag,WRITE_Center,3,s);
        DISPLAYLOGO(ActPlayer,Area_CenterX-64,150);
        PRINTGLOBALINFOS(ActPlayer-1);
        if (Save.PlayMySelf)
        {
            Delay(PAUSE);
        }
        WAITLOOP(Save.PlayMySelf);
        if (0 == Display)
        {
            DRAWSTARS(MODE_REDRAW);
        } else {
            DRAWSYSTEM(MODE_REDRAW,Display,NULL);
        }
    }
}
