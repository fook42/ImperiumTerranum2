#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

/* write system-information to minimap-area (name, num of planets, distance, stargate...) */
void WRITEGALAXYDATA(uint8 ActSys, uint8 ShipMaxMove)
{
    int     i;
    uint32  distance;
    char    s[20];
    char*   _s;

    for(i = 0; i < Save.Systems; i++)
    {
        if ((MouseX(0) >= (SystemX[i]-20)) && (MouseX(0) <= (SystemX[i]+20))
         && (MouseY(0) >= (SystemY[i]-5))  && (MouseY(0) <= (SystemY[i]+5))
         && (LastSystem != (i+1)))
        {
            LastSystem = (i+1);
            RECTWIN(MyRPort_PTR[0],0,522,9,629,116);      // clear Minimap
            WRITEWIN(528,12,12,0,MyRPort_PTR[0],4,Save.SystemName.data[i]);
            if ((0 != Save.CivPlayer[ActPlayer-1])
             && (0 != (SystemFlags[ActPlayer-1][i] & FLAG_KNOWN)))
            {
                _s = dez2out(SystemHeader[i].Planets, 0, s);
                *_s++=' ';
                strcpy(_s, _PT_Planeten);
                WRITEWIN(528,29,12,0,MyRPort_PTR[0],4, s);
                _s = GETCIVNAME(GETCIVVAR(SystemFlags[0][i] & FLAG_CIV_MASK));
                WRITEWIN(528,46,SystemFlags[0][i] & FLAG_CIV_MASK,0,MyRPort_PTR[0],4, _s);
            } else {
                WRITEWIN(528,29,12,0,MyRPort_PTR[0],4, _PT_Unbekannt);
            }
            if (0 != ActSys)
            {
                /* show distance
                   here:  abs(X2-X1) + abs(Y2-Y1)
                   right: sqrt( (X2-X1)^2 + (Y2-Y1)^2 )
                */
                distance = abs(SystemX[ActSys-1]-SystemX[i]) + abs(SystemY[ActSys-1]-SystemY[i]);
                distance = (uint32) (distance / ShipMaxMove);
                (void) dez2out(distance, 4, s);
                WRITEWIN(547,70,8,0,MyRPort_PTR[0],2,s);
                WRITEWIN(550,87,12,0,MyRPort_PTR[0],4, _PT_Jahre);
            } else if (TARGET_STARGATE == SystemHeader[i].FirstShip.SType)
            {
                WRITEWIN(528,77,12,0,MyRPort_PTR[0],4,"Stargate");
            }
        }
    }
}