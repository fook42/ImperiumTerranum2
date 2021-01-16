#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool OPENMAINSCREENS()
{

    struct NewScreen  NeuScreen = {0,0,640,512,0,0,0,HIRES+LACE,CUSTOMSCREEN|SCREENQUIET, NULL,NULL,NULL,NULL};
    struct NewWindow  NeuWindow = {0,0,640,512,0,0,RAWKEY,SMART_REFRESH+BACKDROP+BORDERLESS, NULL,NULL,NULL,NULL,NULL,640,512,640,512,CUSTOMSCREEN};
//  struct NewWindow  NeuWindow = {0,0,640,512,0,0,RAWKEY,SIMPLE_REFRESH+BACKDROP+BORDERLESS, NULL,NULL,NULL,NULL,NULL,640,512,640,512,CUSTOMSCREEN};
    int     i;

    if ((HighRes_Width < 640) || (HighRes_Height < 512))
    {
        NeuScreen.Type |= AUTOSCROLL;
    }
    INITSTDTAGS();
    ColSpec = (ColSpecArr) {{ 0,0,0,0,-1 }};
    for (i = 0; i < 2; i++)
    {
        MyScreen[i] = NULL;
        NeuScreen.Depth = 7+i;
        MyScreen[i] = OpenScreenTagList( &NeuScreen, Tags );
        if (NULL == MyScreen[i])
        {
            return false;
        }
        MyScreen[i]->BarHeight = 0;
        MyScreen[i]->WBorTop = 0;
        MyScreen[i]->MenuVBorder = 0;
        MyScreen[i]->MenuHBorder = 0;

        MyWindow[i] = NULL;
        NeuWindow.Screen = MyScreen[i];
        MyWindow[i] = OpenWindow( &NeuWindow );
        if (NULL == MyWindow[i])
        {
            return false;
        }
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
    }
    return true;
}
