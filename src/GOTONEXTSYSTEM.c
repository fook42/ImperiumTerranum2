#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 GOTONEXTSYSTEM(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    sint32  distance,SysEntfernung;
    uint8   i,j;
    uint8   SysID, CivVar;
    time_t  t;
    srand((unsigned) time(&t));

    --ActSys;   // shifted arrays
    if ((8 > MyShipPtr->SType) || (24 < MyShipPtr->SType))
    {
        MyShipPtr->SType = 8;
        MyShipPtr->Owner = 0;
    }
    CivVar = GETCIVVAR(MyShipPtr->Owner);
    SysEntfernung = 255;
    SysID = MAXSYSTEMS;
    for(i = 0; i < Save.Systems; ++i)
    {
        if (i != ActSys)
        {
            distance = abs(SystemX[ActSys] - SystemX[i]);
            if (abs(SystemY[ActSys] - SystemY[i]) > distance)
            {
                distance = abs(SystemY[ActSys]-SystemY[i]);
            }

            if ((distance < SysEntfernung) && (0 == (rand()%2)))
            {
                if (((Save.GlobalFlags[CivVar-1] == GFLAG_EXPLORE) && ((SystemHeader[i].State & STATE_TACTICAL) == 0))
                    || ((MyShipPtr->Flags == SHIPFLAG_WATER)
                        && (((MyShipPtr->Fracht>0) && ((SystemFlags[0][i] & FLAG_CIV_MASK) != 0))
                            || (MyShipPtr->Fracht == 0))))
                {
                    SysEntfernung = distance;
                    SysID = i;
                    if ((TARGET_STARGATE == SystemHeader[ActSys].FirstShip.SType)
                     && (TARGET_STARGATE == SystemHeader[ SysID].FirstShip.SType))
                    {
                        SysEntfernung = 255;
                    }
                }
            }
        }
    }
    if (MAXSYSTEMS == SysID)
    {
        Save.GlobalFlags[CivVar-1] = GFLAG_ATTACK;
        MyShipPtr->Moving = 0;
        return (ActSys+1);
    }
    if (0 == SystemHeader[SysID].Planets)
    {
        CREATENEWSYSTEM(SysID, CivVar-1, 1);
    }
    if ((TARGET_STARGATE == SystemHeader[ActSys].FirstShip.SType)
     && (TARGET_STARGATE == SystemHeader[ SysID].FirstShip.SType))
    {
        MyShipPtr->Target = TARGET_ENEMY_SHIP;
        MyShipPtr->TargetShip = &SystemHeader[ActSys].FirstShip;
        MyShipPtr->Source = SysID;
        return (ActSys+1);
    }
    for(i = 0; i < MAXHOLES; ++i)
    {
        if (FLAG_KNOWN == MyWormHole[i].CivKnowledge[CivVar-1])
        {
            for(j = 0; j < 2; ++j)
            {
                if ((MyWormHole[i].System[j  ] == (ActSys+1))
                 && (MyWormHole[i].System[1-j] == (SysID+1)))
                {
                    MyShipPtr->Target = -(i+1);
                    MyShipPtr->Source = -(j+1);
                    return (ActSys+1);
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
    return (SysID+1);
}