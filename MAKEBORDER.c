#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void MAKEWINBORDER(struct RastPort* RPort, uint16 LEdge, uint16 TEdge, uint16 REdge, uint16 BEdge, uint8 Col1, uint8 Col2, uint8 Darken)
{
    if (1 != Darken)
    {
        SetAPen(RPort, Darken);
        RectFill(RPort, LEdge+2, TEdge+2, REdge-2, BEdge-2);
    }

    SetAPen(RPort, Col2);
    RectFill(RPort, LEdge  ,BEdge-1,REdge,  BEdge);

    SetAPen(RPort, Col1);
    RectFill(RPort, LEdge,  TEdge,  LEdge+1,BEdge-1);
    RectFill(RPort, LEdge+2,TEdge,  REdge,  TEdge+1);

    SetAPen(RPort, Col2);
    RectFill(RPort, REdge-1,TEdge+1,REdge,  BEdge-1);
}

struct Window* MAKEWINDOW(uint16 LEdge, uint16 TEdge, uint16 SizeX, uint16 SizeY, struct Screen* onScreen)
{
    struct NewWindow MK_NeuWindow = {LEdge,TEdge,SizeX,SizeY,0,0,RAWKEY,SMART_REFRESH+BORDERLESS, NULL,NULL,NULL,onScreen,NULL,SizeX,SizeY,SizeX,SizeY,CUSTOMSCREEN};

    return( OpenWindow( &MK_NeuWindow ) );
}
