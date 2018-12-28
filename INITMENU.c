#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITMENU()
{
    char    s[40];

    Screen2 = 0;
    strcpy(s, PathStr[7]);
    strcat(s, "Menu.pal");
    (void) SETCOLOR(MyScreen[1], s);

    strcpy(s+strlen(s)-3, "img");
    if (!DISPLAYIMAGE(s,0,0,640,512,6,MyScreen[1],4))
    {
        DisplayBeep(NULL);
    }
}
