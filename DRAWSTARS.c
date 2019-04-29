#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DRAWSTARS(uint8 Mode, uint8 DS_ActPlayer)
{
    int             i, j;
    r_ShipHeader*   MyShipPtr;
    uint32          MaxVal;
    uint16          pos;
    double          Factor;
    uint32          CVal[MAXCIVS];
    uint8           DS_ActPlayerFlag;
    char            s[20];

    DS_ActPlayerFlag = GETCIVFLAG(DS_ActPlayer);
    DS_ActPlayer--;     // todo.. due to shifted arrays
    Display = 0;
    if ((MODE_REDRAW   == Mode)
     || (MODE_STARGATE == Mode))
    {
        pos = 416;
        for (i = 0; i < 3; ++i)
        {
            DrawImage(MyRPort_PTR[0], &GadImg1, 518, pos);
            pos += 28;
        }
        WRITE(576,418,0, WRITE_Center              ,MyRPort_PTR[0],3,_PT_Regierung);
        WRITE(576,446,0, WRITE_Center              ,MyRPort_PTR[0],3,_PT_Hauptmenue);
        WRITE(576,474,8,(WRITE_Center|WRITE_Shadow),MyRPort_PTR[0],3,_PT_Rundenende);
        RECTWIN(MyRPort_PTR[0],0,0,0,511,511);
        SetAPen(MyRPort_PTR[0],6);
        for (i = 0; i<310; ++i)
        {
            WritePixel(MyRPort_PTR[0],(rand()%255)+(rand()%255),(rand()%255)+(rand()%255));
        }
    }
    if (Save.PlayMySelf)
    {
        MaxVal = 0;
        for (i = 0; i < (MAXCIVS-1); ++i)
        {
            CVal[i] = Save.Bevoelkerung[i] + Save.WarPower[i]*20 + Save.ImperatorState[i]*10;
            if (CVal[i]>MaxVal) { MaxVal = CVal[i]; }
        }
        Factor = MaxVal/20.0;
        pos = 0;
        for (i = 0; i < (MAXCIVS-1); ++i)
        {
            if ((i<7) || ((0 != Save.WorldFlag) && (WFLAG_FIELD != Save.WorldFlag)))
            {
                RECTWIN(MyRPort_PTR[0],0              , pos, 490, pos+62, 511-it_round(CVal[i]/Factor));
                RECTWIN(MyRPort_PTR[0],GETCIVFLAG(i+1), pos, 511-it_round(CVal[i]/Factor), pos+62, 511);
                (void) dez2out(CVal[i], 0, s);
                WRITE(pos+66,502,45,WRITE_Right,MyRPort_PTR[0],0,s);
            }
            pos += 64;
        }
    }
    SetAPen(MyRPort_PTR[0],12);
    if (Informed)
    {
        for (j = 0; j < MAXHOLES; ++j)
        {
            if (FLAG_KNOWN == MyWormHole[j].CivKnowledge[DS_ActPlayer])
            {
                Move(MyRPort_PTR[0],SystemX[MyWormHole[j].System[0]-1],SystemY[MyWormHole[j].System[0]-1]);
                Draw(MyRPort_PTR[0],SystemX[MyWormHole[j].System[1]-1],SystemY[MyWormHole[j].System[1]-1]);
            }
        }
    }

    for (i = 0; i< Save.Systems; ++i)
    {
        if ((NULL != SystemHeader[i].FirstShip.NextShip)
            && (MODE_STARGATE != Mode))
        {
            SetAPen(MyRPort_PTR[0],DS_ActPlayerFlag);
            MyShipPtr = SystemHeader[i].FirstShip.NextShip;

            while ((NULL != MyShipPtr) && (MyShipPtr->NextShip != MyShipPtr))
            {
                if ((0 > MyShipPtr->Moving) && (DS_ActPlayerFlag == MyShipPtr->Owner)
                    && (0 != Save.CivPlayer[DS_ActPlayer]) && (Informed)
                    && (1 <= MyShipPtr->Source) && (Save.Systems >= MyShipPtr->Source)
                    && (1 <= MyShipPtr->Target) && (Save.Systems >= MyShipPtr->Target))
                {
                    Move(MyRPort_PTR[0],SystemX[MyShipPtr->Source-1],SystemY[MyShipPtr->Source-1]);
                    Draw(MyRPort_PTR[0],SystemX[MyShipPtr->Target-1],SystemY[MyShipPtr->Target-1]);
                }
                MyShipPtr = MyShipPtr->NextShip;
            }
        }
    }
    for (i = 0; i < Save.Systems; ++i)
    {
        if ((!Save.PlayMySelf)
            && (((((SystemFlags[DS_ActPlayer][i] & FLAG_KNOWN) != FLAG_KNOWN) || (Save.CivPlayer[DS_ActPlayer] == 0)
                || (!Informed)) && (SystemHeader[i].vNS != FLAG_KNOWN)) || ((Mode == MODE_STARGATE)
                && (SystemHeader[i].FirstShip.SType != TARGET_STARGATE))))
        {
            RECTWIN(MyRPort_PTR[0],0,(SystemX[i]-1),(SystemY[i]-1),(SystemX[i]+1),(SystemY[i]+1));
            SetAPen(MyRPort_PTR[0],12);
            WritePixel(MyRPort_PTR[0],SystemX[i],SystemY[i]);
            WRITE(SystemX[i]+3,SystemY[i]+3,12,1,MyRPort_PTR[0],0,Save.SystemName.data[i]);
        } else if ((SystemFlags[0][i] & FLAG_CIV_MASK) != 0)
        {
            SetAPen(MyRPort_PTR[0],SystemFlags[0][i] & FLAG_CIV_MASK);
            if (TARGET_STARGATE == SystemHeader[i].FirstShip.SType)
            {
                DrawEllipse(MyRPort_PTR[0],SystemX[i],SystemY[i],4,4);
            } else {
                RectFill(MyRPort_PTR[0],(SystemX[i]-1),(SystemY[i]-1),(SystemX[i]+1),(SystemY[i]+1));
            }
            WRITE(SystemX[i]+3,SystemY[i]+3,SystemFlags[0][i] & FLAG_CIV_MASK,1,MyRPort_PTR[0],0,Save.SystemName.data[i]);
        } else {
            SetAPen(MyRPort_PTR[0],12);
            if (TARGET_STARGATE == SystemHeader[i].FirstShip.SType)
            {
                DrawEllipse(MyRPort_PTR[0],SystemX[i],SystemY[i],4,4);
            } else {
                RectFill(MyRPort_PTR[0],(SystemX[i]-1),(SystemY[i]-1),(SystemX[i]+1),(SystemY[i]+1));
            }
            WRITE(SystemX[i]+3,SystemY[i]+3,12,1,MyRPort_PTR[0],0,Save.SystemName.data[i]);
        }
    }
    PRINTGLOBALINFOS(DS_ActPlayer);
    if (!Save.PlayMySelf) { ScreenToFront(MyScreen[0]); }
}
