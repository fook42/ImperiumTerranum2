#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FINDENEMYSYSTEM(uint8 ActSys, uint8 CivVar, r_ShipHeader* ShipPtr)
{
    sint32  distance, distanceY, SysEntfernung;
    uint8   i, j;
    uint8   SysID;
    r_ShipHeader*   MyShipPtr;

    MyShipPtr = ShipPtr;
    SysEntfernung = 255;
    SysID = MAXSYSTEMS;
    for(i = 0; i < Save.Systems; ++i)
    {
        if (i == ActSys) { continue; }

        distance = abs(SystemX[ActSys] - SystemX[i]);
        distanceY= abs(SystemY[ActSys] - SystemY[i]);
        if (distanceY > distance)
        {
            distance = distanceY;
        }

        if ((((  SystemFlags[ActPlayer-1][i] & FLAG_CIV_MASK) != 0)
            && ((SystemFlags[          0][i] & FLAG_CIV_MASK) != 0))
            || (MyShipPtr->Flags == SHIPFLAG_WATER))
        {
            if (distance < SysEntfernung)
            {
                if ((MyShipPtr->Flags == SHIPFLAG_WATER) && (0 == (rand()%3)))
                {
                    SysEntfernung = distance;
                    SysID = i;
                    if (   (TARGET_STARGATE == SystemHeader[ActSys].FirstShip.SType)
                        && (TARGET_STARGATE == SystemHeader[ SysID].FirstShip.SType))
                    {
                        SysEntfernung = 255;
                    }
                } else if (((SystemFlags[0][i] & FLAG_CIV_MASK) != 0)
                        && ((Save.WarState[CivVar][GETCIVVAR(SystemFlags[0][i])-1]==LEVEL_WAR)
                        ||  (Save.WarState[CivVar][GETCIVVAR(SystemFlags[0][i])-1]==LEVEL_COLDWAR)))
                {
                    SysEntfernung = distance;
                    SysID = i;
                }
            }
        }
    }
    if (MAXSYSTEMS == SysID)
    {
        SysID = (rand()%Save.Systems);
    }

    if (0 == SystemHeader[SysID].Planets)
    {
        CREATENEWSYSTEM(SysID, CivVar, 1);
    }
    if ((TARGET_STARGATE == SystemHeader[ActSys].FirstShip.SType)
     && (TARGET_STARGATE == SystemHeader[ SysID].FirstShip.SType))
    {
        MyShipPtr->Target = TARGET_ENEMY_SHIP;
        MyShipPtr->TargetShip = &SystemHeader[ActSys].FirstShip;
        MyShipPtr->Source = SysID+1;
        return;
    }
    for(i = 0; i < MAXHOLES; ++i)
    {
        if (0 == MyWormHole[i].System[0]) { continue; }

        if (FLAG_KNOWN == MyWormHole[i].CivKnowledge[CivVar])
        {
            for(j = 0; j < 2; ++j)
            {
                if ((MyWormHole[i].System[j] == (ActSys+1))
                 && ((SystemFlags[0][MyWormHole[i].System[1-j]-1] & FLAG_CIV_MASK) != 0))
                {
                    if ((Save.WarState[CivVar][GETCIVVAR(SystemFlags[0][MyWormHole[i].System[1-j]-1])-1]==LEVEL_WAR)
                      ||(Save.WarState[CivVar][GETCIVVAR(SystemFlags[0][MyWormHole[i].System[1-j]-1])-1]==LEVEL_COLDWAR))
                    {
                        SysID = MyWormHole[i].System[1-j]-1;
                        /* *** Feindliche Systeme mit Wurmloch bevorzugt angreifen *** */
                    }
                }
            }
        }
    }
    for(i = 0; i < MAXHOLES; i++)
    {
        if (0 == MyWormHole[i].System[0]) { continue; }

        if (FLAG_KNOWN == MyWormHole[i].CivKnowledge[CivVar])
        {
            for(j = 0; j < 2; j++)
            {
                if ((MyWormHole[i].System[j  ] == (ActSys+1))
                 && (MyWormHole[i].System[1-j] == (SysID+1)))
                {
                    MyShipPtr->Target = -(i+1);
                    MyShipPtr->Source = -(j+1);
                    return;
                }
            }
        }
    }
    MyShipPtr->Source = (ActSys+1);
    MyShipPtr->Target = (SysID+1);
    distance = -(SysEntfernung / ShipData(MyShipPtr->SType).MaxMove)-1;
    if (-127 > distance)
    {
        distance = -127;
    }
    MyShipPtr->Moving = distance;

    LINKSHIP(MyShipPtr, &SystemHeader[SysID].FirstShip, 1);
}
