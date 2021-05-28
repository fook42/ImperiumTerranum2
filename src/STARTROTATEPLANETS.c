#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATEINFOBOX(struct Window** CIB_Window, struct RastPort** CIB_RPort_PTR)
{
    char    s[60];
    char*   _s;

    if (!Informed)
    {
        ++ActPlayer;    // for array shifting...
        INFORMUSER();
        --ActPlayer;    // for array shifting...

        *CIB_Window = MAKEWINDOWBORDER(30,100,451,171,MyScreen[0]);
        if (NULL != (*CIB_Window))
        {
            *CIB_RPort_PTR = (*CIB_Window)->RPort;


            _s=my_strcpy(s, PText[646]);
            *_s++ = ' ';
            _s = dez2out(Year, 0, _s);
            *_s++ = ':';
            *_s = 0;
            WRITE(225, 10,ActPlayerFlag,WRITE_Center|JAM1,*CIB_RPort_PTR,3,s);
        }
        PRINTGLOBALINFOS(ActPlayer);
    }
}

void STARTROTATEPLANETS()
{
    uint8   i;
    uint16  j;
    char    s[99];
    char*   _s;
    struct Window*   SRP_Window = NULL;
    struct RastPort* RPort_PTR = NULL;

    const char* InfoBox_Texts[] = {"Auf der Erde nimmt Dank Gene Roddenberry", "eine Legende ihren Anfang!",
                                   "Auf der Erde wird ein", "genialer Programmierer geboren!",
                                   "Stanley Kubricks & Arthur C. Clarkes", "Vision wird Wirklichkeit!",
                                   "Peter Hyams & Arthur C. Clarkes", "Vision wird Wirklichkeit!",
                                   "Auf der Erde findet", "\"Der erste Kontakt\" statt!" };
    const int InfoBox_Years[] = { 1963, 1973, 2001, 2010, 2063 };


    for(i = 0; i < MAXCIVS; i++)
    {
        Save.LastWarState[ActPlayer-1][i] = Save.WarState[ActPlayer-1][i];
    }
    if (Informed)
    {
        LastDisplay[ActPlayer] = Display;
    }

    ++ActPlayer; // select next Player... or wrap around
    if (ActPlayer > MAXCIVS)
    {
        ActPlayer = 1;
    }
    ActPlayerFlag = GETCIVFLAG(ActPlayer);

    if (0 != Display)
    {
        DRAWSTARS(MODE_REDRAW);
    }
    Informed = false;
    j = 18;
    --ActPlayer;    // for array shifting...

    if ((Year>1900) && (Save.CivPlayer[ActPlayer] != 0))
    {
        if (LEVEL_DIED != Save.WarState[ActPlayer][ActPlayer])
        {
            for(i = 0; i < (MAXCIVS-1); ++i)
            {
                if (Save.LastWarState[ActPlayer][i] != Save.WarState[ActPlayer][i])
                {
                    j += 18;
                    if (LEVEL_DIED == Save.WarState[ActPlayer][i])
                    {
                        _s=my_strcpy(s, PText[600]);
                        *_s++ = ' ';
                        _s=my_strcpy(_s, GETCIVNAME(i+1));
                        *_s++ = ' ';
                        (void) my_strcpy(_s, PText[601]);
                    }
                    else if ((Save.WarState[ActPlayer][i] == LEVEL_PEACE)
                            && (Save.LastWarState[ActPlayer][i] == LEVEL_WAR))
                    {
                        _s=my_strcpy(s, GETCIVNAME(i+1));
                        *_s++ = ' ';
                        (void) my_strcpy(_s, PText[647]);
                    }
                    else if (Save.WarState[ActPlayer][i] == LEVEL_WAR)
                    {
                        _s=my_strcpy(s, GETCIVNAME(i+1));
                        *_s++ = ' ';
                        _s=my_strcpy(_s, PText[648]);
                        *_s++ = ' ';
                        _s=my_strcpy(_s, GETCIVNAME(ActPlayer+1));
                        *_s++ = ' ';
                        (void) my_strcpy(_s, PText[649]);
                    }
                    else if (Save.WarState[ActPlayer][i] == LEVEL_ALLIANZ)
                    {
                        _s=my_strcpy(s, GETCIVNAME(i+1));
                        *_s++ = ' ';
                        _s=my_strcpy(_s, PText[650]);
                        *_s++ = ' ';
                        _s=my_strcpy(_s, GETCIVNAME(ActPlayer+1));
                        (void) my_strcpy(_s, PText[651]);
                    }
                    else
                    {
                        j -= 18;
                    }
                }
                if (18 < j)
                {
                    CREATEINFOBOX(&SRP_Window, &RPort_PTR);
                    WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3,s);
                }
            }
        }

        if ((Save.WarState[ActPlayer-1][7] == LEVEL_PEACE)
         && (Save.LastWarState[ActPlayer-1][7] == LEVEL_UNKNOWN)
         && ((Save.WorldFlag==FLAG_TERRA) || (Save.WorldFlag==FLAG_KLEGAN) || (Save.WorldFlag==FLAG_REMALO)
           ||(Save.WorldFlag==FLAG_CARDAC)|| (Save.WorldFlag==FLAG_FERAGI) || (Save.WorldFlag==FLAG_BAROJA)
           ||(Save.WorldFlag==FLAG_VOLKAN)))
        {
            _s=my_strcpy(s, GETCIVNAME(FLAG_OTHER));
            *_s++ = ' ';
            (void) my_strcpy(_s, PText[655]);
            CREATEINFOBOX(&SRP_Window, &RPort_PTR);
            j += 18;
            WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3,s);
            j += 18;
            WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3,PText[656]);
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
                _s=my_strcpy(s, GETCIVNAME(FLAG_OTHER));
                *_s++ = ' ';
                (void) my_strcpy(_s, PText[657]);
                CREATEINFOBOX(&SRP_Window, &RPort_PTR);
                j += 18;
                WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3,s);
            }
        }
        if (0 != GetPlanetSys[ActPlayer])
        {
            _s=my_strcpy(s, _PT_Planet);
            *_s++ = ' ';
            _s=my_strcpy(_s, GetPlanet[ActPlayer]->PName);
            *_s++ = ' ';
            (void) my_strcpy(_s, PText[658]);
            CREATEINFOBOX(&SRP_Window, &RPort_PTR);
            j += 18;
            WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3,s);
        }
        if (Verschrottung[ActPlayer]>0)
        {
            if (Verschrottung[ActPlayer]>1)
            {
                _s = dez2out(Verschrottung[ActPlayer], 0, s);
                *_s++ = ' ';
                (void) my_strcpy(_s, PText[659]);
                _s = s;
            } else {
                _s = PText[660];
            }
            CREATEINFOBOX(&SRP_Window, &RPort_PTR);
            j += 18;
            WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3, _s);
            Verschrottung[ActPlayer] = 0;
        }

        if (vNSonde[ActPlayer])
        {
            _s=my_strcpy(s, GETCIVNAME(ActPlayer+1));
            *_s++ = ' ';
            (void) my_strcpy(_s, PText[662]);
            CREATEINFOBOX(&SRP_Window, &RPort_PTR);
            j += 18;
            WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3,s);
            vNSonde[ActPlayer] = false;
        }
    }

    if ((0 != Save.CivPlayer[ActPlayer]) && (0 == (rand()%10)))
    {
        for (i = 0; i < (sizeof(InfoBox_Years)/sizeof(InfoBox_Years[0])); ++i)
        {
            if (InfoBox_Years[i] == Year)
            {
                CREATEINFOBOX(&SRP_Window, &RPort_PTR);
                j += 18;
                WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3, InfoBox_Texts[i*2]);
                j += 18;
                WRITE(225,j,12,WRITE_Center|JAM1,RPort_PTR,3, InfoBox_Texts[i*2+1]);
                break;
            }
        }
    }
    ++ActPlayer;    // for array shifting...

    if (18 < j)
    {
        if (Save.PlayMySelf) { Delay(PAUSE); }
        WAITLOOP(Save.PlayMySelf);
        if (NULL != SRP_Window)
        {
            CloseWindow(SRP_Window);
        }
        // REFRESHDISPLAY();
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
            DRAWSTARS(MODE_REDRAW);
        } else if (0 != LastDisplay[ActPlayer])
        {
            HANDLESYSTEM(&LastDisplay[ActPlayer],NULL);
        }
    }
    if ((0 != Save.CivPlayer[ActPlayer-1]) && (Informed))
    {
        RECT_RP0_C0(520,291,632,308);
        WRITE_RP0(521,292,12,JAM2,3,PText[663]);
    }
}
