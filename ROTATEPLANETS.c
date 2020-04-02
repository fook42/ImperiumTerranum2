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
    bool        vonNeuMannSonde_exists;
    bool        b, FreeSystem;
    ByteArr42*  ActPProjects;
    sint16      FromX, FromY;
    double      sin_rot, cos_rot, d;
    uint8       CivVar,PCreativity,PProd,btx,XState,Fight;
    char        s[99];
    uint32      len;
    sint8       NewTech[7];
    time_t      t;
    uint8       NewPNames;
    uint8       i, j, k;
    struct Window*   ROT_Window;
    struct Window*   ROT_Window2;
    struct RastPort* RPort_PTR;
    struct RastPort* RPort_PTR2;

    if ((2000 < Year) && (1 == ActPlayer)) { DOHUMANITY(); }
    RECT_RP0(0,520,291,632,308);
    Valid = false;
    if (1 == ActPlayer) { ++Year; }
    FreeSystem = false;
    if ((0 == (Year % 10)) && (!Save.PlayMySelf)) { INFORMUSER(); }
    AllCreative[ActPlayer-1] = 0;
    for(j = 1; j < 8; ++j)
    {
        if (Save.ProjectCosts[ActPlayer-1].data[j] <= 0)
        {
            if (0 < Year)
            {
                Save.ProjectCosts[ActPlayer-1].data[j] = 50 * Year * j;
            } else {
                Save.ProjectCosts[ActPlayer-1].data[j] = 180000 * j;
            }
        }
    }

    vonNeuMannSonde_exists = true;
    if (1 == ActPlayer)
    {
        MaquesShips = 0;
        for(i = 0; i < MAXCIVS; ++i)
        {
            Save.WarPower[i] = 0;
            Save.Bevoelkerung[i] = 0;
            Militaerausgaben[i] = 0;
        }
    }
    for(i = 0; i < Save.Systems; ++i)
    {
        if ((Save.ProjectCosts[ActPlayer-1].data[39] <= 0) && (vonNeuMannSonde_exists)
          && (SystemHeader[i].vNS != FLAG_KNOWN) && ((rand()%5) == 0))
        {
            SystemHeader[i].vNS = FLAG_KNOWN;
            vonNeuMannSonde_exists = false;
        }
    }

    if (1 == ActPlayer)
    {
        for(i = 0; i < Save.Systems; ++i)
        {
            if (0 == SystemHeader[i].Planets) { continue; }

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
                d = 1.0/(sqrt(SystemHeader[i].FirstShip.PosX * SystemHeader[i].FirstShip.PosX
                            + SystemHeader[i].FirstShip.PosY * SystemHeader[i].FirstShip.PosY));
                // rotate the stargate around the sun...
                sin_rot = sin(d);
                cos_rot = cos(d);
                d = SystemHeader[i].FirstShip.PosX;
                SystemHeader[i].FirstShip.PosX = it_round(d * cos_rot - SystemHeader[i].FirstShip.PosY * sin_rot);
                SystemHeader[i].FirstShip.PosY = it_round(d * sin_rot + SystemHeader[i].FirstShip.PosY * cos_rot);

                // MyShipPtr = &SystemHeader[i].FirstShip; // not needed .. MyShipPtr is not changed at all.
                if (FINDOBJECT(i, 256+(SystemHeader[i].FirstShip.PosX+OffsetX)*32,
                                  256+(SystemHeader[i].FirstShip.PosY+OffsetY)*32,
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
                if ((SystemHeader[i].FirstShip.PosX>=-3) && (SystemHeader[i].FirstShip.PosX<=2)
                 && (SystemHeader[i].FirstShip.PosY>=-3) && (SystemHeader[i].FirstShip.PosY<=2))
                {
                    // if the stargate is too close to the sun (-3..+2) .. its orbit collapses/it is destroyed
/* OpenWindow() ---- */
                    MAKEBORDER(MyScreen[0],85,120,425,200,12,6,0);
                    strcpy(s, _PT_System);
                    strcat(s, ": ");
                    strcat(s, Save.SystemName.data[i]);
                    WRITE(256,131,12,WRITE_Center,MyRPort_PTR[0],3, s);
                    WRITE(256,151,12,WRITE_Center,MyRPort_PTR[0],3, _PT_Umlaufbahn_kollabiert);
                    WRITE(256,171,12,WRITE_Center,MyRPort_PTR[0],3, _PT_Stargate_zerstoert);
                    if (Save.PlayMySelf) { Delay(PAUSE); }
                    WAITLOOP(Save.PlayMySelf);
                    RECT_RP0(0,85,120,425,200);
/* CloseWindow() ---- */
                    SystemHeader[i].FirstShip.SType = 0;
                }
            }
            REFRESHSHIPS(SystemHeader[i].FirstShip.NextShip,i+1,1);
        }
    }

    if (ActPlayer<MAXCIVS)
    {
        for(i = 0; i < Save.Systems; ++i)
        {
            if (0 == SystemHeader[i].Planets) { continue; }

            for(j = 1; j <= SystemHeader[i].Planets; ++j)
            {
                PlanetHeader = &(SystemHeader[i].PlanetMemA[j-1]);
                if (NULL == PlanetHeader) { continue; }

                if ((PlanetHeader->Ethno == FLAG_OTHER) && (Save.WorldFlag != WFLAG_JAHADR))
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
                                Save.ProjectCosts[GETCIVVAR(PlanetHeader->PFlags)-1].data[k] = 0;
                            }
                        }
                    }
                    CREATEPANIC(PlanetHeader, i+1, j);
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
                        }
                        if (0 < PlanetHeader->Industrie)
                        {
                            PlanetHeader->Industrie = abs(PlanetHeader->Industrie-(rand()%7));
                        }
                    } else if ((CivVar<=MAXCIVS) && (NULL != PlanetHeader->ProjectPtr))
                    {
                        ActPProjects = PlanetHeader->ProjectPtr;
                        if (((PlanetHeader->Class == CLASS_EARTH)        && ((PlanetHeader->Population / 1176) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_HALFEARTH) && ((PlanetHeader->Population /  991) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_ICE)       && ((PlanetHeader->Population /  743) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_WATER)     && ((PlanetHeader->Population /  745) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_STONES)    && ((PlanetHeader->Population /  929) <= PlanetHeader->Size))
                            || ((PlanetHeader->Class == CLASS_DESERT)    && ((PlanetHeader->Population /  739) <= PlanetHeader->Size)))
                        {
                            PlanetHeader->Population += 1+(ActPProjects->data[41]+ActPProjects->data[42])*20;
                            PlanetHeader->Population = it_round(PlanetHeader->Population*1.008);
                            if (Save.ProjectCosts[CivVar-1].data[3] == 0)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.028); }
                            if (Save.ProjectCosts[CivVar-1].data[4] == 0)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.029); }
                            if (PlanetHeader->Population<1000)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.05); }
                            if (PlanetHeader->Population<2000)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.009); }
                            if (PlanetHeader->Population<3000)
                                { PlanetHeader->Population = it_round(PlanetHeader->Population*1.005); }
                        }
                        Save.Bevoelkerung[CivVar-1] += PlanetHeader->Population;
                    } else if (NULL == PlanetHeader->ProjectPtr)
                    {
                        PlanetHeader->ProjectPtr = (ByteArr42*) AllocMem(sizeof(ByteArr42),MEMF_CLEAR);
                    }

                    if ((PlanetHeader->Class == CLASS_PHANTOM) && (NULL != PlanetHeader->FirstShip.NextShip))
                    {
                        PlanetHeader->FirstShip.NextShip->Owner = 0;
                    }
                    REFRESHSHIPS(PlanetHeader->FirstShip.NextShip, i+1, 0);
                    d = 1.0/((j*3.0)+1);
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

    if (ActSys>0)
    {
        DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    }
    if (ActPlayer<MAXCIVS)
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

                        ROT_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
                        RPort_PTR = ROT_Window->RPort;
                        MAKEWINBORDER(RPort_PTR,0,0,340,80,12,6,1);

                        strcpy(s, _PT_System);
                        strcat(s, ": ");
                        strcat(s, Save.SystemName.data[i]);
                        WRITE(171, 7,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);
                        strcpy(s, _PT_Planet);
                        strcat(s, ": ");
                        strcat(s, PlanetHeader->PName);
                        WRITE(171, 27,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);

                        strcpy(s, PText[578]);
                        switch (PlanetHeader->Class)
                        {
                            case CLASS_DESERT    : strcat(s, " D"); break;
                            case CLASS_HALFEARTH : strcat(s, " H"); break;
                            case CLASS_EARTH     : strcat(s, " M"); break;
                            case CLASS_WATER     : strcat(s, " W"); break;
                            default: { }
                        }
                        WRITE(171,53,12,(1|WRITE_Center),RPort_PTR,3,s);
                        if (Save.PlayMySelf) { Delay(PAUSE); }
                        WAITLOOP(Save.PlayMySelf);
                        CloseWindow(ROT_Window);
                    }
                }
                if ((PlanetHeader->PFlags & FLAG_CIV_MASK) == ActPlayerFlag)
                {
                    if ((((PlanetHeader->ProjectID>=1) && (PlanetHeader->ProjectID<=7)) || (39 == PlanetHeader->ProjectID))
                        && (Save.ProjectCosts[ActPlayer-1].data[PlanetHeader->ProjectID] <= 0))
                    {
                        if ((39 == PlanetHeader->ProjectID) && (Save.ProjectCosts[ActPlayer-1].data[39]>0))
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
                    if ((ActPProjects->data[34]>=1) && (ActPProjects->data[34]<=99))
                        { ActPProjects->data[34] = it_round(ActPProjects->data[34]*1.2)+1; }

                    if ((ActPProjects->data[40]>=1) && (ActPProjects->data[40]<=99))
                        { ActPProjects->data[40] = it_round(ActPProjects->data[40]*1.2)+1; }

                    SystemFlags[ActPlayer-1][i] |= FLAG_KNOWN;

                    PProd = 11+(ActPProjects->data[31]+ActPProjects->data[37]
                                +ActPProjects->data[38]+ActPProjects->data[41]
                                +ActPProjects->data[42])*6;

                    PCreativity = 20+(ActPProjects->data[33]+ActPProjects->data[35]+ActPProjects->data[36]
                                        +ActPProjects->data[38]+ActPProjects->data[42])*10;

                    AllCreative[ActPlayer-1] += it_round(PCreativity*(PlanetHeader->Biosphaere/80.0+PlanetHeader->Population/400.0))+1;

                    if (0 == ActPProjects->data[41])
                    {       /* Keine MICROIDEN, Biosphaeren-Abbau */
                        l = 4+( ActPProjects->data[30]+               /* Recycling-Anl. */
                                ActPProjects->data[31]+               /* Fusions-Kraftwerk */
                                ActPProjects->data[32]+               /* Hydro-Kraftwerk */
                                ActPProjects->data[37]+               /* intell. Fabrik */
                                ActPProjects->data[42])*2;            /* Wetter-Sat */

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
                                case -3:{   PlanetHeader->Biosphaere += 9;
                                            if (PlanetHeader->Biosphaere>200)
                                            {
                                                l = 1;
                                                PlanetHeader->Biosphaere = 200;
                                            }
                                        } break;
                                case -2:{   PlanetHeader->Infrastruktur += 9;
                                            if (PlanetHeader->Infrastruktur>200)
                                            {
                                                l = 1;
                                                PlanetHeader->Infrastruktur = 200;
                                            }
                                        } break;
                                default:{   PlanetHeader->Industrie += 9;
                                            if (PlanetHeader->Industrie>200)
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

                        if ((PlanetHeader->ProjectID>0) && (abs(PlanetHeader->XProjectPayed) > PlanetHeader->XProjectCosts))
                        {
                            PlanetHeader->XProjectPayed -= PlanetHeader->XProjectCosts;
                            ActPProjects = PlanetHeader->ProjectPtr;
                            if ((PlanetHeader->ProjectID==34) || (PlanetHeader->ProjectID==40))
                            {
                                ActPProjects->data[PlanetHeader->ProjectID] = 100;
                            } else if (((PlanetHeader->ProjectID>=1)  && (PlanetHeader->ProjectID<=7))
                                        ||((PlanetHeader->ProjectID>=25) && (PlanetHeader->ProjectID<=38))
                                        ||((PlanetHeader->ProjectID>=40) && (PlanetHeader->ProjectID<=42)))
                            {
                                ActPProjects->data[PlanetHeader->ProjectID]++;
                            } else if  ((PlanetHeader->ProjectID>=8)  && (PlanetHeader->ProjectID<=24))
                            {
                                if (MAXMAQUES > MaquesShips)
                                {
                                    (void) CREATEMAQUESSHIP(i,PlanetHeader->ProjectID);
                                }
                                ActShipPtr = (r_ShipHeader*) AllocMem(sizeof(r_ShipHeader),MEMF_CLEAR);
                                if (NULL != ActShipPtr)
                                {
                                    *ActShipPtr = (r_ShipHeader) {0,PlanetHeader->ProjectID, PlanetHeader->PFlags & FLAG_CIV_MASK,0,0,0,0,0,0,
                                                                ShipData(PlanetHeader->ProjectID).MaxShield,1,1,
                                                                ShipData(PlanetHeader->ProjectID).MaxMove,0,0,0,NULL,NULL,NULL};
                                                    /*Age,SType,Owner,Flags,ShieldBonus,Ladung,Fracht,PosX,PosY,
                                                    Shield,Weapon,Repair
                                                    Moving,Source,Target,Tactical,TargetShip,BeforeShip,NextShip*/
                                    ActShipPtr->Weapon = WEAPON_GUN;
                                    if (Save.TechCosts[ActPlayer-1].data[15] <= 0) { ActShipPtr->Weapon = WEAPON_LASER; }
                                    if (Save.TechCosts[ActPlayer-1].data[24] <= 0) { ActShipPtr->Weapon = WEAPON_PHASER; }
                                    if (Save.TechCosts[ActPlayer-1].data[32] <= 0) { ActShipPtr->Weapon = WEAPON_DISRUPTOR; }
                                    if (Save.TechCosts[ActPlayer-1].data[27] <= 0) { ActShipPtr->Weapon = WEAPON_PTORPEDO; }

                                    if ((PlanetHeader->ProjectID == 21) && (SystemHeader[i].FirstShip.SType == 0)
                                        && ((Save.CivPlayer[GETCIVVAR(ActShipPtr->Owner)-1] == 0) || Save.PlayMySelf))
                                    {
                                        ActShipPtr->PosX = it_round(PlanetHeader->PosX);
                                        ActShipPtr->PosY = it_round(PlanetHeader->PosY);
                                        LINKSHIP(ActShipPtr, &(SystemHeader[i].FirstShip), 0);
                                    } else {
                                        LINKSHIP(ActShipPtr, &(PlanetHeader->FirstShip),0);
                                    }
                                }
                            }
                            if (((PlanetHeader->ProjectID>=1) && (PlanetHeader->ProjectID<=7))
                                || (PlanetHeader->ProjectID==39))
                            {
                                Save.ProjectCosts[ActPlayer-1].data[PlanetHeader->ProjectID] = 0;
                            }
                            l = 1;
                        }
                        if (l == 1)
                        {
                            if ((PlanetHeader->ProjectID>=1) && (PlanetHeader->ProjectID<=7))
                            {
                                Save.ImperatorState[ActPlayer-1] += 150;
                                if (Save.stProject[PlanetHeader->ProjectID-1] == 0)
                                {
                                    INFORMUSER();
                                    Save.ImperatorState[ActPlayer-1] += 50;
                                    Save.stProject[PlanetHeader->ProjectID-1] = ActPlayer;

                                    ROT_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
                                    RPort_PTR = ROT_Window->RPort;
                                    MAKEWINBORDER(RPort_PTR,0,0,340,80,12,6,1);

                                    strcpy(s, GETCIVNAME(ActPlayer));
                                    strcat(s, " ");
                                    strcat(s, PText[579]); // fuehren als erste
                                    WRITE(171, 7,GETCIVFLAG(ActPlayer),(1|WRITE_Center),RPort_PTR,3,s);
                                    strcpy(s, Project.data[PlanetHeader->ProjectID]);
                                    strcat(s, "-");
                                    WRITE(171,29,                   12,(1|WRITE_Center),RPort_PTR,3,s);
                                    s[0]=0;
                                    if ((1 <= PlanetHeader->ProjectID)
                                     && (3 >= PlanetHeader->ProjectID))
                                    {
                                        strcpy(s, PText[582]); // Projekt
                                        strcat(s, " ");
                                    }
                                    strcat(s, PText[580]); // durch
                                    WRITE(171,53,12,(1|WRITE_Center),RPort_PTR,3,s);
                                    if (Save.PlayMySelf) { Delay(PAUSE); }
                                    WAITLOOP(Save.PlayMySelf);

                                    CloseWindow(ROT_Window);
                                }
                            } else if ((8 <= PlanetHeader->ProjectID) && (PlanetHeader->ProjectID <= 24))
                            {
                                if (0 != Save.CivPlayer[ActPlayer-1])
                                {
                                    btx = 1;
                                    XState = 0;
                                    Fight = 0;
                                    for(k = 1; k <= 6; k++)
                                    {
                                        if ((Save.Military[ActPlayer-1] & btx) == btx)
                                        {
                                            XState = XState+i+1;
                                            Fight = Fight+8;
                                        }
                                        btx = btx*2;
                                    }
                                    Save.ImperatorState[ActPlayer-1] -= XState;
                                    ActShipPtr->ShieldBonus = Fight;
                                } else {
                                    ActShipPtr->ShieldBonus = it_round(Level*2.0-2);
                                }
                            } else if ((PlanetHeader->ProjectID==26)
                                    || ((PlanetHeader->ProjectID>=28) && (PlanetHeader->ProjectID<=42)))
                            {
                                Save.ImperatorState[ActPlayer-1] += 10;
                            }
                            if ((PlanetHeader->ProjectID==26) || (PlanetHeader->ProjectID==27))
                            {
                                PlanetHeader->Population -= 10;
                            }
                        }

                        if ((l == 1) && (Save.CivPlayer[ActPlayer-1] != 0))
                        {
                            INFORMUSER();
                            ROT_Window=MAKEWINDOW(85,120,341,81,MyScreen[0]);
                            RPort_PTR = ROT_Window->RPort;
                            MAKEWINBORDER(RPort_PTR,0,0,340,80,12,6,1);

                            strcpy(s, _PT_System);
                            strcat(s, ": ");
                            strcat(s, Save.SystemName.data[i]);
                            WRITE(171, 7,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);

                            strcpy(s, _PT_Planet);
                            strcat(s, ": ");
                            strcat(s, PlanetHeader->PName);
                            WRITE(171,27,ActPlayerFlag,(1|WRITE_Center),RPort_PTR,3,s);

                            if (0 < PlanetHeader->ProjectID)
                            {
                                strcpy(s, PText[583]);  // baut
                                strcat(s, " ");
                                strcat(s, Project.data[PlanetHeader->ProjectID]);
                            } else {
                                switch (PlanetHeader->ProjectID)
                                {
                                    case -3: strcpy(s, _PT_Biosphaere_gereinigt);    break;
                                    case -2: strcpy(s, _PT_Infrastructur_repariert); break;
                                    default: strcpy(s, _PT_IndustrAnlage_repariert);
                                }
                            }
                            WRITE(171,53,12,(1|WRITE_Center),RPort_PTR,3,s);

                            Delay(5);
                            if (( 8 <= PlanetHeader->ProjectID)
                             && (24 >= PlanetHeader->ProjectID) && (!Save.PlayMySelf))
                            {
                                // building ships... where to place it?
                                ROT_Window2=MAKEWINDOW(85,208,341,41,MyScreen[0]);
                                RPort_PTR2 = ROT_Window2->RPort;
                                MAKEWINBORDER(RPort_PTR2,0,0,340,40,12,6,1);

                                BltBitMapRastPort((struct BitMap*) &ImgBitMap4,(PlanetHeader->ProjectID-8)*32,32,RPort_PTR2,10, 4,32,32,192);
                                DrawImage(RPort_PTR2,&GadImg1, 55,10);
                                DrawImage(RPort_PTR2,&GadImg1,195,10);

                                WRITE(113,12,0,WRITE_Center,RPort_PTR2,3,PText[587]);   // Space
                                WRITE(253,12,0,WRITE_Center,RPort_PTR2,3,PText[588]);   // Orbit
                                b = false;
                                do
                                {
                                    Delay(RDELAY);
                                    if (LMB_PRESSED)
                                    {
                                        if ((ROT_Window2->MouseY >= 10) && (ROT_Window2->MouseY <= 30))
                                        {
                                            if ((ROT_Window2->MouseX >= 55) && (ROT_Window2->MouseX <= 171))
                                            {
                                                KLICKWINGAD(RPort_PTR2,55,10);
                                                b = true;
                                                ActShipPtr->PosX = it_round(PlanetHeader->PosX);
                                                ActShipPtr->PosY = it_round(PlanetHeader->PosY);
                                                LINKSHIP(ActShipPtr, &(SystemHeader[i].FirstShip), 1);
                                            } else if ((ROT_Window2->MouseX >= 195) && (ROT_Window2->MouseX <= 311))
                                            {
                                                KLICKWINGAD(RPort_PTR2,195,10);
                                                b = true;
                                            }
                                        }
                                    }
                                }
                                while (!b);
                                CloseWindow(ROT_Window2);
                            } else {
                                if (Save.PlayMySelf) { Delay(PAUSE); }
                                WAITLOOP(Save.PlayMySelf);
                            }
                            CloseWindow(ROT_Window);

//                            REFRESHDISPLAY();
                            if ((( 8  > PlanetHeader->ProjectID) || (24  < PlanetHeader->ProjectID))
                              && (24 != PlanetHeader->ProjectID) && (27 != PlanetHeader->ProjectID))
                            {
                                PlanetHeader->ProjectID = 0;
                            }
                            if (!Save.PlayMySelf)
                            {
                                HANDLEKNOWNPLANET(i+1, 1, PlanetHeader);
                            }
                        }

                        if (((l == 1) || ((PlanetHeader->ProjectID>=-3) && (PlanetHeader->ProjectID<=0)))
                            && ((Save.CivPlayer[ActPlayer-1] == 0) || Save.PlayMySelf))
                        {
                            PlanetHeader->ProjectID = 0;
                            for(k = 8; k <= 24; k++)
                            {
                                if ((Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[k]] <= 0)
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
                                        PlanetHeader->ProjectID = 0;
                                    }
                                }
                            }
                            if ((ActPlayer == 8) && (Save.WarPower[7]>8000))
                            {
                                PlanetHeader->ProjectID = 0;
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
                                        if (ActPProjects->data[27]<1)
                                        {
                                            PlanetHeader->ProjectID = 27;
                                        }
                                        while ((NULL != MyShipPtr) && (NULL != MyShipPtr->NextShip))
                                        {
                                            if ((0 == (MyShipPtr->Ladung & MASK_LTRUPPS))
                                                && (0 < ActPProjects->data[27])
                                                && (ShipData(MyShipPtr->SType).MaxLoad >
                                                    (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                            {
                                                ActPProjects->data[27]--;
                                                MyShipPtr->Ladung++;
                                            }
                                            ActShipPtr = MyShipPtr->NextShip;
                                            MyShipPtr->PosX = it_round(PlanetHeader->PosX);
                                            MyShipPtr->PosY = it_round(PlanetHeader->PosY);
                                            LINKSHIP(MyShipPtr, &(SystemHeader[i].FirstShip), 1);
                                            FINDENEMYOBJECT(i+1,MyShipPtr);
                                            MyShipPtr = ActShipPtr;
                                        }
                                    } else if (((SystemHeader[i].State & STATE_TACTICAL) != STATE_ALL_OCC)
                                                && (Save.GlobalFlags[ActPlayer-1] == GFLAG_EXPLORE))
                                    {
                                        /* *** Noch nicht alle Planeten des Systems besiedelt! *** */
                                        if (ActPProjects->data[26] >= ShipData(MyShipPtr->SType).MaxLoad)
                                        {
                                            for(k = 1; k <= ShipData(MyShipPtr->SType).MaxLoad; k++)
                                            {
                                                if ((ActPProjects->data[26]>0)
                                                    && (ShipData(MyShipPtr->SType).MaxLoad >
                                                        (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                                {
                                                    ActPProjects->data[26]--;
                                                    MyShipPtr->Ladung += 16;
                                                }
                                            }
                                            MyShipPtr->PosX = it_round(PlanetHeader->PosX);
                                            MyShipPtr->PosY = it_round(PlanetHeader->PosY);
                                            l = GOTONEXTPLANET(i+1,MyShipPtr);
                                        } else {
                                            PlanetHeader->ProjectID = 26;
                                        }
                                    } else if (((SystemHeader[i].State & STATE_TACTICAL) == STATE_ALL_OCC)
                                                && (Save.GlobalFlags[ActPlayer-1] == GFLAG_EXPLORE))
                                    {
                                        /* *** Expedition in andere Galaxie *** */
                                        if ((rand()%100)<15)
                                        {
                                            l = 2;
                                        } else {
                                            if ((ActPProjects->data[26]+ActPProjects->data[27]) > ShipData(MyShipPtr->SType).MaxLoad)
                                            {
                                                MyShipPtr->PosX = it_round(PlanetHeader->PosX);
                                                MyShipPtr->PosY = it_round(PlanetHeader->PosY);
                                                for(k = 1; k <= ShipData(MyShipPtr->SType).MaxLoad; k++)
                                                {
                                                    if ((ActPProjects->data[26]>1)
                                                        && (ShipData(MyShipPtr->SType).MaxLoad >
                                                        (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                                    {
                                                        ActPProjects->data[26]--;
                                                        MyShipPtr->Ladung += 16;
                                                    }
                                                    if ((ActPProjects->data[27]>1)
                                                        && (ShipData(MyShipPtr->SType).MaxLoad >
                                                        (((MyShipPtr->Ladung & MASK_SIEDLER) / 16) + (MyShipPtr->Ladung & MASK_LTRUPPS))))
                                                    {
                                                        ActPProjects->data[27]--;
                                                        MyShipPtr->Ladung++;
                                                    }
                                                }
                                                l = GOTONEXTSYSTEM(i+1,MyShipPtr);
                                            } else {
                                                if (ActPProjects->data[26]<ShipData(MyShipPtr->SType).MaxLoad)
                                                    { PlanetHeader->ProjectID = 26; }
                                                if (ActPProjects->data[27]<2)
                                                {
                                                    PlanetHeader->ProjectID = 27;
                                                } else {
                                                    PlanetHeader->ProjectID = 26;
                                                }
                                            }
                                        }
                                    } else if (Save.GlobalFlags[ActPlayer-1] == GFLAG_ATTACK)
                                    {
                                        /* Alles Besetzt, Krieg beginnen */
                                        l = 0;
                                        for(k = 1; k < MAXCIVS; k++)
                                        {
                                            if ((k != ActPlayer) && (Save.WarState[ActPlayer-1][k-1] == LEVEL_NO_ALLIANZ))
                                                { l = k; }
                                        }
                                        if (l == 0)
                                        {
                                            for(k = 1; k < (MAXCIVS-1); k++)
                                            {
                                                if ((k != ActPlayer)
                                                    && (Save.WarState[ActPlayer-1][k-1]!=LEVEL_UNKNOWN)
                                                    && (Save.WarState[ActPlayer-1][k-1]!=LEVEL_DIED))
                                                    { l = k; }
                                            }
                                        }
                                        if (l != 0)
                                        {
                                            Save.WarState[ActPlayer-1][l-1] = LEVEL_COLDWAR;
                                        }
                                    }
                                    if (PlanetHeader->FirstShip.NextShip != NULL)
                                    {
                                        if ((PlanetHeader->FirstShip.NextShip->NextShip != NULL)
                                            && ((SystemHeader[i].State & STATE_TACTICAL) == STATE_ALL_OCC)
                                            && (ActPProjects->data[26] == 0))
                                        {
                                            if ((((PlanetHeader->Water / PlanetHeader->Size)<55) && EXISTSPLANET(ActPlayer,i+1,1))
                                                || (((PlanetHeader->Water / PlanetHeader->Size)>56) && EXISTSPLANET(ActPlayer,i+1,2)))
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
                                    if ((Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[PriorityList[k]]] <= 0)
                                            /* Technologie vorhanden */
                                        && ((ActPProjects->data[ProjectNeedsProject[PriorityList[k]]]>0)
                                        || (Save.ProjectCosts[ActPlayer-1].data[ProjectNeedsProject[PriorityList[k]]] == 0)))
                                    {
                                        /* evtl. noetiges Projekt vorhanden */
                                        if ((PriorityList[k]>=1) && (PriorityList[k]<=7)
                                            && (Save.ProjectCosts[ActPlayer-1].data[ProjectNeedsProject[PriorityList[k]]] == 0)
                                            && (Save.ProjectCosts[ActPlayer-1].data[PriorityList[k]]>0))
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
                            if (Save.ProjectCosts[ActPlayer-1].data[1]>0)
                                { PlanetHeader->ProjectID = 1; }
                            if (0 == PlanetHeader->ProjectID)
                            {
                                if (200 > PlanetHeader->Industrie)     { PlanetHeader->ProjectID = -1; }
                                if (200 > PlanetHeader->Infrastruktur) { PlanetHeader->ProjectID = -2; }
                                if (200 > PlanetHeader->Biosphaere)    { PlanetHeader->ProjectID = -3; }
                            } else if (0 < PlanetHeader->ProjectID)
                            {
                                PlanetHeader->XProjectCosts = Save.ProjectCosts[ActPlayer-1].data[PlanetHeader->ProjectID];
                            }
                        }
                        if ((0 == Save.CivPlayer[ActPlayer-1]) || (Save.PlayMySelf))
                        {
                            if (170 > PlanetHeader->Industrie)     { PlanetHeader->ProjectID = -1; }
                            if (170 > PlanetHeader->Infrastruktur) { PlanetHeader->ProjectID = -2; }
                            if (170 > PlanetHeader->Biosphaere)    { PlanetHeader->ProjectID = -3; }
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
                    Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] = 0;
                } else {
                    Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] -= AllCreative[ActPlayer-1];
                }
                if (Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] <= 0)
                {
                    Save.ImperatorState[ActPlayer-1] += 50;
                    if (Save.CivPlayer[ActPlayer-1] != 0)
                    {
                        INFORMUSER();
                        DISPLAYTECH(Save.ActTech[ActPlayer-1]);
                    }
                    Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] = 0;
                }
            }
            if ((Save.CivPlayer[ActPlayer-1] == 0) || (Save.PlayMySelf))
            {
                if (Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] <= 0)
                {
                    Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] = 0;
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
                        if (Save.TechCosts[ActPlayer-1].data[j]>0)
                        {
                            if (TechUse1[j] == 0)
                            {
                                Save.ActTech[ActPlayer-1] = j;
                            } else if ((Save.TechCosts[ActPlayer-1].data[TechUse1[j]] <= 0)
                                    && (Save.TechCosts[ActPlayer-1].data[TechUse2[j]] <= 0))
                            {
                                Save.ActTech[ActPlayer-1] = j;
                            }
                       }
                       l = -1;
                    }
                    while ((Save.ActTech[ActPlayer-1] == 0) && (Save.TechCosts[ActPlayer-1].data[42] > 0));
                }
                if ((ActPlayer == 8) && (Save.ActTech[ActPlayer-1] == 42))
                    { Save.ActTech[ActPlayer-1] = 0; }

                if ((Save.ActTech[ActPlayer-1] == 42) && (Save.ProjectCosts[ActPlayer-1].data[6]>0))
                    { Save.ActTech[ActPlayer-1] = 0; }

            } else if ((Save.ActTech[ActPlayer-1] == 0) || (Save.TechCosts[ActPlayer-1].data[Save.ActTech[ActPlayer-1]] <= 0))
            {
                Save.ActTech[ActPlayer-1] = 0;
                for(j = 1; j <= 6; j++)
                {
                    NewTech[j] = 0;
                }
                j = 1;
                i = 1;
                do
                {
                    if (Save.TechCosts[ActPlayer-1].data[i]>0)
                    {
                        if (TechUse1[i] == 0)
                        {
                            NewTech[j] = i;
                            j++;
                        } else if ((Save.TechCosts[ActPlayer-1].data[TechUse1[i]] <= 0)
                                && (Save.TechCosts[ActPlayer-1].data[TechUse2[i]] <= 0))
                        {
                            NewTech[j] = i;
                            j++;
                        }
                    }
                    i++;
                }
                while ((i<=42) && (j<=6));
                if (0 != NewTech[1])
                {
                    if ((42 == NewTech[1]) && (0 < Save.ProjectCosts[ActPlayer-1].data[6]))
                    {
                        if (0 == (Year % 13))
                        {
                            ROT_Window=MAKEWINDOW(100,100,311,81,MyScreen[0]);
                            RPort_PTR = ROT_Window->RPort;
                            MAKEWINBORDER(RPort_PTR,0,0,310,80,12,6,1);

                            WRITE(156,10,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[590]); // Wissenschaftler fordern
                            WRITE(156,32,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[591]); // Durchführung des
                            WRITE(156,54,           12,WRITE_Center,RPort_PTR,3,PText[592]); // GENESIS-Projects
                            WAITLOOP(false);
                            CloseWindow(ROT_Window);
                        }
                    }
                    INFORMUSER();
                    ROT_Window=MAKEWINDOW(60,100,309,168,MyScreen[0]);
                    RPort_PTR = ROT_Window->RPort;
                    MAKEWINBORDER(RPort_PTR,0,0,308,167,12,6,1);

                    WRITE(154, 10,ActPlayerFlag,WRITE_Center,RPort_PTR,3,PText[593]); // Was soll entwickelt werden?
                    for(j = 1; j <= 6; j++)
                    {
                        if (NewTech[j]>0)
                        {
                            WRITE(10,j*20+20,12,0,RPort_PTR,3,TechnologyL.data[NewTech[j]]);
                        }
                    }
                    i = 0;
                    do
                    {
                        Delay(RDELAY);
                        j = (ROT_Window->MouseY-20) / 20;
                        if ((i != j) && (j>=1) && (j<=6))
                        {
                            i = j;
                            for(j = 1; j <= 6; j++)
                            {
                                if (NewTech[j]>0)
                                {
                                    WRITE(10,j*20+20,12,0,RPort_PTR,3,TechnologyL.data[NewTech[j]]);
                                }
                            }
                            if (NewTech[i]>0)
                            {
                                WRITE(10,i*20+20,ActPlayerFlag,0,RPort_PTR,3,TechnologyL.data[NewTech[i]]);
                            }
                        }
                        if (LMB_PRESSED)
                        {
                            PLAYSOUND(1,300);
                            if (i>0)
                            {
                                if (NewTech[i]>0)
                                {
                                    Save.ActTech[ActPlayer-1] = NewTech[i];
                                }
                            }
                        }
                    }
                    while (Save.ActTech[ActPlayer-1]<=0);
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
                    if ((Save.CivPlayer[i] == 0) || (Save.PlayMySelf))
                    {
                        if (((Save.Staatstopf[i]<(Militaerausgaben[i]*7)) || (Save.Staatstopf[i]<3000))
                            && ((Save.GSteuer[i]+Save.JSteuer[i])<90))
                        {
                            Save.GSteuer[i]++;
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
                    if ((Year % 10) == 0) { DOINFLATION(i); }
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
                            WRITE(256,136,GETCIVFLAG(i+1),(1|WRITE_Center),MyRPort_PTR[0],3,GETCIVNAME(i+1));
                            WRITE(256,156,12,(1|WRITE_Center),MyRPort_PTR[0],3,PText[597]);
                            WRITE(256,176,GETCIVFLAG(j+1),(1|WRITE_Center),MyRPort_PTR[0],3,GETCIVNAME(j+1));
                            WRITE(256,196,12,(1|WRITE_Center),MyRPort_PTR[0],3,PText[598]);
                            DrawImage(&(MyScreen[0]->RastPort),&GadImg1,105,225);
                            DrawImage(&(MyScreen[0]->RastPort),&GadImg1,290,225);
                            WRITE(162,227,8,WRITE_Center,MyRPort_PTR[0],3,_PT_Annehmen);
                            WRITE(348,227,8,WRITE_Center,MyRPort_PTR[0],3,_PT_Ablehnen);
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
                                    AUTOVERHANDLUNG(i+1,ActPlayer,ActSys,MODE_BELEIDIGUNG);
                                }
                            }
                            RECT_RP0(0,85,120,425,265);
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
                        strcpy(s, PText[600]);
                        strcat(s, " ");
                        strcat(s, GETCIVNAME(i+1));
                        WRITE(256,140,GETCIVFLAG(i+1),(1|WRITE_Center),MyRPort_PTR[0],3,s);
                        WRITE(256,165,12,(1|WRITE_Center),MyRPort_PTR[0],3,PText[601]);
                        if (Save.PlayMySelf) { Delay(PAUSE); }
                        WAITLOOP(Save.PlayMySelf);
                        RECT_RP0(0,85,120,425,210);
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
                    WRITE(256,136,FLAG_OTHER,(1|WRITE_Center),MyRPort_PTR[0],3,PText[602]);
                    WRITE(256,156,FLAG_OTHER,(1|WRITE_Center),MyRPort_PTR[0],3,PText[603]);
                    strcpy(s, GETCIVNAME(i));
                    strcat(s, " ");
                    strcat(s, PText[604]);
                    WRITE(256,176,FLAG_OTHER,(1|WRITE_Center),MyRPort_PTR[0],3,s);
                    l = strlen(PText[605]);
                    memcpy(s, PText[605], l);
                    s[l++]=' ';
                    (void) dez2out(abs(Year)*11, 0, s+l);
                    l = abs(Year)*11;
                    WRITE(256,196,FLAG_OTHER,(1|WRITE_Center),MyRPort_PTR[0],3,s);
                    DrawImage(&(MyScreen[0]->RastPort),&GadImg1,105,225);
                    DrawImage(&(MyScreen[0]->RastPort),&GadImg1,290,225);
                    WRITE(162,227,8,WRITE_Center,MyRPort_PTR[0],3,_PT_Annehmen);
                    WRITE(348,227,8,WRITE_Center,MyRPort_PTR[0],3,_PT_Ablehnen);
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
                    RECT_RP0(0,80,120,430,265);
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
            WRITE(256,150,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,s);
            strcpy(s, GETCIVNAME(ActPlayer));
            strcat(s, " ");
            strcat(s, PText[615]);
            WRITE(256,127,12,WRITE_Center,MyRPort_PTR[0],3,s);
            strcpy(s, PText[616]);
            strcat(s, " ");
            strcat(s, Project.data[btx]);
            strcat(s, " ");
            strcat(s, PText[617]);
            WRITE(256,175,12,WRITE_Center,MyRPort_PTR[0],3,s);
            if (Save.PlayMySelf) { Delay(PAUSE); }
            WAITLOOP(Save.PlayMySelf);
            RECT_RP0(0,35,110,475,210);
            REFRESHDISPLAY();
        }
    }

    if ((Year>1973) && (Save.CivPlayer[ActPlayer-1] != 0))
    {
        if ((Save.ImperatorState[ActPlayer-1]<700) && (Warnung[ActPlayer] == 0))
        {
            INFORMUSER();
            MAKEBORDER(MyScreen[0],30,80,480,205,12,6,0);
            WRITE(256, 95,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[620]);
            WRITE(256,115,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[621]);
            WRITE(256,135,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[622]);
            WRITE(256,155,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[623]);
            WRITE(256,175,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[624]);
            Warnung[ActPlayer] = 1;
            Save.ImperatorState[ActPlayer-1] = 700;
            WAITLOOP(false);
            RECT_RP0(0,30,80,480,205);
            REFRESHDISPLAY();
        } else if ((Save.ImperatorState[ActPlayer-1]<500) && (Warnung[ActPlayer] == 1))
        {
            INFORMUSER();
            MAKEBORDER(MyScreen[0],30,80,480,185,12,6,0);
            WRITE(256, 95,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[626]);
            WRITE(256,115,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[627]);
            WRITE(256,135,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[628]);
            WRITE(256,155,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[629]);
            Warnung[ActPlayer] = 2;
            WAITLOOP(false);
            RECT_RP0(0,30,80,480,185);
            REFRESHDISPLAY();
        }
        if (Save.ImperatorState[ActPlayer-1] >= 1000)
        {
            Warnung[ActPlayer] = 0;
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
            DRAWSTARS(MODE_REDRAW,ActPlayer);
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
            len = strlen(_PT_Die);
            memcpy(s, _PT_Die, len);
            s[len++]=' ';
            strcpy(s+len, GETCIVNAME(ActPlayer));
            len=strlen(s);
            s[len++]=' ';
            strcpy(s+len, PText[631]);
            len=strlen(s);
            s[len++]=' ';
            ActPlayer--; // @TODO .. only to shift the arrays!!!
            (void)dez2out(Save.CivPlayer[ActPlayer], 0, s+len);
            WRITE(256,140,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,s);
            WRITE(256,160,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[632]);
            WRITE(256,180,ActPlayerFlag,WRITE_Center,MyRPort_PTR[0],3,PText[633]);
            WAITLOOP(false);
            RECT_RP0(0,30,120,480,215);
            Save.CivPlayer[ActPlayer] = 0;
            Save.Staatstopf[ActPlayer] += Year*120;
            for(i = 1; i < 8; i++)
            {
                Save.TechCosts[ActPlayer].data[(rand()%30)+1] = 0;
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
    NewPNames = (rand()%MAXPLANETS);    // aus INITVARS.c hierher.. nur hier benoetigt!
    if (((ActPlayer != 1) || (MultiPlayer)) && (Save.WorldFlag != WFLAG_CEBORC)
        && (ActPlayer <= (MAXCIVS-2)) && (Save.Bevoelkerung[ActPlayer-1] <= 0))
    {
        j = rand()%Save.Systems;
        if ((SystemFlags[0][j] & FLAG_CIV_MASK) == 0)
        {
            if (SystemHeader[j].Planets == 0)
            {
                CREATENEWSYSTEM(j,ActPlayer);
            }
            for(k = 0; k < SystemHeader[j].Planets; k++)
            {
                if (Save.Bevoelkerung[ActPlayer-1] == 0)
                {
                    PlanetHeader = &(SystemHeader[j].PlanetMemA[k]);
                    if ((PlanetHeader->Class==CLASS_WATER) || (PlanetHeader->Class==CLASS_ICE)
                      ||(PlanetHeader->Class==CLASS_DESERT)|| (PlanetHeader->Class==CLASS_HALFEARTH)
                      ||(PlanetHeader->Class==CLASS_STONES))
                    {
                        SystemHeader[j].vNS = 0;
                        Save.CivPlayer[ActPlayer-1] = 0;
                        strcpy(PlanetHeader->PName, PNames[0].data[NewPNames]);
                        NewPNames++;
                        if (NewPNames >= MAXPLANETS) { NewPNames = 0; }

                        PlanetHeader->PFlags = ActPlayerFlag;
                        PlanetHeader->Ethno = PlanetHeader->PFlags;
                        PlanetHeader->Population = abs(Year*5);
                        if (PlanetHeader->Population > (PlanetHeader->Size*500))
                        {
                            PlanetHeader->Population = PlanetHeader->Size*500;
                        }
                        Save.Bevoelkerung[ActPlayer-1] = PlanetHeader->Population;
                        PlanetHeader->Biosphaere = 200;
                        PlanetHeader->Infrastruktur = 190;
                        PlanetHeader->Industrie = 180;
                        PlanetHeader->ProjectPtr = (ByteArr42*) (AllocMem(sizeof(ByteArr42),MEMF_CLEAR));
                        PlanetHeader->ProjectPtr->data[1]  = 1;
                        PlanetHeader->ProjectPtr->data[26] = 5;
                        for(i = 0; i < (MAXCIVS-1); i++)
                        {
                            if (i != (ActPlayer-1))
                            {
                                Save.WarState[ActPlayer-1][i] = LEVEL_UNKNOWN;
                                Save.WarState[i][ActPlayer-1] = LEVEL_UNKNOWN;
                            }
                        }
                        Save.WarState[ActPlayer-1][ActPlayer-1] = LEVEL_PEACE;
                    }
                }
            }
            if ((Year<2120) && ((rand()%50) == 0))
            {
                CREATEJAHADR(ActPlayer-1);
            }
        }
    }
    if (((rand()%219) == 0) && ((rand()%219) == 0))
        { CEBORCATTACK(0); }

    if (((rand()%240) == 0) && ((rand()%50) == 0))
    {
        l = 0;
        for(i = 0; i < (MAXCIVS-2); i++)
        {
            if ((i != (ActPlayer-1)) && (Save.WarState[ActPlayer-1][1] != LEVEL_UNKNOWN))
            {
                l = 1;
            }
        }
        if (l == 0)
        {
            CREATEJAHADR(ActPlayer-1);
        }
    }
}
