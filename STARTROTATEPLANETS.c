#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEINFOBOX(struct Window** CIB_Window, struct RastPort** CIB_RPort_PTR)
{
    char    s[60];
    char*   _s;
    int     len;

    ++ActPlayer;    // for array shifting...
    INFORMUSER();
    --ActPlayer;    // for array shifting...

    *CIB_Window = MAKEWINDOW(30,100,451,171,MyScreen[0]);
    if (NULL != (*CIB_Window))
    {
        *CIB_RPort_PTR = (*CIB_Window)->RPort;
        MAKEWINBORDER(*CIB_RPort_PTR,0,0,450,170,12,6,1);
        len = strlen(PText[646]);
        memcpy(s, PText[646], len);
        s[len++]=' ';
        _s = dez2out(Year, 0, s+len);
        _s[0] = ':';
        _s[1] = 0;
        WRITE(225, 10,ActPlayerFlag,WRITE_Center,*CIB_RPort_PTR,3,s);
    }
    PRINTGLOBALINFOS(ActPlayer);
}

void STARTROTATEPLANETS()
{
    uint8   i, j;
    char    s[99];
    char*   _s;
    int     len;
    struct Window*   SRP_Window = NULL;
    struct RastPort* RPort_PTR = NULL;

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
        if (LEVEL_DIED != Save.WarState[ActPlayer][ActPlayer])
        {
            for(i = 0; i < (MAXCIVS-1); ++i)
            {
                if (Save.LastWarState[ActPlayer][i] != Save.WarState[ActPlayer][i])
                {
                    j++;
                    if (LEVEL_DIED == Save.WarState[ActPlayer][i])
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
                    if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
                    WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,s);
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
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,s);
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,PText[656]);
        }

        if (0 != Save.CivilWar)
        {
            if ((6 == ActPlayer) && (Save.WarState[Save.CivilWar-1][7] == Save.LastWarState[Save.CivilWar-1][7]))
            {
                Save.CivilWar = 0;
            } else if ((LEVEL_UNKNOWN == Save.WarState[Save.CivilWar-1][7])
                    && (LEVEL_WAR == Save.LastWarState[Save.CivilWar-1][7])
                    && (GETCIVVAR(Save.WorldFlag)>=1) && (GETCIVVAR(Save.WorldFlag)<=7))
            {
                strcpy(s, GETCIVNAME(FLAG_OTHER));
                strcat(s, " ");
                strcat(s, PText[657]);
                if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
                j++;
                WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,s);
            }
        }
        if (0 != GetPlanetSys[ActPlayer])
        {
            strcpy(s, _PT_Planet);
            strcat(s, " ");
            strcat(s, GetPlanet[ActPlayer]->PName);
            strcat(s, " ");
            strcat(s, PText[658]);
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,s);
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
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3, _s);
            Verschrottung[ActPlayer] = 0;
        }

        if (vNSonde[ActPlayer])
        {
            strcpy(s, GETCIVNAME(ActPlayer+1));
            strcat(s, " ");
            strcat(s, PText[662]);
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,s);
            vNSonde[ActPlayer] = false;
        }
    }

    if ((0 != Save.CivPlayer[ActPlayer]) && (0 == (rand()%10)))
    {
        if        (1963 == Year)
        {
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"Auf der Erde nimmt Dank Gene Roddenberry");
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"eine Legende ihren Anfang!");
        } else if (1973 == Year)
        {
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"Auf der Erde wird ein");
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"genialer Programmierer geboren!");
        } else if (2001 == Year)
        {
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"Stanley Kubricks & Arthur C. Clarkes");
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"Vision wird Wirklichkeit!");
        } else if (2010 == Year)
        {
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"Peter Hyams & Arthur C. Clarkes");
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"Vision wird Wirklichkeit!");
        } else if (2063 == Year)
        {
            if (!Informed) { CREATEINFOBOX(&SRP_Window, &RPort_PTR); }
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"Auf der Erde findet");
            j++;
            WRITE(225,j*18,12,WRITE_Center,RPort_PTR,3,"\"Der erste Kontakt\" statt!");
        }
    }
    ++ActPlayer;    // for array shifting...

    if (j>1)
    {
        if (Save.PlayMySelf) { delay(PAUSE); }
        WAITLOOP(Save.PlayMySelf);
        if (NULL != SRP_Window)
        {
            CloseWindow(SRP_Window);
        }
//        REFRESHDISPLAY();
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
    if ((0 != Save.CivPlayer[ActPlayer-1]) && (Informed))
    {
        RECTWIN(MyRPort_PTR[0],0,520,291,632,308);
        WRITE(521,292,12,1,MyRPort_PTR[0],3,PText[663]);
    }
    // Printf("## out STARTROTATEPLANETS ##\n");
}
