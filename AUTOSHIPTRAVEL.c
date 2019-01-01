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
    struct Window*   AST_Window;
    struct RastPort* RPort_PTR;

    Printf("## AUTOSHIPTRAVEL >>>> ###\n");
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
            if (NULL != MyShipPtr)
            {
                do
                {
                    CivVar = GETCIVVAR(MyShipPtr->Owner);
                    if ((SHIPTYPE_FLEET == MyShipPtr->SType) && (CivVar>=1) && (CivVar<=MAXCIVS)
                        && ((0 == Save.CivPlayer[CivVar-1]) || (Save.PlayMySelf)))
                    {
                        KILLFLEET(MyShipPtr);
                    }
                    if ((SHIPTYPE_FLEET == MyShipPtr->SType) && (0 == CivVar))
                    {
                        KILLFLEET(MyShipPtr);
                    }
                    if ((CivVar>=1) && (CivVar<=MAXCIVS))
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
                        if ((MyShipPtr->Target>0) && (CivVar>=1) && (CivVar<=MAXCIVS)
                            && (Save.CivPlayer[CivVar-1] == 0))
                        {
                            MyPlanetHeader = &(SystemHeader[i-1].PlanetMemA[MyShipPtr->Target -1]);
                            if ((GETCIVVAR(MyPlanetHeader->PFlags)>=1)
                                && (GETCIVVAR(MyPlanetHeader->PFlags)<=MAXCIVS))
                            {
                                if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                                {
                                    b = true;
                                    if (Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0)
                                    {
                                        Visible = true;
                                    }
                                } else {
                                    b = false;
                                }
                            }
                        }
                        /**** Computership mit Playership als Ziel ****/
                        if (MyShipPtr->Target == TARGET_ENEMY_SHIP)
                        {
                            FINDENEMYOBJECT(i,MyShipPtr);
                        }
                        if ((NULL != MyShipPtr->TargetShip) && (CivVar>=1) && (CivVar<=MAXCIVS)
                            && (SHIPTYPE_FLEET != MyShipPtr->SType))
                        {
                            if ((GETCIVVAR(MyShipPtr->TargetShip->Owner)>=1)
                                && (GETCIVVAR(MyShipPtr->TargetShip->Owner)<=MAXCIVS)
                                && (Save.CivPlayer[GETCIVVAR(MyShipPtr->TargetShip->Owner)-1] != 0))
                            {
                                if ((GETCIVVAR(MyShipPtr->TargetShip->Owner) == ActPlayer)
                                    && (Save.CivPlayer[CivVar-1] == 0))
                                {
                                    b = true;
                                    if (Save.CivPlayer[GETCIVVAR(MyShipPtr->TargetShip->Owner)-1] != 0)
                                    {
                                        Visible = true;
                                    }
                                } else {
                                    b = false;
                                }
                            }
                        }
                        /**** ComputerShip erreicht PlayerSystem ****/
                        if ((CivVar>=1) && (CivVar<=MAXCIVS)
                            && (GETCIVVAR(SystemFlags[0][i-1])>=1) && (GETCIVVAR(SystemFlags[0][i-1])<=MAXCIVS))
                        {
                            if ((Save.CivPlayer[GETCIVVAR(SystemFlags[0][i-1])-1] != 0)
                                && (Save.CivPlayer[CivVar-1] == 0) && (GETCIVVAR(SystemFlags[0][i-1]) == ActPlayer))
                            {
                                b = true;
                                Visible = true;
                            }
                        }
                        if (Save.CivPlayer[CivVar-1] != 0)
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
                            if ((MyShipPtr->Moving>0) && (MyShipPtr->Target != TARGET_POSITION))
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
                                if (MyShipPtr->Moving>0)
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
                                if (CivVar2>0)
                                {
                                    if (Save.WarState[CivVar-1][CivVar2-1] == LEVEL_UNKNOWN)
                                      { Save.WarState[CivVar-1][CivVar2-1] = LEVEL_PEACE; }
                                    if (Save.WarState[CivVar2-1][CivVar-1] == LEVEL_UNKNOWN)
                                      { Save.WarState[CivVar2-1][CivVar-1] = LEVEL_PEACE; }
                                }
                                MOVESHIP(i,MyShipPtr,Visible);
                            } else if ((0 > MyShipPtr->Moving) && (MODE_ALL == Mode))
                            {
                                ++MyShipPtr->Moving;
                                if (0 <= MyShipPtr->Moving)
                                {
                                    CivVar2 = GETCIVVAR(SystemFlags[0][i-1]);
                                    if (CivVar2>0)
                                    {
                                        if (Save.WarState[CivVar-1][CivVar2-1] == LEVEL_UNKNOWN)
                                          { Save.WarState[CivVar-1][CivVar2-1] = LEVEL_PEACE; }
                                        if (Save.WarState[CivVar2-1][CivVar-1] == LEVEL_UNKNOWN)
                                          { Save.WarState[CivVar2-1][CivVar-1] = LEVEL_PEACE; }
                                    }
                                    SystemFlags[CivVar-1][i-1] = SystemFlags[CivVar-1][i-1] | FLAG_KNOWN;
                                    switch (rand()%2) {
                                        case 0: MyShipPtr->PosX = -35; break;
                                        case 1: MyShipPtr->PosX =  35; break;
                                        default: { }
                                    }
                                    switch (rand()%3) {
                                        case 0: MyShipPtr->PosY = -35; break;
                                        case 1: MyShipPtr->PosY =  35; break;
                                        case 2: MyShipPtr->PosY =   0; break;
                                        default: { }
                                    }
                                    do
                                    {
                                        switch (rand()%4) {
                                            case 0: MyShipPtr->PosX++; break;
                                            case 1: MyShipPtr->PosY++; break;
                                            case 2: MyShipPtr->PosX--; break;
                                            case 3: MyShipPtr->PosY--; break;
                                            default: { }
                                        }
                                    }
                                    while (FINDOBJECT(i-1,256+(MyShipPtr->PosX+OffsetX)*32,256+(MyShipPtr->PosY+OffsetY)*32,MyShipPtr));

                                    if ((((SystemFlags[0][i-1] & ActPlayerFlag) == ActPlayerFlag) || (CivVar == ActPlayer))
                                        && (Save.CivPlayer[ActPlayer-1] != 0) && (!DconDone))
                                    {
                                        INFORMUSER();
                                        AST_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
                                        if (NULL != AST_Window)
                                        {
                                            RPort_PTR = AST_Window->RPort;
                                            MAKEWINBORDER(RPort_PTR,0,0,340,80,12,6,1);

                                            strcpy(s, GETMYADJ(MyShipPtr->Owner, &DconDone));
                                            if (!DconDone)
                                            {
                                                if (SHIPTYPE_FLEET == MyShipPtr->SType)
                                                {
                                                    s[strlen(s)-1] = ' ';
                                                    strcat(s, PText[552]);
                                                } else {
                                                    strcat(s, " ");
                                                    strcat(s, PText[553]);
                                                }
                                            }
                                            WRITEWIN(195,16,MyShipPtr->Owner,(1|WRITE_Center),RPort_PTR,4,s);
                                            strcpy(s, _PT_System);
                                            strcat(s, " ");
                                            strcat(s, Save.SystemName.data[i-1]);
                                            if ((SystemFlags[0][i-1] & FLAG_CIV_MASK) != 0)
                                            {
                                                WRITEWIN(195,43,SystemFlags[0][i-1] & FLAG_CIV_MASK,(1|WRITE_Center),RPort_PTR,4,s);
                                            } else {
                                                WRITEWIN(195,43,                                 12,(1|WRITE_Center),RPort_PTR,4,s);
                                            }
                                            if (SHIPTYPE_FLEET != MyShipPtr->SType)
                                            {
                                                BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(MyShipPtr->SType-8)*32            ,32,RPort_PTR,8,20,32,32,192);
                                            } else {
                                                BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(MyShipPtr->TargetShip->SType-8)*32,32,RPort_PTR,8,20,32,32,192);
                                            }
                                            if (Save.PlayMySelf) { delay(PAUSE); }
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
                                            AUTOVERHANDLUNG(FLAG_REMALO,MyShipPtr->Owner,ActSys,MODE_TERRITORIUM);
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
                }
                while ((NULL != MyShipPtr) && (MODE_SHIPS != Mode));
            }
        }
    }
    while ((MAXSYSTEMS > i) && (MODE_SHIPS != Mode));
    Printf("## AUTOSHIPTRAVEL ---- ###\n");


    if ((Display != ActSys) && (MODE_SHIPS != Mode))
    {
        if (0 == ActSys)
        {
            DRAWSTARS(MODE_REDRAW,ActPlayer);
        } else {
            DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
        }
    }
    Printf("## AUTOSHIPTRAVEL <<<< ###\n");

}
