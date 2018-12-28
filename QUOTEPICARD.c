#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void QUOTEPICARD()
{
    const char* Txt_Picard[] = {"Ich frage mich, ob es Kaiser Honorius klar",
                                "war, als er sah, dass die Westgoten den siebten",
                                "Hügel Roms überwanden, dass das Römische Reich",
                                "fallen würde ... Das hier ist nur eine weitere",
                                "Episode der Geschichte. Wird hier unsere",
                                "Zivilisation enden?",
                                "Jean Luc Picard",
                                "Mission \"Wolf 359\"" };
    struct Window* QPI_Window;
    struct RastPort* RPort_PTR;
    QPI_Window=MAKEWINDOW(20,70,471,221,MyScreen[0]);
    if (NULL == QPI_Window)
    {
        return;
    }
    RPort_PTR = QPI_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,470,220,12,6,1);

    WRITEWIN(236, 20,12,WRITE_Center,RPort_PTR,4,Txt_Picard[0]);
    WRITEWIN(236, 42,12,WRITE_Center,RPort_PTR,4,Txt_Picard[1]);
    WRITEWIN(236, 64,12,WRITE_Center,RPort_PTR,4,Txt_Picard[2]);
    WRITEWIN(236, 86,12,WRITE_Center,RPort_PTR,4,Txt_Picard[3]);
    WRITEWIN(236,108,12,WRITE_Center,RPort_PTR,4,Txt_Picard[4]);
    WRITEWIN(236,130,12,WRITE_Center,RPort_PTR,4,Txt_Picard[5]);
    WRITEWIN(460,158,12,WRITE_Right ,RPort_PTR,4,Txt_Picard[6]);
    WRITEWIN(460,180,12,WRITE_Right ,RPort_PTR,4,Txt_Picard[7]);
    WAITLOOP(false);

    CloseWindow(QPI_Window);
}
