#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INIT_HOMEPLANETS(uint8 Civ, uint8 HomeSystem, uint8 HomePlanet)
{
    uint8   i, value;
    uint32  HomePlanetProd;
    r_PlanetHeader* PlanetHeader;
    APTR    MemPointer;

    // having more than 1 Homeplanet means, having more planets of our own
    value = HomePlanets-1;

    HomePlanetProd = SystemHeader[HomeSystem].PlanetMemA[HomePlanet].Size;

    for (i = 0; i < SystemHeader[HomeSystem].Planets; ++i)
    {
        PlanetHeader = &(SystemHeader[HomeSystem].PlanetMemA[i]);
        if ((PlanetHeader->PFlags==FLAG_UNKNOWN) &&
            (0 != ClassLifeFactor[PlanetHeader->Class]))
        {
            PlanetHeader->PFlags = GETCIVFLAG(Civ+1);
            PlanetHeader->Ethno  = PlanetHeader->PFlags;
            PlanetHeader->Water  = PlanetHeader->Water / PlanetHeader->Size;
            PlanetHeader->Size   = (rand()%15)+5;
            PlanetHeader->Water  = PlanetHeader->Water*PlanetHeader->Size;
            PlanetHeader->Population    = PlanetHeader->Size*400;
            PlanetHeader->Biosphaere    = 170;
            PlanetHeader->Infrastruktur = 165;
            PlanetHeader->Industrie     = 160;
            // get memory for Projects on home planet
            MemPointer = AllocMem(sizeof(ByteArr42), MEMF_ANY|MEMF_CLEAR);
            if (NULL == MemPointer)
            {
                // @TODO out-of-memory handling needed...
                return;
            }
            PlanetHeader->ProjectPtr    = (ByteArr42*) MemPointer;
            HomePlanetProd += PlanetHeader->Size;
            --value;
        }
        if (0 == value)
        {
            break;
        }
    }
    if (7 < HomePlanetProd)
    {
        for (i = 1; i < 43; ++i)
        {
            Save.TechCosts[Civ][i] = it_round((double) Save.TechCosts[Civ][i]*(HomePlanetProd/15.0));
        }
    }
    Save.ImperatorState[Civ] -= HomePlanetProd*3;
}

void INIT_SOLARSYSTEM(void)
{
    APTR            MemPointer;
    r_PlanetHeader* PlanetHeader;
    uint8           i, j;
    double          sin_rot, cos_rot, d;

    // prepare the home-system of Terraners (player 1)
    //

    SystemHeader[0].Planets = 9;
    // get memory for Planets
    MemPointer = AllocMem(SystemHeader[0].Planets*sizeof(r_PlanetHeader), MEMF_ANY|MEMF_CLEAR);
    if (NULL == MemPointer)
    {
        // @TODO out-of-memory handling needed...
        return;
    }
    SystemFlags[0][0] = FLAG_TERRA|FLAG_KNOWN;

    SystemHeader[0] = (r_SystemHeader) {(r_PlanetHeader*) MemPointer, 0, DefaultShip, SystemHeader[0].Planets, 0, FLAG_TERRA};
    SystemHeader[0].FirstShip.Owner = FLAG_TERRA;

    // get memory for Projects on home planet
    MemPointer = AllocMem(sizeof(ByteArr42), MEMF_ANY|MEMF_CLEAR);
    if (NULL == MemPointer)
    {
        // @TODO out-of-memory handling needed...
        return;
    }

    PlanetHeader = &(SystemHeader[0].PlanetMemA[0]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_DESERT,   2,FLAG_UNKNOWN,0,"",4.0,4.0,0,24,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[1]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_HALFEARTH,7,FLAG_UNKNOWN,0,"",7.0,7.0,0,283,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[2]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_EARTH,   10,FLAG_TERRA,FLAG_TERRA,"",10.0,10.0,4000,760,170,165,160,0,0,0,DefaultShip,(ByteArr42*) MemPointer};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[3]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_DESERT,   5,FLAG_UNKNOWN,0,"",13.0,13.0,0,61,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[4]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_GAS,    115,FLAG_UNKNOWN,0,"",16.0,16.0,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[5]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_SATURN,  82,FLAG_UNKNOWN,0,"",19.0,19.0,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[6]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_GAS,     41,FLAG_UNKNOWN,0,"",22.0,22.0,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[7]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_GAS,     45,FLAG_UNKNOWN,0,"",25.0,25.0,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[8]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_DESERT,   1,FLAG_UNKNOWN,0,"",30.0,30.0,0,11,0,0,0,0,0,0,DefaultShip,NULL};
    
    for(i=0; i < 9; ++i)
    {
        (void) my_strcpy(SystemHeader[0].PlanetMemA[i].PName, PNames[1].data[i]);
    }

    if (1 < HomePlanets)
    {
        INIT_HOMEPLANETS(0, 0, 2);
    }

    // rotate the planets inside this system a few cycles (200x)
    for (i = 0; i < 200; ++i)
    {
        for (j = 0; j < SystemHeader[0].Planets; ++j)
        {
            PlanetHeader = &(SystemHeader[0].PlanetMemA[j]);
            d = 1.0/((j*3.0)+4.0);
            sin_rot = sin(d);
            cos_rot = cos(d);
            d = PlanetHeader->PosX; // added... to store previous PosX... d is unused at this point
            PlanetHeader->PosX = d*cos_rot - PlanetHeader->PosY*sin_rot;
            PlanetHeader->PosY = d*sin_rot + PlanetHeader->PosY*cos_rot;
        }
    }

    // place the system somewhere
    SystemX[0] = 10+(rand()%458);
    SystemY[0] = 10+(rand()%490);
}

