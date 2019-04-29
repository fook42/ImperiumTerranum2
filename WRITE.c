#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void WRITE(const uint16 IPosX, const uint16 IPosY, const uint8 Color, const uint8 DMode, struct RastPort* RPort, const uint8 TA, const char* WText)
{
    struct IntuiText MyText = {Color, 0, DMode & 0x0F, IPosX, IPosY, &CustomTA[TA], (STRPTR) WText, NULL};

    if (NULL != WText)
    {
        if (0 != WText[0])
        {
            if (DMode & WRITE_Center) { MyText.LeftEdge = IPosX-(IntuiTextLength(&MyText) >> 1); }
            if (DMode & WRITE_Right ) { MyText.LeftEdge = IPosX-IntuiTextLength(&MyText)-(CustomTA[TA].ta_YSize >> 1); }
            if (DMode & WRITE_Shadow)   // shadow-effect - write a black Text at (+1,+1)
            {
                MyText.FrontPen = 0;
                PrintIText(RPort, &MyText, 1, 1);
                MyText.FrontPen = Color;
            }
            PrintIText(RPort, &MyText, 0, 0);
        }
    }
}
