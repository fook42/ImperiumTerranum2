#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DOSSIER(bool Cheat)
{
    uint8   i, j;
    uint8   civflag;
    uint16  posy;
    char    s[30];
    char*   _s;
    struct Window* DOS_Window;
    struct RastPort* RPort_PTR;
    DOS_Window=MAKEWINDOW(0,50,511,393,MyScreen[0]);
    if (NULL == DOS_Window)
    {
        return;
    }
    RPort_PTR = DOS_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,  0,510,205,12,6,1);
    MAKEWINBORDER(RPort_PTR,0,206,510,392,12,6,1);

    WRITE(175,10,12,WRITE_Right,RPort_PTR,2,_PT_Status);
    WRITE(275,10,12,WRITE_Right,RPort_PTR,2,_PT_Bevoelkerung);
    WRITE(345,10,12,WRITE_Right,RPort_PTR,2,_PT_Militaer);
    WRITE(430,10,12,WRITE_Right,RPort_PTR,2,_PT_Guthaben);
    WRITE(500,10,12,WRITE_Right,RPort_PTR,2,_PT_Wertung);
    posy = 20;
    // i = 1..8
    for (i = 1; i < MAXCIVS; ++i)
    {
        if (((LEVEL_UNKNOWN != Save.WarState[i-1][ActPlayer-1]) || (i == ActPlayer) || (Cheat))
         && ((8 > i) || ((0           != Save.WorldFlag) &&
                         (WFLAG_FIELD != Save.WorldFlag)) ))
        {
            posy += 20;
            civflag = GETCIVFLAG(i);
            _s = GETCIVNAME(i);
            WRITE(14, posy, civflag,0,RPort_PTR,2,_s);
            if (8 > i)
            {
                WRITE(i*73-35,215,civflag,WRITE_Center,RPort_PTR,2,_s);
            }
            switch (Save.WarState[ActPlayer-1][i-1])
            {
                case LEVEL_ALLIANZ: _s = _PT_Allianz;    break;
                case LEVEL_WAR:     _s = _PT_Krieg;      break;
                case LEVEL_DIED:    _s = _Txt_Separator; break;
                default:            _s = _PT_Frieden;
            }
            if (i != ActPlayer)
            {
                WRITE(175, posy, civflag,WRITE_Right,RPort_PTR,2,_s);
            }
            if ((i == ActPlayer) || (Save.WarState[ActPlayer-1][i-1] == LEVEL_DIED)
             || (Save.SSMoney[ActPlayer-1][i-1] > (Save.WarPower[i-1]*39)) || (Cheat))
            {
                _s = dez2out(Save.Bevoelkerung[i-1], 0, s);
                strcpy(_s, " Mio");
                WRITE(275, posy,civflag,WRITE_Right,RPort_PTR,2,s);

                (void) dez2out(Save.WarPower[i-1], 0, s);
                WRITE(345, posy,civflag,WRITE_Right,RPort_PTR,2,s);

                (void) dez2out(Save.Staatstopf[i-1], 0, s);
                WRITE(430, posy,civflag,WRITE_Right,RPort_PTR,2,s);
                if ( ((0 < i) && (i < 8)) || ((0 != Save.WorldFlag) && (WFLAG_FIELD != Save.WorldFlag)) )
                {
                    for (j = 1; j < 8; ++j)
                    {
                        if ((   (LEVEL_DIED    != Save.WarState[i-1][j-1])
                             && (LEVEL_UNKNOWN != Save.WarState[i-1][j-1]))
                           || (Cheat))
                        {
                            if ((i != j) || (i == GETCIVVAR(Save.WorldFlag)))
                            {
                                switch (Save.WarState[i-1][j-1])
                                {
                                   case LEVEL_ALLIANZ: _s = _PT_Allianz;    break;
                                   case LEVEL_DIED:    _s = _Txt_Separator; break;
                                   case LEVEL_WAR:     _s = _PT_Krieg;      break;
                                   default:
                                   {
                                      _s = _PT_Frieden;
                                      if ((WFLAG_JAHADR == Save.WorldFlag) && (8 == i) && (0 < Save.JSteuer[j-1]))
                                      {
                                         _s = PText[696];
                                      }
                                   }
                                }
                                if ((i == j) && (i == GETCIVVAR(Save.WorldFlag)))
                                {
                                    WRITE(j*73-35,217+i*18,civflag,(5|WRITE_Center),RPort_PTR,2, _PT_Krieg);
                                } else {
                                    WRITE(j*73-35,217+i*18,civflag,   WRITE_Center ,RPort_PTR,2, _s);
                                }
                            }
                        }
                    }
                }
            } else {
                WRITE(210, posy,civflag,0,RPort_PTR,2, _PT_keineInfos);
            }

            (void) dez2out(Save.ImperatorState[i-1], 0, s);
            WRITE(500, posy,civflag,WRITE_Right,RPort_PTR,2,s);
        }
    }
    WAITLOOP(false);
    CloseWindow(DOS_Window);
}
