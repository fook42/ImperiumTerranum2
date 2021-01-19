#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

#define STEPS 1100

sint32  WORMHOLE_ShipShield;

ITBitMap ImgBitMapW4;
uint16* WHSoundMemA[2]; // 0..1
uint32  WHSoundMemL[2]; // 0..1
bool    Error;

void WORMHOLE_LOADSOUND(char* FName, uint8 SID)
{
    uint32  size;
    BPTR    FHandle;

    FHandle = OPENSMOOTH(FName,MODE_OLDFILE);
    if (0 != FHandle)
    {
        (void) Seek(FHandle, 0, OFFSET_END);
        size = Seek(FHandle, 0, OFFSET_BEGINNING);
        WHSoundMemL[SID] = size >> 1;
        if (0 == SID)
        {
            WHSoundMemL[SID] += STEPS*3;
        }
        WHSoundMemA[SID] = (uint16*) AllocMem(WHSoundMemL[SID]<<1, MEMF_CHIP | MEMF_CLEAR);
        if (NULL != WHSoundMemA[SID])
        {
            (void) Read(FHandle, (APTR) WHSoundMemA[SID], WHSoundMemL[SID]<<1);
        }
        Close(FHandle);
    }
}

void WORMHOLE_INITSOUNDS()
{
    char s[40];
    char* _s;
    
    _s=my_strcpy(s,PathStr[6]);
    (void)my_strcpy(_s, "Sensor.RAW");
    WORMHOLE_LOADSOUND(s, 0);       /*SFX/Sensor.RAW*/
    (void)my_strcpy(_s, "FightSoundDS.RAW");
    WORMHOLE_LOADSOUND(s, 1);       /*SFX/FightsoundDS.RAW*/
}

bool WORMHOLE_INITIMAGES()
{
    char    s[40];
    char*   _s;

    _s=my_strcpy(s, PathStr[4]);
    _s=my_strcpy(_s, "Wormhole.img");
    if (!RAWLOADIMAGE(s,0,0,640,512,4, &ImgBitMapW4))
        { return false; }

    (void)my_strcpy(_s-3, "pal");
    (void) SETCOLOR(MyScreen[0],s);
    (void) SETCOLOR(MyScreen[1],s);
    return true;
}

