#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

#define MAXGADGETS  7

#define GADGET_ANGRIFF  1
#define GADGET_ORBIT    2
#define GADGET_LADEN    3
#define GADGET_LANDUNG  4
#define GADGET_DIPLOMAT 8
#define GADGET_ATTACK   16

#define OPT_NONE    10
#define OPT_SYSTEM  20
#define OPT_TECH    30
#define OPT_MONEY   40
#define OPT_WAR     50
#define OPT_HELP    60
#define OPT_SHIP    70

r_ShipHeader*   PLANET_MyShipPtr;
r_PlanetHeader* PLANET_MyPlanetHeader;
ByteArr42*      ActPProjects;
//

void NEGATIVEANSWER()
{
    char    s[40];
    char*   _s;
    struct Window* ANS_Window;
    struct RastPort* RPort_PTR;

    ANS_Window = MAKEWINDOW(85,120,341,81,MyScreen[0]);
    if (NULL != ANS_Window)
    {
        RPort_PTR = ANS_Window->RPort;
        MAKEWINBORDER(RPort_PTR, 0, 0, 340, 80, 12, 6, 1);
        _s=my_strcpy(s, _PT_Die);
        *_s++ = ' ';
        _s=my_strcpy(_s, GETCIVADJ(ActPlayer));
        (void) my_strcpy(_s, PText[416]);
        WRITE(171, 20, ActPlayerFlag, WRITE_Center, RPort_PTR,3, s);
        WRITE(171, 45, ActPlayerFlag, WRITE_Center, RPort_PTR,3, PText[417]);
        WAITLOOP(false);
        CloseWindow(ANS_Window);
    }
}

void POSITIVEANSWER(uint8 CivVar)
{
    char    s[40];
    char*   _s;
    struct Window* ANS_Window;
    struct RastPort* RPort_PTR;

    ANS_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
    if (NULL != ANS_Window)
    {
        RPort_PTR = ANS_Window->RPort;
        MAKEWINBORDER(RPort_PTR,0,0,340,80,12,6,1);
        _s=my_strcpy(s, _PT_Die);
        *_s++ = ' ';
        _s=my_strcpy(_s, GETCIVADJ(ActPlayer));
        (void) my_strcpy(_s, PText[418]);
        WRITE(171, 20, ActPlayerFlag, WRITE_Center, RPort_PTR,3, s);
        WRITE(171, 45, ActPlayerFlag, WRITE_Center, RPort_PTR,3, PText[419]);
        WAITLOOP(false);
        CloseWindow(ANS_Window);
    }
    Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_PEACE;
    Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_PEACE;
}

uint8 GETOPTION(const int Opts, char (*OptArr)[40], uint8 CivFlag)
{
    sint8   Pos = 0, i = 0;
    uint16  ypos;
    struct Window* GOP_Window;
    struct RastPort* RPort_PTR;

    GOP_Window=MAKEWINDOW(50,100,411,141,MyScreen[0]);
    if (NULL != GOP_Window)
    {
        RPort_PTR = GOP_Window->RPort;
        MAKEWINBORDER(RPort_PTR,0,0,410,140,12,6,1);
        ypos = 10;
        for(i = 0; i < Opts; ++i)
        {
            WRITE(10, ypos,12,0,RPort_PTR,3,OptArr[i]);
            ypos += 20;
        }

        do
        {
            Delay(RDELAY);
            if ( (9           < GOP_Window->MouseY) &&
                ((11+Opts*20) > GOP_Window->MouseY))
            {
                i = (GOP_Window->MouseY-10) / 20;
                if ((i != Pos) && (0 <= i) && (i < Opts))
                {
                    Pos = i;
                    ypos = 10;
                    for(i = 0; i < Opts; ++i)
                    {
                        if (i == Pos)
                        {
                            WRITE(10, ypos,CivFlag,0,RPort_PTR,3,OptArr[i]);
                        } else {
                            WRITE(10, ypos,     12,0,RPort_PTR,3,OptArr[i]);
                        }
                        ypos += 20;
                    }
                }
            }
        }
        while ((LMB_NOTPRESSED) && (RMB_NOTPRESSED));
        ++Pos;
        if (RMB_PRESSED) { Pos = 0; }
        PLAYSOUND(0,300);
        CloseWindow(GOP_Window);
    }
    return ((uint8) Pos);
}

bool SMALLREQUEST(char* s, uint8 CivVar, uint8 CivFlag)
{
    char    s2[60];
    char*   _s;
    bool    _SMALLREQUEST = false;
    struct RastPort* RPort_PTR;

    struct Window* SRE_Window;

    SRE_Window=MAKEWINDOW(35,110,441,111,MyScreen[0]);
    if (NULL != SRE_Window)
    {
        RPort_PTR = SRE_Window->RPort;
        MAKEWINBORDER(RPort_PTR,0,0,440,110,12,6,1);
        _s=my_strcpy(s2, _PTx_Player);
        *_s++ = ' ';
        *_s++ = '0'+Save.CivPlayer[CivVar-1];
        *_s++ = ',';
        *_s++ = ' ';
        (void) my_strcpy(_s, PText[421]);
        WRITE(221,20,CivFlag,WRITE_Center,RPort_PTR,3,s2);
        WRITE(221,45,CivFlag,WRITE_Center,RPort_PTR,3,s);
        DrawImage(RPort_PTR,&GadImg1, 20,80);
        DrawImage(RPort_PTR,&GadImg1,302,80);
        WRITE( 80, 82, 8, WRITE_Center, RPort_PTR,3, _PT_Annehmen);
        WRITE(362, 82, 8, WRITE_Center, RPort_PTR,3, _PT_Ablehnen);
        do
        {
            Delay(RDELAY);
        }
        while ((LMB_NOTPRESSED)
            || (((SRE_Window->MouseX<20) || (SRE_Window->MouseX>148)) && ((SRE_Window->MouseX<302) || (SRE_Window->MouseX>430)))
             || ((SRE_Window->MouseY<80) || (SRE_Window->MouseY>100)));

        if (SRE_Window->MouseX<149)
        {
            KLICKWINGAD(RPort_PTR, 20,80);
            _SMALLREQUEST = true;
        } else {
            KLICKWINGAD(RPort_PTR,302,80);
            _SMALLREQUEST = false;
        }
        CloseWindow(SRE_Window);
    }
    return _SMALLREQUEST;
}

