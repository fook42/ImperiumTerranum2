#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DOSSIER()
{
    uint8   i, j;
    uint8   civ_color;
    uint16  posx, posy;
    uint16  posx2, posy2;
    const char*  DOS_RaceInfos[] = {NULL,_PT_Status,_PT_Bevoelkerung,_PT_Militaer,_PT_Guthaben,_PT_Wertung};
    const uint16 DOS_borderX = 10;
    const uint16 Tab6_spaceX = (Area_Width-2*DOS_borderX)/(sizeof(DOS_RaceInfos)/sizeof(DOS_RaceInfos[0]));
    const uint16 Tab7_spaceX = (Area_Width-2*DOS_borderX)/7;

    char    s[30];
    char*   _s;
    struct Window* DOS_Window;
    struct RastPort* RPort_PTR;
    DOS_Window=MAKEWINDOW(0,50,Area_Width,393,MyScreen[0]);
    if (NULL == DOS_Window)
    {
        return;
    }
    RPort_PTR = DOS_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,  0,Area_Width-1,205,12,6,1);
    MAKEWINBORDER(RPort_PTR,0,206,Area_Width-1,392,12,6,1);

    posx = DOS_borderX;
    for (i=0; i<(sizeof(DOS_RaceInfos)/sizeof(DOS_RaceInfos[0])); ++i)
    {
        posx += Tab6_spaceX;
        WRITE(posx,10,12,WRITE_Right,RPort_PTR,2,DOS_RaceInfos[i]);
    }
    posx = 38;
    posy = 40;
    posy2= 235;
    // i = 1..8
    for (i = 0; i < (MAXCIVS-1); ++i)
    {
        if (((LEVEL_UNKNOWN != Save.WarState[i][ActPlayer-1]) || (i == (ActPlayer-1)))
         && ((7 > i) || ((0           != Save.WorldFlag) &&
                         (WFLAG_FIELD != Save.WorldFlag)) ))
        {
            civ_color = GETCIVFLAG(i+1);
            _s = GETCIVNAME(i+1);
            WRITE(DOS_borderX, posy, civ_color,0,RPort_PTR,2,_s);
            if (7 > i)
            {
                WRITE(posx,215,civ_color,WRITE_Center,RPort_PTR,2,_s);
            }
            switch (Save.WarState[ActPlayer-1][i])
            {
                case LEVEL_ALLIANZ: _s = _PT_Allianz;    break;
                case LEVEL_WAR:     _s = _PT_Krieg;      break;
                case LEVEL_DIED:    _s = _Txt_Separator; break;
                default:            _s = _PT_Frieden;
            }
            if (i != (ActPlayer-1))
            {
                WRITE(DOS_borderX+2*Tab6_spaceX, posy, civ_color,WRITE_Right,RPort_PTR,2,_s);
            }
            if ((i == (ActPlayer-1)) || (Save.WarState[ActPlayer-1][i] == LEVEL_DIED)
             || (Save.SSMoney[ActPlayer-1][i] > (Save.WarPower[i]*39)))
            {
                _s = dez2out(Save.Bevoelkerung[i], 0, s);
                (void) my_strcpy(_s, "Mio");
                WRITE(DOS_borderX+3*Tab6_spaceX, posy,civ_color,WRITE_Right,RPort_PTR,2,s);

                (void) dez2out(Save.WarPower[i], 0, s);
                WRITE(DOS_borderX+4*Tab6_spaceX, posy,civ_color,WRITE_Right,RPort_PTR,2,s);

                (void) dez2out(Save.Staatstopf[i], 0, s);
                WRITE(DOS_borderX+5*Tab6_spaceX, posy,civ_color,WRITE_Right,RPort_PTR,2,s);

                posx2 = 38;
                for (j = 0; j < 7; ++j)
                {
                    if (   (LEVEL_DIED    != Save.WarState[i][j])
                        && (LEVEL_UNKNOWN != Save.WarState[i][j]))
                    {
                        if ((i != j) || (i == (GETCIVVAR(Save.WorldFlag)-1)))
                        {
                            switch (Save.WarState[i][j])
                            {
                                case LEVEL_ALLIANZ: _s = _PT_Allianz;    break;
                                case LEVEL_WAR:     _s = _PT_Krieg;      break;
                                default:
                                {
                                    _s = _PT_Frieden;
                                    if ((WFLAG_JAHADR == Save.WorldFlag) && (7 == i) && (0 < Save.JSteuer[j]))
                                    {
                                        _s = PText[696];
                                    }
                                }
                            }
                            if ((i == j) && (i == (GETCIVVAR(Save.WorldFlag)-1)))
                            {
                                WRITE(posx2, posy2, civ_color,(5|WRITE_Center),RPort_PTR,2, _PT_Krieg);
                            } else {
                                WRITE(posx2, posy2, civ_color,   WRITE_Center ,RPort_PTR,2, _s);
                            }
                        }
                    }
                    posx2 += Tab7_spaceX;
                }
            } else {
                WRITE(DOS_borderX+3*Tab6_spaceX, posy,civ_color,0,RPort_PTR,2, _PT_keineInfos);
            }

            (void) dez2out(Save.ImperatorState[i], 0, s);
            WRITE(DOS_borderX+6*Tab6_spaceX, posy,civ_color,WRITE_Right,RPort_PTR,2,s);

            posy += 20;
        }
        posx += Tab7_spaceX;
        posy2+= 18;
    }
    WAITLOOP(false);
    CloseWindow(DOS_Window);
}
