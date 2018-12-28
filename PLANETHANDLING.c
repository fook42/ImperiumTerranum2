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
    char    s[80];
    struct Window* ANS_Window;

    ANS_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
    if (NULL != ANS_Window)
    {
        MAKEWINBORDER(ANS_Window->RPort,0,0,340,80,12,6,1);
        strcpy(s, _PT_Die);
        strcat(s, " ");
        strcat(s, GETCIVADJ(ActPlayer));
        strcat(s, PText[416]);
        WRITEWIN(171,20,ActPlayerFlag,WRITE_Center,ANS_Window->RPort,4,s);
        WRITEWIN(171,45,ActPlayerFlag,WRITE_Center,ANS_Window->RPort,4,PText[417]);
        WAITLOOP(false);
        CloseWindow(ANS_Window);
    }
}

void POSITIVEANSWER(uint8 CivVar)
{
    char    s[80];
    struct Window* ANS_Window;

    ANS_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
    if (NULL != ANS_Window)
    {
        MAKEWINBORDER(ANS_Window->RPort,0,0,340,80,12,6,1);
        strcpy(s, _PT_Die);
        strcat(s, " ");
        strcat(s, GETCIVADJ(ActPlayer));
        strcat(s, PText[418]);
        WRITEWIN(171,20,ActPlayerFlag,WRITE_Center,ANS_Window->RPort,4,s);
        WRITEWIN(171,45,ActPlayerFlag,WRITE_Center,ANS_Window->RPort,4,PText[419]);
        WAITLOOP(false);
        CloseWindow(ANS_Window);
    }
    Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_PEACE;
    Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_PEACE;
}

uint8 GETOPTION(uint8 Opts, char (*OptArr)[40], uint8 CivFlag)
{
    uint8 Pos = 0, i = 0;
    struct Window* GOP_Window;

    GOP_Window=MAKEWINDOW(50,100,411,141,MyScreen[0]);
    if (NULL != GOP_Window)
    {
        MAKEWINBORDER(GOP_Window->RPort,0,0,410,140,12,6,1);
        for(i = 0; i < Opts; i++)
        {
            WRITEWIN(10,i*20+10,12,0,GOP_Window->RPort,4,OptArr[i]);
        }

        do
        {
            delay(RDELAY);
            if ( (10          <= GOP_Window->MouseY) &&
                ((10+Opts*20) >= GOP_Window->MouseY))
            {
                i = (GOP_Window->MouseY-10) / 20;
                if ((i != Pos) && (0 <= i) && (i < Opts))
                {
                    Pos = i;
                    for(i = 0; i < Opts; i++)
                    {
                        if (i == Pos)
                        {
                            WRITEWIN(10,i*20+10,CivFlag,0,GOP_Window->RPort,4,OptArr[i]);
                        } else {
                            WRITEWIN(10,i*20+10,     12,0,GOP_Window->RPort,4,OptArr[i]);
                        }
                    }
                }
            }
        }
        while ((LMB_NOTPRESSED) && (RMB_NOTPRESSED));

        if (RMB_PRESSED) { Pos = 0; } else { Pos++; }
        PLAYSOUND(1,300);
        CloseWindow(GOP_Window);
    }
    return Pos;
}

bool SMALLREQUEST(char* s, uint8 CivVar, uint8 CivFlag)
{
    char    s2[60];
    int     slen;
    bool    _SMALLREQUEST;

    MAKEBORDER(MyScreen[0],35,110,475,220,12,6,0);
    strcpy(s2, "Player ");
    slen=strlen(s2);
    s2[slen++]='0'+Save.CivPlayer[CivVar-1];
    s2[slen++]=',';
    s2[slen++]=' ';
    strcpy(s2+slen, PText[421]);
    WRITE(256,130,CivFlag,WRITE_Center,MyScreen[0],4,s2);
    WRITE(256,155,CivFlag,WRITE_Center,MyScreen[0],4,s);
    DrawImage(&(MyScreen[0]->RastPort),&GadImg1,55,190);
    DrawImage(&(MyScreen[0]->RastPort),&GadImg1,337,190);
    WRITE(115,192,8,WRITE_Center,MyScreen[0],4,PText[245]);
    WRITE(397,192,8,WRITE_Center,MyScreen[0],4,PText[246]);
    do
    {
        delay(RDELAY);
    }
    while ((LMB_NOTPRESSED)
        || (((MouseX(0)<55) || (MouseX(0)>183))
            && ((MouseX(0)<337) || (MouseX(0)>465)))
        || ((MouseY(0)<190) || (MouseY(0)>210)));

    if ((MouseX(0)>=55) && (MouseX(0)<=183))
    {
        KLICKGAD(55,190);
        _SMALLREQUEST = true;
    } else {
        KLICKGAD(337,190);
        _SMALLREQUEST = false;
    }
    RECT(MyScreen[0],0,35,110,475,220);
    REFRESHDISPLAY();
    return _SMALLREQUEST;
}

