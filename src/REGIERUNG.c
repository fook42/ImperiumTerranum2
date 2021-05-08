#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REGIERUNG()
{
    int     i, ypos, PTextIndex;

    struct Window* REG_Window;
    struct RastPort* RPort_PTR;
    void (*functions[])() = { MILITAER,
                              FINANZEN,
                              DOSSIER,
                              PROJEKTE,
                              STATISTIK,
                              FORSCHUNG,
                              PUMPUPTHELEVEL };

    REG_Window=MAKEWINDOW(194,119,123,158,MyScreen[0]);
    if (NULL == REG_Window)
    {
        return;
    }
    RPort_PTR = REG_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,122,157,12,6,1);

    ypos = 3;
    PTextIndex=746;
    for (i = 0; i<7; ++i)
    {
        DrawImage(RPort_PTR,&GadImg1,4,ypos);
        WRITE( 61,2+ypos,((i<6)?0:8),WRITE_Center,RPort_PTR,3,PText[PTextIndex]);
        ++PTextIndex;
        ypos += 22;
    }

    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((1 < REG_Window->MouseX) && (119 > REG_Window->MouseX))
            {
                ypos = 2;
                for (i = 0; i<7; ++i)
                {
                    if ((ypos < REG_Window->MouseY) && ((ypos+22) > REG_Window->MouseY))
                    {
                        KLICKWINGAD(RPort_PTR, 4, ypos+1);
                        CloseWindow(REG_Window);
                        REG_Window = NULL;
                        (*functions[i])();

                        break;
                    }
                    ypos += 22;
                }
            }
        }
    }
    while ((NULL != REG_Window) && RMB_NOTPRESSED);
    if (NULL != REG_Window)
    {
        CloseWindow(REG_Window);
    }
    //    DRAWSTARS(MODE_REDRAW, ActPlayer);
}
