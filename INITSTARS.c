#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool INITSTARS()
{
    uint32          l,HomePlanetProd;
    bool            system_too_close;
    APTR            PMemA[MAXCIVS][6];
    int             i,j,k;
    double          sin_rot, cos_rot, d;
    r_PlanetHeader* PlanetHeader;
    ByteArr42*      ActPProjects;

    time_t  t;
    srand((unsigned) time(&t));

    SystemFlags[0][0] = FLAG_TERRA|FLAG_KNOWN;
    for (i = 1; i<(MAXCIVS-1); i++)
    {
        for (j = 1; j <= HomePlanets; j++)
        {
            PMemA[i][j] = AllocMem(sizeof(ByteArr42), MEMF_CLEAR);
            if (NULL == PMemA[i][j]) { return false; }
            ActPProjects = (ByteArr42*) PMemA[i][j];
            ActPProjects->data[0] = 1;
        }
    }

    for (i = 0; i < MAXSYSTEMS; i++)
    {
        SystemHeader[i] = (r_SystemHeader) {NULL,0,DefaultShip,0,0,0};
    }
    SystemHeader[0].FirstShip.Owner = FLAG_TERRA;
    SystemHeader[0].Planets = 9;
    SystemHeader[0].PlanetMemA = (r_PlanetHeader*) AllocMem(SystemHeader[0].Planets*sizeof(r_PlanetHeader),MEMF_CLEAR);
    if (NULL == SystemHeader[0].PlanetMemA) { return false; }

    PlanetHeader = &(SystemHeader[0].PlanetMemA[0]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_DESERT,   2,FLAG_UNKNOWN,0,"",4,4,0,24,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[1]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_HALFEARTH,7,FLAG_UNKNOWN,0,"",7,7,0,283,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[2]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_EARTH,   10,FLAG_TERRA,FLAG_TERRA,"",10,10,4000,760,170,165,160,0,0,0,DefaultShip,(ByteArr42*) PMemA[1][1]};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[3]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_DESERT,   5,FLAG_UNKNOWN,0,"",13,13,0,61,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[4]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_GAS,    115,FLAG_UNKNOWN,0,"",16,16,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[5]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_SATURN,  82,FLAG_UNKNOWN,0,"",19,19,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[6]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_GAS,     41,FLAG_UNKNOWN,0,"",22,22,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[7]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_GAS,     45,FLAG_UNKNOWN,0,"",25,25,0,0,0,0,0,0,0,0,DefaultShip,NULL};

    PlanetHeader = &(SystemHeader[0].PlanetMemA[8]);
    *PlanetHeader = (r_PlanetHeader) {CLASS_DESERT,   1,FLAG_UNKNOWN,0,"",30,30,0,11,0,0,0,0,0,0,DefaultShip,NULL};
    
    for(i=0; i<9; i++)
    {
        strcpy(SystemHeader[0].PlanetMemA[i].PName, PNames[1].data[i]);
    }
    SystemHeader[0].SysOwner = FLAG_TERRA;

    HomePlanetProd = 0;
    if (HomePlanets > 1)
    {
        l = HomePlanets;
        for (j = 0; j < SystemHeader[0].Planets; j++)
        {
            PlanetHeader = &(SystemHeader[0].PlanetMemA[j]);
            if (  (PlanetHeader->Class==CLASS_DESERT) || (PlanetHeader->Class==CLASS_HALFEARTH)
                ||(PlanetHeader->Class==CLASS_EARTH)  || (PlanetHeader->Class==CLASS_WATER)
                ||(PlanetHeader->Class==CLASS_STONES) || (PlanetHeader->Class==CLASS_ICE))
            {
                if ((l>0) && ((l>1) || (j>1)))
                {
                    PlanetHeader->PFlags = FLAG_TERRA;
                    PlanetHeader->Ethno  = PlanetHeader->PFlags;
                    PlanetHeader->Water  = PlanetHeader->Water / PlanetHeader->Size;
                    PlanetHeader->Size   = (rand()%15)+5;
                    PlanetHeader->Water  = PlanetHeader->Water*PlanetHeader->Size;
                    PlanetHeader->Population    = PlanetHeader->Size*400;
                    PlanetHeader->Biosphaere    = 170;
                    PlanetHeader->Infrastruktur = 165;
                    PlanetHeader->Industrie     = 160;
                    PlanetHeader->ProjectPtr    = (ByteArr42*) PMemA[1][l];
                    l--;
                    HomePlanetProd += PlanetHeader->Size;
                }
            }
        }
        Save.ImperatorState[0] -= HomePlanetProd*3;
        if (it_round(HomePlanetProd/15.0)>1)
        {
            for (i = 1; i<=42; i++)
            {
                Save.TechCosts[0].data[i] = it_round((double) Save.TechCosts[0].data[i]*(HomePlanetProd/15.0));
            }
        }
    }

    for (i = 0; i < 200; i++)
    {
        for (j = 0; j < SystemHeader[0].Planets; j++)
        {
            PlanetHeader = &(SystemHeader[0].PlanetMemA[j]);
            d = 1.0/((j*3.0)+4);
            sin_rot = sin(d);
            cos_rot = cos(d);
            d = PlanetHeader->PosX; // added... to store previous PosX... d is unused at this point
            PlanetHeader->PosX = PlanetHeader->PosX*cos_rot - PlanetHeader->PosY*sin_rot;
            PlanetHeader->PosY = d*sin_rot                  + PlanetHeader->PosY*cos_rot;
        }
    }
    SystemX[0] = 10+(rand()%250)+(rand()%208);
    SystemY[0] = 10+(rand()%250)+(rand()%240);

    for (i = 1; i < MAXSYSTEMS; i++)
    {
        do
        {
            SystemX[i] = 10+(rand()%250)+(rand()%208);
            SystemY[i] = 10+(rand()%250)+(rand()%240);
            system_too_close = false;
            // check if the new system is too close to any previous one..
            // reduced calculation: dist^2=x^2+y^2 ... x,y = delta x,y
            // if x+y < 30 -> dist is max. sqrt(550)=~ 23
            for (j = 0; j < i; j++)
            {
                if ((abs(SystemX[i]-SystemX[j])+abs(SystemY[i]-SystemY[j])) < 30)
                {
                    system_too_close = true;
                }
            }
        }
        while (system_too_close);
        SystemHeader[i].Planets = 0;
        SystemHeader[i].PlanetMemA = NULL;
        memcpy(&SystemHeader[i].FirstShip, &DefaultShip, sizeof(r_ShipHeader));
        for (j = 0; j < (MAXCIVS-2); j++)
        {
            SystemFlags[j][i] = FLAG_UNKNOWN;
        }
    }
    for(k = 2; k < (MAXCIVS-1); k++)
    {
        do
        {
            i = (rand()%(MAXSYSTEMS-1))+1;
        }
        while (SystemHeader[i].Planets != 0);
        SystemFlags[0][i] = GETCIVFLAG(k);
        SystemFlags[k-1][i] = FLAG_KNOWN;
        CREATENEWSYSTEM(i, k);

        for(j = 0; j < SystemHeader[i].Planets; j++)
        {
            SystemHeader[i].FirstShip.Owner = GETCIVFLAG(k);
            PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            if (j == 2)
            {
                *PlanetHeader = (r_PlanetHeader) {CLASS_EARTH,1,GETCIVFLAG(k),GETCIVFLAG(k),"",
                    13,0,4000,73,170,165,160,0,0,0, DefaultShip, (ByteArr42*) PMemA[k][1]};
            }
            strcpy(PlanetHeader->PName, PNames[k].data[j]);
            PlanetHeader->Water = PlanetHeader->Water / PlanetHeader->Size;
            PlanetHeader->Size  = (rand()%15)+5;
            PlanetHeader->Water = PlanetHeader->Water * PlanetHeader->Size;
        }
        SystemFlags[k-1][i] += FLAG_KNOWN;
        SystemHeader[i].SysOwner = GETCIVFLAG(k);

        if (HomePlanets>1)
        {
            HomePlanetProd = 0;
            l = HomePlanets;
            for(j = 0; j <= SystemHeader[i].Planets; j++)
            {
                PlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                if (   (PlanetHeader->Class==CLASS_DESERT) || (PlanetHeader->Class==CLASS_HALFEARTH)
                    || (PlanetHeader->Class==CLASS_EARTH)  || (PlanetHeader->Class==CLASS_WATER)
                    || (PlanetHeader->Class==CLASS_STONES) || (PlanetHeader->Class==CLASS_ICE))
                {
                    if ((l>0) && ((l>1) || (j>1)))
                    {
                        PlanetHeader->PFlags = GETCIVFLAG(k);
                        PlanetHeader->Ethno  = PlanetHeader->PFlags;
                        PlanetHeader->Water  = PlanetHeader->Water / PlanetHeader->Size;
                        PlanetHeader->Size   = (rand()%15)+5;
                        PlanetHeader->Water  = PlanetHeader->Water * PlanetHeader->Size;
                        PlanetHeader->Population    = PlanetHeader->Size*400;
                        PlanetHeader->Biosphaere    = 170;
                        PlanetHeader->Infrastruktur = 165;
                        PlanetHeader->Industrie     = 160;
                        PlanetHeader->ProjectPtr    = (ByteArr42*) PMemA[k][l];
                        l--;
                        HomePlanetProd += PlanetHeader->Size;
                    }
                }
            }
            Save.ImperatorState[k-1] -= (HomePlanetProd*3);
            if ((HomePlanetProd/15)>1)
            {
                for(i = 1; i <= 42; i++)
                {
                    Save.TechCosts[k-1].data[i] = it_round(Save.TechCosts[k-1].data[i]*(HomePlanetProd/15.0));
                }
            }
        }
    }
    if (HomePlanets>1)
    {
        for(k = 0; k < (HomePlanets+3); k++)
        {
            for(i = 0; i < (MAXCIVS-2); i++)
            {
                if (Save.CivPlayer[i] != 0)
                {
                    for(j = 1; j <= 42; j++)
                    {
                        Save.ProjectCosts[i].data[j] = it_round(Save.ProjectCosts[i].data[j]*INFLATION);
                    }
                }
            }
        }
    }

    for (j = 0; j < MAXHOLES; j++)
    {
        /* no civilization shall know about the wormholes */
        for (i = 0; i < MAXCIVS; i++)
        {
            MyWormHole[j].CivKnowledge[i] = 0;
        }
        /* pick random start and end systems */
        MyWormHole[j].System[0] = rand()%(MAXSYSTEMS+1);    /* generate a number 0 <= x <= MAXSYSTEMS */
        MyWormHole[j].System[1] = rand()%(MAXSYSTEMS+1);
        /* if the wormhole starts/ends in solar system (id = 0)
         * or if it starts where it ends, it will be eleminated ... */
        if ((MyWormHole[j].System[0] == 0) ||
            (MyWormHole[j].System[1] == 0) ||
            (MyWormHole[j].System[0] == MyWormHole[j].System[1]))
        {
            MyWormHole[j].System[0] = 0;
            MyWormHole[j].System[1] = 0;
        } else {
            /* if the wormhole is valid, the position will be detemined */
            for (i = 0; i < 2; i++)
            {
                /* pick any number  4 <= x <= 35 */
                MyWormHole[j].PosX[i] = 35-(rand()%32);
                if ((rand()%2) == 0) { MyWormHole[j].PosX[i] = -MyWormHole[j].PosX[i]; }

                MyWormHole[j].PosY[i] = 35-(rand()%32);
                if ((rand()%2) == 0) { MyWormHole[j].PosY[i] = -MyWormHole[j].PosY[i]; }
            }
        }
    }
    return true;
}