void TRAVEL()
{
    sint16  X[6],Y[6],S[6];     // 1..5
    sint16  XOff, YOff;
    volatile uint16*    Joy;
    sint16   DirCnt, DVert, DHoriz, JVert, JHoriz;
    uint16  i, Clear;
    uint8   j, random_wert;
    uint8   AScr, RectCol;
    time_t  t;

    RECT(MyScreen[0],7,311,0,319,255);
    RECT(MyScreen[1],7,311,0,319,255);

    if (Audio_enable)
    {
        SPAddrA = WHSoundMemA[0];                SPLengthA = WHSoundMemL[0];     SPVolA = 45; SPFreqA = 300;
        SPAddrC = WHSoundMemA[1];                SPLengthC = WHSoundMemL[1] / 2; SPVolC = 64; SPFreqC = 400;
        SPAddrD = WHSoundMemA[1]+WHSoundMemL[1]; SPLengthD = WHSoundMemL[1] / 2; SPVolD = 64; SPFreqD = 400;
        SPAddrB = 0; SPLengthB = 1;
        custom.dmacon = BITSET | DMAF_AUDIO;
    }
    srand((unsigned) time(&t));

    X[1] = 155;
    Y[1] = 128;
    S[1] = 10;
    XOff = 0;
    YOff = 0;
    Clear = 0;
    for(j = 1; j < 5; ++j)
    {
        X[j+1] = X[j];
        Y[j+1] = Y[j];
        S[j+1] = it_round(S[j]*1.6);
    }
    AScr = 0;
    DirCnt = 1;
    DVert = 0;
    DHoriz = 0;
    Joy = (uint16*) 0xDFF00C;

    for(i = 1; i <= STEPS; ++i)
    {
        if ((STEPS-55) > i)
        {
            if (8 < DirCnt)
            {
                DirCnt = 0;
            } else if (0 == DirCnt)
            {
                DHoriz = ((rand()%3)-1)*3;
                if (0 == DHoriz)
                {
                    DVert = ((rand()%3)-1)*3;
                }
                DirCnt = 1;
            } else {
                ++DirCnt;
            }
        } else {
            DHoriz = 0;
            DVert = 0;
        }

        JVert = 0;
        JHoriz = 0;
        // 0x1  = unten
        // 0x2  = links unten
        // 0x3  = links
        // 0x100= oben
        // 0x200= oben rechts
        // 0x300= rechts

/*
        if (((*Joy & 0x3) == 0x3) || ((*Joy & 0x3) == 0x2))
        {
            JHoriz = -8;
        } else if (((*Joy & 0x300) == 0x300) || ((*Joy & 0x300) == 0x200))
        {
            JHoriz = 8;
        }
*/

        if ((*Joy & 0x2) == 0x2)
        {
            JHoriz = -8;        // right
        } else if ((*Joy & 0x200) == 0x200)
        {
            JHoriz = 8;         // left
        }

        if (((*Joy & 0x3) == 0x1) || ((*Joy & 0x3) == 0x2))
        {
            JVert = 8;
        } else if (((*Joy & 0x300) == 0x100) || ((*Joy & 0x300) == 0x200))
        {
            JVert = -8;
        }
        if (0 != JHoriz)
        {
            for(j = 1; j < 6; ++j)
            {
                X[j] += JHoriz;
            }
            XOff += JHoriz;
        }
        if (0 != JVert)
        {
            for(j = 1; j < 6; ++j)
            {
                Y[j] += JVert;
            }
            YOff += JVert;
        }

        XOff += DHoriz;
        YOff += DVert;
        if (XOff >  145) { XOff =  145; }
        if (XOff < -145) { XOff = -145; }
        if (YOff >  122) { YOff =  122; }
        if (YOff < -122) { YOff = -122; }

        for(j = 1; j < 6; ++j)
        {
            if (154 > X[j])
            {
                X[j] = 155-it_round((double) (155-X[j])*0.98);
            } else if (156 < X[j])
            {
                X[j] = it_round((double) X[j]+0.98);
            }

            if (127 > Y[j])
            {
                Y[j] = 128-it_round((double) (128-Y[j])*0.98);
            } else if (129 < Y[j])
            {
                Y[j] = it_round((double) Y[j]+0.98);
            }
            S[j] = it_round(S[j]*1.06);
        }

        if (240 < S[5])
        {
            for (j = 4; j > 0; --j)
            {
                X[j+1] = X[j];
                Y[j+1] = Y[j];
                S[j+1] = S[j];
            }
            X[1] = 155+XOff;
            Y[1] = 128+YOff;
            S[1] = 10;
        }

        BltBitMapRastPort((struct BitMap*) &ImgBitMapW4,170-XOff,128-YOff, MyRPort_PTR[AScr],0,0,310,256,192);
        SetAPen(MyRPort_PTR[AScr],6);
        for(j = 1; j < 6; ++j)
        {
            if (((X[j]-S[j])>=1) && ((X[j]-S[j])<=309) && ((Y[j]-S[j])>=1) && ((Y[j]-S[j])<=255)
              &&((X[j]+S[j])>=1) && ((X[j]+S[j])<=309) && ((Y[j]+S[j])>=1) && ((Y[j]+S[j])<=255))
            {
                BOX(MyRPort_PTR[AScr], X[j]-S[j], Y[j]-S[j], X[j]+S[j], Y[j]+S[j]);
            }
        }
        if ((X[5]<120) || (X[5]>200) || (Y[5]<90) || (Y[5]>165))
        {
            if ((X[5]>=105) && (X[5]<=215) && (Y[5]>=75) && (Y[5]<=180))
            {
                if (Audio_enable)
                {
                    SPFreqB    = 120;
                }
                WORMHOLE_ShipShield -= 2;
            } else {
                if (Audio_enable)
                {
                    SPFreqB    = 80;
                }
                WORMHOLE_ShipShield -= 4;
            }
            if (Audio_enable)
            {
                SPAddrB = WHSoundMemA[0]; SPLengthB = 1500; SPVolB = 52;
            }

            RectCol = 15;
            if ((WORMHOLE_ShipShield >= 13) && (WORMHOLE_ShipShield <= 762))
            {
                RECTWIN(MyRPort_PTR[AScr],0, 312, 1, 318, 258-it_round(WORMHOLE_ShipShield/3.0));
            }
            if (WORMHOLE_ShipShield < 0)
            {
                if (Audio_enable)
                {
                    custom.dmacon = BITCLR | DMAF_AUDIO;
                }
                random_wert = (rand()%5)+5;
                for(j = 0; j < random_wert; ++j)
                {
                    PLAYSOUND(2,1100);
                    SetRGB4(MyVPort_PTR[1-AScr],0,8,8,15);
                    Delay(3);
                    SetRGB4(MyVPort_PTR[1-AScr],0,0,0,3);
                    Delay(3);
                }
                return;
            }
        } else {
            RectCol = 9;
        }

        SetAPen(MyRPort_PTR[AScr], 5);
        Move(MyRPort_PTR[AScr], XOff+150, YOff+128);
        for(j = 1; j < 6; ++j)
        {
            if (((X[j]-S[j])>=1) && ((X[j]-S[j])<=309) && ((Y[j]-S[j])>=1) && ((Y[j]-S[j])<=255))
            {
                Draw(MyRPort_PTR[AScr], X[j]-S[j], Y[j]-S[j]);
            }
        }

        Move(MyRPort_PTR[AScr], XOff+150, YOff+128);
        for(j = 1; j < 6; ++j)
        {
            if (((X[j]+S[j])>=1) && ((X[j]+S[j])<=309) && ((Y[j]-S[j])>=1) && ((Y[j]-S[j])<=255))
            {
                Draw(MyRPort_PTR[AScr], X[j]+S[j], Y[j]-S[j]);
            }
        }

        Move(MyRPort_PTR[AScr], XOff+150, YOff+128);
        for(j = 1; j < 6; ++j)
        {
            if (((X[j]+S[j])>=1) && ((X[j]+S[j])<=309) && ((Y[j]+S[j])>=1) && ((Y[j]+S[j])<=255))
            {
                Draw(MyRPort_PTR[AScr], X[j]+S[j], Y[j]+S[j]);
            }
        }

        Move(MyRPort_PTR[AScr], XOff+150, YOff+128);
        for(j = 1; j < 6; ++j)
        {
            if (((X[j]-S[j])>=1) && ((X[j]-S[j])<=309) && ((Y[j]+S[j])>=1) && ((Y[j]+S[j])<=255))
            {
                Draw(MyRPort_PTR[AScr], X[j]-S[j], Y[j]+S[j]);
            }
        }

        SetAPen(MyRPort_PTR[AScr], RectCol);
        BOX(MyRPort_PTR[AScr],60,35,250,220);

        if (Audio_enable)
        {
            SPAddrB   = ZeroSound; SPLengthB = 1;
        }
        if (i>(STEPS-30))
        {
            if (Audio_enable)
            {
                SPAddrA   = ZeroSound; SPLengthA = 1;
            }
            Clear += 4;
            RECTWIN(MyRPort_PTR[AScr],0,160-Clear,128-Clear,160+Clear,128+Clear);
        } else {
            if (Audio_enable)
            {
                SPLengthA = WHSoundMemL[0]-i-i-i;
            }
        }
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
    }
}

