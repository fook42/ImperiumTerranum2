#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void RECT_RP0_C0(const uint16 LEdge, const uint16 TEdge, const uint16 REdge, const uint16 BEdge)
{
    SetAPen(MyRPort_PTR[0], 0);
    RectFill(MyRPort_PTR[0], LEdge, TEdge, REdge, BEdge);
}

void RECT_RP1(const uint16 Color, const uint16 LEdge, const uint16 TEdge, const uint16 REdge, const uint16 BEdge)
{
    SetAPen(MyRPort_PTR[1], Color);
    RectFill(MyRPort_PTR[1], LEdge, TEdge, REdge, BEdge);
}

void RECTWIN(struct RastPort* RPort, const uint16 Color, const uint16 LEdge, const uint16 TEdge, const uint16 REdge, const uint16 BEdge)
{
    SetAPen(RPort, Color);
    RectFill(RPort, LEdge, TEdge, REdge, BEdge);
}