void CALLOTHERPLAYER(const int CivVar, const int CivFlag)
{
    PLAYERJINGLE(CivVar-1);
    MAKEBORDER(MyScreen[0],35,80,475,290,12,6,0);
    WRITE_RP0(256, 95,12,WRITE_Center,3,PText[422]);
    WRITE_RP0(256,125,12,WRITE_Center,3,"-");
    WRITE_RP0(160,125,ActPlayerFlag,WRITE_Center,3,GETCIVNAME(ActPlayer));
    WRITE_RP0(351,125,      CivFlag,WRITE_Center,3,GETCIVNAME(CivVar));
    DISPLAYLOGO(ActPlayer,96,150);
    DISPLAYLOGO(CivVar,287,150);
    WAITLOOP(false);
    RECT_RP0_C0(35,80,475,290);
    REFRESHDISPLAY();
}

void DIPLOMACY()
{
    uint8   XSystem, XTech, CivVar, CivFlag, Answer;
    int     Opts;
    uint32  XCosts;
    char    OptArr[8][40];
    uint8   OptID[8];
    char    s2[60];
    char*   _s;
    uint8   i;

    CivFlag = PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK;
    CivVar = GETCIVVAR(CivFlag);
    XSystem = 0;
    for(i = 0; i < Save.Systems; ++i)
    {
        if (((SystemFlags[CivVar-1][i] & FLAG_CIV_MASK) == CivFlag)
         && (SystemHeader[i].SysOwner != CivFlag))
        {
            XSystem = i+1;
        }
    }
    XTech = 0;
    for (i = 42; i > 0; --i)
    {
        if ((0 < Save.TechCosts[ActPlayer-1].data[i]) && (0 >= Save.TechCosts[CivVar-1].data[i]))
        {
            XTech = i;
        }
    }
    XCosts = abs(Year) * 43;
    if (XCosts > Save.Staatstopf[CivVar-1])
    {
        XCosts = Save.Staatstopf[CivVar-1];
    }
    Opts = 0;
    if (LEVEL_WAR == Save.WarState[ActPlayer-1][CivVar-1])
    {
        (void) my_strcpy(OptArr[Opts], PText[423]);           // Frieden ohne Bedingungen
        OptID[Opts++] = OPT_NONE;
        if (0 != XSystem)
        {
            (void) my_strcpy(OptArr[Opts], PText[424]);       // Frieden gegen Sonnensystem
            OptID[Opts++] = OPT_SYSTEM;
        }
        if (0 != XTech)
        {
            (void) my_strcpy(OptArr[Opts], PText[425]);       // Frieden gegen Technologie
            OptID[Opts++] = OPT_TECH;
        }
        (void) my_strcpy(OptArr[Opts], PText[426]);           // Frieden gegen Reparationen
        OptID[Opts++] = OPT_MONEY;
    } else
    {
        if (0 != XSystem)
        {
            (void) my_strcpy(OptArr[Opts], PText[427]);       // Sonnensystem für Frieden
            OptID[Opts++] = OPT_SYSTEM;
        }
        if (0 != XTech)
        {
            (void) my_strcpy(OptArr[Opts], PText[428]);       // Technologie für Frieden
            OptID[Opts++] = OPT_TECH;
        }
        (void) my_strcpy(OptArr[Opts], PText[429]);           // Tributzahlungen für Frieden
        OptID[Opts++] = OPT_MONEY;
        (void) my_strcpy(OptArr[Opts], PText[430]);           // Gemeinsamen Feldzug anbieten
        OptID[Opts++] = OPT_WAR;
        if (0 == Save.CivPlayer[CivVar-1])
        {
            (void) my_strcpy(OptArr[Opts], PText[431]);       // Entwicklungshilfe leisten
            OptID[Opts++] = OPT_HELP;
            if (Save.WarPower[CivVar-1] < Save.WarPower[ActPlayer-1])
            {
                (void) my_strcpy(OptArr[Opts], PText[432]);   // Schiff übergeben
                OptID[Opts++] = OPT_SHIP;
            }
        }
    }
    Answer = GETOPTION(Opts, OptArr, CivFlag);
    if (0 == Answer)
    {
        return;
    }
    --Answer;
    if ((CIVVAR_OTHER == CivVar) &&
        (WFLAG_JAHADR == Save.WorldFlag) &&
        (OPT_HELP != OptID[Answer]))
    {
        OptID[Answer] = OPT_NONE;
    }
    if ((0 != Save.CivPlayer[CivVar-1]) && (!Save.PlayMySelf))
    {
        if (OPT_WAR != OptID[Answer])
        {
            CALLOTHERPLAYER(CivVar, CivFlag);
        }
        switch(OptID[Answer])
        {
            case OPT_NONE:  {
                                if (SMALLREQUEST(PText[435], CivVar, CivFlag))
                                {
                                    POSITIVEANSWER(CivVar);
                                } else {
                                    NEGATIVEANSWER();
                                }
                            } break;
            case OPT_SYSTEM:{
                                _s=my_strcpy(s2, PText[436]); // "das System"
                                *_s++ = ' ';
                                _s=my_strcpy(_s, Save.SystemName.data[XSystem-1]);
                                *_s++ = ' ';
                                (void) my_strcpy(_s, PText[437]); // "zu übergeben"
                                SYSINFO(XSystem,CivFlag);
                                if (SMALLREQUEST(s2, CivVar, CivFlag))
                                {
                                    POSITIVEANSWER(CivVar);
                                    SYSTEMTOENEMY(XSystem-1,ActPlayerFlag,CivFlag);
                                } else {
                                    NEGATIVEANSWER();
                                }
                                RECT_RP0_C0(30,250,480,360);   // clear -Area
                            } break;
            case OPT_TECH:  {
                                _s=my_strcpy(s2, PText[438]);
                                *_s++ = ' ';
                                _s=my_strcpy(_s, TechnologyL.data[XTech]);
                                *_s++ = ' ';
                                (void) my_strcpy(_s, PText[437]); // "zu übergeben"
                                if (SMALLREQUEST(s2, CivVar, CivFlag))
                                {
                                    POSITIVEANSWER(CivVar);
                                    Save.TechCosts[ActPlayer-1].data[XTech] = 0;
                                    DISPLAYTECH(XTech);
                                } else {
                                    NEGATIVEANSWER();
                                }
                            } break;
            case OPT_MONEY: {
                                _s = dez2out(XCosts, 0, s2);
                                (void) my_strcpy(_s, PText[439]);
                                if (SMALLREQUEST(s2, CivVar, CivFlag))
                                {
                                    POSITIVEANSWER(CivVar);
                                    Save.Staatstopf[CivVar-1] -= XCosts;
                                    Save.Staatstopf[ActPlayer-1] += XCosts;
                                    PRINTGLOBALINFOS(ActPlayer-1);
                                } else {
                                    NEGATIVEANSWER();
                                }
                            } break;
            case OPT_WAR:   {
                                Opts = 0;
                                for(i = 1; i < MAXCIVS; ++i)
                                {
                                    if ((i != CivVar) && (i != ActPlayer)
                                        && (Save.WarState[ActPlayer-1][i-1] != LEVEL_DIED)
                                        && (Save.WarState[ActPlayer-1][i-1] != LEVEL_UNKNOWN)
                                        && ((i<8) || (Save.WorldFlag != 0)))
                                    {
                                        _s=my_strcpy(OptArr[Opts], PText[440]);
                                        *_s++ = ' ';
                                        (void) my_strcpy(_s, GETCIVNAME(i));
                                        OptID[Opts++] = i;
                                    }
                                }
                                if (0 != Opts)
                                {
                                    Answer = GETOPTION(Opts, OptArr, CivFlag);
                                    if (0 == Answer)
                                    {
                                        return;
                                    }
                                    --Answer;
                                    CALLOTHERPLAYER(CivVar, CivFlag);
                                    _s=my_strcpy(s2, PText[441]);
                                    *_s++ = ' ';
                                    _s=my_strcpy(_s, GETCIVNAME(OptID[Answer]));
                                    *_s++ = ' ';
                                    (void) my_strcpy(_s, PText[442]);
                                    if (SMALLREQUEST(s2, CivVar, CivFlag))
                                    {
                                        POSITIVEANSWER(CivVar);
                                        GOTOWAR(ActPlayerFlag,GETCIVFLAG(OptID[Answer]));
                                        GOTOWAR(CivFlag,GETCIVFLAG(OptID[Answer]));
                                        Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_ALLIANZ;
                                        Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_ALLIANZ;
                                    } else {
                                        NEGATIVEANSWER();
                                    }
                                } else {
                                    MAKEWINBORDER(MyRPort_PTR[0],85,120,425,200,12,6,0);
                                    WRITE_RP0(256,140,12,WRITE_Center,3,PText[445]);
                                    WRITE_RP0(256,160,12,WRITE_Center,3,PText[446]);
                                    WAITLOOP(false);
                                    RECT_RP0_C0(85,120,425,200);
                                    REFRESHDISPLAY();
                                }
                            } break;
            default:        { }
        }
        return;
    }
    switch (OptID[Answer])
    {
        case OPT_NONE:  {
                            VERHANDLUNG(CivFlag,MODE_FORCE);
                        } break;
        case OPT_SYSTEM:{
                            if (((Save.WarPower[CivVar-1]*4) > Save.WarPower[ActPlayer-1]) || (XSystem == 0))
                            {
                                NEGATIVEANSWER();
                            } else if (0 == (rand()%2))
                            {
                                POSITIVEANSWER(CivVar);
                                SYSTEMTOENEMY(XSystem-1,ActPlayerFlag,CivFlag);
                                if ((Save.WorldFlag == ActPlayerFlag) && (CivVar == 8))
                                {
                                    STOPCIVILWAR(0);
                                }
                            } else {
                                VERHANDLUNG(CivFlag,MODE_OFFENSIV);
                            }
                        } break;
        case OPT_TECH:  {
                            if (((Save.WarPower[CivVar-1]*3) > Save.WarPower[ActPlayer-1]) || (XTech == 0))
                            {
                                NEGATIVEANSWER();
                            } else if (0 != (rand()%3))
                            {
                                POSITIVEANSWER(CivVar);
                                Save.TechCosts[ActPlayer-1].data[XTech] = 0;
                                DISPLAYTECH(XTech);
                                if ((Save.WorldFlag == ActPlayerFlag) && (CivVar == 8))
                                {
                                    STOPCIVILWAR(0);
                                }
                            } else {
                                VERHANDLUNG(CivFlag,MODE_OFFENSIV);
                            }
                        } break;
        case OPT_MONEY: {
                            if (((Save.WarPower[CivVar-1]*2) > Save.WarPower[ActPlayer-1]) || (XCosts<10000))
                            {
                                NEGATIVEANSWER();
                            } else if (0 != (rand()%3))
                            {
                                Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_PEACE;
                                Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_PEACE;
                                Save.Staatstopf[CivVar-1]    -= XCosts;
                                Save.Staatstopf[ActPlayer-1] += XCosts;
                                MAKEWINBORDER(MyRPort_PTR[0],85,120,425,200,12,6,0);
                                WRITE_RP0(256,127,ActPlayerFlag,WRITE_Center,3,PText[447]); // Sie erhalten Reparationen
                                WRITE_RP0(256,147,ActPlayerFlag,WRITE_Center,3,PText[448]); // in Höhe von
                                (void) dez2out(XCosts, 0, s2);
                                WRITE_RP0(256,173,      CivFlag,WRITE_Center,3,s2);
                                WAITLOOP(false);
                                RECT_RP0_C0(85,120,425,200);
                                if ((Save.WorldFlag == ActPlayerFlag) && (CivVar == 8))
                                {
                                    STOPCIVILWAR(0);
                                }
                            } else {
                                VERHANDLUNG(CivFlag,MODE_OFFENSIV);
                            }
                        } break;
        case OPT_WAR:   {
                            Opts = 0;
                            for(i = 1; i < MAXCIVS; ++i)
                            {
                                if ((i!=CivVar) && (i!=ActPlayer)
                                    && (Save.WarState[ActPlayer-1][i-1]!=LEVEL_DIED) && (Save.WarState[ActPlayer-1][i-1]!=LEVEL_UNKNOWN)
                                    && ((i<8) || (Save.WorldFlag != 0)))
                                {
                                    _s=my_strcpy(OptArr[Opts], PText[440]);
                                    *_s++ = ' ';
                                    (void) my_strcpy(_s, GETCIVNAME(i));
                                    OptID[Opts++] = i;
                                }
                            }
                            if (Opts>0)
                            {
                                Answer = GETOPTION(Opts, OptArr, CivFlag);
                                if (Answer == 0)
                                {
                                    return;
                                }
                                --Answer;
                                if (((CivVar>=2) && (CivVar<=4) && (Save.WarPower[CivVar-1]>80))
                                  || ((Save.GlobalFlags[CivVar-1] == GFLAG_ATTACK) &&
                                        ((Save.WarPower[ActPlayer-1]+Save.WarPower[OptID[Answer]-1]) > Save.WarPower[CivVar-1]))
                                  || (Save.WarState[CivVar-1][OptID[Answer]-1] == LEVEL_WAR))
                                {
                                    POSITIVEANSWER(CivVar);
                                    GOTOWAR(ActPlayerFlag,GETCIVFLAG(OptID[Answer]));
                                    GOTOWAR(CivFlag,GETCIVFLAG(OptID[Answer]));
                                    if ((CivVar != 8) && (ActPlayer != 8))
                                    {
                                        Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_ALLIANZ;
                                        Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_ALLIANZ;
                                    }
                                } else {
                                    NEGATIVEANSWER();
                                }
                            } else {
                                MAKEWINBORDER(MyRPort_PTR[0],85,120,425,200,12,6,0);
                                WRITE_RP0(256,140,12,WRITE_Center,3,PText[445]);    // Keine weiteren
                                WRITE_RP0(256,160,12,WRITE_Center,3,PText[446]);    // Zivilisationen bekannt.
                                WAITLOOP(false);
                                RECT_RP0_C0(85,120,425,200);
                                REFRESHDISPLAY();
                            }
                        } break;
        case OPT_HELP:  {
                            i = rand()%3; // 0,1,2 ...
                            if (2 != i)
                            {
                                XTech = 0;
                                for (i = 40; i > 0; --i)
                                {
                                    if ((Save.TechCosts[ActPlayer-1].data[i] <= 0) && (Save.TechCosts[CivVar-1].data[i]>0))
                                    {
                                        XTech = i;
                                    }
                                }
                                if (0 == XTech)
                                {
                                    i = 2;
                                } else {
                                    i = 0;
                                    _s=my_strcpy(s2, TechnologyL.data[XTech]);
                                    (void) my_strcpy(_s, ".");
                                    REQUEST(PText[450],s2,ActPlayerFlag,ActPlayerFlag);
                                    Save.TechCosts[CivVar-1].data[XTech] = 0;
                                    Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_ALLIANZ;
                                    Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_ALLIANZ;
                                }
                            }
                            if (2 == i)
                            {
                                XCosts = abs(Year)*27;
                                if (XCosts > (Save.Staatstopf[ActPlayer-1] / 5))
                                {
                                    XCosts = Save.Staatstopf[ActPlayer-1] / 5;
                                }
                                if (0 == XCosts)
                                {
                                    i = 3;
                                } else {
                                    _s=my_strcpy(s2, PText[452]);
                                    *_s++ = ' ';
                                    _s = dez2out(XCosts, 0, _s);
                                    *_s++ = '.';
                                    *_s = 0;
                                    REQUEST(PText[451],s2,ActPlayerFlag,ActPlayerFlag);
                                    Save.Staatstopf[CivVar-1]    += XCosts;
                                    Save.Staatstopf[ActPlayer-1] -= XCosts;
                                    Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_ALLIANZ;
                                    Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_ALLIANZ;
                                    PRINTGLOBALINFOS(ActPlayer-1);
                                    i = 0;
                                }
                            }
                            if (3 == i)
                            {
                                REQUEST(PText[453],PText[454],ActPlayerFlag,ActPlayerFlag);
                            }
                            RECT_RP0_C0(85,120,425,200);
                            REFRESHDISPLAY();
                        } break;
        case OPT_SHIP:  {
                            if (PLANET_MyShipPtr->SType == SHIPTYPE_FLEET)
                            {
                                PLAYSOUND(0,600);
                            } else {
                                PLANET_MyShipPtr->Owner = CivFlag;
                                PLANET_MyShipPtr->Moving = 0;
                            }
                            RECT_RP0_C0(85,120,425,200);
                            REFRESHDISPLAY();
                            Delay(15);
                        } break;
        default:        { }
    }
}

