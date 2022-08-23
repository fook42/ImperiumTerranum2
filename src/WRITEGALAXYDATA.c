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

    for(i = 0; i < Save.Systems; ++i)
    {
        if ((MouseX(0) > (SystemX[i]-21)) && (MouseX(0) < (SystemX[i]+21))
         && (MouseY(0) > (SystemY[i]-6))  && (MouseY(0) < (SystemY[i]+6))
         && (LastSystem != (i+1)))
        {
            LastSystem = (i+1);
            RECT_RP0_C0(HighRes_Width-118,9,HighRes_Width-11,117);      // clear Minimap
            WRITE_RP0(HighRes_Width-65,12,12,WRITE_Center,3,Save.SystemName[i]);
            if ((0 != Save.CivPlayer[ActPlayer-1])
             && (0 != (SystemFlags[ActPlayer-1][i] & FLAG_KNOWN)))
            {
                _s = dez2out(SystemHeader[i].Planets, 0, s);
                *_s++ = ' ';
                (void) my_strcpy(_s, _PT_Planeten);
                WRITE_RP0(HighRes_Width-65,29,12,WRITE_Center,3, s);
                _s = GETCIVNAME(GETCIVVAR(SystemFlags[0][i] & FLAG_CIV_MASK));
                WRITE_RP0(HighRes_Width-65,46,SystemFlags[0][i] & FLAG_CIV_MASK,WRITE_Center,3, _s);
            } else {
                WRITE_RP0(HighRes_Width-65,29,12,WRITE_Center,3, _PT_Unbekannt);
            }
            if (0 != ActSys)
            {
                /* show distance */
                distance = sqrt(   (SystemX[ActSys-1]-SystemX[i])*(SystemX[ActSys-1]-SystemX[i])
                                 + (SystemY[ActSys-1]-SystemY[i])*(SystemY[ActSys-1]-SystemY[i]));
                distance = distance / ShipMaxMove;
                if (127 < distance) { distance = 127; }
                (void) dez2out(distance, 3, s);
                WRITE_RP0(HighRes_Width-65,70, 8,WRITE_Center,1,s);
                WRITE_RP0(HighRes_Width-65,87,12,WRITE_Center,3, _PT_Jahre);
            } else if (TARGET_STARGATE == SystemHeader[i].FirstShip.SType)
            {
                WRITE_RP0(HighRes_Width-65,77,12,WRITE_Center,3,"Stargate");
            }
        }
    }
}
