#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void RECTWIN(struct RastPort* RPort, uint16 Color, uint16 LEdge, uint16 TEdge, uint16 REdge, uint16 BEdge)
{
    SetAPen(RPort, Color);
    RectFill(RPort, LEdge, TEdge, REdge, BEdge);
}
