#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void MOVESHIP_EXPLODE(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    int     i;

    if (NULL != MyShipPtr)
    {
        if (0 != ActSys)
        {
            ActSys--; // to shift the arrays
            if (((FLAG_KNOWN == (SystemFlags[ActPlayer-1][ActSys] & FLAG_KNOWN))
                    && (0 != Save.CivPlayer[ActPlayer-1]))
                || ((0 != GETCIVVAR(SystemFlags[0][ActSys]))
                    && (0 != Save.CivPlayer[GETCIVVAR(SystemFlags[0][ActSys])-1])))
            {
                MOVESHIP_DRAWSCENE((ActSys+1), MyShipPtr);
                PLAYSOUND(1,1100);
                MOVESHIP_x = Area_CenterX+(MyShipPtr->PosX+OffsetX)*32;
                MOVESHIP_y = Area_CenterY+(MyShipPtr->PosY+OffsetY)*32;
                if ((0 != MyShipPtr->PosX) || (0 != MyShipPtr->PosY))
                {
                    MyShipPtr->PosX = MOVESHIP_FromX;
                    MyShipPtr->PosY = MOVESHIP_FromY;
                }
                if ((0 <= MOVESHIP_x) && (481 > MOVESHIP_x) &&
                    (0 <= MOVESHIP_y) && (481 > MOVESHIP_y) &&
                    ((0 != MyShipPtr->PosX) || (0 != MyShipPtr->PosY)))
                {
                    RECT(MyScreen[0],0,MOVESHIP_x,MOVESHIP_y,MOVESHIP_x+31,MOVESHIP_y+31);
                    for(i = 0; i < 16; ++i)
                    {
                        BltBitMapRastPort((struct BitMap*) &ImgBitMap4,i*32,0,&(MyScreen[0]->RastPort),MOVESHIP_x,MOVESHIP_y,31,31,192);
                        Delay(5);
                    }
                    RECT(MyScreen[0],0,MOVESHIP_x,MOVESHIP_y,MOVESHIP_x+31,MOVESHIP_y+31);
                    Delay(10);
                    if (Audio_enable)
                    {
                        custom.dmacon = BITCLR | DMAF_AUD0 | DMAF_AUD1;
                    }
                }
            }
        }
        MyShipPtr->Owner  = 0;
        MyShipPtr->Moving = 0;
    }
    REFRESHDISPLAY();
    Delay(10);
}