bool SMALLWORMFLIGHT(r_ShipHeader* MyShipPtr)
{
    WORMHOLE_ShipShield = it_round((double) WORMHOLE_ShipShield*((rand()%45)+15)/100.0);
    if (WORMHOLE_ShipShield < 1)
    {
        return false;
    } else {
        MyShipPtr->Shield = it_round((double) WORMHOLE_ShipShield / 760.0 * ShipData(MyShipPtr->SType).MaxShield);
        return true;
    }
}

bool WORMEXIT(bool _WORMEXIT, r_ShipHeader* MyShipPtr, uint8 ActSys)
{
    uint8   i;

    DoClock = false;
    CLOSEMYSCREENS();
    for(i = 0; i < 2; ++i)
    {
        if (NULL != WHSoundMemA[i])
        {
            FreeMem((APTR) WHSoundMemA[i], WHSoundMemL[i]<<1);
            WHSoundMemA[i] = NULL;
        }
    }
    if (NULL != ImgBitMapW4.MemA)
    {
        FreeMem(ImgBitMapW4.MemA, ImgBitMapW4.MemL);
    }
    ImgBitMapW4.MemA = NULL;
    custom.dmacon = BITCLR | DMAF_AUDIO;
    (void) OPENMAINSCREENS();
    (void) INITDESK(0);
    if (Error)
    {
        _WORMEXIT = SMALLWORMFLIGHT(MyShipPtr);
    }
    Screen2 = 0;
    Display = 0;
    DRAWSYSTEM(MODE_REDRAW, ActSys, NULL);
    return _WORMEXIT;
}