bool INITSTARS()
{
    bool            system_too_close;
    APTR            ProjectMem;
    int             i,j,k;
    r_PlanetHeader* PlanetHeader;

    time_t  t;
    srand((unsigned) time(&t));

    INIT_SOLARSYSTEM();

    // -----
    for (i = 1; i < MAXSYSTEMS; ++i)
    {
        // every star-system gets an "empty" Ship
        SystemHeader[i] = (r_SystemHeader) {NULL,0,DefaultShip,0,0,0};
        do
        {
            SystemX[i] = 10+(rand()%458);
            SystemY[i] = 10+(rand()%490);
            system_too_close = false;
            // check if the new system is too close to any previous one..
            // reduced calculation: dist^2=x^2+y^2 ... x,y = delta x,y
            // if x+y < 30 -> dist is max. sqrt(550)=~ 23
            for (j = 0; j < i; ++j)
            {
                if ((abs(SystemX[i]-SystemX[j])+abs(SystemY[i]-SystemY[j])) < 30)
                {
                    system_too_close = true;
                    break;
                }
            }
        }
        while (system_too_close);
        for (j = 0; j < (MAXCIVS-2); ++j)
        {
            SystemFlags[j][i] = FLAG_UNKNOWN;
        }
    }

    for(k = 1; k < (MAXCIVS-2); ++k)
    {
        WRITE(20+k*8,30,1,WRITE_Left|WRITE_Shadow,&(XScreen->RastPort),0,"O");
        // pick the next, empty star-system as homesystem for this civ
        for (i = 1; i < MAXSYSTEMS; ++i)
        {
            if (0 == SystemHeader[i].Planets)
            {
                break;
            }
        }
        // mark this system (i) for _all other_ players (j) as "taken" by Civ (k)
        for (j = 0; j < (MAXCIVS-2); ++j) { SystemFlags[j][i] = GETCIVFLAG(k+1); }
        CREATENEWSYSTEM(i, k, HomePlanets); // create at least #HomePlanets in this system
        if (0 == SystemHeader[i].Planets)
        {
            // @TODO cleanup
            // errorhandling - out-of-memory
            return false;
        }

        SystemHeader[i].FirstShip.Owner = GETCIVFLAG(k+1);
        SystemHeader[i].SysOwner        = GETCIVFLAG(k+1);
        for(j = 0; j < SystemHeader[i].Planets; ++j)
        {
            PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            if (2 == j)
            {
                // always turn the 2nd Planet in the system into the 1st HomePlanet of this Civ
                ProjectMem = AllocMem(sizeof(ByteArr42), MEMF_ANY|MEMF_CLEAR);
                if (NULL == ProjectMem)
                {
                    // @TODO cleanup and free some memory..
                    // @TODO out-of-memory handling needed...
                    return false;
                }
                *PlanetHeader = (r_PlanetHeader) {CLASS_EARTH, 1,GETCIVFLAG(k+1),GETCIVFLAG(k+1),"", 13, 0,
                                                  4000, 73,170,165,160,0,0,0,DefaultShip,(ByteArr42*) ProjectMem};
            }
            (void) my_strcpy(PlanetHeader->PName, PNames[k+1].data[j]);
            PlanetHeader->Water = PlanetHeader->Water / PlanetHeader->Size;
            PlanetHeader->Size  = (rand()%15)+5;
            PlanetHeader->Water = PlanetHeader->Water * PlanetHeader->Size;
        }
        SystemFlags[k][i] |= FLAG_KNOWN;

        if (1 < HomePlanets)
        {
            INIT_HOMEPLANETS(k, i, 2);
        }
    }

    // raise project costs for all human/civ players... by 3%
    if (1 < HomePlanets)
    {
        for(i = 0; i < (MAXCIVS-2); ++i)
        {
            if (0 != Save.CivPlayer[i])
            {
                for(j = 1; j < 43; ++j)
                {
                    Save.ProjectCosts[i][j] += (HomePlanets+3)*((Save.ProjectCosts[i][j] * 0x7B)>>12); // 0x7B >> 12 =~ 0.03
                }
            }
        }
    }

    // create wormholes...
    for (j = 0; j < MAXHOLES; ++j)
    {
        /* no civilization shall know about the wormholes */
        for (i = 0; i < MAXCIVS; ++i)
        {
            MyWormHole[j].CivKnowledge[i] = 0;
        }
        /* pick random start and end systems */
        MyWormHole[j].System[0] = rand()%(MAXSYSTEMS+1);    /* generate a number 0 <= x <= MAXSYSTEMS */
        MyWormHole[j].System[1] = rand()%(MAXSYSTEMS+1);
        /* if the wormhole starts/ends in zero system (id = 0 -> no real star system)
         * or if it starts where it ends, it will be eleminated ... */
        if ((0 == MyWormHole[j].System[0]) ||
            (0 == MyWormHole[j].System[1]) ||
            (MyWormHole[j].System[0] == MyWormHole[j].System[1]))
        {
            MyWormHole[j].System[0] = 0;
            MyWormHole[j].System[1] = 0;
        } else {
            /* if the wormhole is valid, the position will be determined */
            for (i = 0; i < 2; ++i)
            {
                /* pick any number  4 <= x <= 35 */
                MyWormHole[j].PosX[i] = 4+(rand()%32);
                if (0 == (rand()%2)) { MyWormHole[j].PosX[i] = -MyWormHole[j].PosX[i]; }

                MyWormHole[j].PosY[i] = 4+(rand()%32);
                if (0 == (rand()%2)) { MyWormHole[j].PosY[i] = -MyWormHole[j].PosY[i]; }
            }
        }
    }
    return true;
}
