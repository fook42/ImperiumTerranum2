#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 GOTONEXTSYSTEM(uint8 ActSys, r_ShipHeader* ShipPtr)
{
    sint32  l,SysEntfernung;
    int     i,j;
    uint8   SysID,CivVar;
    r_ShipHeader*   MyShipPtr;
    time_t  t;
    srand((unsigned) time(&t));

    MyShipPtr = ShipPtr;
    if ((MyShipPtr->SType<8) || (MyShipPtr->SType>24))
    {
        MyShipPtr->SType = 8;
        MyShipPtr->Owner = 0;
    }
    CivVar = GETCIVVAR(MyShipPtr->Owner);
    SysEntfernung = 255;
    SysID = 0;
    for(i = 1; i <= Save.Systems; i++)
    {
        if (i != ActSys)
        {
            l = abs(SystemX[ActSys-1]-SystemX[i-1]);
            if (abs(SystemY[ActSys-1]-SystemY[i-1]) > l)
            {
                l = abs(SystemY[ActSys-1]-SystemY[i-1]);
            }

            if ((l<SysEntfernung) && ((rand()%2) == 0))
            {
                if (((Save.GlobalFlags[CivVar-1] == GFLAG_EXPLORE) && ((SystemHeader[i-1].State & STATE_TACTICAL) == 0))
                    || ((MyShipPtr->Flags == SHIPFLAG_WATER)
                        && (((MyShipPtr->Fracht>0) && ((SystemFlags[0][i-1] & FLAG_CIV_MASK) != 0))
                            || (MyShipPtr->Fracht == 0))))
                {
                    SysEntfernung = l;
                    SysID = i;
                    if ((SystemHeader[ActSys-1].FirstShip.SType == TARGET_STARGATE)
                     && (SystemHeader[ SysID-1].FirstShip.SType == TARGET_STARGATE))
                    {
                        SysEntfernung = 255;
                    }
                }
            }
        }
    }
    if (SysID == 0)
    {
        Save.GlobalFlags[CivVar-1] = GFLAG_ATTACK;
        MyShipPtr->Moving = 0;
        return ActSys;
    }
    if (SystemHeader[SysID-1].Planets == 0)
    {
        CREATENEWSYSTEM(SysID-1, CivVar);
    }
    if ((SystemHeader[ActSys-1].FirstShip.SType == TARGET_STARGATE)
     && (SystemHeader[ SysID-1].FirstShip.SType == TARGET_STARGATE))
    {
        MyShipPtr->Target = TARGET_ENEMY_SHIP;
        MyShipPtr->TargetShip = &SystemHeader[ActSys-1].FirstShip;
        MyShipPtr->Source = SysID;
        return ActSys;
    }
    for(i = 0; i < MAXHOLES; i++)
    {
        if (MyWormHole[i].CivKnowledge[CivVar-1] == FLAG_KNOWN)
        {
            for(j = 0; j < 2; j++)
            {
                if ((MyWormHole[i].System[j] == ActSys)
                 && (MyWormHole[i].System[1-j] == SysID))
                {
                    MyShipPtr->Target = -(i+1);
                    MyShipPtr->Source = -(j+1);
                    return ActSys;
                }
            }
        }
    }
    MyShipPtr->Source = ActSys;
    MyShipPtr->Target = SysID;
    l = -(SysEntfernung / ShipData(MyShipPtr->SType).MaxMove)-1;
    if (l<-127)
    {   l = -127; }
    MyShipPtr->Moving = l;

    LINKSHIP(MyShipPtr, &SystemHeader[SysID-1].FirstShip, 1);
    return SysID;
}