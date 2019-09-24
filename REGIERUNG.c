#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void REGIERUNG()
{
    uint8   i;
    uint16  ypos;
    bool    leave_dialog = false;

    struct Window* REG_Window;
    struct RastPort* RPort_PTR;

    REG_Window=MAKEWINDOW(194,119,123,158,MyScreen[0]);
    if (NULL == REG_Window)
    {
        return;
    }
    RPort_PTR = REG_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,122,157,12,6,1);

    ypos = 3;
    for (i = 0; i<7; ++i)
    {
        DrawImage(RPort_PTR,&GadImg1,4,ypos);
        ypos += 22;
    }
    WRITE( 61,  5,0,WRITE_Center,RPort_PTR,3,PText[746]);
    WRITE( 61, 27,0,WRITE_Center,RPort_PTR,3,PText[747]);
    WRITE( 61, 49,0,WRITE_Center,RPort_PTR,3,PText[748]);
    WRITE( 61, 71,0,WRITE_Center,RPort_PTR,3,PText[749]);
    WRITE( 61, 93,0,WRITE_Center,RPort_PTR,3,PText[750]);
    WRITE( 61,115,0,WRITE_Center,RPort_PTR,3,PText[751]);
    WRITE( 61,137,8,WRITE_Center,RPort_PTR,3,PText[752]);

    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            if ((1 < REG_Window->MouseX) && (119 > REG_Window->MouseX))
            {
                if ((2 < REG_Window->MouseY) && (24 > REG_Window->MouseY))
                {
                    KLICKWINGAD(RPort_PTR, 4,  3);
                    CloseWindow(REG_Window);
                    MILITAER();
                    leave_dialog = true;
                } else if ((24 < REG_Window->MouseY) && (46 > REG_Window->MouseY))
                {
                    KLICKWINGAD(RPort_PTR, 4, 25);
                    CloseWindow(REG_Window);
                    FINANZEN();
                    leave_dialog = true;
                } else if ((46 < REG_Window->MouseY) && (68 > REG_Window->MouseY))
                {
                    KLICKWINGAD(RPort_PTR, 4, 47);
                    CloseWindow(REG_Window);
                    DOSSIER(false);
                    leave_dialog = true;
                } else if ((68 < REG_Window->MouseY) && (90 > REG_Window->MouseY))
                {
                    KLICKWINGAD(RPort_PTR, 4, 69);
                    CloseWindow(REG_Window);
                    PROJEKTE();
                    leave_dialog = true;
                } else if ((90 < REG_Window->MouseY) && (112 > REG_Window->MouseY))
                {
                    KLICKWINGAD(RPort_PTR, 4, 91);
                    CloseWindow(REG_Window);
                    STATISTIK();
                    leave_dialog = true;
                } else if ((112 < REG_Window->MouseY) && (134 > REG_Window->MouseY))
                {
                    KLICKWINGAD(RPort_PTR, 4,113);
                    CloseWindow(REG_Window);
                    FORSCHUNG();
                    leave_dialog = true;
                } else if ((134 < REG_Window->MouseY) && (156 > REG_Window->MouseY))
                {
                    KLICKWINGAD(RPort_PTR, 4,135);
                    CloseWindow(REG_Window);
                    PUMPUPTHELEVEL();
                    leave_dialog = true;
                }
            }
        }
    }
    while ((!leave_dialog) && RMB_NOTPRESSED);
    if (!leave_dialog)
    {
        CloseWindow(REG_Window);
    }
    //    DRAWSTARS(MODE_REDRAW, ActPlayer);
}
