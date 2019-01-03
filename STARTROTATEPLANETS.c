#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEINFOBOX()
{
    char    s[60];
    char*   _s;
    int     len;

    ++ActPlayer;    // for array shifting...
    INFORMUSER();
    --ActPlayer;    // for array shifting...

    MAKEWINBORDER(MyRPort_PTR[0],30,100,480,270,12,6,0);
    len = strlen(PText[646]);
    memcpy(s, PText[646], len);
    s[len++]=' ';
    _s = dez2out(Year, 0, s+len);
    _s[0] = ':';
    _s[1] = 0;
    WRITEWIN(255,110,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],4,s);
    PRINTGLOBALINFOS(ActPlayer);
}

void STARTROTATEPLANETS()
{
    uint8   i, j;
    char    s[99];
    char*   _s;
    int     len;

    // Printf("## in STARTROTATEPLANETS ##\n");
    for(i = 0; i < MAXCIVS; i++)
    {
        Save.LastWarState[ActPlayer-1][i] = Save.WarState[ActPlayer-1][i];
    }
    if (Informed)
    {
        LastDisplay[ActPlayer] = Display;
    }
    ++ActPlayer;
    if (ActPlayer > MAXCIVS)
    {
        ActPlayer = 1;
    }
    ActPlayerFlag = GETCIVFLAG(ActPlayer);
    if (0 != Display)
    {
        DRAWSTARS(MODE_REDRAW, ActPlayer);
    }
    Informed = false;
    j = 1;
    --ActPlayer;    // for array shifting...

    if ((Year>1900) && (Save.CivPlayer[ActPlayer] != 0))
    {
        if (Save.WarState[ActPlayer][ActPlayer] != LEVEL_DIED)
        {
            for(i = 0; i < (MAXCIVS-1); ++i)
            {
                if (Save.LastWarState[ActPlayer][i] != Save.WarState[ActPlayer][i])
                {
                    j++;
                    if (Save.WarState[ActPlayer][i] == LEVEL_DIED)
                    {
                        len = strlen(PText[600]);
                        memcpy(s, PText[600], len);
                        s[len++]=' ';
                        strcpy(s+len, GETCIVNAME(i+1));
                        len = strlen(s);
                        s[len++]=' ';
                        strcpy(s+len, PText[601]);
                    } else if ((Save.WarState[ActPlayer][i] == LEVEL_PEACE)
                            && (Save.LastWarState[ActPlayer][i] == LEVEL_WAR))
                    {
                        strcpy(s, GETCIVNAME(i+1));
                        strcat(s, " ");
                        strcat(s, PText[647]);

                    } else if (Save.WarState[ActPlayer][i] == LEVEL_WAR)
                    {
                        strcpy(s, GETCIVNAME(i+1));
                        strcat(s, " ");
                        strcat(s, PText[648]);
                        strcat(s, " ");
                        strcat(s, GETCIVNAME(ActPlayer+1));
                        strcat(s, " ");
                        strcat(s, PText[649]);
                    } else if (Save.WarState[ActPlayer][i] == LEVEL_ALLIANZ) 
                    {
                        strcpy(s, GETCIVNAME(i+1));
                        strcat(s, " ");
                        strcat(s, PText[650]);
                        strcat(s, " ");
                        strcat(s, GETCIVNAME(ActPlayer+1));
                        strcat(s, PText[651]);
                    } else {
                        j--;
                    }
                }
                if (j>1)
                {
                    if (!Informed) { CREATEINFOBOX(); }
                    WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,s);
                }
            }
        }

        if ((Save.WarState[ActPlayer-1][7] == LEVEL_PEACE)
         && (Save.LastWarState[ActPlayer-1][7] == LEVEL_UNKNOWN)
         && ((Save.WorldFlag==FLAG_TERRA) || (Save.WorldFlag==FLAG_KLEGAN) || (Save.WorldFlag==FLAG_REMALO)
           ||(Save.WorldFlag==FLAG_CARDAC)|| (Save.WorldFlag==FLAG_FERAGI) || (Save.WorldFlag==FLAG_BAROJA)
           ||(Save.WorldFlag==FLAG_VOLKAN)))
        {
            strcpy(s, GETCIVNAME(FLAG_OTHER));
            strcat(s, " ");
            strcat(s, PText[655]);
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,s);
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,PText[656]);
        }

        if ((0 != Save.CivilWar) && (6 == ActPlayer)
         && (Save.WarState[Save.CivilWar-1][7] == Save.LastWarState[Save.CivilWar-1][7]))
        {
            Save.CivilWar = 0;
        }
        if (0 != Save.CivilWar)
        {
            if ((Save.WarState[Save.CivilWar-1][7] == LEVEL_UNKNOWN)
             && (Save.LastWarState[Save.CivilWar-1][7] == LEVEL_WAR)
             && (GETCIVVAR(Save.WorldFlag)>=1) && (GETCIVVAR(Save.WorldFlag)<=7))
            {
                strcpy(s, GETCIVNAME(FLAG_OTHER));
                strcat(s, " ");
                strcat(s, PText[657]);
                if (!Informed) { CREATEINFOBOX(); }
                j++;
                WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,s);
            }
        }

        if (GetPlanetSys[ActPlayer] != 0)
        {
            strcpy(s, _PT_Planet);
            strcat(s, " ");
            strcat(s, GetPlanet[ActPlayer]->PName);
            strcat(s, " ");
            strcat(s, PText[658]);
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,s);
        }
        if (Verschrottung[ActPlayer]>0)
        {
            if (Verschrottung[ActPlayer]>1)
            {
                _s = dez2out(Verschrottung[ActPlayer], 0, s);
                *_s++=' ';
                strcpy(_s, PText[659]);
                _s = s;
            } else {
                _s = PText[660];
            }
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4, _s);
            Verschrottung[ActPlayer] = 0;
        }

        if (vNSonde[ActPlayer])
        {
            strcpy(s, GETCIVNAME(ActPlayer+1));
            strcat(s, " ");
            strcat(s, PText[662]);
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,s);
            vNSonde[ActPlayer] = false;
        }
    }

    if ((0 != Save.CivPlayer[ActPlayer]) && (0 == (rand()%10)))
    {
        if        (1963 == Year)
        {
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"Auf der Erde nimmt Dank Gene Roddenberry");
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"eine Legende ihren Anfang!");
        } else if (1973 == Year)
        {
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"Auf der Erde wird ein");
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"genialer Programmierer geboren!");
        } else if (2001 == Year)
        {
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"Stanley Kubricks & Arthur C. Clarkes");
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"Vision wird Wirklichkeit!");
        } else if (2010 == Year)
        {
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"Peter Hyams & Arthur C. Clarkes");
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"Vision wird Wirklichkeit!");
        } else if (2063 == Year)
        {
            if (!Informed) { CREATEINFOBOX(); }
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"Auf der Erde findet");
            j++;
            WRITEWIN(255,100+j*18,12,WRITE_Center,MyRPort_PTR[0],4,"\"Der erste Kontakt\" statt!");
        }
    }
    ++ActPlayer;    // for array shifting...

    if (j>1)
    {
        if (Save.PlayMySelf) { delay(PAUSE); }
        WAITLOOP(Save.PlayMySelf);
        RECTWIN(MyRPort_PTR[0],0,30,100,480,270);
        REFRESHDISPLAY();
        if (0 != GetPlanetSys[ActPlayer-1])
        {
            if ((!Save.PlayMySelf) && (0 != Save.CivPlayer[ActPlayer-1]))
            {
                HANDLEKNOWNPLANET(GetPlanetSys[ActPlayer-1],0,GetPlanet[ActPlayer-1]);
            }
        }
    }
    GetPlanetSys[ActPlayer-1] = 0;
    ROTATEPLANETS(Display);
    if (Informed)
    {
        if ((0 == LastDisplay[ActPlayer]) && (0 != Display))
        {
            DRAWSTARS(MODE_REDRAW,ActPlayer);
        } else if (0 != LastDisplay[ActPlayer])
        {
            HANDLESYSTEM(&LastDisplay[ActPlayer],NULL);
        }
    }
    if ((Save.CivPlayer[ActPlayer-1] != 0) && (Informed))
    {
        RECTWIN(MyRPort_PTR[0],0,520,291,632,308);
        WRITEWIN(521,292,12,1,MyRPort_PTR[0],4,PText[663]);
    }
    // Printf("## out STARTROTATEPLANETS ##\n");
}
