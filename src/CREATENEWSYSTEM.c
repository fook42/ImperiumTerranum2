#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const char* const Romanum[] = {" I"," II"," III"," IV"," V"," VI"," VII"," VIII"," IX"," X"," XI"};
const uint8 ClassWaterFactor[] = {42, 0, 92, 0, 38, 13, 73, 0, 85};
//                                St, G,  W,Sa, HE, De,  E, P,  I

// create a new starsystem (ActSys) with >= "minHomePlanets" planets
void CREATENEWSYSTEM(const int ActSys, const int CivVar, const int minHomePlanets)
{
    int     i,j,l;
    double  sin_rot,cos_rot,d;
    uint8   life_possible;
    char*   _s;
    r_PlanetHeader* MyPlanetHeader;

    // give credits to the discoverer
    Save.ImperatorState[CivVar] += 50;

    SystemHeader[ActSys].Planets    = (rand()%(MAXPLANETS-3))+4; // 4..MAXPLANETS
    if (minHomePlanets > SystemHeader[ActSys].Planets) { SystemHeader[ActSys].Planets = minHomePlanets; }
    SystemHeader[ActSys].PlanetMemA = (r_PlanetHeader*) AllocMem(SystemHeader[ActSys].Planets*sizeof(r_PlanetHeader),MEMF_CLEAR);
    if (NULL == SystemHeader[ActSys].PlanetMemA)
    {
        // no memory, no planets
        SystemHeader[ActSys].Planets = 0;
        return;
    }
    life_possible = 0;
    for (i = 0; i < SystemHeader[ActSys].Planets; ++i)
    {
        // create new planet
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);

        MyPlanetHeader->Class = rand()%CLASS_MAX_TYPES;

        life_possible += ClassLifeFactor[MyPlanetHeader->Class];

        MyPlanetHeader->Size = (rand()%206)+1;

        // each class has its specific water-ammount
        MyPlanetHeader->Water = MyPlanetHeader->Size * ClassWaterFactor[MyPlanetHeader->Class];
        MyPlanetHeader->PFlags = 0;

        // generate the name by using the systemname + romanum-number (I, II, III...etc)
        _s=my_strcpy(MyPlanetHeader->PName, Save.SystemName.data[ActSys]);
        (void) my_strcpy(_s, Romanum[i]);

        MyPlanetHeader->PosX = (float) (i+i+i+4);
        MyPlanetHeader->PosY = (rand()%2 == 0) ? MyPlanetHeader->PosX : (-MyPlanetHeader->PosX);
        if (((CLASS_EARTH == MyPlanetHeader->Class) || (CLASS_WATER == MyPlanetHeader->Class)) && (0 == (rand()%10)))
        {
            MyPlanetHeader->Biosphaere = 150+(rand()%50);
        } else {
            MyPlanetHeader->Biosphaere = 0;
        }
        MyPlanetHeader->Infrastruktur = 0;
        MyPlanetHeader->Industrie = 0;
        MyPlanetHeader->Population = 0;
        MyPlanetHeader->XProjectCosts = 0;
        MyPlanetHeader->XProjectPayed = 1;
        MyPlanetHeader->ProjectID = 0;
        MyPlanetHeader->ProjectPtr = NULL;
    }

    // while less than "minHomePlanets" are inhabitable, randomly pick one and convert the planet
    while (minHomePlanets > life_possible)
    {
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[rand()%SystemHeader[ActSys].Planets]);
        if (0 == ClassLifeFactor[MyPlanetHeader->Class])
        {
            MyPlanetHeader->Class = CLASS_EARTH;
            MyPlanetHeader->Water = MyPlanetHeader->Size*60;
            MyPlanetHeader->Biosphaere = 50+(rand()%50);
            ++life_possible;
        }
    }

    // now rotate the planets in the system a bit.
    l = SystemHeader[ActSys].Planets*(20+(rand()%100));
    for (i = 0; i < l; ++i)
    {
        for (j = 0; j < SystemHeader[ActSys].Planets; ++j)
        {
            MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[j]);

            d = 1.0/((float) (j+j+j+4));
            sin_rot = sin(d);
            cos_rot = cos(d);
            d = MyPlanetHeader->PosX;
            MyPlanetHeader->PosX = d * cos_rot - MyPlanetHeader->PosY*sin_rot;
            MyPlanetHeader->PosY = d * sin_rot + MyPlanetHeader->PosY*cos_rot;
        }
    }
}
