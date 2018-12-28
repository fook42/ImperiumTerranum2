#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void LINKSHIP(r_ShipHeader* SPtr, r_ShipHeader* TPtr, uint8 Mode)
{
    r_ShipHeader*   SourcePtr;
    r_ShipHeader*   TargetPtr;

    SourcePtr = SPtr;
    TargetPtr = TPtr;
    if (Mode == 1)
    {
        SourcePtr->BeforeShip->NextShip = SourcePtr->NextShip;
        if (SourcePtr->NextShip != NULL)
        {
            SourcePtr->NextShip->BeforeShip = SourcePtr->BeforeShip;
        }
    }
    SourcePtr->BeforeShip = TargetPtr;
    SourcePtr->NextShip = TargetPtr->NextShip;
    TargetPtr->NextShip = SourcePtr;
    if (SourcePtr->NextShip != NULL)
    {
        SourcePtr->NextShip->BeforeShip = SourcePtr;
    }
}
//  SrcB <-> Src <-> SrcN ...  TrgB <-> Trg <-> TrgN
//
//  SrcB <-> SrcN ...  TrgB <-> Trg <-> Src <-> TrgN