bool WORMFLIGHT(r_ShipHeader* ShipPtr, uint8 ActSys)
{
    uint8   i;
    bool    _WORMFLIGHT = false;

    r_ShipHeader*   MyShipPtr;
    struct NewScreen WH_NeuScreen = {0,0,320,256,4,0,0,GENLOCK_VIDEO,CUSTOMSCREEN|SCREENQUIET,NULL,NULL,NULL,NULL};
    struct TagItem  newTags[] ={{SA_DisplayID,  ScreenModeID_LowRes},
                            {SA_Interleaved,    true},
                            {SA_Draggable,      false},
                            {SA_Colors,         (uint32) &ColSpec},
                            {TAG_DONE,0},
                            {TAG_DONE,0},
                            {TAG_DONE,0}};
 
    if ((LowRes_Width < 320) || (LowRes_Height < 256))
    {
        WH_NeuScreen.Type |= AUTOSCROLL;
    }

    Error = true;
    MyShipPtr = ShipPtr;
    WORMHOLE_ShipShield = it_round(MyShipPtr->Shield / ShipData(MyShipPtr->SType).MaxShield * 760.0);
    if (Save.NoWorm)
    {
        return SMALLWORMFLIGHT(MyShipPtr);
    }
    for(i = 0; i < 2; ++i)
    {
        WHSoundMemA[i] = NULL;
    }
    CLOSEMYSCREENS();
    if (!FillITBitMap(&ImgBitMapW4, 80,512,4))
    {
        return _WORMFLIGHT;
    }

    memcpy(Tags, newTags, (7*sizeof(struct TagItem)));
    for(i = 0; i < 2; ++i)
    {
        MyScreen[i] = OpenScreenTagList(&WH_NeuScreen, Tags);
        if (NULL == MyScreen[i])
        {
            return WORMEXIT(false, MyShipPtr, ActSys);
        }
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
    }
    if (!WORMHOLE_INITIMAGES())
    {
        return WORMEXIT(false, MyShipPtr, ActSys);
    }
    WORMHOLE_INITSOUNDS();
    TRAVEL();
    if (WORMHOLE_ShipShield < 1)
    {
        _WORMFLIGHT = false;
    } else {
        MyShipPtr->Shield = it_round((double) WORMHOLE_ShipShield / 760.0 * ShipData(MyShipPtr->SType).MaxShield);
        _WORMFLIGHT = true;
    }
    Error = false;
    return WORMEXIT(_WORMFLIGHT, MyShipPtr, ActSys);
}

