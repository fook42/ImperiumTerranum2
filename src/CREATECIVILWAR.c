#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATECIVILWAR(int CivVar)
{
    int     i,j;
    uint8   CivFlag;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    char    s[99];
    char*   _s;
    APTR    ModC = NULL;

    CivFlag = GETCIVFLAG(CivVar);

    if ((0 != Save.WorldFlag) && (CivFlag != Save.WorldFlag)) { return; }

    if  (0 == Save.WorldFlag)
    {
        --CivVar; // to shift the arrays ...
        if (0 != Save.CivPlayer[CivVar])
        {
            _s=my_strcpy(s, PText[385]);        // das Militär der ..
            *_s++ = ' ';
            _s=my_strcpy(_s, GETCIVNAME(CivVar+1));
            *_s++ = ' ';
            (void) my_strcpy(_s, PText[386]);   // putscht und ..

            ModC = GETTHESOUND(1);
            REQUEST(s,PText[387],CivFlag,CivFlag);  // spaltet das Imperium...
        }
        for(i = 0; i < (MAXCIVS-1); ++i)
        {
            Save.WarState[i][7] = LEVEL_PEACE;
            Save.WarState[7][i] = LEVEL_PEACE;
        }
        Save.WarState[CivVar][7] = LEVEL_WAR;
        Save.WarState[7][CivVar] = LEVEL_WAR;
        Save.GSteuer[7] = 0;
        Save.Staatstopf[7] = -5000;
        for(i = 1; i < 43; ++i)
        {
            Save.TechCosts[   7].data[i] = Save.TechCosts[   CivVar].data[i];
            Save.ProjectCosts[7].data[i] = Save.ProjectCosts[CivVar].data[i];
        }
        ++CivVar; // to shift the arrays ...
    }
    Save.WorldFlag = CivFlag;
    Save.CivilWar = (uint8) CivVar;
    SETWORLDCOLORS();
    for(i = 0; i < Save.Systems; ++i)
    {
        if (0 == SystemHeader[i].Planets) { continue; }

        for(j = 0; j < SystemHeader[i].Planets; ++j)
        {
            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            if (NULL == MyPlanetHeader) { continue; }

            if ((GETCIVVAR(MyPlanetHeader->PFlags) == CivVar) && (49 < (rand()%100)))
            {
                MyPlanetHeader->PFlags = FLAG_OTHER;
                Save.Bevoelkerung[7] += MyPlanetHeader->Population;
                MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                while (NULL != MyShipPtr)
                {
                    MyShipPtr->Owner = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                    Save.WarPower[7] += it_round((double) ShipData(MyShipPtr->SType).WeaponPower * (MyShipPtr->Weapon/10.0+1));
                    MyShipPtr = MyShipPtr->NextShip;
                }
            }
        }
    }
    if (NULL != ModC)
    {
        StopPlayer();
        UnLoadModule(ModC);
    }

    if ((0 < Save.WarPower[7]) && ((Save.WarPower[7]*4) < Save.WarPower[CivVar-1]))
    {
        CREATECIVILWAR(CivVar);
    }
}
