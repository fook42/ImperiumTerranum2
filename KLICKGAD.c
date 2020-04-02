#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void KLICKGAD(const uint16 x, const uint16 y)
{
    /* saves 1 argument to be specified in the calls of KLICKGAD */
    ClipBlit(MyRPort_PTR[0],x,y,&(XScreen->RastPort),0,0,116,20,192);
    DrawImage(MyRPort_PTR[0],&GadImg2,x,y);
    PLAYSOUND(1,300);
    ClipBlit(&(XScreen->RastPort),0,0,MyRPort_PTR[0],x,y,116,20,192);
}

void KLICKWINGAD(struct RastPort* RPort, const uint16 x, const uint16 y)
{
    ClipBlit(RPort,x,y,&(XScreen->RastPort),0,0,116,20,192);
    DrawImage(RPort,&GadImg2,x,y);
    PLAYSOUND(1,300);
    ClipBlit(&(XScreen->RastPort),0,0,RPort,x,y,116,20,192);
}

