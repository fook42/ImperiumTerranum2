#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void RECT_RP0_C0(const int LEdge, const int TEdge, const int REdge, const int BEdge)
{
    SetAPen(MyRPort_PTR[0], 0);
    RectFill(MyRPort_PTR[0], LEdge, TEdge, REdge, BEdge);
}

void RECT_RP1(const int Color, const int LEdge, const int TEdge, const int REdge, const int BEdge)
{
    SetAPen(MyRPort_PTR[1], Color);
    RectFill(MyRPort_PTR[1], LEdge, TEdge, REdge, BEdge);
}

void RECTWIN(struct RastPort* RPort, const int Color, const int LEdge, const int TEdge, const int REdge, const int BEdge)
{
    SetAPen(RPort, Color);
    RectFill(RPort, LEdge, TEdge, REdge, BEdge);
}
