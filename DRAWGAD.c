#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWGAD(uint16 GadID, uint16 GadSelect)
{
    uint8   Col1, Col2;
    if (1 == GadSelect)
    {
        Col1 = 100;
        Col2 = 107;
    } else {
        Col1 = 40;
        Col2 = 14;
    }
    MAKEBORDER(MyScreen[1],60,GadID,124,GadID+63,Col1,Col2,1);
    if (1 == GadSelect)
    {
        Col1 = 40;
        Col2 = 14;
    } else {
        Col1 = 100;
        Col2 = 107;
    }
    MAKEBORDER(MyScreen[1],150,GadID,214,GadID+63,Col1,Col2,1);
}