void MAKELOADWINDOW()
{
    uint8   i;
    uint16  xpos;

    MAKEWINBORDER(MyRPort_PTR[0],42,148,469,273,12,6,0);
    BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(PLANET_MyShipPtr->SType-8)*32,32,MyRPort_PTR[0],59,163,32,32,192);
    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,PLANET_MyPlanetHeader->Class*32,0,MyRPort_PTR[0],59,227,32,32,192);
    xpos = 112;
    for(i = 0; i < 3; ++i)
    {
        MAKEWINBORDER(MyRPort_PTR[0], xpos, 163, xpos+100, 193, 12, 6, 1);
        xpos += 115;
    }

    if ((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
    {
        MAKEWINBORDER(MyRPort_PTR[0],112,228,212,258,12,6,1);
        MAKEWINBORDER(MyRPort_PTR[0],227,228,327,258,12,6,1);
        MAKEWINBORDER(MyRPort_PTR[0],342,228,442,258,12,6,1);
    } else
    {
        if (0 == PLANET_MyPlanetHeader->Population)
        {
            MAKEWINBORDER(MyRPort_PTR[0],112,228,212,258,12,6,1);
        } else  // if (0 < PLANET_MyPlanetHeader->Population) .... implicit .. Population is uint .. >=0 !
        {
            MAKEWINBORDER(MyRPort_PTR[0],227,228,327,258,12,6,1);
        }
        if ((((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == 0)
        || ((ActPProjects->data[34] == 0) && (ActPProjects->data[40] == 0))
        || (PLANET_MyPlanetHeader->ProjectPtr == NULL)) && (PLANET_MyPlanetHeader->Class != CLASS_STONES))
        {
            MAKEWINBORDER(MyRPort_PTR[0],342,228,442,258,12,6,1);
        }
    }

    WRITE_RP0(133,203,ActPlayerFlag,0,3,PText[456]);
    WRITE_RP0(232,203,ActPlayerFlag,0,3,PText[457]);
    WRITE_RP0(363,203,ActPlayerFlag,0,3,PText[458]);
}

void CLIPPLANET(uint8 Class)
{
    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,Class*32,0,MyRPort_PTR[0],59,227,32,32,192);
}

void WRITELOADDATA(int LTOut)
{
    // ActPProjects, ActPlayerFlag, PLANET_MyShipPtr, PLANET_MyPlanetHeader
    char    s[6];
    char*   _s;

    _s = dez2out(((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16), 2, s);
    *_s++ = ' ';
    (void) dez2out(ShipData(PLANET_MyShipPtr->SType).MaxLoad, 2, _s);
    WRITE_RP0(128,171,8,1,1,s);

    _s = dez2out((PLANET_MyShipPtr->Ladung & MASK_LTRUPPS), 2, s);
    *_s++ = ' ';
    (void) dez2out(ShipData(PLANET_MyShipPtr->SType).MaxLoad, 2, _s);
    WRITE_RP0(243,171,8,1,1,s);

    _s = dez2out(it_round((double) PLANET_MyShipPtr->Fracht/ShipData(PLANET_MyShipPtr->SType).MaxLoad*100.0), 3, s);
    *_s++ = '%'; *_s = 0;
    WRITE_RP0(364,171,8,1,1,s);

    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag) || (PLANET_MyPlanetHeader->Population == 0))
    {
        (void) dez2out(ActPProjects->data[26], 3, s);
        WRITE_RP0(141,236,8,1,1,s);
    }
    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag) || (PLANET_MyPlanetHeader->Population>0))
    {
        if ((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
        {
            (void) dez2out(ActPProjects->data[27], 3, s);
        } else if (PLANET_MyPlanetHeader->Population>0)
        {
            (void) dez2out(LTOut, 3, s);
        }
        WRITE_RP0(256,236,8,1,1,s);
    }
    if ((((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK)==ActPlayerFlag) || ((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK)==0)
        || ((ActPProjects->data[34] == 0) && (ActPProjects->data[40] == 0)) || (PLANET_MyPlanetHeader->ProjectPtr == NULL))
      && (PLANET_MyPlanetHeader->Class != CLASS_STONES))
    {
        _s = dez2out((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size), 3, s);
        *_s++ = '%'; *_s = 0;
        WRITE_RP0(353,236,8,1,1,s);
        _s = s;
        if ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)>80)
        {
            if (PLANET_MyPlanetHeader->Class == CLASS_ICE)
            {
                *_s++ = ' '; *_s++ = 'I'; *_s++ = ' '; *_s++ = ' ';
                CLIPPLANET(CLASS_ICE);
            } else {
                *_s++ = 'W';
                CLIPPLANET(CLASS_WATER);
            }
        } else if (((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)>=55) && ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<=80))
        {
            *_s++ = 'M';
            CLIPPLANET(CLASS_EARTH);
        } else if (((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)>=21) && ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<=54))
        {
            *_s++ = 'H'; *_s++ = ' ';
            CLIPPLANET(CLASS_HALFEARTH);
        } else if ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<21)
        {
            *_s++ = 'D'; *_s++ = ' ';
            CLIPPLANET(CLASS_DESERT);
        }
        *_s = 0;
        WRITE_RP0(419,235,ActPlayerFlag,1,3,s);
    }
}

