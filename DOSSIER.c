#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DOSSIER(bool Cheat)
{
    uint8   i, j;
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
    MAKEWINBORDER(RPort_PTR,0,0,510,205,12,6,0);
    MAKEWINBORDER(RPort_PTR,0,206,510,392,12,6,0);

	WRITEWIN(175,10,12,WRITE_Right,RPort_PTR,3,_PT_Status);
    WRITEWIN(275,10,12,WRITE_Right,RPort_PTR,3,_PT_Bevoelkerung);
    WRITEWIN(345,10,12,WRITE_Right,RPort_PTR,3,_PT_Militaer);
    WRITEWIN(430,10,12,WRITE_Right,RPort_PTR,3,_PT_Guthaben);
    WRITEWIN(500,10,12,WRITE_Right,RPort_PTR,3,_PT_Wertung);
    for (i = 1; i<MAXCIVS; i++)
    {
        if (((Save.WarState[i-1][ActPlayer-1] != LEVEL_UNKNOWN) || (i == ActPlayer) || (Cheat))
         && ((i<8) || ((Save.WorldFlag!=0) && (Save.WorldFlag!=WFLAG_FIELD))))
        {
            _s = GETCIVNAME(i);
            WRITEWIN(14,i*20+20,GETCIVFLAG(i),0,RPort_PTR,3,_s);
            if (i<8)
            {
                WRITEWIN(i*73-35,215,GETCIVFLAG(i),WRITE_Center,RPort_PTR,3,_s);
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
                WRITEWIN(175,i*20+20,GETCIVFLAG(i),WRITE_Right,RPort_PTR,3,_s);
            }
            if ((i == ActPlayer) || (Save.WarState[ActPlayer-1][i-1] == LEVEL_DIED)
             || (Save.SSMoney[ActPlayer-1][i-1] > (Save.WarPower[i-1]*39)) || (Cheat))
            {
                _s = dez2out(Save.Bevoelkerung[i-1], 0, s);
                strcpy(_s, " Mio");
                WRITEWIN(275,i*20+20,GETCIVFLAG(i),WRITE_Right,RPort_PTR,3,s);

                (void) dez2out(Save.WarPower[i-1], 0, s);
                WRITEWIN(345,i*20+20,GETCIVFLAG(i),WRITE_Right,RPort_PTR,3,s);

                (void) dez2out(Save.Staatstopf[i-1], 0, s);
                WRITEWIN(430,i*20+20,GETCIVFLAG(i),WRITE_Right,RPort_PTR,3,s);
                for (j = 1; j<=7; j++)
                {
                    if (( ((i>=1) && (i<=7)) || ((Save.WorldFlag!=0) && (Save.WorldFlag!=WFLAG_FIELD)) )
                     && (((Save.WarState[i-1][j-1]!=LEVEL_DIED) && (Save.WarState[i-1][j-1]!=LEVEL_UNKNOWN)) || (Cheat))
                     && ((i != j) || (GETCIVVAR(Save.WorldFlag) == i)))
                    {
                        switch (Save.WarState[i-1][j-1])
                        {
                           case LEVEL_ALLIANZ: _s = _PT_Allianz;    break;
                           case LEVEL_DIED:    _s = _Txt_Separator; break;
                           case LEVEL_WAR:     _s = _PT_Krieg;      break;
                           default:
                           {
                              _s = _PT_Frieden;
                              if ((Save.WorldFlag == WFLAG_JAHADR) && (i == 8) && (Save.JSteuer[j-1]>0))
                              {
                                 _s = PText[696];
                              }
                           }
                        }
                        if ((i == j) && (GETCIVVAR(Save.WorldFlag) == i))
                        {
                            WRITEWIN(j*73-35,217+i*18,GETCIVFLAG(i),(5|WRITE_Center),RPort_PTR,3, _PT_Krieg);
                        } else {
                            WRITEWIN(j*73-35,217+i*18,GETCIVFLAG(i),WRITE_Center,RPort_PTR,3, _s);
                        }
                    }
                }
            } else {
                WRITEWIN(210,i*20+20,GETCIVFLAG(i),0,RPort_PTR,3, _PT_keineInfos);
            }

            (void) dez2out(Save.ImperatorState[i-1], 0, s);
            WRITEWIN(500,i*20+20,GETCIVFLAG(i),WRITE_Right,RPort_PTR,3,s);
        }
    }
    WAITLOOP(false);
	CloseWindow(DOS_Window);
}
