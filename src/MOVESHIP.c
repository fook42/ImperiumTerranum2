#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"


void MOVESHIP(uint8 ActSys, r_ShipHeader* ShipPtr, bool Visible)
{
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   OtherShipPtr;
    r_ShipHeader*   FleetShipPtr;
    r_PlanetHeader* MyPlanetHeader;
    APTR            newMemPtr;
    uint8           blink;
    uint8           i, j;
    UWORD           RawCode;
    uint8           CivVar,CivFlag,OldMoving,SysID;
    ByteArr42*      ActPProjects;

    char            s[80], s2[80];
    char*           _s;
    int             slen;

    CLOCK();
    OldMoving = 0;
    newMemPtr = NULL;
    MyShipPtr = ShipPtr;
    CivVar = GETCIVVAR(MyShipPtr->Owner);
    if ((CIVVAR_NONE == CivVar) || (0 >= MyShipPtr->Moving))
    {
        return;
    }
    MOVESHIP_x = 256+(MyShipPtr->PosX+OffsetX)*32;
    MOVESHIP_y = 256+(MyShipPtr->PosY+OffsetY)*32;
    if ((((MOVESHIP_x<1) || (MOVESHIP_x>478) || (MOVESHIP_y<1) || (MOVESHIP_y>478)) && (Display == ActSys))
         && (((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) == FLAG_KNOWN)
             || (Visible)))
    {
        OffsetX = (-MyShipPtr->PosX)-1;
        OffsetY = (-MyShipPtr->PosY)-1;
        MOVESHIP_x = 256+(MyShipPtr->PosX + OffsetX)*32;
        MOVESHIP_y = 256+(MyShipPtr->PosY + OffsetY)*32;
        if (((MyShipPtr->Owner == ActPlayerFlag) && (Save.CivPlayer[ActPlayer-1] != 0))
            || (Visible))
        {
            DRAWSYSTEM(MODE_REDRAW,ActSys,MyShipPtr);
        }
    }
    if (MyShipPtr->Owner == ActPlayerFlag)
    {
        RECT(MyScreen[0],0,520,291,632,308);
    }
    if ((0 == Save.CivPlayer[CivVar-1])
     || (SHIPFLAG_WATER == MyShipPtr->Flags) || (Save.PlayMySelf))
    {
        if ((1 == MyShipPtr->Moving) && (0 == (Year % 2)))
        {
            MyShipPtr->Moving++;
        }
        Delay(2);
        do
        {
            MOVESHIP_FromX = MyShipPtr->PosX;
            MOVESHIP_FromY = MyShipPtr->PosY;
            if ((SHIPTYPE_STARGATE == MyShipPtr->SType) && (TARGET_STARGATE != SystemHeader[ActSys-1].FirstShip.SType))
            {
                /**** STARGATE INSTALLIEREN ****/
                if (sqrt((MyShipPtr->PosX*MyShipPtr->PosX)+(MyShipPtr->PosY*MyShipPtr->PosY))<10)
                {
                    if (!FINDOBJECT(ActSys-1,(MyShipPtr->PosX+OffsetX)*32,(MyShipPtr->PosY+OffsetY)*32,MyShipPtr))
                    {
                        SystemHeader[ActSys-1].FirstShip.SType = TARGET_STARGATE;
                        SystemHeader[ActSys-1].FirstShip.PosX = MyShipPtr->PosX;
                        SystemHeader[ActSys-1].FirstShip.PosY = MyShipPtr->PosY;
                        MyShipPtr->Owner = 0;
                        if ((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) == FLAG_KNOWN)
                        {
                            REFRESHDISPLAY();
                        }
                        Delay(20);
                    } else {
                        MyShipPtr->Moving = 0;
                    }
                    return;
                }
                // move the Stargate closer to the sun each round
                if (0 < MyShipPtr->PosX)
                {
                    --MyShipPtr->PosX;
                } else if (0 > MyShipPtr->PosX)
                {
                    ++MyShipPtr->PosX;
                }
                if (0 < MyShipPtr->PosY)
                {
                    --MyShipPtr->PosY;
                } else if (0 > MyShipPtr->PosY)
                {
                    ++MyShipPtr->PosY;
                }
                // ---
            } else if (TARGET_ENEMY_SHIP == MyShipPtr->Target)
            {
                /**** FEINDLICHES SCHIFF ABFANGEN ****/
                if      (MyShipPtr->TargetShip->PosX < MyShipPtr->PosX) { MyShipPtr->PosX--; }
                else if (MyShipPtr->TargetShip->PosX > MyShipPtr->PosX) { MyShipPtr->PosX++; }
                if      (MyShipPtr->TargetShip->PosY < MyShipPtr->PosY) { MyShipPtr->PosY--; }
                else if (MyShipPtr->TargetShip->PosY > MyShipPtr->PosY) { MyShipPtr->PosY++; }
            } else if ((MyShipPtr->Source<0) && (MyShipPtr->Target<0))
            {
                /**** STABILES WURMLOCH BENUTZEN ****/
                if      (MyWormHole[(-MyShipPtr->Target)-1].PosX[(-MyShipPtr->Source)-1] < MyShipPtr->PosX) { MyShipPtr->PosX--; }
                else if (MyWormHole[(-MyShipPtr->Target)-1].PosX[(-MyShipPtr->Source)-1] > MyShipPtr->PosX) { MyShipPtr->PosX++; }

                if      (MyWormHole[(-MyShipPtr->Target)-1].PosY[(-MyShipPtr->Source)-1] < MyShipPtr->PosY) { MyShipPtr->PosY--; }
                else if (MyWormHole[(-MyShipPtr->Target)-1].PosY[(-MyShipPtr->Source)-1] > MyShipPtr->PosY) { MyShipPtr->PosY++; }
            } else {
                /**** PLANETEN ANFLIEGEN ****/
                while (MyShipPtr->Target > SystemHeader[ActSys-1].Planets)
                {
                    /* Unkorrektes Ziel für Bewässerung */
                    SysID = FINDNEXTPLANET(ActSys-1,MyShipPtr)+1;
                    if (ActSys != SysID)
                    {
                        AUTOSHIPTRAVEL(SysID,MODE_SHIPS,MyShipPtr);
                    }
                }
                MyPlanetHeader = &(SystemHeader[ActSys-1].PlanetMemA[MyShipPtr->Target-1]);
                if      (it_round(MyPlanetHeader->PosX) < MyShipPtr->PosX) { MyShipPtr->PosX--; }
                else if (it_round(MyPlanetHeader->PosX) > MyShipPtr->PosX) { MyShipPtr->PosX++; }
                if      (it_round(MyPlanetHeader->PosY) < MyShipPtr->PosY) { MyShipPtr->PosY--; }
                else if (it_round(MyPlanetHeader->PosY) > MyShipPtr->PosY) { MyShipPtr->PosY++; }
            }
            if (CHECKSUN(MyShipPtr)) { return; }
            MOVESHIP_ToX = 256+(MyShipPtr->PosX+OffsetX)*32;
            MOVESHIP_ToY = 256+(MyShipPtr->PosY+OffsetY)*32;
            if ((abs(MyShipPtr->PosX)>50) || (abs(MyShipPtr->PosY)>50)
             || ( ((MyShipPtr->Target<1) || (MyShipPtr->Target>SystemHeader[ActSys-1].Planets))
                && (MyShipPtr->Target!=TARGET_POSITION) && (MyShipPtr->Target!=TARGET_STARGATE)
                && (MyShipPtr->Target!=TARGET_ENEMY_SHIP)))
            {
                /*Bugfix Mad Ship*/
                MyShipPtr->Owner = 0;
                return;
            }
            if (FINDOBJECT(ActSys-1,MOVESHIP_ToX+16-256,MOVESHIP_ToY+16-256,MyShipPtr))
            {
                if (MyShipPtr->Flags == SHIPFLAG_WATER)
                {
                    if ((TYPE_STARGATE != ObjType) && (TYPE_WORMHOLE != ObjType))
                    {
                        MyShipPtr->PosX = MOVESHIP_FromX;
                        MyShipPtr->PosY = MOVESHIP_FromY;
                        /* if (ObjType != TYPE_PLANET) { ObjType = 0; } */
                    }
                }
                switch (ObjType) {
                    case TYPE_PLANET: if (SHIPFLAG_WATER == MyShipPtr->Flags)
                                    {
                                        SystemFlags[ActPlayer-1][ActSys-1] |= FLAG_KNOWN;
                                        if (AUTOWATERTRANSPORT((r_PlanetHeader*) ObjPtr, MyShipPtr, ActSys))
                                        {
                                            return;
                                        }
                                        MyShipPtr->Moving = 0;
                                    } else {
                                        if (((r_PlanetHeader*) ObjPtr) == &(SystemHeader[ActSys-1].PlanetMemA[MyShipPtr->Target-1]))
                                        {
                                            MyShipPtr->PosX = MOVESHIP_FromX;
                                            MyShipPtr->PosY = MOVESHIP_FromY;
                                            MyPlanetHeader = (r_PlanetHeader*) ObjPtr;
                                            OtherShipPtr = MyPlanetHeader->FirstShip.NextShip;
                                            while ((OtherShipPtr != NULL) && (OtherShipPtr->Owner == 0))
                                            {
                                                OtherShipPtr = OtherShipPtr->NextShip;
                                            }
                                            if ((OtherShipPtr != NULL)
                                                && ((Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1]==LEVEL_WAR)
                                                 || (Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1]==LEVEL_COLDWAR)))
                                            {
                                                /**** Feindliches Schiff im Orbit ****/
                                                if ((Save.CivPlayer[ActPlayer-1] == 0)
                                                    && (Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0))
                                                {
                                                    return;
                                                }
                                                if (Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1] == LEVEL_COLDWAR)
                                                {
                                                    GOTOWAR(MyShipPtr->Owner,MyPlanetHeader->PFlags);
                                                }
                                                MyShipPtr->Moving = 0;
                                                if (OtherShipPtr->Owner != MyShipPtr->Owner)
                                                {
                                                    // if the ship in orbit is not of the same owner (=foreign ship)
                                                    // start the fight...
                                                    Delay(20);
                                                    if (BIGSHIPFIGHT(MyShipPtr,OtherShipPtr,MODE_ALL,ActSys) == 1)
                                                    {
                                                        return;
                                                    }
                                                    REFRESHDISPLAY();
                                                    Delay(10);
                                                    AUTOVERHANDLUNG(MyShipPtr->Owner,MyPlanetHeader->PFlags,MODE_REFRESH);
                                                    return;
                                                }
                                                LINKTOORBIT(MyPlanetHeader, MyShipPtr, ActSys-1);
                                                return;
                                            } else if (0 == (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
                                            {
                                                if (((MyShipPtr->Ladung & MASK_SIEDLER)>0)
                                                    && (0 != ClassLifeFactor[MyPlanetHeader->Class]))
                                                {
                                                    /**** Planet unbewohnt ****/
                                                    if (NULL != MyPlanetHeader->ProjectPtr)
                                                    {
                                                        ActPProjects = MyPlanetHeader->ProjectPtr;
                                                        if ((0 < ActPProjects->data[PROJECT_SDI])
                                                         || (0 < ActPProjects->data[PROJECT_SPACEPHALANX]))
                                                        {
                                                            MOVESHIP_SMALLANDING(MyPlanetHeader,MyShipPtr,ActSys);
                                                            return;
                                                        }
                                                    }
                                                    Save.ImperatorState[CivVar-1] += 25;
                                                    if (NULL == MyPlanetHeader->ProjectPtr)
                                                    {
                                                        newMemPtr = AllocMem(sizeof(ByteArr42),MEMF_CLEAR);
                                                        if (NULL == newMemPtr)
                                                        {
                                                            return;
                                                        }
                                                        MyPlanetHeader->ProjectPtr = (ByteArr42*) newMemPtr;
                                                        MyPlanetHeader->Infrastruktur = 1;
                                                        MyPlanetHeader->Industrie = 1;
                                                        if (MyPlanetHeader->Class == CLASS_EARTH)
                                                        {
                                                            MyPlanetHeader->Biosphaere = 200;
                                                        } else if ((MyPlanetHeader->Class==CLASS_HALFEARTH)
                                                                || (MyPlanetHeader->Class==CLASS_WATER))
                                                        {
                                                            MyPlanetHeader->Biosphaere = 90;
                                                        } else if (MyPlanetHeader->Class == CLASS_DESERT)
                                                        {
                                                            MyPlanetHeader->Biosphaere = 50;
                                                        } else {
                                                            MyPlanetHeader->Biosphaere = 30;
                                                        }
                                                    }
                                                    ActPProjects = MyPlanetHeader->ProjectPtr;
                                                    ActPProjects->data[0] = 1;
                                                    if (0 == (SystemFlags[0][ActSys-1] & FLAG_CIV_MASK))
                                                    {
                                                        SystemFlags[0][ActSys-1] = MyShipPtr->Owner;
                                                    }
                                                    MyPlanetHeader->PFlags = MyShipPtr->Owner;
                                                    MyPlanetHeader->Ethno  = MyShipPtr->Owner;
                                                    MyShipPtr->Ladung -= 16;
                                                    MyPlanetHeader->Population += 10;
                                                    if ((MyShipPtr->Ladung & MASK_SIEDLER)>0)
                                                    {
                                                        SysID = FINDNEXTPLANET(ActSys-1,MyShipPtr)+1;
                                                        if (ActSys != SysID)
                                                        {
                                                            AUTOSHIPTRAVEL(SysID,MODE_SHIPS,MyShipPtr);
                                                        }
                                                    }
                                                }
                                                if ((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) != 0)
                                                {
                                                    REFRESHDISPLAY();
                                                    Delay(10);
                                                }
                                                MyShipPtr->Moving = 0;
                                                return;
                                            } else if (((MyPlanetHeader->PFlags & FLAG_CIV_MASK) == MyShipPtr->Owner)
                                                    && ((MyShipPtr->Ladung & MASK_SIEDLER) == 0))
                                            {
                                                if (AUTOWATERTRANSPORT(MyPlanetHeader, MyShipPtr, ActSys))
                                                {
                                                    return;
                                                }
                                                /**** Eigener Planet, Schiff ohne Siedler, kein Wassertransport ****/
                                                LINKTOORBIT(MyPlanetHeader, MyShipPtr, ActSys-1);
                                                return;
                                            } else if ((Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1]==LEVEL_WAR)
                                                    || (Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1]==LEVEL_COLDWAR))
                                            {
                                                /**** Feindlicher Planet ****/
                                                if ((Save.CivPlayer[ActPlayer-1] == 0)
                                                    && (Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0))
                                                {
                                                    return;
                                                }
                                                if (Save.WarState[CivVar-1][GETCIVVAR(MyPlanetHeader->PFlags)-1] == LEVEL_COLDWAR)
                                                {
                                                    GOTOWAR(MyShipPtr->Owner,MyPlanetHeader->PFlags);
                                                }
                                                ActPProjects = MyPlanetHeader->ProjectPtr;
                                                if ((0 < ActPProjects->data[PROJECT_SDI])
                                                 || (0 < ActPProjects->data[PROJECT_SPACEPHALANX]))
                                                {
                                                    if ((Save.CivPlayer[GETCIVVAR(MyShipPtr->Owner)-1] != 0)
                                                     || (Save.CivPlayer[GETCIVVAR(MyPlanetHeader->PFlags)-1] != 0))
                                                    {
                                                        MOVESHIP_DRAWSCENE(ActSys, MyShipPtr);
                                                    }
                                                    MOVESHIP_SMALLANDING(MyPlanetHeader,MyShipPtr,ActSys);
                                                    return;
                                                }
                                                if ((Save.WorldFlag == WFLAG_CEBORC) && (CIVVAR_OTHER == CivVar))
                                                {
                                                    MyPlanetHeader->PFlags        = 0;
                                                    MyPlanetHeader->Ethno         = 0;
                                                    MyPlanetHeader->Population    = 0;
                                                    MyPlanetHeader->Biosphaere    = rand()%150;
                                                    MyPlanetHeader->Industrie     = rand()%150;
                                                    MyPlanetHeader->Infrastruktur = rand()%150;
                                                    MyPlanetHeader->XProjectCosts = 0;
                                                    MyPlanetHeader->XProjectPayed = 0;
                                                    MyPlanetHeader->ProjectID     = PROJECT_NONE;

                                                    for(j = 1; j <= 42; j++)
                                                    {
                                                        ActPProjects->data[j] = 0;
                                                    }
                                                    if (Visible)
                                                    {
                                                        PLAYSOUND(1,1300);
                                                        REFRESHDISPLAY();
                                                        Delay(20);
                                                    }
                                                    return;
                                                } else if (CIVVAR_MAQUES == CivVar)
                                                {
                                                    MyPlanetHeader->Population    -= (MyPlanetHeader->Population   *0x15)>>11; // -0.01x
                                                    MyPlanetHeader->Biosphaere    -= (MyPlanetHeader->Biosphaere   *0x2D)>>11; // -0.03x
                                                    MyPlanetHeader->Industrie     -= (MyPlanetHeader->Industrie    *0x67)>>11; // -0.05x
                                                    MyPlanetHeader->Infrastruktur -= (MyPlanetHeader->Infrastruktur*0x8F)>>11; // -0.07x

                                                    _s=my_strcpy(s, PText[470]);
                                                    *_s++ = ' ';
                                                    (void) my_strcpy(_s, MyPlanetHeader->PName);

                                                    _s=my_strcpy(s2, PText[471]);
                                                    *_s++ = ' ';
                                                    _s = dez2out((MyPlanetHeader->XProjectPayed / 5), 0, _s);
                                                    *_s++ = '!';
                                                    *_s = 0;
                                                    if (Visible)
                                                    {
                                                        REQUEST(s,s2,12,12);
                                                    }
                                                    SUPPORTCIVI(MyPlanetHeader->XProjectPayed / 5);
                                                    MyPlanetHeader->XProjectPayed -= (MyPlanetHeader->XProjectPayed*0xCD)>>10; // -0.2x
                                                    (void) GOTONEXTSYSTEM(ActSys-1,MyShipPtr);
                                                } else if ((MyShipPtr->Ladung & MASK_LTRUPPS)>0)
                                                {
                                                    CHECKPLANET(MyPlanetHeader);
                                                    Bool_var = TAKETECH(MyShipPtr->Owner, MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                                                    Save.Staatstopf[GETCIVVAR(MyShipPtr->Owner)-1] += (MyPlanetHeader->XProjectPayed / 5);
                                                    MyPlanetHeader->XProjectPayed -= (MyPlanetHeader->XProjectPayed*0xCD)>>10; // -0.2x
                                                    AUTOVERHANDLUNG(MyShipPtr->Owner, MyPlanetHeader->PFlags, MODE_REFRESH);
                                                    CHECKPROJECTS(MyPlanetHeader, MyShipPtr->Owner);
                                                    MyPlanetHeader->PFlags = MyShipPtr->Owner;
                                                    if ((MyPlanetHeader->Population<(MyPlanetHeader->Size*13)) || ((rand()%(5)) == 0))
                                                    {
                                                        MyPlanetHeader->Ethno = MyShipPtr->Owner;
                                                    }
                                                    MyPlanetHeader->ProjectID = PROJECT_NONE;
                                                    MyShipPtr->Moving = 0;
                                                    if ((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) != 0)
                                                    {
                                                        REFRESHDISPLAY();
                                                        Delay(10);
                                                    }
                                                    return;
                                                } else {
                                                    FINDENEMYOBJECT(ActSys-1,MyShipPtr);
                                                }
                                            } else {
                                                MyShipPtr->PosX = MOVESHIP_FromX;
                                                MyShipPtr->PosY = MOVESHIP_FromY;
                                                MyShipPtr->Moving = 0;
                                                if (((Save.WarState[CivVar-1][0]==LEVEL_WAR) || (Save.WarState[CivVar-1][0]==LEVEL_COLDWAR))
                                                 || ((Save.WarState[CivVar-1][1]==LEVEL_WAR) || (Save.WarState[CivVar-1][1]==LEVEL_COLDWAR))
                                                 || ((Save.WarState[CivVar-1][2]==LEVEL_WAR) || (Save.WarState[CivVar-1][2]==LEVEL_COLDWAR))
                                                 || ((Save.WarState[CivVar-1][3]==LEVEL_WAR) || (Save.WarState[CivVar-1][3]==LEVEL_COLDWAR))
                                                 || ((Save.WarState[CivVar-1][4]==LEVEL_WAR) || (Save.WarState[CivVar-1][4]==LEVEL_COLDWAR))
                                                 || ((Save.WarState[CivVar-1][5]==LEVEL_WAR) || (Save.WarState[CivVar-1][5]==LEVEL_COLDWAR))
                                                 || ((Save.WarState[CivVar-1][6]==LEVEL_WAR) || (Save.WarState[CivVar-1][6]==LEVEL_COLDWAR))
                                                 || ((Save.WarState[CivVar-1][7]==LEVEL_WAR) || (Save.WarState[CivVar-1][7]==LEVEL_COLDWAR)))
                                                {
                                                    // FINDENEMYOBJECT(i,MyShipPtr); // ... todo .. i is uninitialized .. where are we looking for enemy objects?
                                                    FINDENEMYOBJECT(ActSys-1, MyShipPtr);
                                                } else {
                                                    (void) GOTONEXTSYSTEM(ActSys-1,MyShipPtr);
                                                }
                                                return;
                                            }
                                        }
                                    } break;
                    case TYPE_SHIP:     {   OtherShipPtr = (r_ShipHeader*) ObjPtr;
                                            if ((OtherShipPtr->Owner != MyShipPtr->Owner)
                                                && ((Save.WarState[CivVar-1][GETCIVVAR(OtherShipPtr->Owner)-1] == LEVEL_WAR)
                                                 || (Save.WarState[CivVar-1][GETCIVVAR(OtherShipPtr->Owner)-1] == LEVEL_COLDWAR)))
                                            {
                                                if ((Save.CivPlayer[ActPlayer-1] == 0)
                                                    && (Save.CivPlayer[GETCIVVAR(OtherShipPtr->Owner)-1] != 0))
                                                {
                                                    MyShipPtr->PosX = MOVESHIP_FromX;
                                                    MyShipPtr->PosY = MOVESHIP_FromY;
                                                    return;
                                                }
                                                if (Save.WarState[CivVar-1][GETCIVVAR(OtherShipPtr->Owner)-1] == LEVEL_COLDWAR)
                                                {
                                                    GOTOWAR(MyShipPtr->Owner,OtherShipPtr->Owner);
                                                }
                                                MyShipPtr->PosX = MOVESHIP_FromX;
                                                MyShipPtr->PosY = MOVESHIP_FromY;
                                                Delay(20);
                                                if (BIGSHIPFIGHT(MyShipPtr,OtherShipPtr,MODE_ONCE,ActSys) == 1)
                                                {
                                                    MyShipPtr->Moving = 0;
                                                    return;
                                                }
                                            } else {
                                                MyShipPtr->PosX = MOVESHIP_FromX;
                                                MyShipPtr->PosY = MOVESHIP_FromY;
                                                return;
                                            }
                                        } break;
                    case TYPE_STARGATE: if (MyShipPtr->Target == TARGET_ENEMY_SHIP)
                                        {
                                            if (MyShipPtr->TargetShip == &SystemHeader[ActSys-1].FirstShip)
                                            {
                                                SysID = MyShipPtr->Source;
                                                FADESTARGATE(SysID,ActSys,MyShipPtr);
                                                if (MyShipPtr->Moving <= 0)
                                                {
                                                    SystemFlags[ActPlayer-1][ActSys-1] |= FLAG_KNOWN;
                                                }
                                                return;
                                            }
                                        } break;
                    case TYPE_WORMHOLE: DOWORMHANDLING(MyShipPtr, ActSys, Visible);
                                        break;
                    default: { }
                }
            }
            for(i = 0; i < MAXHOLES; i++)
            {
                if (0 == MyWormHole[i].System[0]) { continue; }

                for(j = 0; j < 2; j++)
                {
                    if (MyWormHole[i].System[j] == ActSys)
                    {
                        if ((MyShipPtr->PosX>=(MyWormHole[i].PosX[j]-10)) && (MyShipPtr->PosX<=(MyWormHole[i].PosX[j]+10))
                          &&(MyShipPtr->PosY>=(MyWormHole[i].PosY[j]-10)) && (MyShipPtr->PosY<=(MyWormHole[i].PosY[j]+10)))
                        {
                            if (CIVVAR_NONE != GETCIVVAR(MyShipPtr->Owner))
                            {
                                MyWormHole[i].CivKnowledge[GETCIVVAR(MyShipPtr->Owner)-1] = FLAG_KNOWN;
                            }
                        }
                    }
                }
            }
            MyShipPtr->Moving--;
            if (Visible)
            {
                DRAWMOVINGSHIP(MyShipPtr, ActSys);
                if ((MyShipPtr->Moving >= 0) && (OldMoving != MyShipPtr->Moving))
                {
                    _s = dez2out(MyShipPtr->Moving, 3, s);
                    *_s++ = '-';
                    _s = dez2out(it_round((MyShipPtr->Shield+MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, _s);
                    *_s++ = '%'; *_s = 0;
                    WRITE_RP0(521,293,GETCIVFLAG(ActPlayer),1,1,s);
                    OldMoving = MyShipPtr->Moving;
                }
            }
        }
        while (MyShipPtr->Moving >0);
        return;
    }
    blink = 0;
    do
    {
        CLOCK();
        Delay(1);
        MOVESHIP_FromX = MyShipPtr->PosX;
        MOVESHIP_FromY = MyShipPtr->PosY;
        ++blink;
        if (15 == blink)
        {
            if ((MOVESHIP_x>=0) && (MOVESHIP_x<=480) && (MOVESHIP_y>=0) && (MOVESHIP_y<=480))
            {
                DRAWRECT(MOVESHIP_x,MOVESHIP_y, MyShipPtr, 0);
            }

            if ((MOVESHIP_x>=0) && (MOVESHIP_x<=505) && (MOVESHIP_y>=0) && (MOVESHIP_y<=512) && (MyShipPtr->SType != SHIPTYPE_FLEET))
            {
                s[0]=0;
                slen=0;
                if ((MyShipPtr->Ladung & MASK_SIEDLER)>0) { s[slen++] = 'S'; }
                if ((MyShipPtr->Ladung & MASK_LTRUPPS)>0) { s[slen++] = 'L'; }
                if (MyShipPtr->Fracht>0)                  {      s[0] = 'W'; slen=1; }
                if (MyShipPtr->SType == SHIPTYPE_STARGATE)
                {
                    s[slen++] = ' ';
                    _s = dez2out(it_round((double) sqrt((MyShipPtr->PosX*MyShipPtr->PosX)+(MyShipPtr->PosY*MyShipPtr->PosY))), 0, s+slen);
                    slen = _s - s;
                }
                s[slen]=0;
                WRITE_RP0(MOVESHIP_x+2,MOVESHIP_y+21,MyShipPtr->Owner,0,0,s);
            }
        } else if (30 == blink)
        {
            if ((MOVESHIP_x>=0) && (MOVESHIP_x<=480) && (MOVESHIP_y>=0) && (MOVESHIP_y<=480))
            {
                DRAWRECT(MOVESHIP_x,MOVESHIP_y, MyShipPtr, MyShipPtr->Owner);
            }
            blink = 0;
        }

        RawCode = GETRAWCODE();

        if ((LMB_PRESSED)
            || ((28 < RawCode) && (32 > RawCode)) || (45 == RawCode) || (47 == RawCode)
            || ((60 < RawCode) && (64 > RawCode)) || ((75 < RawCode) && (80 > RawCode)))
        {
            if (((MouseX(0)>=0) && (MouseX(0)<513))
                || ((28 < RawCode) && (32 > RawCode)) || (45 == RawCode) || (47 == RawCode)
                || ((60 < RawCode) && (64 > RawCode)) || ((75 < RawCode) && (80 > RawCode)))
            {
                if (FINDOBJECT(ActSys-1,MouseX(0)-256,MouseY(0)-256,NULL))
                {   // clicked on something?
                    PLAYSOUND(0,300);
                    switch (ObjType) {
                        case TYPE_PLANET:   PLANETINFO(ActSys, (r_PlanetHeader*) ObjPtr); break;
                        case TYPE_SHIP:     {
                                                if (SHIPTYPE_FLEET == ((r_ShipHeader*) ObjPtr)->SType)
                                                {
                                                    ORBITINFO((r_ShipHeader*) ObjPtr, _PT_Flotte, ActSys, 0, 0);
                                                } else {
                                                    SHIPINFO(ActSys, (r_ShipHeader*) ObjPtr);
                                                } break;
                                            }
                        default: {}
                    }
                } else {
                    // clicked in space to move the ship there
                    if (((MouseX(0) < MOVESHIP_x) && (0 == RawCode))
                        || (29 == RawCode) || (45 == RawCode) || (61 == RawCode) || (79 == RawCode))
                    {
                        MyShipPtr->PosX--;
                    }
                    if (((MouseX(0) > (MOVESHIP_x+32)) && (0 == RawCode))
                        || (31 == RawCode) || (47 == RawCode) || (63 == RawCode) || (78 == RawCode))
                    {
                        MyShipPtr->PosX++;
                    }
                    if (((MouseY(0) < MOVESHIP_y) && (0 == RawCode))
                        || ((60 < RawCode) && (64 > RawCode)) || (76 == RawCode))
                    {
                        MyShipPtr->PosY--;
                    }
                    if (((MouseY(0) > (MOVESHIP_y+32)) && (0 == RawCode))
                        || ((28 < RawCode) && (32 > RawCode)) || (77 == RawCode))
                    {
                        MyShipPtr->PosY++;
                    }
                    if ((MOVESHIP_FromX == MyShipPtr->PosX) && (MOVESHIP_FromY == MyShipPtr->PosY))
                    {
                        /* clicked on ship itself */
                        SHIPINFO(ActSys, MyShipPtr);
                    } else {
                        /* limit the action-area to -50..+50 */
                        if ( 50 < MyShipPtr->PosX) { MyShipPtr->PosX =  50; }
                        if (-50 > MyShipPtr->PosX) { MyShipPtr->PosX = -50; }
                        if ( 50 < MyShipPtr->PosY) { MyShipPtr->PosY =  50; }
                        if (-50 > MyShipPtr->PosY) { MyShipPtr->PosY = -50; }
                        /* */
                        MOVESHIP_ToX = 256+((MyShipPtr->PosX+OffsetX)*32);
                        MOVESHIP_ToY = 256+((MyShipPtr->PosY+OffsetY)*32);
                        if (FINDOBJECT(ActSys-1,MOVESHIP_ToX+16-256,MOVESHIP_ToY+16-256,MyShipPtr))
                        {
                            switch (ObjType) {
                                case TYPE_PLANET:   {
                                                        PLAYSOUND(0,300);
                                                        if (!PLANETHANDLING(ActSys,MyShipPtr,(r_PlanetHeader*) ObjPtr))
                                                        {
                                                            MyShipPtr->Moving = 0;
                                                            return;
                                                        }
                                                    } break;
                                case TYPE_SHIP:     {
                                                        OtherShipPtr = (r_ShipHeader*) ObjPtr;
                                                        if (OtherShipPtr->Owner != MyShipPtr->Owner)
                                                        {
                                                            CivFlag = OtherShipPtr->Owner;
                                                            MyShipPtr->PosX = MOVESHIP_FromX;
                                                            MyShipPtr->PosY = MOVESHIP_FromY;
                                                            if (STARTBIGSHIPFIGHT(MyShipPtr,OtherShipPtr,MODE_ONCE,ActSys) == 1)
                                                            {
                                                                AUTOVERHANDLUNG(ActPlayerFlag,CivFlag,MODE_REFRESH);
                                                                MyShipPtr->Moving = 0;
                                                                return;
                                                            }
                                                            AUTOVERHANDLUNG(ActPlayerFlag,CivFlag,MODE_REFRESH);
                                                        } else if (SHIPTYPE_FLEET != MyShipPtr->SType)
                                                        {
                                                            // moved into our own ship .. lets form a fleet
                                                            FleetShipPtr = OtherShipPtr;
                                                            if (SHIPTYPE_FLEET != OtherShipPtr->SType)
                                                            {
                                                                // if it wasnt a Fleet already, create a 1-ship fleet:
                                                                //  before: A = otherShip
                                                                // after: A = Fleetship -SType=Fleet,TargetShip=A,NextShip=NULL .. A->Before=F

                                                                newMemPtr = AllocMem(sizeof(r_ShipHeader),MEMF_CLEAR);
                                                                if (NULL == newMemPtr)
                                                                {
                                                                    DisplayBeep(NULL);
                                                                    // more error handling necessary!!! -> setting of Targetship etc..
                                                                } else {
                                                                    CopyMemQuick((APTR) OtherShipPtr, newMemPtr, sizeof(r_ShipHeader));
                                                                    OtherShipPtr->SType = SHIPTYPE_FLEET;
                                                                    OtherShipPtr->TargetShip = (r_ShipHeader*) newMemPtr;
                                                                    OtherShipPtr->TargetShip->BeforeShip = OtherShipPtr;
                                                                    OtherShipPtr->TargetShip->NextShip = NULL;
                                                                    OtherShipPtr->TargetShip->Flags = 0;
                                                                }
                                                            }
                                                            // OtherShipPtr is now the Fleet-ship.
                                                            if (NULL != OtherShipPtr->TargetShip)
                                                            {
                                                                OtherShipPtr->Flags = 0;
                                                                OtherShipPtr = OtherShipPtr->TargetShip;    // first ship in fleet

                                                                // search for last ship or strongest ship
                                                                while ((OtherShipPtr->SType < MyShipPtr->SType)
                                                                        && (NULL != OtherShipPtr->NextShip))
                                                                {
                                                                    OtherShipPtr = OtherShipPtr->NextShip;
                                                                }

                                                                if (SHIPTYPE_FLEET == OtherShipPtr->BeforeShip->SType)
                                                                {
                                                                    // if the "last ship" was the first of the fleet...
                                                                    MyShipPtr->BeforeShip->NextShip = MyShipPtr->NextShip;
                                                                    if (NULL != MyShipPtr->NextShip)
                                                                    {
                                                                        MyShipPtr->NextShip->BeforeShip = MyShipPtr->BeforeShip;
                                                                    }
                                                                    MyShipPtr->BeforeShip = OtherShipPtr->BeforeShip;
                                                                    MyShipPtr->NextShip = OtherShipPtr;

                                                                    OtherShipPtr->BeforeShip->TargetShip = MyShipPtr;
                                                                    OtherShipPtr->BeforeShip = MyShipPtr;
                                                                } else {
                                                                    LINKSHIP(MyShipPtr,OtherShipPtr,1);
                                                                }
                                                                DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
                                                            }
                                                            MyShipPtr = FleetShipPtr;
                                                        } else {
                                                            MyShipPtr->PosX = MOVESHIP_FromX;
                                                            MyShipPtr->PosY = MOVESHIP_FromY;
                                                            PLAYSOUND(0,600);
                                                        }
                                                    } break;
                                case TYPE_STARGATE: {
                                                        DRAWSTARS(MODE_STARGATE);
                                                        SysID = 0;
                                                        do
                                                        {
                                                            Delay(RDELAY);
                                                            if (LMB_PRESSED)
                                                            {
                                                                if (0 != LastSystem)
                                                                {
                                                                    if (TARGET_STARGATE == SystemHeader[LastSystem-1].FirstShip.SType)
                                                                    {
                                                                        SysID = LastSystem;
                                                                    }
                                                                }
                                                                PLAYSOUND(0,300);
                                                            } else {
                                                                WRITEGALAXYDATA(0,TARGET_STARGATE);
                                                            }
                                                        }
                                                        while ((RMB_NOTPRESSED) && (0 == SysID));
                                                        if ((RMB_PRESSED) || (ActSys == SysID))
                                                        {
                                                            MyShipPtr->PosX = MOVESHIP_FromX;
                                                            MyShipPtr->PosY = MOVESHIP_FromY;
                                                            PLAYSOUND(0,300);
                                                            DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
                                                        }
                                                        if ((0 != SysID) && (ActSys != SysID))
                                                        {
                                                            MOVESHIP_x = MOVESHIP_ToX;
                                                            MOVESHIP_y = MOVESHIP_ToY;
                                                            FADESTARGATE(SysID,ActSys,MyShipPtr);
                                                            return;
                                                        }
                                                    } break;
                                case TYPE_WORMHOLE: DOWORMHANDLING(MyShipPtr, ActSys, Visible);
                                                    break;
                                default: {
                                            /* writeln('FINDOBJ-Err,TerraShip'); */
                                            DisplayBeep(NULL);
                                        }
                            }
                        } else if ((TARGET_STARGATE != SystemHeader[ActSys-1].FirstShip.SType)
                                && (SHIPTYPE_FLEET != MyShipPtr->SType)
                                && (((Year+(MyShipPtr->PosX * MyShipPtr->PosY)) % 335) == 0))
                        {
                            DRAWMOVINGSHIP(MyShipPtr, ActSys);
                            if (!WORMHOLE(MyShipPtr, ActSys))
                            {
                                MyShipPtr->Owner = 0;
                                return;
                            }
                        } else {
                            DRAWMOVINGSHIP(MyShipPtr, ActSys);
                        }
                        if (0 < MyShipPtr->Moving)
                        {
                            --MyShipPtr->Moving;
                        }

                        if ((-2 <= MyShipPtr->PosX) && (2 >= MyShipPtr->PosX) && (-2 <= MyShipPtr->PosY) && (2 >= MyShipPtr->PosY))
                        {
                            // too close to the sun.. ship explodes.
                            MOVESHIP_FromX = MyShipPtr->PosX;
                            MOVESHIP_FromY = MyShipPtr->PosY;
                            MOVESHIP_EXPLODE(ActSys,MyShipPtr);
                            if (SHIPTYPE_FLEET == MyShipPtr->SType)
                            {
                                KILLFLEET(MyShipPtr);
                            } else if ((16 < MyShipPtr->SType) && (25 > MyShipPtr->SType))
                            {
                                STARDESASTER(ActSys,MyShipPtr);
                            }
                            return;
                        }
                    }
                }
            } else if ((0 == RawCode) && (521 < MouseX(0)) && (630 > MouseX(0))
                                      && (8   < MouseY(0)) && (118 > MouseY(0)))
            {
                // clicked on minimap to move the view
                OffsetX = 576-MouseX(0);
                OffsetY =  63-MouseY(0);
                DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
            }
        } else if (RMB_PRESSED)
        {
            if ((521 < MouseX(0)) && (630 > MouseX(0)) && (8 < MouseY(0)) && (118 > MouseY(0)))
            {
                // clicked on minimap area
                if (( 30 > MouseY(0)) && ( 42 > OffsetY)) { OffsetY += 2; }
                if (( 96 < MouseY(0)) && (-42 < OffsetY)) { OffsetY -= 2; }
                if ((539 > MouseX(0)) && ( 42 > OffsetX)) { OffsetX += 2; }
                if ((613 < MouseX(0)) && (-42 < OffsetX)) { OffsetX -= 2; }
                DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
            } else {
                PLAYSOUND(0,300);
                if (FINDOBJECT(ActSys-1,MOVESHIP_x+16-256,MOVESHIP_y+16-256,NULL))
                {
                    if (TYPE_SHIP == ObjType)
                    {
                        SYSTEMINFO(ActSys, (r_ShipHeader*) ObjPtr);
                    }
                }
            }
        }
        if ((60 < MyShipPtr->PosX) || (60 < MyShipPtr->PosY))
        {
            SYSTEMINFO(ActSys, MyShipPtr);
        }
        if ((0 <= MyShipPtr->Moving) && (OldMoving != MyShipPtr->Moving))
        {
            _s = dez2out(MyShipPtr->Moving, 3, s);
            *_s++ = '-';
            if (SHIPTYPE_FLEET == MyShipPtr->SType)
            {
                (void) my_strcpy(_s, "----");
            } else {
                _s = dez2out(it_round((MyShipPtr->Shield+MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, _s);
                *_s++ = '%';
                *_s = 0;
            }
            OldMoving = MyShipPtr->Moving;
            WRITE_RP0(521,293,GETCIVFLAG(ActPlayer),1,1,s);
        }
        MOVESHIP_x = 256+((MyShipPtr->PosX+OffsetX)*32);
        MOVESHIP_y = 256+((MyShipPtr->PosY+OffsetY)*32);
        CLEARINTUITION();
    }
    while ((0 < MyShipPtr->Moving)
        && (TARGET_POSITION != MyShipPtr->Target)
        && (0 != MyShipPtr->Owner));

    RECT_RP0_C0(520,291,632,308);
}