bool PLANETHANDLING(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    uint8   GadSet[MAXGADGETS+1];
    uint8   GadCnt;
    bool    b, OldCiviPlanet;
    uint8   ShipsInOrbit;
    uint32  l;
    APTR    MemPtr;
    int     LTOut, SOut, FIn;
    r_ShipHeader*   XShipPtr;
    r_ShipHeader*   OtherShipPtr;
    char    s[60];
    uint8   CivVar;
    uint8   i;
    char*   _s;
    uint16  ypos;

    PLANET_MyShipPtr = MyShipPtr;
    PLANET_MyPlanetHeader = ObjPtr;
    CivVar = GETCIVVAR(PLANET_MyPlanetHeader->PFlags);
    if (CIVVAR_NONE == CivVar)
    {
        PLANET_MyPlanetHeader->Population = 0;
    }
    ActPProjects = PLANET_MyPlanetHeader->ProjectPtr;
    PLANET_MyShipPtr->PosX = MOVESHIP_FromX;
    PLANET_MyShipPtr->PosY = MOVESHIP_FromY;
    GadCnt = 1;
    for(i = 1; i <= MAXGADGETS; ++i)
    {
        GadSet[i] = 0;
    }

    ShipsInOrbit = 0;
    if (NULL != PLANET_MyPlanetHeader->FirstShip.NextShip)
    {
        XShipPtr = PLANET_MyPlanetHeader->FirstShip.NextShip;
        do
        {
            ShipsInOrbit = XShipPtr->Owner;
            XShipPtr = XShipPtr->NextShip;
        }
        while ((0 == ShipsInOrbit) && (NULL != XShipPtr));
    }
    l = PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK;
    if ((0 != l) && (ActPlayerFlag != l))
    {
        GadSet[1] = GADGET_DIPLOMAT;
        GadCnt = 2;
        if (0 != ShipsInOrbit)
        {
            GadSet[GadCnt++] = GADGET_ATTACK;
        }
    }
    else
    {
        if ((0 == ShipsInOrbit) || (ActPlayerFlag == ShipsInOrbit))
        {
            GadSet[GadCnt++] = GADGET_ORBIT;
        }
    }
    if (SHIPTYPE_FLEET != PLANET_MyShipPtr->SType)
    {
        if (((PLANET_MyPlanetHeader->Class==CLASS_DESERT) || (PLANET_MyPlanetHeader->Class==CLASS_HALFEARTH)
          || (PLANET_MyPlanetHeader->Class==CLASS_EARTH)  || (PLANET_MyPlanetHeader->Class==CLASS_ICE)
          || (PLANET_MyPlanetHeader->Class==CLASS_STONES) || (PLANET_MyPlanetHeader->Class==CLASS_WATER))
         && ( (((0 == ShipsInOrbit) || (ActPlayerFlag == ShipsInOrbit))
             && (0 == ActPProjects->data[PROJECT_SDI])
             && (0 == ActPProjects->data[PROJECT_SPACEPHALANX]))
            || (NULL == PLANET_MyPlanetHeader->ProjectPtr) || (ActPlayerFlag == l)))
        {
            GadSet[GadCnt++] = GADGET_LADEN;
        }
        if (((PLANET_MyPlanetHeader->Class==CLASS_DESERT) || (PLANET_MyPlanetHeader->Class==CLASS_HALFEARTH)
          || (PLANET_MyPlanetHeader->Class==CLASS_EARTH)  || (PLANET_MyPlanetHeader->Class==CLASS_ICE)
          || (PLANET_MyPlanetHeader->Class==CLASS_STONES) || (PLANET_MyPlanetHeader->Class==CLASS_WATER))
          && ((NULL == PLANET_MyPlanetHeader->ProjectPtr) || (0 != l)
             || (0 != ActPProjects->data[PROJECT_SDI])
             || (0 != ActPProjects->data[PROJECT_SPACEPHALANX]))
          && ((ActPlayerFlag == l) || (0 == ShipsInOrbit)))
        {
            GadSet[GadCnt++] = GADGET_ANGRIFF;
        }
        if ((ActPlayerFlag == l) && (0 == ShipsInOrbit)
         && (((0 == ActPProjects->data[PROJECT_SDI]) && (0 == ActPProjects->data[PROJECT_SPACEPHALANX]))
            || (NULL == PLANET_MyPlanetHeader->ProjectPtr)))
        {
            GadSet[GadCnt++] = GADGET_LANDUNG;
        }
    }
    GadCnt--;
    MAKEWINBORDER(MyRPort_PTR[0],194,119,316,122+GadCnt*22,12,6,1);
    ypos = 122;
    for(i = 1; i <= GadCnt; i++)
    {
        DrawImage(MyRPort_PTR[0],&GadImg1, 198, ypos);
        switch (GadSet[i]) {
            case GADGET_LADEN:    _s = PText[460]; break;
            case GADGET_ORBIT:    _s = PText[461]; break;
            case GADGET_LANDUNG:  _s = PText[462]; break;
            case GADGET_DIPLOMAT: _s = PText[463]; break;
            default:              _s = PText[464];
        }
        WRITE_RP0(256, ypos+3,0,WRITE_Center,3, _s);
        ypos += 22;
    }
    b = false;
    LTOut = 0;
    SOut = 0;
    FIn = 0;
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((MouseX(0) > 197) && (MouseX(0) < 316)
             && (MouseY(0) > 121) && (MouseY(0) < (100+(GadCnt+1)*22)))
            {
                l = (MouseY(0)-100) / 22;
                KLICKGAD(198,100+l*22);
                RECT_RP0_C0(194,119,316,122+GadCnt*22);
                REFRESHDISPLAY();
                switch (GadSet[l])
                {
                    case GADGET_ATTACK:
                        {
                            OtherShipPtr = PLANET_MyPlanetHeader->FirstShip.NextShip;
                            while ((OtherShipPtr != NULL) && (OtherShipPtr->Owner == 0))
                            {
                                OtherShipPtr = OtherShipPtr->NextShip;
                            }
                            if ((OtherShipPtr != NULL) && (OtherShipPtr->Owner != ActPlayerFlag))
                            {
                                PLANET_MyShipPtr->Moving = 0;
                                PLANET_MyShipPtr->PosX = MOVESHIP_FromX;
                                PLANET_MyShipPtr->PosY = MOVESHIP_FromY;
                                if (STARTBIGSHIPFIGHT(PLANET_MyShipPtr,OtherShipPtr,MODE_ALL,ActSys) == 1)
                                {
                                    AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,MODE_REFRESH);
                                    PLANET_MyShipPtr->Moving = 0;
                                    return false;
                                }
                                AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,MODE_REFRESH);
                                return false;
                            }
                        }   break;
                    case GADGET_ANGRIFF:
                        {
                            b = true;
                            STARLANDING(PLANET_MyPlanetHeader,PLANET_MyShipPtr,ActSys);
                            AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,MODE_REFRESH);
                        }   break;
                    case GADGET_ORBIT:
                        {
                            if (PLANET_MyShipPtr->SType == SHIPTYPE_FLEET)
                            {
                                XShipPtr = &PLANET_MyPlanetHeader->FirstShip;
                                while (XShipPtr->NextShip != NULL)
                                {
                                    XShipPtr = XShipPtr->NextShip;
                                }
                                XShipPtr->NextShip = PLANET_MyShipPtr->TargetShip;
                                PLANET_MyShipPtr->TargetShip->BeforeShip = XShipPtr;
                                PLANET_MyShipPtr->Owner = 0;
                            } else {
                                LINKSHIP(PLANET_MyShipPtr, &PLANET_MyPlanetHeader->FirstShip, 1);
                                DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
                            }
                            return false;
                        }   break;
                    case GADGET_DIPLOMAT:
                        {
                            DIPLOMACY();
                            b = true;
                        }   break;
                    default:
                        {
                        /*GADGET_LADEN,GADGET_LANDUNG*/
                            if (NULL == PLANET_MyPlanetHeader->ProjectPtr)
                            {
                                MemPtr = AllocMem(sizeof(ByteArr42), MEMF_CLEAR);
                                if (NULL == MemPtr)
                                {
                                    return false;
                                } else {
                                    PLANET_MyPlanetHeader->ProjectPtr = (ByteArr42*) MemPtr;
                                }
                                OldCiviPlanet = false;
                            } else {
                                OldCiviPlanet = true;
                            }
                            ActPProjects = PLANET_MyPlanetHeader->ProjectPtr;
                            ActPProjects->data[0] = 1;
                            b = true;
                            MAKELOADWINDOW();
                            WRITELOADDATA(LTOut);
                            do
                            {
                                Delay(RDELAY);
                                if (LMB_PRESSED)
                                {
                                    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                                        || (PLANET_MyPlanetHeader->Population == 0))
                                    {
                                        if ((MouseX(0)>=112) && (MouseX(0)<=212))
                                        {
                                            if ((MouseY(0)>=164) && (MouseY(0)<=194)
                                            && (0 < ActPProjects->data[PROJECT_SETTLERS])
                                            && (ShipData(PLANET_MyShipPtr->SType).MaxLoad > ((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16 + (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS)))
                                            && (15 > ((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16))
                                            && (15 >  (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS))
                                            && (0 == PLANET_MyShipPtr->Fracht))
                                            {
                                                PLAYSOUND(0,300);
                                                --(ActPProjects->data[PROJECT_SETTLERS]);
                                                PLANET_MyShipPtr->Ladung += 16;
                                                --SOut;
                                            }
                                            if ((MouseY(0)>=228) && (MouseY(0)<=258)
                                            && (250 > ActPProjects->data[PROJECT_SETTLERS])
                                            && (0 < (PLANET_MyShipPtr->Ladung & MASK_SIEDLER))
                                            && (0 == PLANET_MyShipPtr->Fracht))
                                            {
                                                PLAYSOUND(0,300);
                                                ++(ActPProjects->data[PROJECT_SETTLERS]);
                                                PLANET_MyShipPtr->Ladung -= 16;
                                                ++SOut;
                                            }
                                        }
                                    }
                                    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                                        || (PLANET_MyPlanetHeader->Population>0))
                                    {
                                        if ((MouseX(0)>=227) && (MouseX(0)<=327))
                                        {
                                            if ((MouseY(0)>=164) && (MouseY(0)<=194)
                                            && (0 < ActPProjects->data[PROJECT_LANDINGTROOPS])
                                            && (ShipData(PLANET_MyShipPtr->SType).MaxLoad>((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16 + (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS)))
                                            && (15 > ((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16))
                                            && (15 >  (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS))
                                            && (0 == PLANET_MyShipPtr->Fracht)
                                            && (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag) || (LTOut>0)))
                                            {
                                                PLAYSOUND(0,300);
                                                --(ActPProjects->data[PROJECT_LANDINGTROOPS]);
                                                ++(PLANET_MyShipPtr->Ladung);
                                                --LTOut;
                                            }
                                            if ((MouseY(0)>=228) && (MouseY(0)<=258)
                                            && (250 > ActPProjects->data[PROJECT_LANDINGTROOPS])
                                            && (0 < (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS))
                                            && (0 == PLANET_MyShipPtr->Fracht))
                                            {
                                                PLAYSOUND(0,300);
                                                ++(ActPProjects->data[PROJECT_LANDINGTROOPS]);
                                                --(PLANET_MyShipPtr->Ladung);
                                                ++LTOut;
                                            }
                                        }
                                    }
                                    if (((((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                                       ||((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == 0))
                                      || ((0 == ActPProjects->data[PROJECT_SDI]) && (0 == ActPProjects->data[PROJECT_SPACEPHALANX])))
                                     && (PLANET_MyPlanetHeader->Class != CLASS_STONES))
                                    {
                                        do
                                        {
                                            Delay(RDELAY);
                                            if ((MouseX(0)>=342) && (MouseX(0)<=442))
                                            {
                                                if ((MouseY(0)>=164) && (MouseY(0)<=194)
                                                && (PLANET_MyShipPtr->Ladung == 0)
                                                && (PLANET_MyShipPtr->Fracht < ShipData(PLANET_MyShipPtr->SType).MaxLoad)
                                                && (PLANET_MyPlanetHeader->Water>10))
                                                {
                                                    Delay(7);
                                                    PLANET_MyPlanetHeader->Water -= 5;
                                                    ++FIn;
                                                    ++(PLANET_MyShipPtr->Fracht);
                                                    WRITELOADDATA(LTOut);
                                                }
                                                if ((MouseY(0)>=228) && (MouseY(0)<=258)
                                                && (PLANET_MyShipPtr->Fracht>0)
                                                && ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<100))
                                                {
                                                    Delay(7);
                                                    PLANET_MyPlanetHeader->Water += 5;
                                                    --FIn;
                                                    --(PLANET_MyShipPtr->Fracht);
                                                    WRITELOADDATA(LTOut);
                                                }
                                            }
                                        }
                                        while (LMB_PRESSED);
                                    }
                                    WRITELOADDATA(LTOut);
                                }
                            }
                            while (RMB_NOTPRESSED);
                            PLAYSOUND(0,300);
                            RECT_RP0_C0(42,148,469,273);
                            REFRESHDISPLAY();
                            if ((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) != ActPlayerFlag)
                            {
                                if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) != 0) && (FIn>0))
                                {
                                    AUTOVERHANDLUNG(ActPlayerFlag, PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK, MODE_REFRESH);
                                }
                                if (0 < SOut)
                                {
                                    Save.ImperatorState[ActPlayer-1] += 25;
                                    PLANET_MyPlanetHeader->PFlags = ActPlayerFlag | FLAG_KNOWN;
                                    PLANET_MyPlanetHeader->Ethno = ActPlayerFlag;
                                    PLANET_MyPlanetHeader->Population += 10*SOut;
                                    ActPProjects->data[PROJECT_SETTLERS] = 0;
                                    if (!OldCiviPlanet)
                                    {
                                        if (PLANET_MyPlanetHeader->Class == CLASS_EARTH)
                                        {
                                            PLANET_MyPlanetHeader->Biosphaere = 200;
                                            PLANET_MyPlanetHeader->Infrastruktur = 1;
                                            PLANET_MyPlanetHeader->Industrie = 1;
                                        } else if ((PLANET_MyPlanetHeader->Class == CLASS_HALFEARTH)
                                                 ||(PLANET_MyPlanetHeader->Class == CLASS_WATER))
                                        {
                                            PLANET_MyPlanetHeader->Biosphaere = 90;
                                            PLANET_MyPlanetHeader->Infrastruktur = 1;
                                            PLANET_MyPlanetHeader->Industrie = 1;
                                        } else if (PLANET_MyPlanetHeader->Class == CLASS_DESERT)
                                        {
                                            PLANET_MyPlanetHeader->Biosphaere = 50;
                                            PLANET_MyPlanetHeader->Infrastruktur = 1;
                                            PLANET_MyPlanetHeader->Industrie = 1;
                                        } else {
                                            PLANET_MyPlanetHeader->Biosphaere = 30;
                                            PLANET_MyPlanetHeader->Infrastruktur = 1;
                                            PLANET_MyPlanetHeader->Industrie = 1;
                                        }
                                    }
                                    HANDLEKNOWNPLANET(ActSys,0,PLANET_MyPlanetHeader);
                                }
                                if (0 < LTOut)
                                {
                                    CHECKPLANET(PLANET_MyPlanetHeader);
                                    CHECKPROJECTS(PLANET_MyPlanetHeader,ActPlayerFlag);
                                    AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,MODE_REFRESH);
                                    REFRESHDISPLAY();
                                    (void) TAKETECH(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                                    if (PLANET_MyPlanetHeader->XProjectPayed>1000)
                                    {
                                        (void) dez2out((PLANET_MyPlanetHeader->XProjectPayed / 5), 0, s);
                                        REQUEST(PText[466],s,ActPlayerFlag,12);
                                        Save.Staatstopf[ActPlayer-1] += (PLANET_MyPlanetHeader->XProjectPayed / 5);
                                        PLANET_MyPlanetHeader->XProjectPayed = it_round(PLANET_MyPlanetHeader->XProjectPayed * 0.8);
                                    }
                                    PLANET_MyPlanetHeader->PFlags = ActPlayerFlag;
                                    if (PLANET_MyPlanetHeader->Population < (PLANET_MyPlanetHeader->Size*13))
                                    {
                                        PLANET_MyPlanetHeader->Ethno = ActPlayerFlag;
                                    }
                                    HANDLEKNOWNPLANET(ActSys,0,PLANET_MyPlanetHeader);
                                }
                            }
                        }
                }
            }
        }
    }
    while ((!b) && (RMB_NOTPRESSED));
    if (RMB_PRESSED) { PLAYSOUND(0,300); }
    RECT_RP0_C0(194,119,318,280);
    REFRESHDISPLAY();

    return true;
}