void CALLOTHERPLAYER(uint8 CivVar, uint8 CivFlag)
{
    PLAYERJINGLE(CivVar);
    MAKEBORDER(MyScreen[0],35,80,475,290,12,6,0);
    WRITE(256, 95,12,WRITE_Center,MyScreen[0],4,PText[422]);
    WRITE(256,125,12,WRITE_Center,MyScreen[0],4,"-");
    WRITE(160,125,ActPlayerFlag,WRITE_Center,MyScreen[0],4,GETCIVNAME(ActPlayer));
    WRITE(351,125,      CivFlag,WRITE_Center,MyScreen[0],4,GETCIVNAME(CivVar));
    DISPLAYLOGO(ActPlayer,96,150);
    DISPLAYLOGO(CivVar,287,150);
    WAITLOOP(false);
    RECT(MyScreen[0],0,35,80,475,290);
    REFRESHDISPLAY();
}

void DIPLOMACY()
{
    uint8   XSystem, XTech, CivVar, CivFlag, Opts;
    uint32  XCosts, l;
    char    OptArr[8][40];
    uint8   OptID[8];
    char    s2[60];
    char*   _s;
    uint8   i;

    CivFlag = PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK;
    CivVar = GETCIVVAR(CivFlag);
    XSystem = 0;
    for(i = 0; i < Save.Systems; i++)
    {
        if (((SystemFlags[CivVar-1][i] & FLAG_CIV_MASK) == CivFlag)
         && (SystemHeader[i].SysOwner != CivFlag))
        {
            XSystem = i+1;
        }
    }
    XTech = 0;
    for (i = 42; i>=1; i--)
    {
        if ((Save.TechCosts[ActPlayer-1].data[i]>0) && (Save.TechCosts[CivVar-1].data[i] <= 0))
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
        strcpy(OptArr[Opts], PText[423]);
        OptID[Opts++] = OPT_NONE;
        if (0 < XSystem)
        {
            strcpy(OptArr[Opts], PText[424]);
            OptID[Opts++] = OPT_SYSTEM;
        }
        if (0 < XTech)
        {
            strcpy(OptArr[Opts], PText[425]);
            OptID[Opts++] = OPT_TECH;
        }
        strcpy(OptArr[Opts], PText[426]);
        OptID[Opts++] = OPT_MONEY;
    } else
    {
        if (0 < XSystem)
        {
            strcpy(OptArr[Opts], PText[427]);
            OptID[Opts++] = OPT_SYSTEM;
        }
        if (0 < XTech)
        {
            strcpy(OptArr[Opts], PText[428]);
            OptID[Opts++] = OPT_TECH;
        }
        strcpy(OptArr[Opts], PText[429]);
        OptID[Opts++] = OPT_MONEY;
        strcpy(OptArr[Opts], PText[430]);
        OptID[Opts++] = OPT_WAR;
        if (0 == Save.CivPlayer[CivVar-1])
        {
            strcpy(OptArr[Opts], PText[431]);
            OptID[Opts++] = OPT_HELP;
            if (Save.WarPower[CivVar-1] < Save.WarPower[ActPlayer-1])
            {
                strcpy(OptArr[Opts], PText[432]);
                OptID[Opts++] = OPT_SHIP;
            }
        }
    }
    l = GETOPTION(Opts, OptArr, CivFlag);
    if (0 == l) { return; }
    if ((8 == CivVar) &&
        (WFLAG_JAHADR == Save.WorldFlag) &&
        (OPT_HELP != OptID[l]))
    {
        OptID[l] = OPT_NONE;
    }
    if ((Save.CivPlayer[CivVar-1] != 0) && (!Save.PlayMySelf))
    {
        if (OptID[l] != OPT_WAR)
        {
            CALLOTHERPLAYER(CivVar, CivFlag);
        }
        switch(OptID[l])
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
                                strcpy(s2, PText[436]);
                                strcat(s2, " ");
                                strcat(s2, Save.SystemName.data[XSystem-1]);
                                strcat(s2, " ");
                                strcat(s2, PText[437]);
                                SYSINFO(XSystem,CivFlag);
                                if (!SMALLREQUEST(s2, CivVar, CivFlag))
                                {
                                    NEGATIVEANSWER();
                                } else {
                                    POSITIVEANSWER(CivVar);
                                    SYSTEMTOENEMY(XSystem-1,ActPlayerFlag,CivFlag);
                                }
                                RECT(MyScreen[0],0,30,250,480,360);
                            } break;
            case OPT_TECH:  {
                                strcpy(s2, PText[438]);
                                strcat(s2, " ");
                                strcat(s2, TechnologyL.data[XTech]);
                                strcat(s2, " ");
                                strcat(s2, PText[437]);
                                if (!SMALLREQUEST(s2, CivVar, CivFlag))
                                {
                                    NEGATIVEANSWER();
                                } else {
                                    POSITIVEANSWER(CivVar);
                                    Save.TechCosts[ActPlayer-1].data[XTech] = 0;
                                    DISPLAYTECH(XTech);
                                }
                            } break;
            case OPT_MONEY: {
                                _s = dez2out(XCosts, 0, s2);
                                strcpy(_s, PText[439]);
                                if (!SMALLREQUEST(s2, CivVar, CivFlag))
                                {
                                    NEGATIVEANSWER();
                                } else {
                                    POSITIVEANSWER(CivVar);
                                    Save.Staatstopf[CivVar-1] -= XCosts;
                                    Save.Staatstopf[ActPlayer-1] += XCosts;
                                    PRINTGLOBALINFOS(ActPlayer-1);
                                }
                            } break;
            case OPT_WAR:   {
                                Opts = 0;
                                for(i = 1; i < MAXCIVS; i++)
                                {
                                    if ((i!=CivVar) && (i!=ActPlayer)
                                        && (Save.WarState[ActPlayer-1][i-1]!=LEVEL_DIED) && (Save.WarState[ActPlayer-1][i-1]!=LEVEL_UNKNOWN)
                                        && ((i<8) || (Save.WorldFlag != 0)))
                                    {
                                        strcpy(OptArr[Opts], PText[440]);
                                        strcat(OptArr[Opts], " ");
                                        strcat(OptArr[Opts], GETCIVNAME(i));
                                        OptID[Opts++] = i;
                                    }
                                }
                                if (Opts>0)
                                {
                                    l = GETOPTION(Opts, OptArr, CivFlag);
                                    if (l == 0) { return; }
                                    CALLOTHERPLAYER(CivVar, CivFlag);
                                    strcpy(s2, PText[441]);
                                    strcat(s2, " ");
                                    strcat(s2, GETCIVNAME(OptID[l]));
                                    strcat(s2, " ");
                                    strcat(s2, PText[442]);
                                    if (!SMALLREQUEST(s2, CivVar, CivFlag))
                                    {
                                        NEGATIVEANSWER();
                                    } else {
                                        POSITIVEANSWER(CivVar);
                                        GOTOWAR(ActPlayerFlag,GETCIVFLAG(OptID[l]));
                                        GOTOWAR(CivFlag,GETCIVFLAG(OptID[l]));
                                        Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_ALLIANZ;
                                        Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_ALLIANZ;
                                    }
                                } else {
                                    MAKEBORDER(MyScreen[0],85,120,425,200,12,6,0);
                                    WRITE(256,140,12,WRITE_Center,MyScreen[0],4,PText[445]);
                                    WRITE(256,160,12,WRITE_Center,MyScreen[0],4,PText[446]);
                                    WAITLOOP(false);
                                    RECT(MyScreen[0],0,85,120,425,200);
                                    REFRESHDISPLAY();
                                }
                            } break;
            default:        { }
        }
        return;
    }
    switch (OptID[l])
    {
        case OPT_NONE:  {
                            VERHANDLUNG(CivFlag,MODE_FORCE);
                        } break;
        case OPT_SYSTEM:{
                            if (((Save.WarPower[CivVar-1]*4) > Save.WarPower[ActPlayer-1]) || (XSystem == 0))
                            {
                                NEGATIVEANSWER();
                            } else if ((rand()%2) == 0)
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
                            } else if ((rand()%3) != 0)
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
                            } else if ((rand()%3) != 0)
                            {
                                Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_PEACE;
                                Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_PEACE;
                                Save.Staatstopf[CivVar-1]    -= XCosts;
                                Save.Staatstopf[ActPlayer-1] += XCosts;
                                MAKEBORDER(MyScreen[0],85,120,425,200,12,6,0);
                                WRITE(256,127,ActPlayerFlag,WRITE_Center,MyScreen[0],4,PText[447]);
                                WRITE(256,147,ActPlayerFlag,WRITE_Center,MyScreen[0],4,PText[448]);
                                (void) dez2out(XCosts, 0, s2);
                                WRITE(256,173,      CivFlag,WRITE_Center,MyScreen[0],4,s2);
                                WAITLOOP(false);
                                RECT(MyScreen[0],0,85,120,425,200);
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
                            for(i = 1; i < MAXCIVS; i++)
                            {
                                if ((i!=CivVar) && (i!=ActPlayer)
                                    && (Save.WarState[ActPlayer-1][i-1]!=LEVEL_DIED) && (Save.WarState[ActPlayer-1][i-1]!=LEVEL_UNKNOWN)
                                    && ((i<8) || (Save.WorldFlag != 0)))
                                {
                                    strcpy(OptArr[Opts], PText[440]);
                                    strcat(OptArr[Opts], " ");
                                    strcat(OptArr[Opts], GETCIVNAME(i));
                                    OptID[Opts++] = i;
                                }
                            }
                            if (Opts>0)
                            {
                                l = GETOPTION(Opts, OptArr, CivFlag);
                                if (l == 0) { return; }
                                if (((CivVar>=2) && (CivVar<=4) && (Save.WarPower[CivVar-1]>80))
                                  || ((Save.GlobalFlags[CivVar-1] == GFLAG_ATTACK) &&
                                        ((Save.WarPower[ActPlayer-1]+Save.WarPower[OptID[l]-1]) > Save.WarPower[CivVar-1]))
                                  || (Save.WarState[CivVar-1][OptID[l]-1] == LEVEL_WAR))
                                {
                                    POSITIVEANSWER(CivVar);
                                    GOTOWAR(ActPlayerFlag,GETCIVFLAG(OptID[l]));
                                    GOTOWAR(CivFlag,GETCIVFLAG(OptID[l]));
                                    if ((CivVar != 8) && (ActPlayer != 8))
                                    {
                                        Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_ALLIANZ;
                                        Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_ALLIANZ;
                                    }
                                } else {
                                    NEGATIVEANSWER();
                                }
                            } else {
                                MAKEBORDER(MyScreen[0],85,120,425,200,12,6,0);
                                WRITE(256,140,12,WRITE_Center,MyScreen[0],4,PText[445]);
                                WRITE(256,160,12,WRITE_Center,MyScreen[0],4,PText[446]);
                                WAITLOOP(false);
                                RECT(MyScreen[0],0,85,120,425,200);
                                REFRESHDISPLAY();
                            }
                        } break;
        case OPT_HELP:  {
                            i = rand()%3;
                            if ((0 == i) || (1 == i))
                            {
                                XTech = 0;
                                for (i = 40; i>=1; i--)
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
                                    strcpy(s2, TechnologyL.data[XTech]);
                                    strcat(s2, ".");
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
                                    i = strlen(PText[452]);
                                    memcpy(s2, PText[452], i);
                                    s2[i++]=' ';
                                    _s = dez2out(XCosts, 0, s2+i);
                                    *_s++='.';
                                    *_s  =0;
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
                            RECT(MyScreen[0],0,85,120,425,200);
                            REFRESHDISPLAY();
                        } break;
        case OPT_SHIP:  {
                            if (PLANET_MyShipPtr->SType == SHIPTYPE_FLEET)
                            {
                                PLAYSOUND(1,600);
                            } else {
                                PLANET_MyShipPtr->Owner = CivFlag;
                                PLANET_MyShipPtr->Moving = 0;
                            }
                            RECT(MyScreen[0],0,85,120,425,200);
                            REFRESHDISPLAY();
                            delay(15);
                        } break;
        default:        { }
    }
}

