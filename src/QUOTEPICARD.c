#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void QUOTEPICARD()
{
    int     i, ypos;
    const char* Txt_Picard[] = {"Ich frage mich, ob es Kaiser Honorius klar",
                                "war, als er sah, da� die Westgoten den siebten",
                                "H�gel Roms �berwanden, da� das R�mische Reich",
                                "fallen w�rde ... Das hier ist nur eine weitere",
                                "Episode der Geschichte. Wird hier unsere",
                                "Zivilisation enden?"};
    const char* Txt_Picard2[] = {"Jean Luc Picard",
                                "Mission \"Wolf 359\"" };

    struct Window* QPI_Window = NULL;
    struct RastPort* RPort_PTR;
    QPI_Window=MAKECENTERWINDOW(471,221,MyScreen[0]);
    if (NULL != QPI_Window)
    {
        RPort_PTR = QPI_Window->RPort;

        ypos = 20;
        for (i = 0; i < (sizeof(Txt_Picard)/sizeof(Txt_Picard[0])); ++i)
        {
            WRITE(236, ypos, 12, WRITE_Center, RPort_PTR, 3, Txt_Picard[i]);
            ypos += 22;
        }

        WRITE(460,158,12,WRITE_Right,RPort_PTR,3, Txt_Picard2[0]);
        WRITE(460,180,12,WRITE_Right,RPort_PTR,3, Txt_Picard2[1]);
        WAITLOOP(false);

        CloseWindow(QPI_Window);
    }
}
