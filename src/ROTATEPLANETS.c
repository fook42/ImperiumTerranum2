#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const uint8 PriorityList[] = {0, 25, 5,1,34, 40,30,6,37, 7,31,41,33, 32,36,42,35, 38,28,29,3, 4,39};


void ROTATEPLANETS(uint8 ActSys)
{
    r_PlanetHeader* PlanetHeader;
    r_ShipHeader*   MyShipPtr;
    r_ShipHeader*   ActShipPtr;

    uint32      SystemOwn[MAXCIVS+1];
    uint32      l, PMoney;
    bool        b, FreeSystem;
    ByteArr42*  ActPProjects;
    sint16      FromX, FromY;
    double      sin_rot, cos_rot, d;
    uint8       CivVar,btx,XState,Fight;
    int         PProd, PCreativity;
    char        s[99];
    char*       _s;
    sint8       NewTech[6];
    time_t      t;
    int         NewPNames;
    int         i, j, k;
    int         ProjID;
    struct Window*   ROT_Window;
    struct Window*   ROT_Window2;
    struct RastPort* RPort_PTR;
    struct RastPort* RPort_PTR2;

    if ((2000 < Year) && (1 == ActPlayer)) { DOHUMANITY(); }
    RECT_RP0_C0(520,291,632,308);
    Valid = false;
    if (1 == ActPlayer) { ++Year; }
    FreeSystem = false;
    if ((0 == (Year % 10)) && (!Save.PlayMySelf)) { INFORMUSER(); }
    --ActPlayer; // ..to shift the arrays..
    AllCreative[ActPlayer] = 0;
    for(j = 1; j < 8; ++j)
    {
        if (0 >= Save.ProjectCosts[ActPlayer][j])
        {
            if (0 < Year)
            {
                Save.ProjectCosts[ActPlayer][j] = 50 * Year * j;
            } else {
                Save.ProjectCosts[ActPlayer][j] = 180000 * j;
            }
        }
    }

    if (0 == ActPlayer)
    {
        MaquesShips = 0;
        for(i = 0; i < MAXCIVS; ++i)
        {
            Save.WarPower[i] = 0;
            Save.Bevoelkerung[i] = 0;
            Militaerausgaben[i] = 0;
        }
    }

    if (0 >= Save.ProjectCosts[ActPlayer][PROJECT_VON_NEUMANN])
    {
        for(i = 0; i < Save.Systems; ++i)
        {
            if ((FLAG_KNOWN != SystemHeader[i].vNS) && (0 == (rand()%5)))
            {
                SystemHeader[i].vNS = FLAG_KNOWN;
                break;
            }
        }
    }
    ++ActPlayer; // ..to shift the arrays..

    for(i = 0; i < Save.Systems; ++i)
    {
        if (0 == SystemHeader[i].Planets) { continue; }

        if (1 == ActPlayer)
        {
            for(j = 0; j < MAXCIVS; ++j)
            {
                SystemFlags[j][i] &= FLAG_CIV_MASK;
            }

            if (TARGET_STARGATE == SystemHeader[i].FirstShip.SType)
            {
                // store old position of the stargate...
                FromX = SystemHeader[i].FirstShip.PosX;
                FromY = SystemHeader[i].FirstShip.PosY;

                // calculate some distance to the sun...
                d = 1.0/(sqrt(FromX * FromX + FromY * FromY));
                // rotate the stargate around the sun...
                sin_rot = sin(d);
                cos_rot = cos(d);
                SystemHeader[i].FirstShip.PosX = it_round(FromX * cos_rot - FromY * sin_rot);
                SystemHeader[i].FirstShip.PosY = it_round(FromX * sin_rot + FromY * cos_rot);

                // MyShipPtr = &SystemHeader[i].FirstShip; // not needed .. MyShipPtr is not changed at all.
                if (FINDOBJECT(i, (SystemHeader[i].FirstShip.PosX+OffsetX)*32,
                                  (SystemHeader[i].FirstShip.PosY+OffsetY)*32,
                                  &SystemHeader[i].FirstShip))
                {
                    // would the stargate "hit" another object? then we dont move it - revert to the previous coordinates
                    SystemHeader[i].FirstShip.PosX = FromX;
                    SystemHeader[i].FirstShip.PosY = FromY;
                }
                if (0 == SystemHeader[i].FirstShip.PosY)
                {
                    // if the stargate is left or right of the sun, then its orbit decreases
                    if (0 < SystemHeader[i].FirstShip.PosX) { SystemHeader[i].FirstShip.PosX--; }
                    if (0 > SystemHeader[i].FirstShip.PosX) { SystemHeader[i].FirstShip.PosX++; }
                }
                d = sqrt(SystemHeader[i].FirstShip.PosX * SystemHeader[i].FirstShip.PosX
                       + SystemHeader[i].FirstShip.PosY * SystemHeader[i].FirstShip.PosY);

                if (4 > d)
                {
                    // if the stargate is too close to the sun (radius < 4) .. its orbit collapses/it is destroyed
/* OpenWindow() ---- */
                    MAKEBORDER(MyScreen[0],85,120,425,200,12,6,0);
                    _s=my_strcpy(s, _PT_System);
                    *_s++ = ':';
                    *_s++ = ' ';
                    (void) my_strcpy(_s, Save.SystemName[i]);
                    WRITE_RP0(256,131,12,WRITE_Center,3, s);
                    WRITE_RP0(256,151,12,WRITE_Center,3, _PT_Umlaufbahn_kollabiert);
                    WRITE_RP0(256,171,12,WRITE_Center,3, _PT_Stargate_zerstoert);
                    if (Save.PlayMySelf) { Delay(PAUSE); }
                    WAITLOOP(Save.PlayMySelf);
                    RECT_RP0_C0(85,120,425,200);
/* CloseWindow() ---- */
                    SystemHeader[i].FirstShip.SType = 0;
                }
            }
            if (NULL != SystemHeader[i].FirstShip.NextShip)
            {
                REFRESHSHIPS(SystemHeader[i].FirstShip.NextShip, i, 1);
            }
        }

        if (MAXCIVS > ActPlayer)
        {
            for(j = 0; j < SystemHeader[i].Planets; ++j)
            {
                PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                if (NULL == PlanetHeader) { continue; }

                if ((FLAG_OTHER == PlanetHeader->Ethno) && (WFLAG_JAHADR != Save.WorldFlag))
                {
                    PlanetHeader->Ethno = PlanetHeader->PFlags & FLAG_CIV_MASK;
                }
                if ((PlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    if ((NULL != PlanetHeader->ProjectPtr) &&
                        (0 != (PlanetHeader->PFlags & FLAG_CIV_MASK)))
                    {
                        ActPProjects = PlanetHeader->ProjectPtr;
                        for(k = 1; k < 8; ++k)
                        {
                            if (ActPProjects->data[k]>0)
                            {
                                Save.ProjectCosts[GETCIVVAR(PlanetHeader->PFlags)-1][k] = 0;
                            }
                        }
                    }
                    CREATEPANIC(PlanetHeader, i+1, j+1);
                }
                if (1 == ActPlayer)
                {
                    /* Bevoelkerungswachstum */
                    CivVar = GETCIVVAR(PlanetHeader->PFlags);
                    if (0 == CivVar)
                    {
                        if (0 < PlanetHeader->Infrastruktur)
                        {
                            PlanetHeader->Infrastruktur = abs(PlanetHeader->Infrastruktur-(rand()%7));
                            // @TODO .. can change from 100% -> 6%  .. better reduce rand-value
                        }
                        if (0 < PlanetHeader->Industrie)
                        {
                            PlanetHeader->Industrie = abs(PlanetHeader->Industrie-(rand()%7));
                            // @TODO .. can change from 100% -> 6%  .. better reduce rand-value
                        }
                    } else if ((MAXCIVS >= CivVar) && (NULL != PlanetHeader->ProjectPtr))
                    {
                        ActPProjects = PlanetHeader->ProjectPtr;
                        if (((PlanetHeader->Class == CLASS_EARTH)        && ((PlanetHeader->Population / 1176) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_HALFEARTH) && ((PlanetHeader->Population /  991) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_ICE)       && ((PlanetHeader->Population /  743) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_WATER)     && ((PlanetHeader->Population /  745) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_STONES)    && ((PlanetHeader->Population /  929) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_DESERT)    && ((PlanetHeader->Population /  739) <= PlanetHeader->Size)))
                        {
                            PlanetHeader->Population += 1+(ActPProjects->data[PROJECT_MICROIDS]+ActPProjects->data[PROJECT_WEATHERSTATION])*20;
                            PlanetHeader->Population = it_round(PlanetHeader->Population*1.008);
                            if (0 == Save.ProjectCosts[CivVar-1][3])
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.028); }
                            if (0 == Save.ProjectCosts[CivVar-1][4])
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.029); }
                            if (1000 > PlanetHeader->Population)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.05); }
                            if (2000 > PlanetHeader->Population)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.009); }
                            if (3000 > PlanetHeader->Population)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.005); }
                        }
                        Save.Bevoelkerung[CivVar-1] += PlanetHeader->Population;
                    } else if (NULL == PlanetHeader->ProjectPtr)
                    {
                        PlanetHeader->ProjectPtr = (ByteArr42*) AllocMem(sizeof(ByteArr42),MEMF_CLEAR);
                    }

                    if (NULL != PlanetHeader->FirstShip.NextShip)
                    {
                        if (CLASS_PHANTOM == PlanetHeader->Class)
                        {
                            PlanetHeader->FirstShip.NextShip->Owner = 0;
                        }
                        REFRESHSHIPS(PlanetHeader->FirstShip.NextShip, i, 0);
                    }
                    d = 1.0/((j*3.0)+4.0);  // d = 1.0/((j*3.0)+1);
                    sin_rot = sin(d);
                    cos_rot = cos(d);
                    d = PlanetHeader->PosX;
                    PlanetHeader->PosX = d * cos_rot - PlanetHeader->PosY*sin_rot;
                    PlanetHeader->PosY = d * sin_rot + PlanetHeader->PosY*cos_rot;

                    if (0 == PlanetHeader->Population)
                    {
                        PlanetHeader->PFlags = 0;
                        PlanetHeader->Ethno = 0;
                    }
                }
            }
        }
    }

    if (0 < ActSys)
    {
        DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    }
    if (MAXCIVS > ActPlayer)
    {
        for(i = 0; i < Save.Systems; ++i)
        {
            if (0 == SystemHeader[i].Planets) { continue; }

            for(j = 1; j <= MAXCIVS; ++j)
            {
                SystemOwn[j] = 0;
            }
            for(j = 0; j < SystemHeader[i].Planets; j++)
            {
                PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);

                CivVar = GETCIVVAR(PlanetHeader->PFlags);
                if ((CivVar>0) && (CivVar<=MAXCIVS))
                {
                    SystemOwn[CivVar] += PlanetHeader->Population;
                }
                if ((  ((PlanetHeader->PFlags & FLAG_CIV_MASK)==0)
                    || ((PlanetHeader->PFlags & FLAG_CIV_MASK)==ActPlayerFlag))
                    && ((PlanetHeader->Class==CLASS_WATER) || (PlanetHeader->Class==CLASS_ICE)
                      ||(PlanetHeader->Class==CLASS_EARTH) || (PlanetHeader->Class==CLASS_HALFEARTH)
                      ||(PlanetHeader->Class==CLASS_DESERT)))
                {
                    b = false;
                    if (((PlanetHeader->Water / PlanetHeader->Size)>80)
                        && (PlanetHeader->Class!=CLASS_WATER) && (PlanetHeader->Class!=CLASS_ICE))
                    {
                        PlanetHeader->Class = CLASS_WATER;     b = true;
                    } else if (((PlanetHeader->Water / PlanetHeader->Size)>=55) && ((PlanetHeader->Water / PlanetHeader->Size)<=80)
                        && (PlanetHeader->Class != CLASS_EARTH))
                    {
                        PlanetHeader->Class = CLASS_EARTH;     b = true;
                    } else if (((PlanetHeader->Water / PlanetHeader->Size)>=21) && ((PlanetHeader->Water / PlanetHeader->Size)<=54)
                        && (PlanetHeader->Class!=CLASS_HALFEARTH) && (PlanetHeader->Class!=CLASS_STONES))
                    {
                        PlanetHeader->Class = CLASS_HALFEARTH; b = true;
                    } else if (((PlanetHeader->Water / PlanetHeader->Size)<21)
                        && (PlanetHeader->Class != CLASS_DESERT))
                    {
                        PlanetHeader->Class = CLASS_DESERT;    b = true;
                    }
                    if ((true == b) && ((PlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                        && (Save.CivPlayer[ActPlayer-1] != 0) && (Year>1900))
                    {
                        INFORMUSER();

                        ROT_Window=MAKEWINDOWBORDER(85,120,341,81,MyScreen[0]);
                        if (ROT_Window)
                        {
                            RPort_PTR = ROT_Window->RPort;

                            _s=my_strcpy(s, _PT_System);
                            *_s++ = ':';
                            *_s++ = ' ';
                            (void) my_strcpy(_s, Save.SystemName[i]);
                            WRITE(171, 7,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);

                            _s=my_strcpy(s, _PT_Planet);
                            *_s++ = ':';
                            *_s++ = ' ';
                            (void) my_strcpy(_s, PlanetHeader->PName);
                            WRITE(171, 27,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);

                            _s=my_strcpy(s, PText[578]);
                            *_s++ = ' ';
                            switch (PlanetHeader->Class)
                            {
                                case CLASS_DESERT    : *_s++ = 'D'; break;
                                case CLASS_HALFEARTH : *_s++ = 'H'; break;
                                case CLASS_EARTH     : *_s++ = 'M'; break;
                                case CLASS_WATER     : *_s++ = 'W'; break;
                                default: { }
                            }
                            *_s = 0;
                            WRITE(171,53,12,(1|WRITE_Center),RPort_PTR,3,s);
                            if (Save.PlayMySelf) { Delay(PAUSE); }
                            WAITLOOP(Save.PlayMySelf);
                            CloseWindow(ROT_Window);
                        }
                    }
                }
                if ((PlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    if ((((PlanetHeader->ProjectID>=1) && (PlanetHeader->ProjectID<=7)) || (39 == PlanetHeader->ProjectID))
                        && (Save.ProjectCosts[ActPlayer-1][PlanetHeader->ProjectID] <= 0))
                    {
                        if ((39 == PlanetHeader->ProjectID) && (Save.ProjectCosts[ActPlayer-1][39]>0))
                        {
                            for(k = 0; k < MAXCIVS; ++k)
                            {
                                if (k != (ActPlayer-1))
                                {
                                    vNSonde[k] = true;
                                }
                            }
                        }
                        if ((Save.CivPlayer[GETCIVVAR(PlanetHeader->PFlags)-1] != 0) && (!Save.PlayMySelf))
                        {
                            INFORMUSER();
                            HANDLEKNOWNPLANET(i+1,1,PlanetHeader);
                        } else {
                            PlanetHeader->ProjectID = 0;
                        }
                    }
                    ActPProjects = PlanetHeader->ProjectPtr;
                    if ((0 < ActPProjects->data[PROJECT_SDI]) && (100 > ActPProjects->data[PROJECT_SDI]))
                        { ActPProjects->data[PROJECT_SDI] = it_round(ActPProjects->data[PROJECT_SDI]*1.2)+1; }

                    if ((0 < ActPProjects->data[PROJECT_SPACEPHALANX]) && (100 > ActPProjects->data[PROJECT_SPACEPHALANX]))
                        { ActPProjects->data[PROJECT_SPACEPHALANX] = it_round(ActPProjects->data[PROJECT_SPACEPHALANX]*1.2)+1; }

                    SystemFlags[ActPlayer-1][i] |= FLAG_KNOWN;

                    PProd = 11+(ActPProjects->data[PROJECT_FUSIONPOWER] +ActPProjects->data[PROJECT_INT_PLANT]
                               +ActPProjects->data[PROJECT_INFO_HIGHWAY]+ActPProjects->data[PROJECT_MICROIDS]
                               +ActPProjects->data[PROJECT_WEATHERSTATION])*6;

                    PCreativity = 20+(ActPProjects->data[PROJECT_PART_ACCEL]     +ActPProjects->data[PROJECT_INTERNET]
                                     +ActPProjects->data[PROJECT_VIRT_UNIVERSITY]+ActPProjects->data[PROJECT_INFO_HIGHWAY]
                                     +ActPProjects->data[PROJECT_WEATHERSTATION])*10;

                    AllCreative[ActPlayer-1] += it_round(PCreativity*(PlanetHeader->Biosphaere/80.0+PlanetHeader->Population/400.0))+1;

                    if (0 == ActPProjects->data[PROJECT_MICROIDS])
                    {       /* Keine MICROIDEN, Biosphaeren-Abbau */
                        l = 4+(  ActPProjects->data[PROJECT_RECYCLINGPLANT]
                                +ActPProjects->data[PROJECT_FUSIONPOWER]
                                +ActPProjects->data[PROJECT_HYDROPOWER]
                                +ActPProjects->data[PROJECT_INT_PLANT]
                                +ActPProjects->data[PROJECT_WEATHERSTATION])*2;

                        if ( (PlanetHeader->Biosphaere >= 2) && (0 == (rand()%l)))
                            { PlanetHeader->Biosphaere -= 2; }

                        if ((PlanetHeader->Biosphaere <= 127) && (PlanetHeader->Population > 30))
                        {
                            /* Biosphaere<73%, Infra-,Industrie- und Bevoelkerungsabbau */
                            if (0 == (rand()%8))
                            {
                                if (PlanetHeader->Infrastruktur >= 2) { PlanetHeader->Infrastruktur -= 2; }
                                if (PlanetHeader->Industrie >= 2)     { PlanetHeader->Industrie     -= 2; }
                            }
                            PlanetHeader->Population = it_round(PlanetHeader->Population*0.95);
                            if ((PlanetHeader->ProjectID != -3) && (Save.CivPlayer[GETCIVVAR(PlanetHeader->PFlags)-1] != 0)
                                && (!Save.PlayMySelf))
                            {
                                DECREASE(i+1,PlanetHeader);
                            }
                        }
                        if ((0 == (rand()%20)) && (PlanetHeader->Infrastruktur > 1)) { PlanetHeader->Infrastruktur--; }
                        if ((0 == (rand()%20)) && (PlanetHeader->Industrie > 1))     { PlanetHeader->Industrie--; }
                    } else {
                        PlanetHeader->Biosphaere    += 2;
                        if (PlanetHeader->Biosphaere    > 200) { PlanetHeader->Biosphaere    = 200; }

                        PlanetHeader->Infrastruktur += 3;
                        if (PlanetHeader->Infrastruktur > 200) { PlanetHeader->Infrastruktur = 200; }

                        PlanetHeader->Industrie     += 2;
                        if (PlanetHeader->Industrie     > 200) { PlanetHeader->Industrie     = 200; }
                    }
                    l = 0;
                    if ((0 == PlanetHeader->ProjectID) && ((Save.CivPlayer[ActPlayer-1] == 0) || Save.PlayMySelf))
                        { PlanetHeader->ProjectID = -1; }

                    if (0 != PlanetHeader->ProjectID)
                    {
                        l = 0;
                        if (PlanetHeader->ProjectID < 0)
                        {
                            switch (PlanetHeader->ProjectID)
                            {
                                case PROJECT_CLEAR_BIOPHERE: {
                                                                PlanetHeader->Biosphaere += 9;
                                                                if (200 < PlanetHeader->Biosphaere)
                                                                {
                                                                    l = 1;
                                                                    PlanetHeader->Biosphaere = 200;
                                                                }
                                                            } break;
                                case PROJECT_REPAIR_INFRA: {
                                                                PlanetHeader->Infrastruktur += 9;
                                                                if (200 < PlanetHeader->Infrastruktur)
                                                                {
                                                                    l = 1;
                                                                    PlanetHeader->Infrastruktur = 200;
                                                                }
                                                            } break;
                                default: {
                                                                PlanetHeader->Industrie += 9;
                                                                if (200 < PlanetHeader->Industrie)
                                                                {
                                                                    l = 1;
                                                                    PlanetHeader->Industrie = 200;
                                                                }
                                                            }
                            }
                        } else {
                            PMoney = 1+it_round(PProd*(PlanetHeader->Infrastruktur/17.0
                                                      +PlanetHeader->Industrie/17.0
                                                      +PlanetHeader->Population/17.0));
                            while (PMoney > MAXPMONEY)
                            {
                                PMoney = it_round(PMoney*0.95);
                            }

                            if (Save.CivPlayer[ActPlayer-1] != 0)
                            {
                                PMoney = it_round(PMoney*(100-Save.JSteuer[ActPlayer-1]-Save.GSteuer[ActPlayer-1]-Save.SService[ActPlayer-1])/100.0);
                            } else {
                                PMoney = it_round(PMoney*(100-Save.JSteuer[ActPlayer-1]-Save.GSteuer[ActPlayer-1])/100.0);
                            }
                            PlanetHeader->XProjectPayed += PMoney;

                            /* *** ueberschuessige Finanzen fuer Planeten ausgeben ****/
                            if ((Save.Staatstopf[ActPlayer-1] > (PlanetHeader->XProjectCosts*10))
                                && (Save.GSteuer[ActPlayer-1] <= 2) && ((Save.CivPlayer[ActPlayer-1] == 0) || Save.PlayMySelf))
                            {
                                Save.Staatstopf[ActPlayer-1] -= (PlanetHeader->XProjectCosts - PlanetHeader->XProjectPayed);
                                PlanetHeader->XProjectPayed   = PlanetHeader->XProjectCosts;
                            }
                            if ((rand()%100)<50)
                            {
                                if (((Save.CivPlayer[ActPlayer-1] == 0) || (Save.PlayMySelf))
                                    && (PlanetHeader->XProjectCosts > PlanetHeader->XProjectPayed)
                                    && (Save.GSteuer[ActPlayer-1]<15) && (Save.Staatstopf[ActPlayer-1]>3000))
                                {
                                    if ((Save.Staatstopf[ActPlayer-1]*0.7) > (Militaerausgaben[ActPlayer-1]*15.0))
                                    {
                                        PlanetHeader->XProjectPayed = it_round(PlanetHeader->XProjectPayed+(Save.Staatstopf[ActPlayer-1]*0.3));
                                        Save.Staatstopf[ActPlayer-1] = it_round(Save.Staatstopf[ActPlayer-1]*0.7);
                                    } else if ((Save.Staatstopf[ActPlayer-1]*0.85) > (Militaerausgaben[ActPlayer-1]*9.0))
                                    {
                                        PlanetHeader->XProjectPayed = it_round(PlanetHeader->XProjectPayed+(Save.Staatstopf[ActPlayer-1]*0.15));
                                        Save.Staatstopf[ActPlayer-1] = it_round(Save.Staatstopf[ActPlayer-1]*0.85);
                                    } else if ((Save.Staatstopf[ActPlayer-1]*0.9 ) > (Militaerausgaben[ActPlayer-1]*8.0))
                                    {
                                        PlanetHeader->XProjectPayed = it_round(PlanetHeader->XProjectPayed+(Save.Staatstopf[ActPlayer-1]*0.1));
                                        Save.Staatstopf[ActPlayer-1] = it_round(Save.Staatstopf[ActPlayer-1]*0.9);
                                    } else if ((Save.Staatstopf[ActPlayer-1] > (Militaerausgaben[ActPlayer-1]*7.0)) && (Year>0))
                                    {
                                        PlanetHeader->XProjectPayed += Year*2;
                                        Save.Staatstopf[ActPlayer-1]  -= Year*2;
                                    }
                                }
                            }

                            if (((PlanetHeader->PFlags & FLAG_CIV_MASK) == FLAG_OTHER) && (PlanetHeader->ProjectID == 7))
                            {
                                PlanetHeader->ProjectID = 0;
                            }
                        }

                        ProjID = PlanetHeader->ProjectID;
                        if ((0 < ProjID) && (abs(PlanetHeader->XProjectPayed) > PlanetHeader->XProjectCosts))
                        {
                            PlanetHeader->XProjectPayed -= PlanetHeader->XProjectCosts;
                            ActPProjects = PlanetHeader->ProjectPtr;
                            if ((PROJECT_SDI == ProjID) || (PROJECT_SPACEPHALANX == ProjID))
                            {
                                ActPProjects->data[ProjID] = 100;
                            } else if (   (8 > ProjID)
                                     || ((24 < ProjID) && (PROJECT_VON_NEUMANN != ProjID) && (PROJECT_NOMORE > ProjID)) )
                            {
                                ActPProjects->data[ProjID]++;
                            } else if  ((7 < ProjID)  && (25 > ProjID))
                            {
                                /* a new ship has been built ... */

                                // create a maques ship of same type if not already too much exists
                                if (MAXMAQUES > MaquesShips)
                                {
                                    (void) CREATEMAQUESSHIP(i, ProjID);
                                }

                                // reserve memory for the new ship and fill in the ship structures
                                ActShipPtr = (r_ShipHeader*) AllocMem(sizeof(r_ShipHeader),MEMF_CLEAR);
                                if (NULL != ActShipPtr)
                                {
                                    *ActShipPtr = (r_ShipHeader) {0,ProjID, PlanetHeader->PFlags & FLAG_CIV_MASK,0,0,0,0,0,0,
                                                                ShipData(ProjID).MaxShield,1,1,
                                                                ShipData(ProjID).MaxMove,0,0,0,NULL,NULL,NULL};
                                                    /*Age,SType,Owner,Flags,ShieldBonus,Ladung,Fracht,PosX,PosY,
                                                    Shield,Weapon,Repair
                                                    Moving,Source,Target,Tactical,TargetShip,BeforeShip,NextShip*/
                                    ActShipPtr->Weapon = WEAPON_GUN;
                                    --ActPlayer;
                                    if (1 > Save.TechCosts[ActPlayer][15]) { ActShipPtr->Weapon = WEAPON_LASER; }
                                    if (1 > Save.TechCosts[ActPlayer][24]) { ActShipPtr->Weapon = WEAPON_PHASER; }
                                    if (1 > Save.TechCosts[ActPlayer][32]) { ActShipPtr->Weapon = WEAPON_DISRUPTOR; }
                                    if (1 > Save.TechCosts[ActPlayer][27]) { ActShipPtr->Weapon = WEAPON_PTORPEDO; }
                                    ++ActPlayer;

                                    if ((21 == ProjID) && (0 == SystemHeader[i].FirstShip.SType)
                                        && ((0 == Save.CivPlayer[GETCIVVAR(ActShipPtr->Owner)-1]) || Save.PlayMySelf))
                                    {
                                        ActShipPtr->PosX = it_round(PlanetHeader->PosX);
                                        ActShipPtr->PosY = it_round(PlanetHeader->PosY);
                                        LINKSHIP(ActShipPtr, &(SystemHeader[i].FirstShip), 0);
                                    } else {
                                        LINKSHIP(ActShipPtr, &(PlanetHeader->FirstShip),0);
                                    }
                                }
                            }
                            if ((8 > ProjID) || (PROJECT_VON_NEUMANN == ProjID))
                            {
                                Save.ProjectCosts[ActPlayer-1][ProjID] = 0;
                            }
                            l = 1;
                        }
                        if (l == 1)
                        {
                            if ((0 < ProjID) && (8 > ProjID))
                            {
                                Save.ImperatorState[ActPlayer-1] += 150;
                                if (0 == Save.stProject[ProjID-1])
                                {
                                    INFORMUSER();
                                    Save.ImperatorState[ActPlayer-1] += 50;
                                    Save.stProject[ProjID-1] = ActPlayer;

                                    ROT_Window=MAKEWINDOWBORDER(85,120,341,81,MyScreen[0]);
                                    if (ROT_Window)
                                    {
                                        RPort_PTR = ROT_Window->RPort;

                                        _s=my_strcpy(s, GETCIVNAME(ActPlayer));
                                        *_s++ = ' ';
                                        (void) my_strcpy(_s, PText[579]); // fuehren als erste
                                        WRITE(171, 7,GETCIVFLAG(ActPlayer),(1|WRITE_Center),RPort_PTR,3,s);

                                        _s=my_strcpy(s, ProjectName[ProjID]);
                                        *_s++ = '-';
                                        *_s = 0;
                                        WRITE(171,29,                   12,(1|WRITE_Center),RPort_PTR,3,s);
                                        // s[0]=0;
                                        _s = s;
                                        if (4 > ProjID)
                                        {
                                            _s=my_strcpy(s, PText[582]); // Projekt
                                            *_s++ = ' ';
                                        }
                                        (void) my_strcpy(_s, PText[580]); // durch
                                        WRITE(171,53,12,(1|WRITE_Center),RPort_PTR,3,s);
                                        if (Save.PlayMySelf) { Delay(PAUSE); }
                                        WAITLOOP(Save.PlayMySelf);

                                        CloseWindow(ROT_Window);
                                    }
                                }
                            } else if ((7 < ProjID) && (25 > ProjID))
                            {
                                if (0 != Save.CivPlayer[ActPlayer-1])
                                {
                                    btx = 1;
                                    XState = 0;
                                    Fight = 0;
                                    for(k = 0; k < 6; ++k)
                                    {
                                        if ((Save.Military[ActPlayer-1] & btx) == btx)
                                        {
                                            XState += i+1;
                                            Fight += 8;
                                        }
                                        btx = btx*2;
                                    }
                                    Save.ImperatorState[ActPlayer-1] -= XState;
                                    ActShipPtr->ShieldBonus = Fight;
                                } else {
                                    ActShipPtr->ShieldBonus = (Level*2)-2;
                                }
                            } else if ((PROJECT_SETTLERS == ProjID)
                                    || ((PROJECT_LANDINGTROOPS < ProjID) && (PROJECT_NOMORE > ProjID)))
                            {
                                Save.ImperatorState[ActPlayer-1] += 10;
                            }
                            if ((PROJECT_SETTLERS == ProjID) || (PROJECT_LANDINGTROOPS == ProjID))
                            {
                                PlanetHeader->Population -= 10;
                            }
                        }

                        if ((l == 1) && (Save.CivPlayer[ActPlayer-1] != 0))
                        {
                            INFORMUSER();
                            ROT_Window=MAKEWINDOWBORDER(85,120,341,81,MyScreen[0]);
                            if (ROT_Window)
                            {
                                RPort_PTR = ROT_Window->RPort;

                                _s=my_strcpy(s, _PT_System);
                                *_s++ = ':';
                                *_s++ = ' ';
                                (void) my_strcpy(_s, Save.SystemName[i]);
                                WRITE(171, 7,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);

                                _s=my_strcpy(s, _PT_Planet);
                                *_s++ = ':';
                                *_s++ = ' ';
                                (void) my_strcpy(_s, PlanetHeader->PName);
                                WRITE(171,27,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);

                                if (PROJECT_NONE < ProjID)
                                {
                                    _s=my_strcpy(s, PText[583]);  // baut
                                    *_s++ = ' ';
                                    (void) my_strcpy(_s, ProjectName[ProjID]);
                                } else {
                                    switch (ProjID)
                                    {
                                        case PROJECT_CLEAR_BIOPHERE: (void) my_strcpy(s, _PT_Biosphaere_gereinigt);    break;
                                        case PROJECT_REPAIR_INFRA:   (void) my_strcpy(s, _PT_Infrastructur_repariert); break;
                                        default: (void) my_strcpy(s, _PT_IndustrAnlage_repariert);
                                    }
                                }
                                WRITE(171,53,12,(1|WRITE_Center),RPort_PTR,3,s);

                                Delay(5);
                                if ((7 < ProjID) && (PROJECT_SPACEDOCK > ProjID)
                                   && (!Save.PlayMySelf))
                                {
                                    // building ships... where to place it?
                                    ROT_Window2=MAKEWINDOWBORDER(85,208,341,41,MyScreen[0]);
                                    if (ROT_Window2)
                                    {
                                        RPort_PTR2 = ROT_Window2->RPort;

                                        BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(ProjID-8)*32,32,RPort_PTR2,10, 4,32,32,192);
                                        DrawImage(RPort_PTR2,&GadImg1, 55,10);
                                        DrawImage(RPort_PTR2,&GadImg1,195,10);

                                        WRITE(113,12,0,WRITE_Center,RPort_PTR2,3,PText[587]);   // Space
                                        WRITE(253,12,0,WRITE_Center,RPort_PTR2,3,PText[588]);   // Orbit
                                        do
                                        {
                                            Delay(RDELAY);
                                            if (LMB_PRESSED)
                                            {
                                                if ((ROT_Window2->MouseY > 9) && (ROT_Window2->MouseY < 31))
                                                {
                                                    if ((ROT_Window2->MouseX > 54) && (ROT_Window2->MouseX < 172))
                                                    {
                                                        KLICKWINGAD(RPort_PTR2,55,10);
                                                        ActShipPtr->PosX = it_round(PlanetHeader->PosX);
                                                        ActShipPtr->PosY = it_round(PlanetHeader->PosY);
                                                        LINKSHIP(ActShipPtr, &(SystemHeader[i].FirstShip), 1);
                                                        break;
                                                    } else if ((ROT_Window2->MouseX > 194) && (ROT_Window2->MouseX < 312))
                                                    {
                                                        KLICKWINGAD(RPort_PTR2,195,10);
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        while (true);
                                        CloseWindow(ROT_Window2);
                                    }
                                } else {
                                    if (Save.PlayMySelf) { Delay(PAUSE); }
                                    WAITLOOP(Save.PlayMySelf);
                                }
                                CloseWindow(ROT_Window);
                            }
                            /* if we did build a ship or a settler or a landing troop, build another one... else no new project is selected */
                            if ((( 8  > PlanetHeader->ProjectID) || (24  < PlanetHeader->ProjectID))
                              && (PROJECT_SETTLERS      != PlanetHeader->ProjectID)
                              && (PROJECT_LANDINGTROOPS != PlanetHeader->ProjectID))
                            {
                                PlanetHeader->ProjectID = PROJECT_NONE;
                            }
                            if (!Save.PlayMySelf)
                            {
                                HANDLEKNOWNPLANET(i+1, 1, PlanetHeader);
                            }
                        }

                        if (((l == 1) || ((PROJECT_CLEAR_BIOPHERE <= PlanetHeader->ProjectID) && (PROJECT_NONE >= PlanetHeader->ProjectID)))
                            && ((Save.CivPlayer[ActPlayer-1] == 0) || Save.PlayMySelf))
                        {
                            PlanetHeader->ProjectID = PROJECT_NONE;
                            for(k = 8; k <= 24; k++)
                            {
                                if ((Save.TechCosts[ActPlayer-1][ProjectNeedsTech[k]] <= 0)
                                    && (ActPProjects->data[ProjectNeedsProject[k]]>0))
                                {
                                    if (((k == 21) && (PlanetHeader->ProjectID<18))
                                        || ((k  > 21) && ((SystemHeader[i].FirstShip.SType & TARGET_STARGATE) == TARGET_STARGATE))
                                        ||  (k <= 21))
                                    {
                                        PlanetHeader->ProjectID = k;
                                    }
                                    if (((PlanetHeader->ProjectID==8) || (PlanetHeader->ProjectID==9))
                                        && (Save.WarPower[ActPlayer-1]>40))
                                    {
                                        PlanetHeader->ProjectID = PROJECT_NONE;
                                    }
                                }
                            }
                            if ((ActPlayer == 8) && (Save.WarPower[7]>8000))
                            {
                                PlanetHeader->ProjectID = PROJECT_NONE;
                            }

                            /* *** DCON`s begrenzen *** */
                            if (NULL != PlanetHeader->FirstShip.NextShip)
                            {
                                if (NULL != PlanetHeader->FirstShip.NextShip->NextShip)
                                {
                                    MyShipPtr = PlanetHeader->FirstShip.NextShip->NextShip;
                                    if ((LEVEL_WAR == Save.WarState[ActPlayer-1][0]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][0]) ||
                                        (LEVEL_WAR == Save.WarState[ActPlayer-1][1]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][1]) ||
                                        (LEVEL_WAR == Save.WarState[ActPlayer-1][2]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][2]) ||
                                        (LEVEL_WAR == Save.WarState[ActPlayer-1][3]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][3]) ||
                                        (LEVEL_WAR == Save.WarState[ActPlayer-1][4]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][4]) ||
                                        (LEVEL_WAR == Save.WarState[ActPlayer-1][5]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][5]) ||
                                        (LEVEL_WAR == Save.WarState[ActPlayer-1][6]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][6]) ||
                                        (LEVEL_WAR == Save.WarState[ActPlayer-1][7]) || (LEVEL_COLDWAR == Save.WarState[ActPlayer-1][7]))
                                    {
                                        /* Krieg mit anderer Zivi */
                                        if (1 > ActPProjects->data[PROJECT_LANDINGTROOPS])
                                        {
                                            PlanetHeader->ProjectID = PROJECT_LANDINGTROOPS;
                                        }
                                        while ((NULL != MyShipPtr) && (NULL != MyShipPtr->NextShip))
                                        {
                                            if ((0 == (MyShipPtr->Ladung & MASK_LTRUPPS))
                                                && (0 < ActPProjects->data[PROJECT_LANDINGTROOPS])
                                                && (ShipData(MyShipPtr->SType).MaxLoad >
                                                    (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                            {
                                                ActPProjects->data[PROJECT_LANDINGTROOPS]--;
                                                MyShipPtr->Ladung++;
                                            }
                                            ActShipPtr = MyShipPtr->NextShip;
                                            MyShipPtr->PosX = it_round(PlanetHeader->PosX);
                                            MyShipPtr->PosY = it_round(PlanetHeader->PosY);
                                            LINKSHIP(MyShipPtr, &(SystemHeader[i].FirstShip), 1);
                                            FINDENEMYOBJECT(i,MyShipPtr);
                                            MyShipPtr = ActShipPtr;
                                        }
                                    } else if (((SystemHeader[i].State & STATE_TACTICAL) != STATE_ALL_OCC)
                                                && (Save.GlobalFlags[ActPlayer-1] == GFLAG_EXPLORE))
                                    {
                                        /* *** Noch nicht alle Planeten des Systems besiedelt! *** */
                                        if (ActPProjects->data[PROJECT_SETTLERS] >= ShipData(MyShipPtr->SType).MaxLoad)
                                        {
                                            for(k = 1; k <= ShipData(MyShipPtr->SType).MaxLoad; k++)
                                            {
                                                if ((ActPProjects->data[PROJECT_SETTLERS]>0)
                                                    && (ShipData(MyShipPtr->SType).MaxLoad >
                                                        (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                                {
                                                    ActPProjects->data[PROJECT_SETTLERS]--;
                                                    MyShipPtr->Ladung += 16;
                                                }
                                            }
                                            MyShipPtr->PosX = it_round(PlanetHeader->PosX);
                                            MyShipPtr->PosY = it_round(PlanetHeader->PosY);
                                            l = GOTONEXTPLANET(i,MyShipPtr)+1;
                                        } else {
                                            PlanetHeader->ProjectID = PROJECT_SETTLERS;
                                        }
                                    } else if (((SystemHeader[i].State & STATE_TACTICAL) == STATE_ALL_OCC)
                                                && (Save.GlobalFlags[ActPlayer-1] == GFLAG_EXPLORE))
                                    {
                                        /* *** Expedition in andere Galaxie *** */
                                        if ((rand()%100)<15)
                                        {
                                            l = 2;
                                        } else {
                                            if ((ActPProjects->data[PROJECT_SETTLERS]+ActPProjects->data[PROJECT_LANDINGTROOPS]) > ShipData(MyShipPtr->SType).MaxLoad)
                                            {
                                                MyShipPtr->PosX = it_round(PlanetHeader->PosX);
                                                MyShipPtr->PosY = it_round(PlanetHeader->PosY);
                                                for(k = 1; k <= ShipData(MyShipPtr->SType).MaxLoad; k++)
                                                {
                                                    if ((ActPProjects->data[PROJECT_SETTLERS]>1)
                                                        && (ShipData(MyShipPtr->SType).MaxLoad >
                                                        (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                                    {
                                                        ActPProjects->data[PROJECT_SETTLERS]--;
                                                        MyShipPtr->Ladung += 16;
                                                    }
                                                    if ((ActPProjects->data[PROJECT_LANDINGTROOPS]>1)
                                                        && (ShipData(MyShipPtr->SType).MaxLoad >
                                                        (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                                    {
                                                        ActPProjects->data[PROJECT_LANDINGTROOPS]--;
                                                        MyShipPtr->Ladung++;
                                                    }
                                                }
                                                l = GOTONEXTSYSTEM(i,MyShipPtr);
                                            } else {
                                                if (ActPProjects->data[PROJECT_SETTLERS]<ShipData(MyShipPtr->SType).MaxLoad)
                                                {
                                                    PlanetHeader->ProjectID = PROJECT_SETTLERS;
                                                }
                                                if (2 > ActPProjects->data[PROJECT_LANDINGTROOPS])
                                                {
                                                    PlanetHeader->ProjectID = PROJECT_LANDINGTROOPS;
                                                } else {
                                                    PlanetHeader->ProjectID = PROJECT_SETTLERS;
                                                }
                                            }
                                        }
                                    } else if (Save.GlobalFlags[ActPlayer-1] == GFLAG_ATTACK)
                                    {
                                        /* Alles Besetzt, Krieg beginnen */
                                        l = 0;
                                        for(k = 1; k < MAXCIVS; ++k)
                                        {
                                            if ((k != ActPlayer) && (LEVEL_NO_ALLIANZ == Save.WarState[ActPlayer-1][k-1]))
                                                { l = k; }
                                        }
                                        if (0 == l)
                                        {
                                            for(k = 1; k < (MAXCIVS-1); ++k)
                                            {
                                                if ((ActPlayer != k)
                                                    && (LEVEL_UNKNOWN != Save.WarState[ActPlayer-1][k-1])
                                                    && (LEVEL_DIED    != Save.WarState[ActPlayer-1][k-1]))
                                                    { l = k; }
                                            }
                                        }
                                        if (0 != l)
                                        {
                                            Save.WarState[ActPlayer-1][l-1] = LEVEL_COLDWAR;
                                        }
                                    }
                                    if (NULL != PlanetHeader->FirstShip.NextShip)
                                    {
                                        if ((NULL != PlanetHeader->FirstShip.NextShip->NextShip)
                                            && ((SystemHeader[i].State & STATE_TACTICAL) == STATE_ALL_OCC)
                                            && (0 == ActPProjects->data[PROJECT_SETTLERS]))
                                        {
                                            if (((55 > (PlanetHeader->Water / PlanetHeader->Size)) && EXISTSPLANET(ActPlayer, i, 1))
                                             || ((56 < (PlanetHeader->Water / PlanetHeader->Size)) && EXISTSPLANET(ActPlayer, i, 2)))
                                            {
                                                MyShipPtr = PlanetHeader->FirstShip.NextShip->NextShip;
                                                MyShipPtr->PosX = it_round(PlanetHeader->PosX);
                                                MyShipPtr->PosY = it_round(PlanetHeader->PosY);
                                                LINKSHIP(MyShipPtr, &(SystemHeader[i].FirstShip), 1);
                                            } else if ((PlanetHeader->FirstShip.NextShip->SType - PlanetHeader->FirstShip.NextShip->NextShip->SType) > 3)
                                            {
                                                PlanetHeader->FirstShip.NextShip->NextShip->Owner = 0;
                                            }
                                        }
                                    }
                                }
                            }


                            if ((rand()%100)<35)
                            {
                                l = 2;
                            } else {
                                l = 0;
                            }
                            if ((Year>2000) && (PlanetHeader->ProjectID>=8) && (PlanetHeader->ProjectID<=24)
                                && (Save.WarPower[ActPlayer-1]<(Year / 30)) && ((rand()%100)<65))
                            {
                                l = 0;
                            }
                            if ((2 == l) || (0 == PlanetHeader->ProjectID))
                            {
                                for (k = 22; k>=1; --k)
                                {
                                    if ((Save.TechCosts[ActPlayer-1][ProjectNeedsTech[PriorityList[k]]] <= 0)
                                            /* Technologie vorhanden */
                                        && ((ActPProjects->data[ProjectNeedsProject[PriorityList[k]]]>0)
                                        || (Save.ProjectCosts[ActPlayer-1][ProjectNeedsProject[PriorityList[k]]] == 0)))
                                    {
                                        /* evtl. noetiges Projekt vorhanden */
                                        if ((PriorityList[k]>=1) && (PriorityList[k]<=7)
                                            && (Save.ProjectCosts[ActPlayer-1][ProjectNeedsProject[PriorityList[k]]] == 0)
                                            && (Save.ProjectCosts[ActPlayer-1][PriorityList[k]]>0))
                                        {
                                            PlanetHeader->ProjectID = PriorityList[k];
                                            /* Großprojekt noch nicht gebaut */
                                        }
                                        if (((PriorityList[k]==25) || ((PriorityList[k]>=28) && (PriorityList[k]<=42)))
                                            && (ActPProjects->data[PriorityList[k]] == 0))
                                        {
                                            PlanetHeader->ProjectID = PriorityList[k];
                                            /* Projekt noch nicht gebaut */
                                        }
                                    }
                                }
                            }
                            if (Save.ProjectCosts[ActPlayer-1][1]>0)
                                { PlanetHeader->ProjectID = 1; }
                            if (0 == PlanetHeader->ProjectID)
                            {
                                if (200 > PlanetHeader->Industrie)     { PlanetHeader->ProjectID = PROJECT_REPAIR_INDUSTRY; }
                                if (200 > PlanetHeader->Infrastruktur) { PlanetHeader->ProjectID = PROJECT_REPAIR_INFRA; }
                                if (200 > PlanetHeader->Biosphaere)    { PlanetHeader->ProjectID = PROJECT_CLEAR_BIOPHERE; }
                            } else if (0 < PlanetHeader->ProjectID)
                            {
                                PlanetHeader->XProjectCosts = Save.ProjectCosts[ActPlayer-1][PlanetHeader->ProjectID];
                            }
                        }
                        if ((0 == Save.CivPlayer[ActPlayer-1]) || (Save.PlayMySelf))
                        {
                            if (170 > PlanetHeader->Industrie)     { PlanetHeader->ProjectID = PROJECT_REPAIR_INDUSTRY; }
                            if (170 > PlanetHeader->Infrastruktur) { PlanetHeader->ProjectID = PROJECT_REPAIR_INFRA; }
                            if (170 > PlanetHeader->Biosphaere)    { PlanetHeader->ProjectID = PROJECT_CLEAR_BIOPHERE; }
                        }
                        PMoney = it_round(PProd*(PlanetHeader->Infrastruktur/17.0+PlanetHeader->Industrie/17.0+PlanetHeader->Population/17.0))+1;
                        while (PMoney>MAXPMONEY)
                        {
                            PMoney = it_round(PMoney*0.95);
                        }
                        Save.Staatstopf[ActPlayer-1] += it_round(PMoney*Save.GSteuer[ActPlayer-1]/100.0);
                        if (WFLAG_JAHADR == Save.WorldFlag)
                        {
                            Save.Staatstopf[7] += it_round(PMoney*Save.JSteuer[ActPlayer-1]/100.0);
                        }
                        /* Steuerentrichtung */
                    } else {
                        PMoney = it_round(PProd*(PlanetHeader->Infrastruktur/17.0+PlanetHeader->Industrie/17.0+PlanetHeader->Population/17.0))+1;
                        while (PMoney>MAXPMONEY)
                        {
                            PMoney = it_round(PMoney*0.95);
                        }
                        Save.Staatstopf[ActPlayer-1] += it_round(PMoney/3.0);
                        if (WFLAG_JAHADR == Save.WorldFlag)
                        {
                            Save.Staatstopf[7] += it_round(PMoney*Save.JSteuer[ActPlayer-1]/100.0);
                        }
                        /* Steuerentrichtung */
                        /* keine Produktion */
                    }
                    if (0 != Save.CivPlayer[ActPlayer-1])
                    {
                        do
                        {
                            k = (rand()%(MAXCIVS-1))+1;
                        }
                        while (k == ActPlayer);
                        if ((LEVEL_DIED != Save.WarState[ActPlayer-1][k-1]) && (LEVEL_UNKNOWN != Save.WarState[ActPlayer-1][k-1]))
                        {
                            Save.SSMoney[ActPlayer-1][k-1] += it_round(PMoney/100.0*Save.SService[ActPlayer-1]);
                        }
                        if ((0 == (Year % 5)) && (0 == (rand()%200)))
                        {
                            Save.SSMoney[ActPlayer-1][k-1] = 0;
                        }
                    }
                }
            }

            l = 0;
            if (SystemOwn[1]>0)            { l = FLAG_TERRA; }
            if (SystemOwn[2]>SystemOwn[1]) { l = FLAG_KLEGAN; SystemOwn[1] = SystemOwn[2]; }
            if (SystemOwn[3]>SystemOwn[1]) { l = FLAG_REMALO; SystemOwn[1] = SystemOwn[3]; }
            if (SystemOwn[4]>SystemOwn[1]) { l = FLAG_CARDAC; SystemOwn[1] = SystemOwn[4]; }
            if (SystemOwn[5]>SystemOwn[1]) { l = FLAG_FERAGI; SystemOwn[1] = SystemOwn[5]; }
            if (SystemOwn[6]>SystemOwn[1]) { l = FLAG_BAROJA; SystemOwn[1] = SystemOwn[6]; }
            if (SystemOwn[7]>SystemOwn[1]) { l = FLAG_VOLKAN; SystemOwn[1] = SystemOwn[7]; }
            if (SystemOwn[8]>SystemOwn[1]) { l = FLAG_OTHER; }

            SystemFlags[0][i] = (SystemFlags[0][i] & FLAG_KNOWN) | l;
            if (0 == (SystemFlags[0][i] & FLAG_CIV_MASK))
                { FreeSystem = true; }
            if (FreeSystem)
            {
                SystemHeader[i].State &= (~STATE_ALL_OCC);
            }
        }
    }
    ActPlayer--;
    if ((FreeSystem) &&
        (Save.WarState[ActPlayer][0] != LEVEL_WAR) && (Save.WarState[ActPlayer][1] != LEVEL_WAR) &&
        (Save.WarState[ActPlayer][2] != LEVEL_WAR) && (Save.WarState[ActPlayer][3] != LEVEL_WAR) &&
        (Save.WarState[ActPlayer][4] != LEVEL_WAR) && (Save.WarState[ActPlayer][5] != LEVEL_WAR) &&
        (Save.WarState[ActPlayer][6] != LEVEL_WAR) && (Save.WarState[ActPlayer][7] != LEVEL_WAR))
    {
        Save.GlobalFlags[ActPlayer] = GFLAG_EXPLORE;
    }
    ActPlayer++;
    Valid = true;
    REFRESHDISPLAY();
    if (ActPlayer == 1)
    {
        ActPlayer--;
        if ((Year % 50) == 0)
        {
            for(i = 0; i < (MAXCIVS-1); i++)
            {
                if ((Save.WarState[ActPlayer][i]==LEVEL_ALLIANZ)
                 || (Save.WarState[ActPlayer][i]==LEVEL_NO_ALLIANZ))
                {
                    Save.WarState[i][ActPlayer] = LEVEL_PEACE;
                    Save.WarState[ActPlayer][i] = LEVEL_PEACE;
                }
            }
        }
        ActPlayer++;
    }
    PRINTGLOBALINFOS(ActPlayer-1);
    if ((((Year % (18+ActPlayer)) == 0) || ((Save.WorldFlag!=0) && (Save.WorldFlag!=WFLAG_CEBORC)
                        && (Save.WorldFlag!=WFLAG_FIELD) && (ActPlayer == 8) && ((Year % 8) == 0)))
        && (Save.CivPlayer[ActPlayer-1] == 0))
    {
        // i shifted -1 ... > ActPlayer shift -1
        ActPlayer--;
        do
        {
            i = (rand()%7);
        }
        while (i == ActPlayer);
        if ((Save.WarState[ActPlayer][i]!=LEVEL_DIED)   && (Save.WarState[ActPlayer][i]!=LEVEL_UNKNOWN)
          &&(Save.WarState[ActPlayer][i]!=LEVEL_ALLIANZ)&& (Save.WarState[ActPlayer][i]!=LEVEL_WAR)
          &&(Save.WarPower[i] < Save.WarPower[ActPlayer]))
        {
            Save.WarState[ActPlayer][i] = LEVEL_COLDWAR;
        }
        ActPlayer++;
    }
    if (((Year % 18) == 0) && (Save.WarPower[1]>42) && (ActPlayer == 2)
        && (Save.CivPlayer[ActPlayer-1] == 0))
    {
        // i == 2 ... shifted to i == 1
        do
        {
            i = (rand()%7);
        }
        while (i == 1);

        if ((Save.WarState[1][i]!=LEVEL_DIED)   && (Save.WarState[1][i]!=LEVEL_UNKNOWN)
          &&(Save.WarState[1][i]!=LEVEL_ALLIANZ)&& (Save.WarState[1][i]!=LEVEL_WAR))
        {
            Save.WarState[1][i] = LEVEL_COLDWAR;
        }
    }
    if (((Year % 17) == 0) && (Save.WarPower[3]>47) && (ActPlayer == 4)
        && (Save.CivPlayer[ActPlayer-1] == 0))
    {
        // i == 4 ... shifted to i == 3
        do
        {
            i = (rand()%7);
        }
        while (i == 3);
        if ((Save.WarState[3][i]!=LEVEL_DIED)   && (Save.WarState[3][i]!=LEVEL_UNKNOWN)
          &&(Save.WarState[3][i]!=LEVEL_WAR))
        {
            Save.WarState[3][i] = LEVEL_COLDWAR;
        }
    }

    if (ActPlayer<MAXCIVS)
    {
        if ((Year>1901) || (Year<0))
        {
            if (Save.ActTech[ActPlayer-1] != 0)
            { /* Forschung */
                if (AllCreative[ActPlayer-1] < 0)
                {
                    Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] = 0;
                } else {
                    Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] -= AllCreative[ActPlayer-1];
                }
                if (Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] <= 0)
                {
                    Save.ImperatorState[ActPlayer-1] += 50;
                    if (Save.CivPlayer[ActPlayer-1] != 0)
                    {
                        INFORMUSER();
                        DISPLAYTECH(Save.ActTech[ActPlayer-1]);
                    }
                    Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] = 0;
                }
            }
            if ((Save.CivPlayer[ActPlayer-1] == 0) || (Save.PlayMySelf))
            {
                if (Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] <= 0)
                {
                    Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] = 0;
                    l = Save.ActTech[ActPlayer-1];
                    Save.ActTech[ActPlayer-1] = 0;
                    do
                    {
                        if      (l == 0)  { j = 3; }
                        else if (l == 3)  { j = 5; }
                        else if (l == 5)  { j = 11; }
                        else if (l == 11) { j = 4; }
                        else if (l == 4)  { j = 16; }
                        else              { j = (rand()%42)+1; }
                        if (Save.TechCosts[ActPlayer-1][j]>0)
                        {
                            if (TechUse1[j] == 0)
                            {
                                Save.ActTech[ActPlayer-1] = j;
                            } else if ((Save.TechCosts[ActPlayer-1][TechUse1[j]] <= 0)
                                    && (Save.TechCosts[ActPlayer-1][TechUse2[j]] <= 0))
                            {
                                Save.ActTech[ActPlayer-1] = j;
                            }
                       }
                       l = -1;
                    }
                    while ((Save.ActTech[ActPlayer-1] == 0) && (Save.TechCosts[ActPlayer-1][42] > 0));
                }
                if ((ActPlayer == 8) && (Save.ActTech[ActPlayer-1] == 42))
                    { Save.ActTech[ActPlayer-1] = 0; }

                if ((Save.ActTech[ActPlayer-1] == 42) && (Save.ProjectCosts[ActPlayer-1][6]>0))
                    { Save.ActTech[ActPlayer-1] = 0; }

            } else if ((Save.ActTech[ActPlayer-1] == 0) || (Save.TechCosts[ActPlayer-1][Save.ActTech[ActPlayer-1]] <= 0))
            {
                Save.ActTech[ActPlayer-1] = 0;
                for(j = 0; j < 6; ++j)
                {
                    NewTech[j] = 0;
                }
                j = 0;
                i = 0;
                do
                {
                    ++i;
                    if (0 < Save.TechCosts[ActPlayer-1][i])
                    {
                        if ( (0 == TechUse1[i])
                             || (   (0 >= Save.TechCosts[ActPlayer-1][TechUse1[i]])
                                 && (0 >= Save.TechCosts[ActPlayer-1][TechUse2[i]])) )
                        {
                            NewTech[j++] = i;
                        }
                    }
                }
                while ((42 > i) && (6 > j));

                if (0 < j)
                {
                    if ((42 == NewTech[0]) && (0 < Save.ProjectCosts[ActPlayer-1][6]))
                    {
                        if (0 == (Year % 13))
                        {
                            ROT_Window=MAKEWINDOWBORDER(100,100,311,81,MyScreen[0]);
                            RPort_PTR = ROT_Window->RPort;


                            WRITE(156,10,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[590]); // Wissenschaftler fordern
                            WRITE(156,32,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[591]); // Durchführung des
                            WRITE(156,54,           12,WRITE_Center,RPort_PTR,3,PText[592]); // GENESIS-Projects
                            WAITLOOP(false);
                            CloseWindow(ROT_Window);
                        }
                    }
                    INFORMUSER();
                    ROT_Window=MAKEWINDOWBORDER(60,100,309,168,MyScreen[0]);
                    RPort_PTR = ROT_Window->RPort;


                    WRITE(154, 10,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[593]); // Was soll entwickelt werden?
                    for(j = 0; j < 6; ++j)
                    {
                        if (0 < NewTech[j])
                        {
                            WRITE(10,j*20+40,12,0,RPort_PTR,3,TechnologyName[NewTech[j]]);
                        }
                    }
                    i = 255;
                    do
                    {
                        Delay(RDELAY);
                        j = (int) ((ROT_Window->MouseY-40) / 20);
                        if ((i != j) && (0 <= j) && (6 > j))
                        {
                            i = j;
                            for(j = 0; j < 6; ++j)
                            {
                                if (0 < NewTech[j])
                                {
                                    WRITE(10,j*20+40,12,0,RPort_PTR,3,TechnologyName[NewTech[j]]);
                                }
                            }
                            if (0 < NewTech[i])
                            {
                                WRITE(10,i*20+40,ActPlayerFlag,0,RPort_PTR,3,TechnologyName[NewTech[i]]);
                            }
                        }
                        if (LMB_PRESSED)
                        {
                            PLAYSOUND(0,300);
                            if (6 > i)
                            {
                                if (0 < NewTech[i])
                                {
                                    Save.ActTech[ActPlayer-1] = NewTech[i];
                                }
                            }
                        }
                    }
                    while (0 >= Save.ActTech[ActPlayer-1]);
                    CloseWindow(ROT_Window);
                }
            }
            for(i = 0; i < (MAXCIVS-1); i++)
            {
                if ((ActPlayer == 1) && ((i<7) || (Save.WorldFlag != 0)))
                {
                    if ((Save.JSteuer[i]+Save.GSteuer[i])>100)
                    {
                        Save.GSteuer[i] = 100-Save.JSteuer[i];
                    }
                    if ((Save.JSteuer[i]+Save.GSteuer[i]+Save.SService[i])>100)
                    {
                        Save.SService[i] = 0;
                    }
                    if ((0 == Save.CivPlayer[i]) || (Save.PlayMySelf))
                    {
                        if (((Save.Staatstopf[i]<(Militaerausgaben[i]*7)) || (Save.Staatstopf[i]<3000))
                            && ((Save.GSteuer[i]+Save.JSteuer[i])<90))
                        {
                            ++Save.GSteuer[i];
                            if ((Save.Staatstopf[i]<0) && ((Save.GSteuer[i]+Save.JSteuer[i]+3) <= 100))
                            {
                                Save.GSteuer[i] += 3;
                            }
                        } else {
                            if ((Save.Staatstopf[i]>0)       && (Save.GSteuer[i]>0)) { Save.GSteuer[i]--; }
                            if ((Save.Staatstopf[i]>2500000) && (Save.GSteuer[i]>5)) { Save.GSteuer[i] -= 5; }
                        }
                        if ((Save.Staatstopf[i]<(-10000)) && (Save.GSteuer[i]>20))
                        {
                            btx = FINDMONEYPLANET(GETCIVFLAG(i+1),i, s);
                        }
                        if ((Save.Bevoelkerung[i]<1000) && (Save.Staatstopf[i]<0))   { Save.Staatstopf[i] = 0; }
                    }
                    for(j = 0; j < MAXCIVS; j++)
                    {
                        if ((i != j) && (Save.Bevoelkerung[i] > Save.Bevoelkerung[j]))
                        {
                            Save.ImperatorState[i] += 2;
                        }
                    }
                    if (0 == (Year % 10)) { DOINFLATION(i); }
                }
                if ((Save.CivPlayer[i] == 0) && (Save.CivPlayer[ActPlayer-1] != 0)
                    && (!Save.PlayMySelf))
                {
                    for(j = 0; j < (MAXCIVS-1); j++)
                    {
                        if ((j != i) && (Save.WarState[j][i] == LEVEL_WAR) && (Save.WarState[j][ActPlayer-1] == LEVEL_WAR)
                            && (Save.WarState[ActPlayer-1][i]!=LEVEL_WAR)     && (Save.WarState[ActPlayer-1][i]!=LEVEL_ALLIANZ)
                            && (Save.WarState[ActPlayer-1][i]!=LEVEL_UNKNOWN) && (Save.WarState[ActPlayer-1][i]!=LEVEL_NO_ALLIANZ))
                        {
                            MAKEBORDER(MyScreen[0],85,120,425,265,12,6,0);
                            WRITE_RP0(256,136,GETCIVFLAG(i+1),(1|WRITE_Center),3,GETCIVNAME(i+1));
                            WRITE_RP0(256,156,12,(1|WRITE_Center),3,PText[597]);
                            WRITE_RP0(256,176,GETCIVFLAG(j+1),(1|WRITE_Center),3,GETCIVNAME(j+1));
                            WRITE_RP0(256,196,12,(1|WRITE_Center),3,PText[598]);
                            DrawImage(&(MyScreen[0]->RastPort),&GadImg1,105,225);
                            DrawImage(&(MyScreen[0]->RastPort),&GadImg1,290,225);
                            WRITE_RP0(162,227,8,WRITE_Center,3,_PT_Annehmen);
                            WRITE_RP0(348,227,8,WRITE_Center,3,_PT_Ablehnen);
                            while (LMB_PRESSED) { };
                            do
                            {
                               Delay(RDELAY);
                            }
                            while (LMB_NOTPRESSED || (MouseX(0)<105) || ((MouseX(0)>221) && (MouseX(0)<290)) || (MouseX(0)>406)
                                                  || (MouseY(0)<225) || (MouseY(0)>245));

                            if ((MouseX(0)>=105) && (MouseX(0)<=221))
                            {
                                KLICKGAD(105,225);
                                Save.WarState[i][ActPlayer-1] = LEVEL_ALLIANZ;
                                Save.WarState[ActPlayer-1][i] = LEVEL_ALLIANZ;
                            } else {
                                KLICKGAD(290,225);
                                Save.WarState[i][ActPlayer-1] = LEVEL_NO_ALLIANZ;
                                Save.WarState[ActPlayer-1][i] = LEVEL_NO_ALLIANZ;
                                if (((i==1) || (i==2)) && ((rand()%3) == 0) && (Save.WarPower[i]>Save.WarPower[ActPlayer-1]))
                                {
                                    AUTOVERHANDLUNG(i+1,ActPlayer,MODE_BELEIDIGUNG);
                                }
                            }
                            RECT_RP0_C0(85,120,425,265);
                            REFRESHDISPLAY();
                        }
                    }
                }
                if (((Save.Bevoelkerung[i] == 0) && (Save.WarState[ActPlayer-1][i]!=LEVEL_DIED)
                    && (Save.WarState[ActPlayer-1][i]!=LEVEL_UNKNOWN) && (i<7))
                 || ((Save.Bevoelkerung[7] == 0) && (i == 7) && (Save.WorldFlag!=0) && (Save.WorldFlag!=WFLAG_FIELD)))
                {
                    Save.WarState[ActPlayer-1][i] = LEVEL_DIED;
                    Save.WarState[i][i] = LEVEL_DIED;
                    if (0 != Save.CivPlayer[ActPlayer-1])
                    {
                        INFORMUSER();
                        MAKEBORDER(MyScreen[0],85,120,425,210,12,6,0);
                        _s=my_strcpy(s, PText[600]);
                        *_s++ = ' ';
                        (void) my_strcpy(_s, GETCIVNAME(i+1));
                        WRITE_RP0(256,140,GETCIVFLAG(i+1),(1|WRITE_Center),3,s);
                        WRITE_RP0(256,165,12,(1|WRITE_Center),3,PText[601]);
                        if (Save.PlayMySelf) { Delay(PAUSE); }
                        WAITLOOP(Save.PlayMySelf);
                        RECT_RP0_C0(85,120,425,210);
                        REFRESHDISPLAY();
                    }
                    if (i == 7)
                    {
                        for(j = 0; j < 8; j++)
                        {
                            Save.JSteuer[j] = 0;
                        }
                        Save.WorldFlag = 0;
                    }
                    Save.JSteuer[i] = 0;
                }
            }
        }
    }
    srand((unsigned) time(&t));

    l = 0;
    if ((Save.WorldFlag == WFLAG_JAHADR) &&
        (Save.WarState[ActPlayer-1][7] != LEVEL_WAR) &&
        (Save.WarState[ActPlayer-1][7] != LEVEL_COLDWAR) &&
        (0 == (Year % 5)))
    {
        for(i = 1; i < (MAXCIVS-1); i++)
        {
            if ((0 == l) && (i != ActPlayer) && (Save.JSteuer[i-1]>0)
                && (Save.WarState[ActPlayer-1][i-1] == LEVEL_WAR))
            {
                if ((0 != Save.CivPlayer[ActPlayer-1]) && (!Save.PlayMySelf))
                {
                    MAKEBORDER(MyScreen[0],80,120,430,265,12,6,0);
                    WRITE_RP0(256,136,FLAG_OTHER,(1|WRITE_Center),3,PText[602]);
                    WRITE_RP0(256,156,FLAG_OTHER,(1|WRITE_Center),3,PText[603]);
                    _s=my_strcpy(s, GETCIVNAME(i));
                    *_s++ = ' ';
                    (void) my_strcpy(_s, PText[604]);
                    WRITE_RP0(256,176,FLAG_OTHER,(1|WRITE_Center),3,s);
                    _s=my_strcpy(s, PText[605]);
                    *_s++ = ' ';
                    (void) dez2out(abs(Year)*11, 0, _s);
                    l = abs(Year)*11;
                    WRITE_RP0(256,196,FLAG_OTHER,(1|WRITE_Center),3,s);
                    DrawImage(&(MyScreen[0]->RastPort),&GadImg1,105,225);
                    DrawImage(&(MyScreen[0]->RastPort),&GadImg1,290,225);
                    WRITE_RP0(162,227,8,WRITE_Center,3,_PT_Annehmen);
                    WRITE_RP0(348,227,8,WRITE_Center,3,_PT_Ablehnen);
                    do
                    {
                        Delay(RDELAY);
                    }
                    while (LMB_NOTPRESSED || (((MouseX(0)<105) || (MouseX(0)>221)) && ((MouseX(0)<290) || (MouseX(0)>406)))
                                          ||  ((MouseY(0)<225) || (MouseY(0)>245)));
                    if ((MouseX(0)>=105) && (MouseX(0)<=221))
                    {
                        KLICKGAD(105,225);
                        Save.Staatstopf[i-1]         += l;
                        Save.Staatstopf[ActPlayer-1] -= l;
                        GOTOPEACE(i-1, ActPlayer-1);
                        if ((0   < Save.JSteuer[ActPlayer-1]) &&
                            (100 > Save.JSteuer[ActPlayer-1]))
                        {
                            Save.JSteuer[ActPlayer-1]++;
                        }
                    } else {
                        KLICKGAD(290,225);
                        Save.WarState[7][ActPlayer-1] = LEVEL_COLDWAR;
                    }
                    RECT_RP0_C0(80,120,430,265);
                    REFRESHDISPLAY();
                } else if (Save.CivPlayer[i-1] == 0)
                {
                    if (Save.JSteuer[ActPlayer-1]>0)
                    {
                        GOTOPEACE(i-1, ActPlayer-1);
                        if ((Save.JSteuer[ActPlayer-1]>=1) && (Save.JSteuer[ActPlayer-1]<=99))
                        {
                            Save.JSteuer[ActPlayer-1]++;
                        }
                        l = 1;
                    } else {
                        Save.WarState[7][ActPlayer-1] = LEVEL_WAR;
                        Save.WarState[ActPlayer-1][7] = LEVEL_WAR;
                        l = 1;
                    }
                }
            }
        }
    }

    if ((Save.WarState[7][ActPlayer-1] == LEVEL_WAR) ||
        (Save.WarState[ActPlayer-1][7] == LEVEL_WAR))
    {
        Save.JSteuer[ActPlayer-1] = 0;
    }

    AUTOSHIPTRAVEL(Display, MODE_ALL, NULL);

    if ((Save.Staatstopf[ActPlayer-1] < (Militaerausgaben[ActPlayer-1]*3)) && ((Year % 2) == 0)
        && (Save.CivPlayer[ActPlayer-1] != 0) && (Save.Staatstopf[ActPlayer-1] >= 0)
        && (!Save.PlayMySelf))
    {
        INFORMUSER();
        REQUEST(PText[607],PText[608],ActPlayerFlag,ActPlayerFlag);
    }
    if (Save.Staatstopf[ActPlayer-1]<0)
    {
        Save.Staatstopf[ActPlayer-1] = it_round(Save.Staatstopf[ActPlayer-1]*1.1);
        if (Save.CivPlayer[ActPlayer-1] != 0)
        {
            PRINTGLOBALINFOS(ActPlayer-1);
        }
    }
    if ((Save.Staatstopf[ActPlayer-1]<=-1) && (Save.Staatstopf[ActPlayer-1]>=-10000)
        && (Save.CivPlayer[ActPlayer-1] != 0) && (!Save.PlayMySelf))
    {
        INFORMUSER();
        REQUEST(PText[609],PText[610],ActPlayerFlag,ActPlayerFlag);
    }
    if (Save.Staatstopf[ActPlayer-1]<-10000)
    {
        btx = FINDMONEYPLANET(ActPlayerFlag, ActPlayer-1, s);
        if ((btx>0) && (Save.CivPlayer[ActPlayer-1] != 0))
        {
            INFORMUSER();
            MAKEBORDER(MyScreen[0],35,110,475,210,12,6,0);
            WRITE_RP0(256,150,ActPlayerFlag,WRITE_Center,3,s);
            _s=my_strcpy(s, GETCIVNAME(ActPlayer));
            *_s++ = ' ';
            (void) my_strcpy(_s, PText[615]);
            WRITE_RP0(256,127,12,WRITE_Center,3,s);
            _s=my_strcpy(s, PText[616]);
            *_s++ = ' ';
            _s=my_strcpy(_s, ProjectName[btx]);
            *_s++ = ' ';
            (void) my_strcpy(_s, PText[617]);
            WRITE_RP0(256,175,12,WRITE_Center,3,s);
            if (Save.PlayMySelf) { Delay(PAUSE); }
            WAITLOOP(Save.PlayMySelf);
            RECT_RP0_C0(35,110,475,210);
            REFRESHDISPLAY();
        }
    }

    if ((Year>1973) && (Save.CivPlayer[ActPlayer-1] != 0))
    {
        if ((Save.ImperatorState[ActPlayer-1]<700) && (Warnung[ActPlayer-1] == 0))
        {
            INFORMUSER();
            MAKEBORDER(MyScreen[0],30,80,480,205,12,6,0);
            WRITE_RP0(256, 95,ActPlayerFlag,WRITE_Center,3,PText[620]);
            WRITE_RP0(256,115,ActPlayerFlag,WRITE_Center,3,PText[621]);
            WRITE_RP0(256,135,ActPlayerFlag,WRITE_Center,3,PText[622]);
            WRITE_RP0(256,155,ActPlayerFlag,WRITE_Center,3,PText[623]);
            WRITE_RP0(256,175,ActPlayerFlag,WRITE_Center,3,PText[624]);
            Warnung[ActPlayer-1] = 1;
            Save.ImperatorState[ActPlayer-1] = 700;
            WAITLOOP(false);
            RECT_RP0_C0(30,80,480,205);
            REFRESHDISPLAY();
        } else if ((Save.ImperatorState[ActPlayer-1]<500) && (Warnung[ActPlayer-1] == 1))
        {
            INFORMUSER();
            MAKEBORDER(MyScreen[0],30,80,480,185,12,6,0);
            WRITE_RP0(256, 95,ActPlayerFlag,WRITE_Center,3,PText[626]);
            WRITE_RP0(256,115,ActPlayerFlag,WRITE_Center,3,PText[627]);
            WRITE_RP0(256,135,ActPlayerFlag,WRITE_Center,3,PText[628]);
            WRITE_RP0(256,155,ActPlayerFlag,WRITE_Center,3,PText[629]);
            Warnung[ActPlayer-1] = 2;
            WAITLOOP(false);
            RECT_RP0_C0(30,80,480,185);
            REFRESHDISPLAY();
        }
        if (Save.ImperatorState[ActPlayer-1] >= 1000)
        {
            Warnung[ActPlayer-1] = 0;
        }
    }

/* *** STATUSCHECK BueRGERKRIEG *** */
    if (((Year % 4) == 0) && (Save.WorldFlag == ActPlayerFlag))
    {
        if (Save.WarPower[7] > (Save.WarPower[ActPlayer-1]*3))
        {
            STOPCIVILWAR(1);
        } else if ((Save.WarPower[7]*3) < Save.WarPower[ActPlayer-1])
        {
            STOPCIVILWAR(2);
        } else if ((Save.Bevoelkerung[7]+Save.Bevoelkerung[ActPlayer-1])<100000)
        {
            STOPCIVILWAR(0);
        }
    }
    if ((Save.WorldFlag == WFLAG_FIELD) && (ActPlayer == 1))
    {
        if (Save.Systems>1)
        {
            Save.Systems--;
        } else {
            Save.Bevoelkerung[ActPlayer-1] = 0;
        }
        if (Display>Save.Systems)
        {
            DRAWSTARS(MODE_REDRAW);
        }
    }
/* *** SPIELER SPIELT NICHT *** */
    if (Save.WarPower[ActPlayer-1] > Save.MaxWarPower[ActPlayer-1])
    {
        Save.MaxWarPower[ActPlayer-1] = Save.WarPower[ActPlayer-1];
    }
    if ((MultiPlayer) && (!Save.PlayMySelf) && (0 != Save.CivPlayer[ActPlayer-1])
            && (Year>2065) && (0 == Save.MaxWarPower[ActPlayer-1]))
    {
        j = 0;
        for(i = 0; i < (MAXCIVS-2); i++)
        {
            if (0 != Save.CivPlayer[i])
            {
                j++;
            }
        }
        if (j > 1)
        {
            INFORMUSER();
            MAKEBORDER(MyScreen[0],30,120,480,215,12,6,0);
            _s=my_strcpy(s, _PT_Die);
            *_s++ = ' ';
            _s=my_strcpy(_s, GETCIVNAME(ActPlayer));
            *_s++ = ' ';
            _s=my_strcpy(_s, PText[631]);
            *_s++ = ' ';
            ActPlayer--; // @TODO .. only to shift the arrays!!!
            (void) dez2out(Save.CivPlayer[ActPlayer], 0, _s);
            WRITE_RP0(256,140,ActPlayerFlag,WRITE_Center,3,s);
            WRITE_RP0(256,160,ActPlayerFlag,WRITE_Center,3,PText[632]);
            WRITE_RP0(256,180,ActPlayerFlag,WRITE_Center,3,PText[633]);
            WAITLOOP(false);
            RECT_RP0_C0(30,120,480,215);
            Save.CivPlayer[ActPlayer] = 0;
            Save.Staatstopf[ActPlayer] += Year*120;
            for(i = 1; i < 8; i++)
            {
                Save.TechCosts[ActPlayer][(rand()%30)+1] = 0;
            }
            for(i = 0; i < (MAXCIVS-1); i++)
            {
                if ((Save.WarState[i][ActPlayer]==LEVEL_WAR) ||
                    (Save.WarState[i][ActPlayer]==LEVEL_COLDWAR))
                {
                    Save.WarState[i][ActPlayer] = LEVEL_PEACE;
                }

                if ((Save.WarState[ActPlayer][i]==LEVEL_WAR) ||
                    (Save.WarState[ActPlayer][i]==LEVEL_COLDWAR))
                {
                    Save.WarState[ActPlayer][i] = LEVEL_PEACE;
                }
            }
            CREATEJAHADR(ActPlayer);
            ActPlayer++; // @TODO .. only to shift the arrays!!!
        }
    }

/* *** WIEDERAUFERSTEHUNG ZIVI *** */
    --ActPlayer;
    NewPNames = (rand()%MAXPLANETS);    // aus INITVARS.c hierher.. nur hier benoetigt!
    if (((ActPlayer != 0) || (MultiPlayer)) && (Save.WorldFlag != WFLAG_CEBORC)
        && (ActPlayer < (MAXCIVS-2)) && (Save.Bevoelkerung[ActPlayer] <= 0))
    {
        j = rand()%Save.Systems;
        if (0 == (SystemFlags[0][j] & FLAG_CIV_MASK))
        {
            if (0 == SystemHeader[j].Planets)
            {
                CREATENEWSYSTEM(j, ActPlayer, 1);
            }
            for(k = 0; k < SystemHeader[j].Planets; k++)
            {
                if (0 == Save.Bevoelkerung[ActPlayer])
                {
                    PlanetHeader = &(SystemHeader[j].PlanetMemA[k]);
                    if ((PlanetHeader->Class==CLASS_WATER) || (PlanetHeader->Class==CLASS_ICE)
                      ||(PlanetHeader->Class==CLASS_DESERT)|| (PlanetHeader->Class==CLASS_HALFEARTH)
                      ||(PlanetHeader->Class==CLASS_STONES))
                    {
                        SystemHeader[j].vNS = 0;
                        Save.CivPlayer[ActPlayer] = 0;
                        (void) my_strcpy(PlanetHeader->PName, PNames[0].data[NewPNames]);
                        NewPNames = (NewPNames+1) % MAXPLANETS;

                        PlanetHeader->PFlags = ActPlayerFlag;
                        PlanetHeader->Ethno = PlanetHeader->PFlags;
                        PlanetHeader->Population = abs(Year*5);
                        if (PlanetHeader->Population > (PlanetHeader->Size*500))
                        {
                            PlanetHeader->Population = PlanetHeader->Size*500;
                        }
                        Save.Bevoelkerung[ActPlayer] = PlanetHeader->Population;
                        PlanetHeader->Biosphaere = 200;
                        PlanetHeader->Infrastruktur = 190;
                        PlanetHeader->Industrie = 180;
                        PlanetHeader->ProjectPtr = (ByteArr42*) (AllocMem(sizeof(ByteArr42),MEMF_CLEAR));
                        PlanetHeader->ProjectPtr->data[1]  = 1;
                        PlanetHeader->ProjectPtr->data[PROJECT_SETTLERS] = 5;
                        for(i = 0; i < (MAXCIVS-1); i++)
                        {
                            if (i != (ActPlayer))
                            {
                                Save.WarState[ActPlayer][i] = LEVEL_UNKNOWN;
                                Save.WarState[i][ActPlayer] = LEVEL_UNKNOWN;
                            }
                        }
                        Save.WarState[ActPlayer][ActPlayer] = LEVEL_PEACE;
                    }
                }
            }
            if ((Year<2120) && ((rand()%50) == 0))
            {
                CREATEJAHADR(ActPlayer);
            }
        }
    }
    if (0 == (rand()%219))
    {
        if (0 == (rand()%219))
        {
            CEBORCATTACK(0);
        }
    }

    if (((rand()%240) == 0) && ((rand()%50) == 0))
    {
        l = 0;
        for(i = 0; i < (MAXCIVS-2); i++)
        {
            if ((i != ActPlayer) && (Save.WarState[ActPlayer][1] != LEVEL_UNKNOWN))
            {
                l = 1;
                break;
            }
        }
        if (0 == l)
        {
            CREATEJAHADR(ActPlayer);
        }
    }
    ++ActPlayer;
}
