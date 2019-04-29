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
    uint32          l;
    uint8           blink;
    uint8           i, j, RawCode;
    uint8           CivVar,CivFlag,OldMoving,SysID;
    ByteArr42*      ActPProjects;

    char            s[80], s2[80];
    char*           _s;
    int             slen;

    CLOCK();
    OldMoving = 0;
    l = 0;
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
        delay(2);
        do
        {
            MOVESHIP_FromX = MyShipPtr->PosX;
            MOVESHIP_FromY = MyShipPtr->PosY;
            if ((SHIPTYPE_STARGATE == MyShipPtr->SType) && (TARGET_STARGATE != SystemHeader[ActSys-1].FirstShip.SType))
            {
                /**** STARGATE INSTALLIEREN ****/
                if (sqrt((MyShipPtr->PosX*MyShipPtr->PosX)+(MyShipPtr->PosY*MyShipPtr->PosY))<10)
                {
                    if (!FINDOBJECT(ActSys-1,256+(MyShipPtr->PosX+OffsetX)*32,256+(MyShipPtr->PosY+OffsetY)*32,MyShipPtr))
                    {
                        SystemHeader[ActSys-1].FirstShip.SType = TARGET_STARGATE;
                        SystemHeader[ActSys-1].FirstShip.PosX = MyShipPtr->PosX;
                        SystemHeader[ActSys-1].FirstShip.PosY = MyShipPtr->PosY;
                        MyShipPtr->Owner = 0;
                        if ((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) == FLAG_KNOWN)
                        {
                            REFRESHDISPLAY();
                        }
                        delay(20);
                    } else {
                        MyShipPtr->Moving = 0;
                    }
                    return;
                }
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
            } else if (MyShipPtr->Target == TARGET_ENEMY_SHIP)
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
                    SysID = FINDNEXTPLANET(ActSys,MyShipPtr);
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
            if (FINDOBJECT(ActSys-1,MOVESHIP_ToX+16,MOVESHIP_ToY+16,MyShipPtr))
            {
                if (MyShipPtr->Flags == SHIPFLAG_WATER)
                {
                    if ((ObjType!=TYPE_STARGATE) && (ObjType!=TYPE_WORMHOLE))
                    {
                        MyShipPtr->PosX = MOVESHIP_FromX;
                        MyShipPtr->PosY = MOVESHIP_FromY;
                        /* if ObjType != TYPE_PLANET  ObjType = 0; */
                    }
                }
                switch (ObjType) {
                    case TYPE_PLANET: if (MyShipPtr->Flags == SHIPFLAG_WATER)
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
                                                    delay(20);
                                                    if (BIGSHIPFIGHT(MyShipPtr,OtherShipPtr,MODE_ALL,ActSys) == 1)
                                                    {
                                                        return;
                                                    }
                                                    REFRESHDISPLAY();
                                                    delay(10);
                                                    AUTOVERHANDLUNG(MyShipPtr->Owner,MyPlanetHeader->PFlags,ActSys,0);
                                                    return;
                                                }
                                                LINKTOORBIT(MyPlanetHeader, MyShipPtr, ActSys);
                                                return;
                                            } else if (0 == (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
                                            {
                                                if (((MyShipPtr->Ladung & MASK_SIEDLER)>0)
                                                    && ((MyPlanetHeader->Class==CLASS_DESERT)
                                                        || (MyPlanetHeader->Class==CLASS_HALFEARTH)
                                                        || (MyPlanetHeader->Class==CLASS_EARTH)
                                                        || (MyPlanetHeader->Class==CLASS_ICE)
                                                        || (MyPlanetHeader->Class==CLASS_STONES)
                                                        || (MyPlanetHeader->Class==CLASS_WATER)))
                                                {
                                                    /**** Planet unbewohnt ****/
                                                    if (NULL != MyPlanetHeader->ProjectPtr)
                                                    {
                                                        ActPProjects = MyPlanetHeader->ProjectPtr;
                                                        if ((ActPProjects->data[34]>0) || (ActPProjects->data[40]>0))
                                                        {
                                                            MOVESHIP_SMALLANDING(MyPlanetHeader,MyShipPtr,ActSys);
                                                            return;
                                                        }
                                                    }
                                                    Save.ImperatorState[CivVar-1] += 25;
                                                    if (NULL == MyPlanetHeader->ProjectPtr)
                                                    {
                                                        l = (uint32) AllocMem(sizeof(ByteArr42),MEMF_CLEAR);
                                                        if (0 == l)
                                                        {
                                                            return;
                                                        } else {
                                                            MyPlanetHeader->ProjectPtr = (ByteArr42*) l;
                                                            l = 99;
                                                        }
                                                    } else {
                                                        l = 13;
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
                                                    if (13 != l)
                                                    {
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
                                                    if ((MyShipPtr->Ladung & MASK_SIEDLER)>0)
                                                    {
                                                        SysID = FINDNEXTPLANET(ActSys,MyShipPtr);
                                                        if (ActSys != SysID)
                                                        {
                                                            AUTOSHIPTRAVEL(SysID,MODE_SHIPS,MyShipPtr);
                                                        }
                                                    }
                                                }
                                                if ((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) != 0)
                                                {
                                                    REFRESHDISPLAY();
                                                    delay(10);
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
                                                LINKTOORBIT(MyPlanetHeader, MyShipPtr, ActSys);
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
                                                if ((ActPProjects->data[34]>0) || (ActPProjects->data[40]>0))
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
                                                    MyPlanetHeader->ProjectID     = 0;

                                                    for(j = 1; j <= 42; j++)
                                                    {
                                                        ActPProjects->data[j] = 0;
                                                    }
                                                    if (Visible)
                                                    {
                                                        PLAYSOUND(2,1300);
                                                        REFRESHDISPLAY();
                                                        delay(20);
                                                    }
                                                    return;
                                                } else if (CIVVAR_MAQUES == CivVar)
                                                {
                                                    MyPlanetHeader->Population    = it_round(MyPlanetHeader->Population*0.99);
                                                    MyPlanetHeader->Biosphaere    = it_round(MyPlanetHeader->Biosphaere*0.97);
                                                    MyPlanetHeader->Industrie     = it_round(MyPlanetHeader->Industrie*0.95);
                                                    MyPlanetHeader->Infrastruktur = it_round(MyPlanetHeader->Infrastruktur*0.93);

                                                    slen = strlen(PText[470]);
                                                    memcpy(s, PText[470], slen+1);
                                                    s[slen++]=' ';
                                                    strcpy(s+slen, MyPlanetHeader->PName);

                                                    slen = strlen(PText[471]);
                                                    memcpy(s2, PText[471], slen);
                                                    s2[slen++]=' ';
                                                    _s = dez2out((MyPlanetHeader->XProjectPayed / 5), 0, s2+slen);
                                                    *_s++='!'; *_s=0;
                                                    if (Visible)
                                                    {
                                                        REQUEST(s,s2,12,12);
                                                    }
                                                    SUPPORTCIVI(MyPlanetHeader->XProjectPayed / 5);
                                                    MyPlanetHeader->XProjectPayed = it_round(MyPlanetHeader->XProjectPayed*0.8);
                                                    l = GOTONEXTSYSTEM(ActSys,MyShipPtr);
                                                } else if ((MyShipPtr->Ladung & MASK_LTRUPPS)>0)
                                                {
                                                    CHECKPLANET(MyPlanetHeader);
                                                    Bool_var = TAKETECH(MyShipPtr->Owner, MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                                                    Save.Staatstopf[GETCIVVAR(MyShipPtr->Owner)-1] += (MyPlanetHeader->XProjectPayed / 5);
                                                    MyPlanetHeader->XProjectPayed = it_round(MyPlanetHeader->XProjectPayed*0.8);
                                                    AUTOVERHANDLUNG(MyShipPtr->Owner, MyPlanetHeader->PFlags, ActSys, 0);
                                                    CHECKPROJECTS(MyPlanetHeader, MyShipPtr->Owner);
                                                    MyPlanetHeader->PFlags = MyShipPtr->Owner;
                                                    if ((MyPlanetHeader->Population<(MyPlanetHeader->Size*13)) || ((rand()%(5)) == 0))
                                                    {
                                                        MyPlanetHeader->Ethno = MyShipPtr->Owner;
                                                    }
                                                    MyPlanetHeader->ProjectID = 0;
                                                    MyShipPtr->Moving = 0;
                                                    if ((SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN) != 0)
                                                    {
                                                        REFRESHDISPLAY();
                                                        delay(10);
                                                    }
                                                    return;
                                                } else {
                                                    FINDENEMYOBJECT(ActSys,MyShipPtr);
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
                                                    FINDENEMYOBJECT(ActSys, MyShipPtr);
                                                } else {
                                                    l = GOTONEXTSYSTEM(ActSys,MyShipPtr);
                                                }
                                                return;
                                            }
                                        }
                                    } break;
                    case TYPE_SHIP:     {   OtherShipPtr = ObjPtr;
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
                                                delay(20);
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
                    *_s++='-';
                    _s = dez2out(it_round((MyShipPtr->Shield+MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, _s);
                    *_s++='%'; *_s=0;
                    WRITE(521,293,GETCIVFLAG(ActPlayer),1,MyRPort_PTR[0],2,s);
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
        delay(1);
        MOVESHIP_FromX = MyShipPtr->PosX;
        MOVESHIP_FromY = MyShipPtr->PosY;
        ++blink;
        if (15 == blink)
        {
            if ((MOVESHIP_x>=0) && (MOVESHIP_x<=480) && (MOVESHIP_y>=0) && (MOVESHIP_y<=480))
            {
                DRAWRECT(MOVESHIP_x,MOVESHIP_y,MyShipPtr, 0);
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
                WRITE(MOVESHIP_x+2,MOVESHIP_y+21,MyShipPtr->Owner,0,MyRPort_PTR[0],1,s);
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

        if ((LMB_PRESSED) || ((RawCode>=29) && (RawCode<=31)) || (RawCode==45) || (RawCode==47)
            || ((RawCode>=61) && (RawCode<=63)) || ((RawCode>=76) && (RawCode<=79)))
        {
            if (((MouseX(0)>=0) && (MouseX(0)<513))
                || ((RawCode>=29) && (RawCode<=31)) || (RawCode==45) || (RawCode==47)
                || ((RawCode>=61) && (RawCode<=63)) || ((RawCode>=76) && (RawCode<=79)))
            {
                if (FINDOBJECT(ActSys-1,MouseX(0),MouseY(0),NULL))
                {   // clicked on something?
                    PLAYSOUND(1,300);
                    switch (ObjType) {
                        case TYPE_PLANET:   PLANETINFO(ActSys); break;
                        case TYPE_SHIP:     if (MyShipPtr->SType == SHIPTYPE_FLEET)
                                            {
                                                ORBITINFO(MyShipPtr, _PT_Flotte, ActSys, 0, 0);
                                            } else {
                                                SHIPINFO(ActSys);
                                            } break;
                        default: {}
                    }
                } else {
                    // clicked in space to move the ship there
                    if (((MouseX(0) < MOVESHIP_x) && (RawCode == 0))
                        || (RawCode==29) || (RawCode==45) || (RawCode==61) || (RawCode==79))
                    {
                        MyShipPtr->PosX--;
                    }
                    if (((MouseX(0) > (MOVESHIP_x+32)) && (RawCode == 0))
                        || (RawCode==31) || (RawCode==47) || (RawCode==63) || (RawCode==78))
                    {
                        MyShipPtr->PosX++;
                    }
                    if (((MouseY(0) < MOVESHIP_y) && (RawCode == 0))
                        || ((RawCode>=61) && (RawCode<=63)) || (RawCode==76))
                    {
                        MyShipPtr->PosY--;
                    }
                    if (((MouseY(0) > (MOVESHIP_y+32)) && (RawCode == 0))
                        || ((RawCode>=29) && (RawCode<=31)) || (RawCode==77))
                    {
                        MyShipPtr->PosY++;
                    }
                    if ((MOVESHIP_FromX == MyShipPtr->PosX) && (MOVESHIP_FromY == MyShipPtr->PosY))
                    {
                        /* clicked on ship itself */
                        ObjPtr = MyShipPtr;
                        SHIPINFO(ActSys);
                    } else {
                        if (MyShipPtr->PosX>50)  { MyShipPtr->PosX = 50; }
                        if (MyShipPtr->PosX<-50) { MyShipPtr->PosX = -50; }
                        if (MyShipPtr->PosY>50)  { MyShipPtr->PosY = 50; }
                        if (MyShipPtr->PosY<-50) { MyShipPtr->PosY = -50; }
                        MOVESHIP_ToX = 256+((MyShipPtr->PosX+OffsetX)*32);
                        MOVESHIP_ToY = 256+((MyShipPtr->PosY+OffsetY)*32);
                        if (FINDOBJECT(ActSys-1,MOVESHIP_ToX+16,MOVESHIP_ToY+16,MyShipPtr))
                        {
                            switch (ObjType) {
                                case TYPE_PLANET:   {
                                                        PLAYSOUND(1,300);
                                                        if (!PLANETHANDLING(ActSys,MyShipPtr))
                                                        {
                                                            MyShipPtr->Moving = 0;
                                                            return;
                                                        }
                                                    } break;
                                case TYPE_SHIP:     {
                                                        OtherShipPtr = ObjPtr;
                                                        if (OtherShipPtr->Owner != MyShipPtr->Owner)
                                                        {
                                                            CivFlag = OtherShipPtr->Owner;
                                                            MyShipPtr->PosX = MOVESHIP_FromX;
                                                            MyShipPtr->PosY = MOVESHIP_FromY;
                                                            if (STARTBIGSHIPFIGHT(MyShipPtr,OtherShipPtr,MODE_ONCE,ActSys) == 1)
                                                            {
                                                                AUTOVERHANDLUNG(ActPlayerFlag,CivFlag,ActSys,0);
                                                                MyShipPtr->Moving = 0;
                                                                return;
                                                            }
                                                            AUTOVERHANDLUNG(ActPlayerFlag,CivFlag,ActSys,0);
                                                        } else if (MyShipPtr->SType != SHIPTYPE_FLEET)
                                                        {
                                                            FleetShipPtr = OtherShipPtr;
                                                            if (OtherShipPtr->SType != SHIPTYPE_FLEET)
                                                            {
                                                                l = (uint32) AllocMem(sizeof(r_ShipHeader),MEMF_CLEAR);
                                                                if (l == 0)
                                                                {
                                                                    DisplayBeep(NULL);
                                                                } else {
                                                                    CopyMemQuick((APTR) OtherShipPtr, (APTR) l, sizeof(r_ShipHeader));
                                                                    OtherShipPtr->SType = SHIPTYPE_FLEET;
                                                                    OtherShipPtr->TargetShip = (r_ShipHeader*) l;
                                                                    OtherShipPtr->TargetShip->BeforeShip = OtherShipPtr;
                                                                    OtherShipPtr->TargetShip->NextShip = NULL;
                                                                    OtherShipPtr->TargetShip->Flags = 0;
                                                                }
                                                            }
                                                            if (OtherShipPtr->TargetShip != NULL)
                                                            {
                                                                OtherShipPtr->Flags = 0;
                                                                OtherShipPtr = OtherShipPtr->TargetShip;

                                                                while ((OtherShipPtr->SType < MyShipPtr->SType)
                                                                        && (OtherShipPtr->NextShip != NULL))
                                                                {
                                                                    OtherShipPtr = OtherShipPtr->NextShip;
                                                                }

                                                                if (OtherShipPtr->BeforeShip->SType == SHIPTYPE_FLEET)
                                                                {
                                                                    MyShipPtr->BeforeShip->NextShip = MyShipPtr->NextShip;
                                                                    if (MyShipPtr->NextShip != NULL)
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
                                                            PLAYSOUND(1,600);
                                                        }
                                                    } break;
                                case TYPE_STARGATE: {
                                                        DRAWSTARS(MODE_STARGATE,ActPlayer);
                                                        SysID = 0;
                                                        do
                                                        {
                                                            delay(RDELAY);
                                                            if (LMB_PRESSED)
                                                            {
                                                                if (LastSystem != 0)
                                                                {
                                                                    if (SystemHeader[LastSystem-1].FirstShip.SType == TARGET_STARGATE)
                                                                    {
                                                                        SysID = LastSystem;
                                                                    }
                                                                }
                                                                PLAYSOUND(1,300);
                                                            } else {
                                                                WRITEGALAXYDATA(0,TARGET_STARGATE);
                                                            }
                                                        }
                                                        while ((RMB_NOTPRESSED) && (SysID == 0));
                                                        if ((RMB_PRESSED) || (SysID == ActSys))
                                                        {
                                                            MyShipPtr->PosX = MOVESHIP_FromX;
                                                            MyShipPtr->PosY = MOVESHIP_FromY;
                                                            PLAYSOUND(1,300);
                                                            DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
                                                        }
                                                        if ((SysID!=0) && (SysID!=ActSys))
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
                        } else if ((SystemHeader[ActSys-1].FirstShip.SType != TARGET_STARGATE)
                                && (MyShipPtr->SType != SHIPTYPE_FLEET)
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
                        if (MyShipPtr->Moving>0)
                        {
                            MyShipPtr->Moving--;
                        }

                        if ((MyShipPtr->PosX>=-3) && (MyShipPtr->PosX<=2) && (MyShipPtr->PosY>=-3) && (MyShipPtr->PosY<=2))
                        {
                            // too close to the sun.. ship explodes.
                            MOVESHIP_FromX = MyShipPtr->PosX;
                            MOVESHIP_FromY = MyShipPtr->PosY;
                            MyShipPtr->Owner = 0;
                            MOVESHIP_EXPLODE(ActSys,MyShipPtr);
                            if (MyShipPtr->SType == SHIPTYPE_FLEET)
                            {
                                KILLFLEET(MyShipPtr);
                            } else if ((MyShipPtr->SType>=17) && (MyShipPtr->SType<=24))
                            {
                                STARDESASTER(ActSys,MyShipPtr);
                            }
                            return;
                        }
                    }
                }
            } else if ((RawCode == 0) && (MouseX(0)>=522) && (MouseX(0)<=629)
                                      && (MouseY(0)>=9)   && (MouseY(0)<=117))
            {
                // clicked on minimap to move the view
                OffsetX = 576-MouseX(0);
                OffsetY =  63-MouseY(0);
                DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
            }
        } else if (RMB_PRESSED)
        {
            if ((MouseX(0)>=518) && (MouseX(0)<=634) && (MouseY(0)>=9) && (MouseY(0)<=117))
            {
                if ((MouseX(0)>=518) && (MouseX(0)<=634) && (MouseY(0)>=9) && (MouseY(0)<=29))
                {
                    if (OffsetY<42) { OffsetY += 2; }
                }
                if ((MouseX(0)>=518) && (MouseX(0)<=634) && (MouseY(0)>=97) && (MouseY(0)<=117))
                {
                    if (OffsetY>-42) { OffsetY -= 2; }
                }
                if ((MouseX(0)>=518) && (MouseX(0)<=538) && (MouseY(0)>=9) && (MouseY(0)<=117))
                {
                    if (OffsetX<42) { OffsetX += 2; }
                }
                if ((MouseX(0)>=614) && (MouseX(0)<=634) && (MouseY(0)>=9) && (MouseY(0)<=117))
                {
                    if (OffsetX>-42) { OffsetX -= 2; }
                }
                DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
            } else {
                PLAYSOUND(1,300);
                if (FINDOBJECT(ActSys-1,MOVESHIP_x+16,MOVESHIP_y+16,NULL))
                {
                    if (TYPE_SHIP == ObjType)
                    {
                        SYSTEMINFO(ActSys);
                    }
                }
            }
        }
        if ((MyShipPtr->PosX > 60) || (MyShipPtr->PosY > 60))
        {
            ObjPtr = MyShipPtr;
            SYSTEMINFO(ActSys);
        }
        if ((MyShipPtr->Moving >= 0) && (OldMoving != MyShipPtr->Moving))
        {
            _s = dez2out(MyShipPtr->Moving, 3, s);
            *_s++='-';
            if (SHIPTYPE_FLEET == MyShipPtr->SType)
            {
                strcpy(_s, "----");
            } else {
                _s = dez2out(it_round((MyShipPtr->Shield+MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, _s);
                *_s++='%'; *_s=0;
            }
            OldMoving = MyShipPtr->Moving;
            WRITE(521,293,GETCIVFLAG(ActPlayer),1,MyRPort_PTR[0],2,s);
        }
        MOVESHIP_x = 256+((MyShipPtr->PosX+OffsetX)*32);
        MOVESHIP_y = 256+((MyShipPtr->PosY+OffsetY)*32);
        CLEARINTUITION();
    }
    while ((MyShipPtr->Moving > 0) && (MyShipPtr->Target != TARGET_POSITION) && (MyShipPtr->Owner != 0));
    RECTWIN(MyRPort_PTR[0],0,520,291,632,308);
}
