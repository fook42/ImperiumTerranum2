#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CLICKRECT(struct Screen* XScreen, uint16 Left, uint16 Top, uint16 Right, uint16 Bottom, uint8 Color)
{
/*    SetAPen(&(XScreen->RastPort),Color);
    BOX(XScreen,Left+3,Top+3,Right-3,Bottom-3);
    PLAYSOUND(1,300);
    SetAPen(&(XScreen->RastPort),0);
    BOX(XScreen,Left+3,Top+3,Right-3,Bottom-3);
*/
    CLICKRECTWIN(&(XScreen->RastPort), Left, Top, Right, Bottom, Color);
}

void CLICKRECTWIN(struct RastPort* RPort, uint16 Left, uint16 Top, uint16 Right, uint16 Bottom, uint8 Color)
{
    SetAPen(RPort,Color);
    BOXWIN(RPort,Left+3,Top+3,Right-3,Bottom-3);
    PLAYSOUND(1,300);
    SetAPen(RPort,0);
    BOXWIN(RPort,Left+3,Top+3,Right-3,Bottom-3);
}

