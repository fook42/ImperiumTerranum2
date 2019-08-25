#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CLOSEMYSCREENS()
{
    uint8   i;
    ScreenToFront(XScreen);
    SWITCHDISPLAY();
    for (i = 0; i < 2; ++i)
    {
        if (NULL != MyScreen[i])
        {
            while (NULL != (MyScreen[i]->FirstWindow))
            {
                CloseWindow(MyScreen[i]->FirstWindow);
            }
            CloseScreen(MyScreen[i]);
            MyScreen[i] = NULL;
            MyRPort_PTR[i] = NULL;
            MyVPort_PTR[i] = NULL;
        }
    }
}