void MAKELOADWINDOW()
{
    uint8   i;

    MAKEBORDER(MyScreen[0],42,148,469,273,12,6,0);
    BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(PLANET_MyShipPtr->SType-8)*32,32,&(MyScreen[0]->RastPort),59,163,32,32,192);
    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,PLANET_MyPlanetHeader->Class*32,0,&(MyScreen[0]->RastPort),59,227,32,32,192);
    for(i = 1; i <= 3; i++)
    {
        MAKEBORDER(MyScreen[0],i*115-3,163,i*115+97,193,12,6,1);
    }

    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag) || (PLANET_MyPlanetHeader->Population == 0))
    {
        MAKEBORDER(MyScreen[0],112,228,212,258,12,6,1);
    }
    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag) || (PLANET_MyPlanetHeader->Population>0))
    {
        MAKEBORDER(MyScreen[0],227,228,327,258,12,6,1);
    }
    if ((((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
     || ((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == 0)
     || ((ActPProjects->data[34] == 0) && (ActPProjects->data[40] == 0))
     || (PLANET_MyPlanetHeader->ProjectPtr == NULL)) && (PLANET_MyPlanetHeader->Class != CLASS_STONES))
    {
        MAKEBORDER(MyScreen[0],342,228,442,258,12,6,1);
    }

    WRITE(133,203,ActPlayerFlag,0,MyScreen[0],4,PText[456]);
    WRITE(232,203,ActPlayerFlag,0,MyScreen[0],4,PText[457]);
    WRITE(363,203,ActPlayerFlag,0,MyScreen[0],4,PText[458]);
}

void CLIPPLANET(uint8 Class)
{
    BltBitMapRastPort((struct BitMap*) &ImgBitMap7,Class*32,0,&(MyScreen[0]->RastPort),59,227,32,32,192);
}

void WRITELOADDATA(int LTOut)
{
    // ActPProjects, ActPlayerFlag, PLANET_MyShipPtr, PLANET_MyPlanetHeader
    char    s[6];
    char*   _s;

    _s = dez2out(((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16), 2, s);
    *_s++=' ';
    (void) dez2out(ShipData(PLANET_MyShipPtr->SType).MaxLoad, 2, _s);
    WRITE(128,171,8,1,MyScreen[0],2,s);

    _s = dez2out((PLANET_MyShipPtr->Ladung & MASK_LTRUPPS), 2, s);
    *_s++=' ';
    (void) dez2out(ShipData(PLANET_MyShipPtr->SType).MaxLoad, 2, _s);
    WRITE(243,171,8,1,MyScreen[0],2,s);

    _s = dez2out(it_round((double) PLANET_MyShipPtr->Fracht/ShipData(PLANET_MyShipPtr->SType).MaxLoad*100.0), 3, s);
    *_s++='%'; *_s = 0;
    WRITE(364,171,8,1,MyScreen[0],2,s);

    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag) || (PLANET_MyPlanetHeader->Population == 0))
    {
        (void) dez2out(ActPProjects->data[26], 3, s);
        WRITE(141,236,8,1,MyScreen[0],2,s);
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
        WRITE(256,236,8,1,MyScreen[0],2,s);
    }
    if ((((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK)==ActPlayerFlag) || ((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK)==0)
        || ((ActPProjects->data[34] == 0) && (ActPProjects->data[40] == 0)) || (PLANET_MyPlanetHeader->ProjectPtr == NULL))
      && (PLANET_MyPlanetHeader->Class != CLASS_STONES))
    {
        _s = dez2out((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size), 3, s);
        *_s++='%'; *_s = 0;
        WRITE(353,236,8,1,MyScreen[0],2,s);
        _s = s;
        if ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)>80)
        {
            if (PLANET_MyPlanetHeader->Class == CLASS_ICE)
            {
                *_s++=' '; *_s++='I'; *_s++=' '; *_s++=' ';
                CLIPPLANET(CLASS_ICE);
            } else {
                *_s++='W';
                CLIPPLANET(CLASS_WATER);
            }
        } else if (((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)>=55) && ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<=80))
        {
            *_s++='M';
            CLIPPLANET(CLASS_EARTH);
        } else if (((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)>=21) && ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<=54))
        {
            *_s++='H'; *_s++=' ';
            CLIPPLANET(CLASS_HALFEARTH);
        } else if ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<21)
        {
            *_s++='D'; *_s++=' ';
            CLIPPLANET(CLASS_DESERT);
        }
        *_s++=0;
        WRITE(419,235,ActPlayerFlag,1,MyScreen[0],4,s);
    }
}

