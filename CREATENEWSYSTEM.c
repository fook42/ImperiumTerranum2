#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const char* const Romanum[] = {" I"," II"," III"," IV"," V"," VI"," VII"," VIII"," IX"," X"," XI"};

void CREATENEWSYSTEM(uint8 ActSys,uint8 CivVar)
{
    int     i,j,l;
    double  sin_rot,cos_rot,d;
    bool    life_possible;
    r_PlanetHeader* MyPlanetHeader;
    time_t  t;
    srand((unsigned) time(&t));

    Save.ImperatorState[CivVar-1] += 50;
    SystemHeader[ActSys].Planets    = (rand()%(MAXPLANETS-3))+4;
    SystemHeader[ActSys].PlanetMemA = (r_PlanetHeader*) AllocMem(SystemHeader[ActSys].Planets*sizeof(r_PlanetHeader),MEMF_CLEAR);
    if (NULL == SystemHeader[ActSys].PlanetMemA)
    {
        SystemHeader[ActSys].Planets = 0;
        return;
    }
    life_possible = false;
    for (i = 0; i < SystemHeader[ActSys].Planets; ++i)
    {
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[i]);

        MyPlanetHeader->Class = rand()%CLASS_MAX_TYPES;
        if ( (false == life_possible ) &&
            ((CLASS_DESERT == MyPlanetHeader->Class) || (CLASS_HALFEARTH == MyPlanetHeader->Class)
          || (CLASS_EARTH  == MyPlanetHeader->Class) || (CLASS_ICE       == MyPlanetHeader->Class)
          || (CLASS_STONES == MyPlanetHeader->Class) || (CLASS_WATER     == MyPlanetHeader->Class)))
        {
            life_possible = true;
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
        MyPlanetHeader->PosY = MyPlanetHeader->PosX;
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

    if (false == life_possible)
    {
        // if no life was possible on any planet in this system.. make one inhabitable
        MyPlanetHeader = &(SystemHeader[ActSys].PlanetMemA[(rand()%2)+2]);

        l = (rand()%50)+5;
        MyPlanetHeader->Class = CLASS_EARTH;
        MyPlanetHeader->Size  = (rand()%197)+1;
        MyPlanetHeader->Ethno = 0;
        MyPlanetHeader->PosY  = 0.0;
        MyPlanetHeader->Population = l;
        MyPlanetHeader->Water = l*73;
        MyPlanetHeader->Biosphaere    = 200;
        MyPlanetHeader->Infrastruktur = (rand()%50)+50;
        MyPlanetHeader->Industrie     = (rand()%50)+50;
        MyPlanetHeader->XProjectPayed = 0;
        memcpy(&MyPlanetHeader->FirstShip, &DefaultShip, sizeof(r_ShipHeader));
    }

    // now rotate the planets in the system a bit.
    l = SystemHeader[ActSys].Planets*20;
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
