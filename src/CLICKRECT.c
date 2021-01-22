#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CLICKRECT(struct RastPort* RPort, int Left, int Top, int Right, int Bottom, const int Color)
{
    Left+=3;
    Top+=3;
    Right-=3;
    Bottom-=3;
    SetAPen(RPort,Color);
    BOX(RPort,Left,Top,Right,Bottom);
    PLAYSOUND(1,300);
    SetAPen(RPort,0);
    BOX(RPort,Left,Top,Right,Bottom);
}
