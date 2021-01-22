#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REQUEST(const char* text1, const char* text2, const int color1, const int color2)
{
    struct Window* REQ_Window;

    ScreenToFront(MyScreen[0]);
    REQ_Window = MAKEWINDOW(35,110,441,81,MyScreen[0]);
    if (NULL != REQ_Window)
    {
        MAKEWINBORDER(REQ_Window->RPort,0,0,440,80,12,6,1);

        WRITE(221, 20, color1, WRITE_Center, REQ_Window->RPort, 3, text1);
        WRITE(221, 45, color2, WRITE_Center, REQ_Window->RPort, 3, text2);
        if (Save.PlayMySelf) { Delay(PAUSE); }
        WAITLOOP(Save.PlayMySelf);
        CloseWindow(REQ_Window);
    }
}
