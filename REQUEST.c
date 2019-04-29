#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REQUEST(char* s1, char* s2, uint8 c1, uint8 c2)
{
    struct Window* REQ_Window;
    REQ_Window = MAKEWINDOW(35,110,441,81,MyScreen[0]);
    if (NULL != REQ_Window)
    {
        MAKEWINBORDER(REQ_Window->RPort,0,0,440,80,12,6,1);

        WRITE(221,20,c1,WRITE_Center,REQ_Window->RPort,4,s1);
        WRITE(221,45,c2,WRITE_Center,REQ_Window->RPort,4,s2);
        if (Save.PlayMySelf) { delay(PAUSE); }
        WAITLOOP(Save.PlayMySelf);
        CloseWindow(REQ_Window);
    }
}
