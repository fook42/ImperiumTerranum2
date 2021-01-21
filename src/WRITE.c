#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void WRITE(const int IPosX, const int IPosY, const int Color, const int DMode, struct RastPort* RPort, const int TA, const char* WText)
{
    struct IntuiText MyText = {(UBYTE) Color, 0, (UBYTE) (DMode & 0x0F), (WORD) IPosX, (WORD) IPosY, &CustomTA[TA], (STRPTR) WText, NULL};

    if (NULL != WText)
    {
        if (0 != WText[0])
        {
            if (DMode & WRITE_Center) { MyText.LeftEdge = (WORD) (IPosX-(IntuiTextLength(&MyText) >> 1)); }
            if (DMode & WRITE_Right ) { MyText.LeftEdge = (WORD) (IPosX-IntuiTextLength(&MyText)-(CustomTA[TA].ta_YSize >> 1)); }
            if (DMode & WRITE_Shadow)   // shadow-effect - write a black Text at (+1,+1)
            {
                MyText.FrontPen = 0;
                PrintIText(RPort, &MyText, 1, 1);
                MyText.FrontPen = (UBYTE) Color;
            }
            PrintIText(RPort, &MyText, 0, 0);
        }
    }
}


void WRITE_RP0(const int IPosX, const int IPosY, const int Color, const int DMode, const int TA, const char* WText)
{
    struct IntuiText MyText = {(UBYTE) Color, 0, (UBYTE) (DMode & 0x0F), (WORD) IPosX, (WORD) IPosY, &CustomTA[TA], (STRPTR) WText, NULL};

    if (NULL != WText)
    {
        if (0 != WText[0])
        {
            if (DMode & WRITE_Center) { MyText.LeftEdge = (WORD) (IPosX-(IntuiTextLength(&MyText) >> 1)); }
            if (DMode & WRITE_Right ) { MyText.LeftEdge = (WORD) (IPosX-IntuiTextLength(&MyText)-(CustomTA[TA].ta_YSize >> 1)); }
            if (DMode & WRITE_Shadow)   // shadow-effect - write a black Text at (+1,+1)
            {
                MyText.FrontPen = 0;
                PrintIText(MyRPort_PTR[0], &MyText, 1, 1);
                MyText.FrontPen = (UBYTE) Color;
            }
            PrintIText(MyRPort_PTR[0], &MyText, 0, 0);
        }
    }
}

void WRITE_RP1(const int IPosX, const int IPosY, const int Color, const int DMode, const int TA, const char* WText)
{
    struct IntuiText MyText = {(UBYTE) Color, 0, (UBYTE) (DMode & 0x0F), (WORD) IPosX, (WORD) IPosY, &CustomTA[TA], (STRPTR) WText, NULL};

    if (NULL != WText)
    {
        if (0 != WText[0])
        {
            if (DMode & WRITE_Center) { MyText.LeftEdge = (WORD) (IPosX-(IntuiTextLength(&MyText) >> 1)); }
            if (DMode & WRITE_Right ) { MyText.LeftEdge = (WORD) (IPosX-IntuiTextLength(&MyText)-(CustomTA[TA].ta_YSize >> 1)); }
            if (DMode & WRITE_Shadow)   // shadow-effect - write a black Text at (+1,+1)
            {
                MyText.FrontPen = 0;
                PrintIText(MyRPort_PTR[1], &MyText, 1, 1);
                MyText.FrontPen = (UBYTE) Color;
            }
            PrintIText(MyRPort_PTR[1], &MyText, 0, 0);
        }
    }
}