bool WORMHOLE(r_ShipHeader* ShipPtr, uint8 ActSys)
{
    r_ShipHeader*   MyShipPtr;
    bool            _WORMHOLE;
    int     i;
    uint8   SysID;
    uint16  Offset;
    char    s[60];
    char*   _s;

    MyShipPtr = ShipPtr;
    if ((MOVESHIP_ToX<0) || (MOVESHIP_ToX>478) || (MOVESHIP_ToY<0) || (MOVESHIP_ToY>478))
    {
        OffsetX = -MyShipPtr->PosX-1;
        OffsetY = -MyShipPtr->PosY-1;
        MOVESHIP_ToX = 256+(MyShipPtr->PosX+OffsetX)*32;
        MOVESHIP_ToY = 256+(MyShipPtr->PosY+OffsetY)*32;
        DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    }
    PLAYSOUND(3,250);
    Delay(7);
    for (i = 15; i>=0; i--)
    {
        BltBitMapRastPort((struct BitMap*) &ImgBitMap7,i*32,32,MyRPort_PTR[0],MOVESHIP_ToX,MOVESHIP_ToY,32,32,192);
        WaitTOF();
        Delay(4);
    }
    if (MOVESHIP_ToY>210)
    {
        Offset = 110;
    } else {
        Offset = 260;
    }
    SysID = (rand()%Save.Systems);  // shifted by -1 for the arrays
    MAKEWINBORDER(MyRPort_PTR[0],70,Offset,440,Offset+85,12,6,0);
    WRITE_RP0(256,Offset+13,ActPlayerFlag,WRITE_Center,3,PText[467]);
    WRITE_RP0(256,Offset+35,ActPlayerFlag,WRITE_Center,3,PText[468]);
    _s=my_strcpy(s, PText[469]);
    *_s++ = ' ';
    (void)my_strcpy(_s, Save.SystemName.data[SysID]);
    WRITE_RP0(256,Offset+57,ActPlayerFlag,WRITE_Center,3,s);
    if (0 == SystemHeader[SysID].Planets)
    {
        CREATENEWSYSTEM(SysID, ActPlayer-1, 1);
    }
    SystemFlags[ActPlayer-1][SysID] |= FLAG_KNOWN;
    LINKSHIP(MyShipPtr, &SystemHeader[SysID].FirstShip,1);
    do
    {
        MyShipPtr->PosX = (rand()%80)-40;
        MyShipPtr->PosY = (rand()%80)-40;
    }
    while ((FINDOBJECT(SysID,256+(MyShipPtr->PosX+OffsetX)*32,256+(MyShipPtr->PosY+OffsetY)*32,MyShipPtr))
        && ((MyShipPtr->PosX<-3) || (MyShipPtr->PosX>3)) && ((MyShipPtr->PosY<-3) || (MyShipPtr->PosY>3)));

    WAITLOOP(false);
    RECT_RP0_C0(70,Offset,440,Offset+85);
    REFRESHDISPLAY();

    PLAYSOUND(3,250);
    Delay(7);
    for(i = 0; i < 16; ++i)
    {
        BltBitMapRastPort((struct BitMap*) &ImgBitMap7,i*32,32,MyRPort_PTR[0],MOVESHIP_ToX,MOVESHIP_ToY,32,32,192);
        WaitTOF();
        Delay(4);
    }
    RECT_RP0_C0(MOVESHIP_ToX,MOVESHIP_ToY,MOVESHIP_ToX+31,MOVESHIP_ToY+31);
    Delay(20);
    if (WORMFLIGHT(MyShipPtr, ActSys))
    {
        _WORMHOLE = true;
        AUTOSHIPTRAVEL(SysID+1,MODE_SHIPS,MyShipPtr);
        if ((0 != (SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN))
         || (MyShipPtr->Owner == ActPlayerFlag))
        {
            DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
        }
        if (0 == MyShipPtr->Moving)
        {
            MyShipPtr->Moving = 1;
        }
    } else {
        _WORMHOLE = false;
    }
    return _WORMHOLE;
}
