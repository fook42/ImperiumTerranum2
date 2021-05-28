#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REGIERUNG()
{
    int     i, ypos;
    int     button_Textcol, button_Textstyle;
    const char* Button_Texts[] = {PText[746], PText[747], PText[748],
                                  PText[749], PText[750], PText[751], PText[752]};
    struct Window* REG_Window;
    struct RastPort* RPort_PTR;
    void (*functions[])() = { MILITAER,
                              FINANZEN,
                              DOSSIER,
                              PROJEKTE,
                              STATISTIK,
                              FORSCHUNG,
                              PUMPUPTHELEVEL };

    REG_Window=MAKEWINDOWBORDER(194,119,123,158,MyScreen[0]);
    if (NULL == REG_Window)
    {
        return;
    }
    RPort_PTR = REG_Window->RPort;


    ypos = 3;
    button_Textcol = 0;
    button_Textstyle = WRITE_Center;
    for(i = 0; i < 7; ++i)
    {
        DrawImage(RPort_PTR,&GadImg1, 4, ypos);
        if (6 == i) { button_Textcol = 8; button_Textstyle |= WRITE_Shadow; }
        WRITE(61, ypos+2, button_Textcol, button_Textstyle, RPort_PTR, 3, Button_Texts[i]);
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
