#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void LINKSHIP(r_ShipHeader* SourcePtr, r_ShipHeader* TargetPtr, const int Mode)
{
    if (1 == Mode)
    {
        SourcePtr->BeforeShip->NextShip = SourcePtr->NextShip;
        if (NULL != SourcePtr->NextShip)
        {
            SourcePtr->NextShip->BeforeShip = SourcePtr->BeforeShip;
        }
    }
    SourcePtr->BeforeShip = TargetPtr;
    SourcePtr->NextShip = TargetPtr->NextShip;
    TargetPtr->NextShip = SourcePtr;
    if (NULL != SourcePtr->NextShip)
    {
        SourcePtr->NextShip->BeforeShip = SourcePtr;
    }
}
// before:  SrcB <-> Src <-> SrcN ...  TrgB <-> Trg <-> TrgN
//
// after:   SrcB <-> SrcN ...  TrgB <-> Trg <-> Src <-> TrgN