bool PLANETHANDLING(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    uint8   GadSet[MAXGADGETS+1];
    uint8   GadCnt;
    bool    b, OldCiviPlanet;
    uint8   ShipsInOrbit;
    uint32  l;
    int     LTOut, SOut, FIn;
    r_ShipHeader*   XShipPtr;
    r_ShipHeader*   OtherShipPtr;
    char    s[60];
    uint8   CivVar;
    uint8   i;

    PLANET_MyShipPtr = MyShipPtr;
    PLANET_MyPlanetHeader = ObjPtr;
    CivVar = GETCIVVAR(PLANET_MyPlanetHeader->PFlags);
    if (CivVar == 0)
    {
        PLANET_MyPlanetHeader->Population = 0;
    }
    ActPProjects = PLANET_MyPlanetHeader->ProjectPtr;
    PLANET_MyShipPtr->PosX = MOVESHIP_FromX;
    PLANET_MyShipPtr->PosY = MOVESHIP_FromY;
    GadCnt = 1;
    for(i = 1; i <= MAXGADGETS; i++)
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
         && ( (((0 == ShipsInOrbit) || (ActPlayerFlag == ShipsInOrbit)) && (0 == ActPProjects->data[34]) && (0 == ActPProjects->data[40]))
          || (NULL == PLANET_MyPlanetHeader->ProjectPtr) || (ActPlayerFlag == l)))
        {
            GadSet[GadCnt++] = GADGET_LADEN;
        }
        if (((PLANET_MyPlanetHeader->Class==CLASS_DESERT) || (PLANET_MyPlanetHeader->Class==CLASS_HALFEARTH)
          || (PLANET_MyPlanetHeader->Class==CLASS_EARTH)  || (PLANET_MyPlanetHeader->Class==CLASS_ICE)
          || (PLANET_MyPlanetHeader->Class==CLASS_STONES) || (PLANET_MyPlanetHeader->Class==CLASS_WATER))
          && ((NULL == PLANET_MyPlanetHeader->ProjectPtr) || (0 != l) || (0 != ActPProjects->data[34]) || (0 != ActPProjects->data[40]))
          && ((ActPlayerFlag == l) || (0 == ShipsInOrbit)))
        {
            GadSet[GadCnt++] = GADGET_ANGRIFF;
        }
        if ((ActPlayerFlag == l) && (0 == ShipsInOrbit)
         && (((0 == ActPProjects->data[34]) && (0 == ActPProjects->data[40]))
            || (NULL == PLANET_MyPlanetHeader->ProjectPtr)))
        {
            GadSet[GadCnt++] = GADGET_LANDUNG;
        }
    }
    GadCnt--;
    MAKEBORDER(MyScreen[0],194,119,316,122+GadCnt*22,12,6,1);
    for(i = 1; i <= GadCnt; i++)
    {
        DrawImage(&(MyScreen[0]->RastPort),&GadImg1,198,100+i*22);
        switch (GadSet[i]) {
            case GADGET_LADEN:    WRITE(256,103+i*22,0,WRITE_Center,MyScreen[0],4,PText[460]); break;
            case GADGET_ORBIT:    WRITE(256,103+i*22,0,WRITE_Center,MyScreen[0],4,PText[461]); break;
            case GADGET_LANDUNG:  WRITE(256,103+i*22,0,WRITE_Center,MyScreen[0],4,PText[462]); break;
            case GADGET_DIPLOMAT: WRITE(256,103+i*22,0,WRITE_Center,MyScreen[0],4,PText[463]); break;
            default:              WRITE(256,102+i*22,0,WRITE_Center,MyScreen[0],4,PText[464]);
        }
    }
    b = false;
    LTOut = 0;
    SOut = 0;
    FIn = 0;
    do
    {
        delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((MouseX(0) >=198) && (MouseX(0)<=315)
             && (MouseY(0) >=122) && (MouseY(0)<(100+(GadCnt+1)*22)))
            {
                l = (MouseY(0)-100) / 22;
                KLICKGAD(198,100+l*22);
                RECT(MyScreen[0],0,194,119,316,122+GadCnt*22);
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
                                    AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,ActSys,0);
                                    PLANET_MyShipPtr->Moving = 0;
                                    return false;
                                }
                                AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,ActSys,0);
                                return false;
                            }
                        }   break;
                    case GADGET_ANGRIFF:
                        {
                            b = true;
                            STARLANDING(PLANET_MyPlanetHeader,PLANET_MyShipPtr,ActSys);
                            AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,ActSys,0);
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
                            if (PLANET_MyPlanetHeader->ProjectPtr == NULL)
                            {
                                l = (uint32) AllocMem(sizeof(ByteArr42),MEMF_CLEAR);
                                if (l == 0)
                                {
                                    return false;
                                } else {
                                    PLANET_MyPlanetHeader->ProjectPtr = (ByteArr42*) l;
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
                                delay(RDELAY);
                                if (LMB_PRESSED)
                                {
                                    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                                        || (PLANET_MyPlanetHeader->Population == 0))
                                    {
                                        if ((MouseX(0)>=112) && (MouseX(0)<=212)
                                         && (MouseY(0)>=164) && (MouseY(0)<=194)
                                         && (ActPProjects->data[26]>0)
                                         && (ShipData(PLANET_MyShipPtr->SType).MaxLoad > ((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16 + (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS)))
                                         && (15 > ((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16))
                                         && (15 >  (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS))
                                         && (PLANET_MyShipPtr->Fracht == 0))
                                        {
                                            PLAYSOUND(1,300);
                                            ActPProjects->data[26]--;
                                            PLANET_MyShipPtr->Ladung += 16;
                                            SOut--;
                                        }
                                        if ((MouseX(0)>=112) && (MouseX(0)<=212)
                                         && (MouseY(0)>=228) && (MouseY(0)<=258)
                                         && (ActPProjects->data[26]<250)
                                         && ((PLANET_MyShipPtr->Ladung & MASK_SIEDLER)>0)
                                         && (PLANET_MyShipPtr->Fracht == 0))
                                        {
                                            PLAYSOUND(1,300);
                                            ActPProjects->data[26]++;
                                            PLANET_MyShipPtr->Ladung -= 16;
                                            SOut++;
                                        }
                                    }
                                    if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                                        || (PLANET_MyPlanetHeader->Population>0))
                                    {
                                        if ((MouseX(0)>=227) && (MouseX(0)<=327)
                                         && (MouseY(0)>=164) && (MouseY(0)<=194)
                                         && (ActPProjects->data[27]>0)
                                         && (ShipData(PLANET_MyShipPtr->SType).MaxLoad>((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16 + (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS)))
                                         && (15 > ((PLANET_MyShipPtr->Ladung & MASK_SIEDLER) / 16))
                                         && (15 >  (PLANET_MyShipPtr->Ladung & MASK_LTRUPPS))
                                         && (PLANET_MyShipPtr->Fracht == 0)
                                         && (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag) || (LTOut>0)))
                                        {
                                            PLAYSOUND(1,300);
                                            ActPProjects->data[27]--;
                                            PLANET_MyShipPtr->Ladung++;
                                            LTOut--;
                                        }
                                        if ((MouseX(0)>=227) && (MouseX(0)<=327)
                                         && (MouseY(0)>=228) && (MouseY(0)<=258)
                                         && (ActPProjects->data[27]<250)
                                         && ((PLANET_MyShipPtr->Ladung & MASK_LTRUPPS)>0)
                                         && (PLANET_MyShipPtr->Fracht == 0))
                                        {
                                            PLAYSOUND(1,300);
                                            ActPProjects->data[27]++;
                                            PLANET_MyShipPtr->Ladung--;
                                            LTOut++;
                                        }
                                    }
                                    if (((((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                                       ||((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) == 0))
                                      || ((ActPProjects->data[34] == 0) && (ActPProjects->data[40] == 0)))
                                     && (PLANET_MyPlanetHeader->Class != CLASS_STONES))
                                    {
                                        do
                                        {
                                            delay(RDELAY);
                                            if ((MouseX(0)>=342) && (MouseX(0)<=442)
                                             && (MouseY(0)>=164) && (MouseY(0)<=194)
                                             && (PLANET_MyShipPtr->Ladung == 0)
                                             && (PLANET_MyShipPtr->Fracht < ShipData(PLANET_MyShipPtr->SType).MaxLoad)
                                             && (PLANET_MyPlanetHeader->Water>10))
                                            {
                                                delay(7);
                                                PLANET_MyShipPtr->Fracht++;
                                                FIn++;
                                                PLANET_MyPlanetHeader->Water -= 5;
                                                WRITELOADDATA(LTOut);
                                            }
                                            if ((MouseX(0)>=342) && (MouseX(0)<=442)
                                             && (MouseY(0)>=228) && (MouseY(0)<=258)
                                             && (PLANET_MyShipPtr->Fracht>0)
                                             && ((PLANET_MyPlanetHeader->Water / PLANET_MyPlanetHeader->Size)<100))
                                            {
                                                delay(7);
                                                PLANET_MyPlanetHeader->Water += 5;
                                                FIn--;
                                                PLANET_MyShipPtr->Fracht--;
                                                WRITELOADDATA(LTOut);
                                            }
                                        }
                                        while (LMB_PRESSED);
                                    }
                                    WRITELOADDATA(LTOut);
                                }
                            }
                            while (RMB_NOTPRESSED);
                            PLAYSOUND(1,300);
                            RECT(MyScreen[0],0,42,148,469,273);
                            REFRESHDISPLAY();
                            if ((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) != ActPlayerFlag)
                            {
                                if (((PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK) != 0) && (FIn>0))
                                {
                                    AUTOVERHANDLUNG(ActPlayerFlag, PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK, ActSys, 0);
                                }
                                if (SOut>0)
                                {
                                    Save.ImperatorState[ActPlayer-1] += 25;
                                    PLANET_MyPlanetHeader->PFlags = ActPlayerFlag | FLAG_KNOWN;
                                    PLANET_MyPlanetHeader->Ethno = ActPlayerFlag;
                                    PLANET_MyPlanetHeader->Population += 10*SOut;
                                    ActPProjects->data[26] = 0;
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
                                if (LTOut>0)
                                {
                                    CHECKPLANET(PLANET_MyPlanetHeader);
                                    CHECKPROJECTS(PLANET_MyPlanetHeader,ActPlayerFlag);
                                    AUTOVERHANDLUNG(ActPlayerFlag,PLANET_MyPlanetHeader->PFlags & FLAG_CIV_MASK,ActSys,0);
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
    if (RMB_PRESSED) { PLAYSOUND(1,300); }
    RECT(MyScreen[0],0,194,119,318,280);
    REFRESHDISPLAY();

    return true;
}