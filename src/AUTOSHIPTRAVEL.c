#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void AUTOSHIPTRAVEL(uint8 ActSys, uint8 Mode, r_ShipHeader* ShipPtr)
{
    int             i = 0;
    r_ShipHeader*   MyShipPtr = NULL;
    r_ShipHeader*   BAKShipPtr = NULL;
    uint8           CivVar,CivVar2,SysSteps,iStep;
    bool            DconDone,b,Visible;
    r_PlanetHeader* MyPlanetHeader;
    char            s[60];
    char*           _s;
    struct Window*   AST_Window;
    struct RastPort* RPort_PTR;
    time_t  t;

    DconDone = false;
    if (MODE_SHIPS == Mode)
    {
        SysSteps = 1;
        i = ActSys-1;
        MyShipPtr = ShipPtr;
    } else {
        SysSteps = 2;
    }

    do
    {
        ++i;
        for(iStep = 0; iStep < SysSteps; ++iStep)
        {
            if (MODE_ALL == Mode)
            {
                MyShipPtr = SystemHeader[i-1].FirstShip.NextShip;
            }

            while (NULL != MyShipPtr)
            {
                CivVar = GETCIVVAR(MyShipPtr->Owner);
                if (SHIPTYPE_FLEET == MyShipPtr->SType)
                {
                    if (((0 < CivVar) && (MAXCIVS >= CivVar) && ((0 == Save.CivPlayer[CivVar-1]) || (Save.PlayMySelf)))
                        || (CIVVAR_NONE == CivVar))
                    {
                        KILLFLEET(MyShipPtr);
                    }
                }
                if ((0 < CivVar) && (MAXCIVS >= CivVar))
                {
                    b = false;
                    Visible = false;
                    if (CivVar == ActPlayer)
                    {
                        b = true;
                    }
                    if (b && (Save.CivPlayer[ActPlayer-1] != 0))
                    {
                        Visible = true;
                    }

                    /**** Computership mit Playerplanet als Ziel ****/
                    if ((0 < MyShipPtr->Target) && (0 == Save.CivPlayer[CivVar-1]))
                    {
                        MyPlanetHeader = &(SystemHeader[i-1].PlanetMemA[MyShipPtr->Target -1]);
                        if ((0 < GETCIVVAR(MyPlanetHeader->PFlags))
                                && (GETCIVVAR(MyPlanetHeader->PFlags) <= MAXCIVS))
                        {
                            if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                            {
                                b = true;
                                if (0 != Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1])
                                {
                                    Visible = true;
                                }
                            } else {
                                b = false;
                            }
                        }
                    }
                    /**** Computership mit Playership als Ziel ****/
                    if (TARGET_ENEMY_SHIP == MyShipPtr->Target)
                    {
                        FINDENEMYOBJECT(i,MyShipPtr);
                    }

                    if ((NULL != MyShipPtr->TargetShip) && (SHIPTYPE_FLEET != MyShipPtr->SType))
                    {
                        CivVar2 = GETCIVVAR(MyShipPtr->TargetShip->Owner);
                        if ((CIVVAR_NONE != CivVar2)
                            && (MAXCIVS >= CivVar2)
                            && (Save.CivPlayer[CivVar2-1] != 0))
                        {
                            if ((CivVar2 == ActPlayer)
                                && (Save.CivPlayer[CivVar-1] == 0))
                            {
                                b = true;
                                if (Save.CivPlayer[CivVar2-1] != 0)
                                {
                                    Visible = true;
                                }
                            } else {
                                b = false;
                            }
                        }
                    }
                    /**** ComputerShip erreicht PlayerSystem ****/
                    CivVar2 = GETCIVVAR(SystemFlags[0][i-1]);
                    if ((CIVVAR_NONE != CivVar2) && (MAXCIVS >= CivVar2))
                    {
                        if ((0 != Save.CivPlayer[CivVar2-1])
                            && (0 == Save.CivPlayer[CivVar-1]) && (CivVar2 == ActPlayer))
                        {
                            b = true;
                            Visible = true;
                        }
                    }
                    if (0 != Save.CivPlayer[CivVar-1])
                    {
                        Visible = true;
                    }
                    if (MultiPlayer && ((MyShipPtr->Flags & SHIPFLAG_WATER) != 0) && (!Informed))
                    {
                        Visible = false;
                    }

                    if (b)
                    {
                        BAKShipPtr = MyShipPtr->BeforeShip;
                        if ((0 < MyShipPtr->Moving) && (TARGET_POSITION != MyShipPtr->Target))
                        {
                            if (((Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1] == 0) || Save.PlayMySelf)
                                && ((MyShipPtr->SType != SHIPTYPE_STARGATE) || (SystemHeader[i-1].FirstShip.SType == TARGET_STARGATE)))
                            {
                                if ((Save.WarState[CivVar-1][0]==LEVEL_WAR) || (Save.WarState[CivVar-1][0]==LEVEL_COLDWAR) ||
                                    (Save.WarState[CivVar-1][1]==LEVEL_WAR) || (Save.WarState[CivVar-1][1]==LEVEL_COLDWAR) ||
                                    (Save.WarState[CivVar-1][2]==LEVEL_WAR) || (Save.WarState[CivVar-1][2]==LEVEL_COLDWAR) ||
                                    (Save.WarState[CivVar-1][3]==LEVEL_WAR) || (Save.WarState[CivVar-1][3]==LEVEL_COLDWAR) ||
                                    (Save.WarState[CivVar-1][4]==LEVEL_WAR) || (Save.WarState[CivVar-1][4]==LEVEL_COLDWAR) ||
                                    (Save.WarState[CivVar-1][5]==LEVEL_WAR) || (Save.WarState[CivVar-1][5]==LEVEL_COLDWAR) ||
                                    (Save.WarState[CivVar-1][6]==LEVEL_WAR) || (Save.WarState[CivVar-1][6]==LEVEL_COLDWAR) ||
                                    (Save.WarState[CivVar-1][7]==LEVEL_WAR) || (Save.WarState[CivVar-1][7]==LEVEL_COLDWAR))
                                {
                                    FINDENEMYOBJECT(i,MyShipPtr);
                                } else {
                                    // l = FINDNEXTPLANET(i,MyShipPtr); // ...todo ... l is unused here
                                    (void) FINDNEXTPLANET(i,MyShipPtr);
                                }
                            }
                            if (Visible) { INFORMUSER(); }
                            if (0 < MyShipPtr->Moving)
                            {
                                if (Visible)
                                {
                                    if (((256+(MyShipPtr->PosX+OffsetX)*32) < 1) || ((256+(MyShipPtr->PosX+OffsetX)*32) > 478)
                                        ||((256+(MyShipPtr->PosY+OffsetY)*32) < 1) || ((256+(MyShipPtr->PosY+OffsetY)*32) > 478)
                                        || (Display != i))
                                    {
                                        OffsetX = -MyShipPtr->PosX-1;
                                        OffsetY = -MyShipPtr->PosY-1;
                                        DRAWSYSTEM(MODE_REDRAW,i,MyShipPtr);
                                    }
                                }
                            }
                            CivVar2 = GETCIVVAR(SystemFlags[0][i-1]);
                            if (CIVVAR_NONE != CivVar2)
                            {
                                if (Save.WarState[CivVar-1][CivVar2-1] == LEVEL_UNKNOWN)
                                    { Save.WarState[CivVar-1][CivVar2-1] = LEVEL_PEACE; }
                                if (Save.WarState[CivVar2-1][CivVar-1] == LEVEL_UNKNOWN)
                                    { Save.WarState[CivVar2-1][CivVar-1] = LEVEL_PEACE; }
                            }
                            MOVESHIP(i,MyShipPtr,Visible);
                        } else if ((0 > MyShipPtr->Moving) && (MODE_ALL == Mode))
                        {
                            ++(MyShipPtr->Moving);
                            if (0 == MyShipPtr->Moving)
                            {
                                CivVar2 = GETCIVVAR(SystemFlags[0][i-1]);
                                if (CIVVAR_NONE != CivVar2)
                                {
                                    if (Save.WarState[CivVar-1][CivVar2-1] == LEVEL_UNKNOWN)
                                    {
                                        Save.WarState[CivVar-1][CivVar2-1] = LEVEL_PEACE;
                                    }
                                    if (Save.WarState[CivVar2-1][CivVar-1] == LEVEL_UNKNOWN)
                                    {
                                        Save.WarState[CivVar2-1][CivVar-1] = LEVEL_PEACE;
                                    }
                                }
                                SystemFlags[CivVar-1][i-1] |= FLAG_KNOWN;

                                srand((unsigned) time(&t));
                                do
                                {
                                    if (0 == (rand()%2))
                                    {
                                        MyShipPtr->PosX = -35;
                                    } else {
                                        MyShipPtr->PosX = +35;
                                    }

                                    switch (rand()%3) {
                                        case 0: MyShipPtr->PosY = -35; break;
                                        case 1: MyShipPtr->PosY = +35; break;
                                        case 2: MyShipPtr->PosY =   0; break;
                                        default: { }
                                    }

                                    switch (rand()%4) {
                                        case 0: ++(MyShipPtr->PosX); break;
                                        case 1: ++(MyShipPtr->PosY); break;
                                        case 2: --(MyShipPtr->PosX); break;
                                        case 3: --(MyShipPtr->PosY); break;
                                        default: { }
                                    }
                                }
                                while (FINDOBJECT(i-1, 256+(MyShipPtr->PosX+OffsetX)*32, 256+(MyShipPtr->PosY+OffsetY)*32, MyShipPtr));

                                if ((((SystemFlags[0][i-1] & ActPlayerFlag) == ActPlayerFlag) || (CivVar == ActPlayer))
                                    && (Save.CivPlayer[ActPlayer-1] != 0) && (!DconDone))
                                {
                                    INFORMUSER();
                                    AST_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
                                    if (NULL != AST_Window)
                                    {
                                        RPort_PTR = AST_Window->RPort;
                                        MAKEWINBORDER(RPort_PTR,0,0,340,80,12,6,1);

                                        _s=my_strcpy(s, GETMYADJ(MyShipPtr->Owner, &DconDone));
                                        if (!DconDone)
                                        {
                                            if (SHIPTYPE_FLEET == MyShipPtr->SType)
                                            {
                                                _s--;
                                                *_s++ = ' ';
                                                (void) my_strcpy(_s, PText[552]);
                                            } else {
                                                *_s++ = ' ';
                                                (void) my_strcpy(_s, PText[553]);
                                            }
                                        }
                                        WRITE(195,16,MyShipPtr->Owner,(1|WRITE_Center),RPort_PTR,3,s);
                                        _s=my_strcpy(s, _PT_System);
                                        *_s++ = ' ';
                                        (void) my_strcpy(_s, Save.SystemName.data[i-1]);
                                        if ((SystemFlags[0][i-1] & FLAG_CIV_MASK) != 0)
                                        {
                                            WRITE(195,43,SystemFlags[0][i-1] & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,3,s);
                                        } else {
                                            WRITE(195,43,                                 12,(1|WRITE_Center),RPort_PTR,3,s);
                                        }
                                        if (SHIPTYPE_FLEET != MyShipPtr->SType)
                                        {
                                            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(MyShipPtr->SType-8)*32            ,32,RPort_PTR,8,20,32,32,192);
                                        } else {
                                            BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(MyShipPtr->TargetShip->SType-8)*32,32,RPort_PTR,8,20,32,32,192);
                                        }
                                        if (Save.PlayMySelf) { Delay(PAUSE); }
                                        WAITLOOP(Save.PlayMySelf);
                                        CloseWindow(AST_Window);
                                    }

                                    if (((SystemFlags[0][i-1] & FLAG_CIV_MASK) == FLAG_REMALO) && (Save.CivPlayer[2] == 0)
                                        && (MyShipPtr->Owner != FLAG_REMALO)
                                        && (Save.WarPower[2] > Save.WarPower[GETCIVVAR(MyShipPtr->Owner)-1])
                                        && (Save.WarState[2][GETCIVVAR(MyShipPtr->Owner)-1] != LEVEL_DIED)
                                        && (Save.WarState[2][GETCIVVAR(MyShipPtr->Owner)-1] != LEVEL_ALLIANZ)
                                        && (Save.WarState[2][GETCIVVAR(MyShipPtr->Owner)-1] != LEVEL_WAR))
                                    {
                                        AUTOVERHANDLUNG(FLAG_REMALO,MyShipPtr->Owner,MODE_TERRITORIUM);
                                    }
                                    REFRESHDISPLAY();
                                }
                            }
                        }
                        MyShipPtr = BAKShipPtr->NextShip;
                    }
                }

                if ((NULL != MyShipPtr) && (MODE_SHIPS != Mode))
                {
                    MyShipPtr = MyShipPtr->NextShip;
                }

                if (MODE_SHIPS == Mode) { break; }
            }
        }
    }
    while ((MAXSYSTEMS > i) && (MODE_SHIPS != Mode));

    if ((Display != ActSys) && (MODE_SHIPS != Mode))
    {
        if (0 == ActSys)
        {
            DRAWSTARS(MODE_REDRAW);
        } else {
            DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
        }
    }
}
