#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void STOPCIVILWAR(uint8 EndText)
{
    uint8   CivVar,CivFlag,i,j;
    char    s[99];
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    APTR    ModC = NULL;
    uint32  ModL;

    if ((0            == Save.WorldFlag) ||
        (WFLAG_CEBORC == Save.WorldFlag) ||
        (WFLAG_FIELD  == Save.WorldFlag) ||
        (WFLAG_DCON   == Save.WorldFlag) ||
        (WFLAG_JAHADR == Save.WorldFlag))
    {
        return;
    }
    INFORMUSER();
    CivVar = GETCIVVAR(Save.WorldFlag)-1;
    CivFlag = Save.WorldFlag;
    Save.WorldFlag = 0;
    Save.Bevoelkerung[7] = 0;
    Save.WarPower[7] = 0;
    Save.Staatstopf[CivVar] += Save.Staatstopf[7];
    Save.Staatstopf[7] = 0;
    if (0 != Save.CivPlayer[CivVar])
    {
        ModC = GETTHESOUND(2);
        ModL = ModMemL[2];
    }
    if (0 == EndText)
    {
        strcpy(s, _PT_Die);
        strcat(s, " ");
        strcat(s, GETCIVNAME(CivVar+1));
        strcat(s, " ");
        strcat(s, PText[216]);
        
        if (0 != Save.CivPlayer[CivVar])
        {
            REQUEST(s,PText[217],CivFlag,CivFlag);
        }
    } else if (1 == EndText)
    {
        if (0 != Save.CivPlayer[CivVar])
        {
            REQUEST(PText[218],PText[219],CivFlag,CivFlag);
        }
        for(i = 0; i < (MAXCIVS-2); i++)
        {
            if (i != CivVar)
            {
                Save.WarState[i][CivVar] = Save.WarState[i][7];
                Save.WarState[CivVar][i] = Save.WarState[7][i];
            }
        }
    } else if (2 == EndText)
    {
        if (0 != Save.CivPlayer[CivVar])
        {
            REQUEST(PText[220],PText[221],CivFlag,CivFlag);
        }
    }
    for(i = 0; i < (MAXCIVS-1); i++)
    {
        Save.WarState[i][7] = LEVEL_UNKNOWN;
        Save.WarState[7][i] = LEVEL_UNKNOWN;
    }

    for(i = 1; i <= 42; i++)
    {
        if (Save.TechCosts[7].data[i] <= 0)
        {
            Save.TechCosts[CivVar].data[i] = 0;
        }
    }
    for(i = 0; i < Save.Systems; i++)
    {
        if ((SystemFlags[0][i] & FLAG_CIV_MASK) == FLAG_OTHER)
        {
            SystemFlags[0][i] = (SystemFlags[0][i] & (~FLAG_OTHER)) | CivFlag;
        }
        if (NULL != SystemHeader[i].FirstShip.NextShip)
        {
            MyShipPtr = SystemHeader[i].FirstShip.NextShip;
            do
            {
                if (MyShipPtr->Owner == FLAG_OTHER)
                {
                    if ((rand()%100)<15)
                    {
                        MyShipPtr->Owner = FLAG_MAQUES;
                        MyShipPtr->Flags = 0;
                        MyShipPtr->Target = 0;
                    } else {
                        MyShipPtr->Owner = CivFlag;
                        Save.WarPower[CivVar] += it_round((double) ShipData(MyShipPtr->SType).WeaponPower*(MyShipPtr->Weapon/10.0+1));
                    }
                }
                MyShipPtr = MyShipPtr->NextShip;
            }
            while (NULL != MyShipPtr);
        }
        for(j = 0; j < SystemHeader[i].Planets; j++)
        {
            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);

            if (8 == GETCIVVAR(MyPlanetHeader->PFlags))
            {
                MyPlanetHeader->PFlags = CivFlag;
                if ((MyPlanetHeader->Ethno & FLAG_CIV_MASK) == FLAG_OTHER)
                {
                    MyPlanetHeader->Ethno = MyPlanetHeader->PFlags;
                }
                Save.Bevoelkerung[CivVar] += MyPlanetHeader->Population;
                if (NULL != MyPlanetHeader->FirstShip.NextShip)
                {
                    MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                    do
                    {
                        MyShipPtr->Owner = CivFlag;
                        Save.WarPower[CivVar] += it_round((double) ShipData(MyShipPtr->SType).WeaponPower*(MyShipPtr->Weapon/10.0+1));
                        MyShipPtr = MyShipPtr->NextShip;
                    }
                    while (NULL != MyShipPtr);
                }
            }
        }
    }
    REFRESHDISPLAY();
    SETWORLDCOLORS();
    if (NULL != ModC)
    {
        StopPlayer();
        FreeMem(ModC, ModL);
    }
}
