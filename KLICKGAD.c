#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void KLICKGAD(const uint16 x, const uint16 y)
{
/*    ClipBlit(&(MyScreen[0]->RastPort),x,y,&(XScreen->RastPort),0,0,116,20,192);
    DrawImage(&(MyScreen[0]->RastPort),&GadImg2,x,y);
    PLAYSOUND(1,300);
    ClipBlit(&(XScreen->RastPort),0,0,&(MyScreen[0]->RastPort),x,y,116,20,192);
*/
    KLICKWINGAD(&(MyScreen[0]->RastPort), x, y);
}


void KLICKWINGAD(const struct RastPort* RPort, const uint16 x, const uint16 y)
{
    ClipBlit(RPort,x,y,&(XScreen->RastPort),0,0,116,20,192);
    DrawImage(RPort,&GadImg2,x,y);
    PLAYSOUND(1,300);
    ClipBlit(&(XScreen->RastPort),0,0,RPort,x,y,116,20,192);
}

