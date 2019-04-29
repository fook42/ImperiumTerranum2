#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CREATECIVILWAR(uint8 CivVar)
{
    int     i,j;
    uint8   len;
    uint8   CivFlag;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    char    s[99];
    APTR    ModC = NULL;
    uint32  ModL;

    CivFlag = GETCIVFLAG(CivVar);
    if ((0 != Save.WorldFlag) && (Save.WorldFlag != CivFlag))
    { return; }
    if  (0 == Save.WorldFlag)
    {
        len = strlen(PText[385]);
        memcpy(s, PText[385], len);
        s[len++]=' ';
        strcpy(s+len, GETCIVNAME(CivVar));
        len = strlen(s);
        s[len++]=' ';
        strcpy(s+len, PText[386]);
        if (0 != Save.CivPlayer[CivVar-1])
        {
            ModC = GETTHESOUND(1);
            ModL = ModMemL[1];
            REQUEST(s,PText[387],CivFlag,CivFlag);
        }
        for(i = 0; i < (MAXCIVS-1); ++i)
        {
            Save.WarState[i][7] = LEVEL_PEACE;
            Save.WarState[7][i] = LEVEL_PEACE;
        }
        Save.WarState[CivVar-1][7] = LEVEL_WAR;
        Save.WarState[7][CivVar-1] = LEVEL_WAR;
        Save.GSteuer[7] = 0;
        Save.Staatstopf[7] = -5000;
        for(i = 1; i < 43; ++i)
        {
            Save.TechCosts[7].data[i]    = Save.TechCosts[CivVar-1].data[i];
            Save.ProjectCosts[7].data[i] = Save.ProjectCosts[CivVar-1].data[i];
        }
    }
    Save.WorldFlag = CivFlag;
    Save.CivilWar = CivVar;
    SETWORLDCOLORS();
    for(i = 0; i < Save.Systems; ++i)
    {
        for(j = 0; j < SystemHeader[i].Planets; ++j)
        {
            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            if ((GETCIVVAR(MyPlanetHeader->PFlags) == CivVar) && ((rand()%100) >= 50))
            {
                MyPlanetHeader->PFlags = FLAG_OTHER;
                Save.Bevoelkerung[7] += MyPlanetHeader->Population;
                if (NULL != MyPlanetHeader->FirstShip.NextShip)
                {
                    MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                    do
                    {
                        MyShipPtr->Owner = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                        Save.WarPower[7] += it_round((double) ShipData(MyShipPtr->SType).WeaponPower * (MyShipPtr->Weapon/10.0+1));
                        MyShipPtr = MyShipPtr->NextShip;
                    }
                    while (NULL != MyShipPtr);
                }
            }
        }
    }
    if (NULL != ModC)
    {
        StopPlayer();
        FreeMem(ModC, ModL);
    }
    if ((Save.WarPower[7]*3) < (Save.WarPower[CivVar-1]-Save.WarPower[7]))
    {
        CREATECIVILWAR(CivVar);
    }
}
