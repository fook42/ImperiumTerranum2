#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const char* const Romanum[] = {" I"," II"," III"," IV"," V"," VI"," VII"," VIII"," IX"," X"," XI"};

void CREATENEWSYSTEM(uint8 ActSys,uint8 CivVar, uint8 minHomePlanets)
{
    int     i,j,l;
    double  sin_rot,cos_rot,d;
    uint8   life_possible;
    r_PlanetHeader* MyPlanetHeader;

    Save.ImperatorState[CivVar-1] += 50;
    SystemHeader[ActSys].Planets    = (rand()%(MAXPLANETS-3))+4;
    if (minHomePlanets > SystemHeader[ActSys].Planets) { SystemHeader[ActSys].Planets = minHomePlanets; }
    SystemHeader[ActSys].PlanetMemA = (r_PlanetHeader*) AllocMem(SystemHeader[ActSys].Planets*sizeof(r_PlanetHeader),MEMF_CLEAR);
    if (NULL == SystemHeader[ActSys].PlanetMemA)
    {
        SystemHeader[ActSys].Planets = 0;
        return;
    }
    life_possible = 0;
    for (i = 0; i < SystemHeader[ActSys].Planets; ++i)
    {
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);

        MyPlanetHeader->Class = rand()%CLASS_MAX_TYPES;
        if ( (CLASS_DESERT == MyPlanetHeader->Class) || (CLASS_HALFEARTH == MyPlanetHeader->Class)
          || (CLASS_EARTH  == MyPlanetHeader->Class) || (CLASS_ICE       == MyPlanetHeader->Class)
          || (CLASS_STONES == MyPlanetHeader->Class) || (CLASS_WATER     == MyPlanetHeader->Class))
        {
            ++life_possible;
        }
        MyPlanetHeader->Size = (rand()%206)+1;
        switch (MyPlanetHeader->Class)
        {
            case CLASS_DESERT:    MyPlanetHeader->Water = MyPlanetHeader->Size*13; break;
            case CLASS_HALFEARTH: MyPlanetHeader->Water = MyPlanetHeader->Size*38; break;
            case CLASS_EARTH:     MyPlanetHeader->Water = MyPlanetHeader->Size*73; break;
            case CLASS_ICE:       MyPlanetHeader->Water = MyPlanetHeader->Size*85; break;
            case CLASS_STONES:    MyPlanetHeader->Water = MyPlanetHeader->Size*42; break;
            case CLASS_WATER:     MyPlanetHeader->Water = MyPlanetHeader->Size*92; break;
            default:              MyPlanetHeader->Water = 0;
        }
        MyPlanetHeader->PFlags = 0;
        strcpy(MyPlanetHeader->PName, Save.SystemName.data[ActSys]);
        strcat(MyPlanetHeader->PName, Romanum[i]);

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
        if (   (CLASS_GAS     == MyPlanetHeader->Class)
            || (CLASS_SATURN  == MyPlanetHeader->Class)
            || (CLASS_PHANTOM == MyPlanetHeader->Class))
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
