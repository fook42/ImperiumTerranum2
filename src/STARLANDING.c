#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"


#define ID_WATER      (-1)
#define ID_WOOD       (0)
#define ID_GRASS      (1<<5)
#define ID_DESERT     (2<<5)
#define ID_HILLS      (3<<5)
#define ID_SPHALANX   (4<<5)
#define ID_SDI        (5<<5)
#define ID_CITY1      (6<<5)
#define ID_CITY2      (7<<5)
#define ID_LAKE       (8<<5)
#define ID_UFER_OUT   (9<<5)
#define ID_UFER_IN   (10<<5)
#define ID_DESTROYED (11<<5)
#define ID_BUSTED    (19<<5)


ITBitMap    ImgBitMap5;
APTR        SLSoundMemA;
uint32      SLSoundSize;
sint16      ShipShield, ShipWeapon, GroundWeapon;
uint8       DefaultColor;

int         Moved[2];
sint16      LandID[8][8];
int         LandShield[8][8];
sint16      HitX, HitY, Fired;
uint32      SDIBaseHit, SPHBaseHit, CityHit, BioHit;

bool STARLANDING_INITIMAGES(r_PlanetHeader* MyPlanetHeader, uint8 Ship_Weapon)
{
    //ImgBitMap5 , SLSoundSize, SLSoundMemA
    char    s[40];
    char*   _s;
    BPTR    FHandle;
    bool    returnvalue = false;

    _s=my_strcpy(s, PathStr[0]);
    _s=my_strcpy(_s, "LandScape");
    switch (MyPlanetHeader->Class) {
        case CLASS_DESERT:   *_s++ = 'D'; break;
        case CLASS_HALFEARTH:*_s++ = 'H'; break;
        case CLASS_EARTH:    *_s++ = 'M'; break;
        case CLASS_ICE:      *_s++ = 'I'; break;
        case CLASS_STONES:   *_s++ = 'T'; break;
        case CLASS_WATER:    *_s++ = 'W'; break;
        default: { }
    }
    _s=my_strcpy(_s, ".img");
    if (RAWLOADIMAGE(s,0,0,640,32,5,&ImgBitMap5))
    {
        (void) my_strcpy(_s-3, "pal");
        (void) SETCOLOR(MyScreen[0],s);
        (void) SETCOLOR(MyScreen[1],s);

        if (Audio_enable)
        {
            _s=my_strcpy(s, PathStr[6]);
            switch (Ship_Weapon) {
                case WEAPON_GUN:       _s=my_strcpy(_s,       "Gun"); break;
                case WEAPON_LASER:     _s=my_strcpy(_s,     "Laser"); break;
                case WEAPON_PHASER:    _s=my_strcpy(_s,    "Phaser"); break;
                case WEAPON_DISRUPTOR: _s=my_strcpy(_s, "Disruptor"); break;
                case WEAPON_PTORPEDO:  _s=my_strcpy(_s,  "PTorpedo"); break;
                default: { }
            }
            _s=my_strcpy(_s, ".RAW");
            FHandle = OPENSMOOTH(s,MODE_OLDFILE);
            if (0 != FHandle)
            {
                (void)        Seek(FHandle, 0, OFFSET_END);
                SLSoundSize = Seek(FHandle, 0, OFFSET_BEGINNING);
                SLSoundMemA = IMemA[0];
                (void) Read(FHandle, SLSoundMemA, SLSoundSize);
                SLSoundSize >>= 1;
                Close(FHandle);
                returnvalue = true;
            }
        } else {
            returnvalue = true;
        }
    }
    return returnvalue;
}

void LANDING_DRAWFIRE()
{
    // r:  ImgBitMap5, AScr, Moved[], ShipWeapon
    // rw: Fired, HitX, HitY, LandID[][], LandShield[][], SDIBaseHit, SPHBaseHit, CityHit, BioHit

    sint16  LandType;

    if (Audio_enable)
    {
        SPAddrA = ZeroSound; SPLengthA = 1;
        SPAddrB = ZeroSound; SPLengthB = 1;
        SPAddrC = ZeroSound; SPLengthC = 1;
        SPAddrD = ZeroSound; SPLengthD = 1;
    }

    if ((HitX>-1) && (HitY>-1))
    {
        LandID[HitX][HitY] += 32;
        LandType = LandID[HitX][HitY];

        BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandType,0,MyRPort_PTR[AScr],HitX*32,HitY*32-Moved[AScr],32,32,192);
        if (ID_BUSTED == LandType)
        {
            if (0 < Moved[1-AScr])
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5, ID_BUSTED,0,MyRPort_PTR[1-AScr],HitX*32,HitY*32-Moved[1-AScr],32,32,192);
            } else {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5, ID_BUSTED,0,MyRPort_PTR[1-AScr],HitX*32,HitY*32-           32,32,32,192);
            }
            HitX = -1;
            HitY = -1;
        }
        
    }
    if (0 < Fired)
    {
        Fired--;
    } else if ((LMB_PRESSED) && (MouseY(AScr) > 64) && (MouseY(AScr) < 383))
    {
        Fired = 7;
        if (Audio_enable)
        {
            custom.dmacon = BITCLR | DMAF_AUD2 | DMAF_AUD3;
            SPAddrC = SLSoundMemA; SPFreqC = 350; SPLengthC = SLSoundSize; SPVolC = 50;
            SPAddrD = SLSoundMemA; SPFreqD = 300; SPLengthD = SLSoundSize; SPVolD = 50;
            custom.dmacon = BITSET | DMAF_AUD2 | DMAF_AUD3;
        }
        HitX = (MouseX(AScr)) >> 5;
        HitY = (MouseY(AScr) + Moved[AScr]) >> 5;

        LandShield[HitX][HitY] -= ShipWeapon;
        LandType = LandID[HitX][HitY];

        if ((((ID_SDI  ==LandType)|| (ID_SPHALANX==LandType)) && (0 > LandShield[HitX][HitY]))
           || (ID_WOOD ==LandType)|| (ID_GRASS==LandType) || (ID_DESERT==LandType)
           || (ID_HILLS==LandType)|| (ID_CITY2==LandType) || (ID_CITY1 ==LandType))
        {
            switch (LandType)
            {
                case ID_SDI:        ++SDIBaseHit; break;
                case ID_SPHALANX:   ++SPHBaseHit; break;
                case ID_CITY1:
                case ID_CITY2:      ++CityHit; break;
                default:            ++BioHit;
            }
            PLAYSOUND(1,800);
            LandID[HitX][HitY] = ID_DESTROYED;
            BltBitMapRastPort((struct BitMap*) &ImgBitMap5, ID_DESTROYED,0,MyRPort_PTR[AScr],HitX*32,HitY*32-Moved[AScr],32,32,192);
        } else {
            HitX = -1;
            HitY = -1;
        }
    }
}

void STARLANDING_LANDING(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr, ByteArr42* ActPProjects)
{
    //ShipShield, GroundWeapon, AScr, DefaultColor

    // not needed elsewhere..
    int     SDIBases     ,SPHBases;
    uint32  SDIBaseDrawed,SPHBaseDrawed,CityDrawed,BioDrawed,CityComp;
    double  Percs;
    int     i, j, k;
    sint16  LandType;

    char    s[80];
    char*   _s;

    for(i = 0; i < 2; ++i)
    {
        RECTWIN(MyRPort_PTR[i],DefaultColor,0,0,255,240);
        RECTWIN(MyRPort_PTR[i],12,0,244,255,255);
        if (255 > ShipShield)
        {
            RECTWIN(MyRPort_PTR[i],0,ShipShield,245,254,254);
        }
    }
    for(i = 0; i < 8; ++i)
    {
        for(j = 0; j < 8; ++j)
        {
            LandID[i][j] = ID_WATER;
            LandShield[i][j] = 0;
        }
        LandID[i][0] = ID_UFER_IN;
    }
    Moved[AScr]   = 32;
    Moved[1-AScr] = 30;
    Fired = 0;
    HitX = -1;
    HitY = -1;
    SDIBases = 0;
    SPHBases = 0;
    SDIBaseDrawed = 0;
    SPHBaseDrawed = 0;
    SDIBaseHit = 0;
    SPHBaseHit = 0;
    CityDrawed = 0;
    CityHit = 0;
    BioDrawed = 0;
    BioHit = 0;
    CityComp = (MyPlanetHeader->Population / 300)+1;
    if (30 < CityComp)
    {
        CityComp = 30;
    }

    do
    {
        /* MOVE UFER_IN */
        ++SDIBases;
        Moved[AScr] -= 4;
        ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
        k = 0;
        for(i = 0; i < 8; ++i)
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr], k,0,32,4,192);
            k += 32;
        }
        LANDING_DRAWFIRE();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
    }
    while (14 > SDIBases);

    SDIBases = (ActPProjects->data[PROJECT_SDI] / 10)-1;
    SPHBases = (ActPProjects->data[PROJECT_SPACEPHALANX] / 10)-1;

    do
    {
        /* MOVE LANDSCAPE AND BATTLE */
        Moved[AScr] -= 4;
        if (0 > Moved[AScr]) { Moved[AScr] += 32; }
        ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
        SetRGB32(MyVPort_PTR[AScr],0,0,0,0);
        if (30 == Moved[AScr])
        {
            k = 0;
            for(i = 0; i < 8; ++i)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0], 0,MyRPort_PTR[AScr], k,2,32,2,192);

                if ((ID_SDI == LandID[i][7]) || (ID_SPHALANX == LandID[i][7]))
                {
                    SetRGB32(MyVPort_PTR[AScr],0,0xFF000000,0xFF000000,0xFF000000);
                    PLAYSOUND(1,400);
                    ShipShield -= GroundWeapon;
                    if (0  >  ShipShield) { ShipShield = 0; }
                    if ((0 <= ShipShield) && (255 >= ShipShield))
                    {
                        RECT_RP0_C0(ShipShield,245,254,254);
                        RECT_RP1_C0(ShipShield,245,254,254);
                    }
                }
                for (j = 6; j>=0; --j)
                {
                    LandID[i][j+1]     = LandID[i][j];
                    LandShield[i][j+1] = LandShield[i][j];
                }
                LandShield[i][0] = 0;

                if ((0 == (rand()%5)) || (CityDrawed > CityComp))
                {
                    LandType = (rand()%9) << 5;     // 0..8 = ID_WOOD .. ID_LAKE
                    switch (LandType) {
                        case ID_SDI:        if (0 > SDIBases)
                                            {
                                                LandType = ID_CITY1;
                                            } else {
                                                --SDIBases;
                                                ++SDIBaseDrawed;
                                                LandShield[i][0] = 18;
                                            } break;
                        case ID_SPHALANX:   if (0 > SPHBases)
                                            {
                                                LandType = ID_CITY2;
                                            } else {
                                                --SPHBases;
                                                ++SPHBaseDrawed;
                                                LandShield[i][0] = 72;
                                            } break;
                        case ID_WOOD:
                        case ID_HILLS:      ++BioDrawed; break;
                        default: { }
                    }
                    if ((ID_CITY1 == LandType) || (ID_CITY2 == LandType))
                    {
                        ++CityDrawed;
                    }
                } else {
                    if ((190 > MyPlanetHeader->Biosphaere) && (5 > (rand()%100)))
                    {
                        LandType = ID_BUSTED; // ???
                    } else {
                        LandType = (rand()%3) << 5; // 0,32,64  = ID_WOOD, ID_GRASS, ID_DESERT
                        ++BioDrawed;
                    }
                }
                LandID[i][0] = LandType;

                BltBitMapRastPort((struct BitMap*) &ImgBitMap5, LandID[i][0], 30,MyRPort_PTR[AScr], k,0,32,2,192);
                k += 32;
            }
            if (-1 < HitY)
            {
                ++HitY;
                if (8 == HitY) { HitY = 0; }
            }
        } else {
            k = 0;
            for(i = 0; i < 8; ++i)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr], k,0,32,4,192);
                k += 32;
            }
        }
        LANDING_DRAWFIRE();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        if ((0 > SDIBases) && (0 > SPHBases))
        {
            --SDIBases;
        }
    }
    while (((0 < ShipShield) && (((-160) <= SDIBases) || (CityDrawed <= CityComp))) || (AScr != 0));

    if (0 > ShipShield)   /* DESTROYED SHIP */
    {
        MyShipPtr->Owner = 0;
        if (Audio_enable)
        {
            custom.dmacon = BITCLR | DMAF_AUDIO;
        }
        SetRGB32(MyVPort_PTR[0],0,0xFF000000,0xFF000000,0xFF000000);
        RECT_RP0_C0(0,0,255,255);
        PLAYSOUND(1,1000);
        if (Audio_enable)
        {
            SPAddrC = SoundMemA[2];              SPFreqC = 900; SPLengthC = SoundSize[2] / 2; SPVolC = 64;
            SPAddrD = SoundMemA[2]+SoundSize[2]; SPFreqD = 900; SPLengthD = SoundSize[2] / 2; SPVolD = 64;
        }
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        WaitTOF();
        if (Audio_enable)
        {
            SPAddrA   = ZeroSound; SPLengthA = 1;
        }
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        WaitTOF();
        if (Audio_enable)
        {
            SPAddrB   = ZeroSound; SPLengthB = 1;
            custom.dmacon = BITSET | DMAF_AUD2 | DMAF_AUD3;
        }
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        WaitTOF();
        if (Audio_enable)
        {
            SPAddrD   = ZeroSound; SPLengthD = 1;
        }
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        WaitTOF();
        if (Audio_enable)
        {
            SPAddrC   = ZeroSound; SPLengthC = 1;
        }

        for(i = 0; i < 20; ++i)
        {
            ScreenToFront(MyScreen[AScr]);
            AScr = 1-AScr;
            WaitTOF();
        }
    } else {    /* MOVE UFER_OUT */
        if (4 < Moved[AScr])
        {
            do
            {
                Moved[AScr] -= 4;
                ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
                k = 0;
                for(i = 0; i < 8; ++i)
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr], k,0,32,4,192);
                    k += 32;
                }
                LANDING_DRAWFIRE();
                ScreenToFront(MyScreen[AScr]);
                AScr = 1-AScr;
            }
            while (3 < Moved[AScr]);
        }

        Moved[AScr] += 28;
        ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
        k = 0;
        for(i = 0; i < 8; ++i)
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap5, LandID[i][0], 0,MyRPort_PTR[AScr], k,2,32,2,192);

            for (j = 6; j>=0; --j)
            {
                LandID[i][j+1]     = LandID[i][j];
                LandShield[i][j+1] = LandShield[i][j];
            }
            LandID[i][0] = ID_UFER_OUT;

            BltBitMapRastPort((struct BitMap*) &ImgBitMap5, ID_UFER_OUT,30,MyRPort_PTR[AScr], k,0,32,2,192);
            
            k += 32;
        }
        if (-1 < HitY)
        {
            ++HitY;
            if (8 == HitY) { HitY = 0; }
        }

        SDIBases = 0;

        LANDING_DRAWFIRE();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        do
        {
            ++SDIBases;
            Moved[AScr] -= 4;
            if (0  >  Moved[AScr]) { Moved[AScr] += 32; }
            if (30 == Moved[AScr])
            {
                for(i = 0; i < 8; ++i)
                {
                    for (j = 6; j>=0; --j)
                    {
                        LandID[i][j+1]     = LandID[i][j];
                        LandShield[i][j+1] = LandShield[i][j];
                    }
                    LandID[i][0] = ID_WATER;
                }
                if (-1 < HitY)
                {
                    ++HitY;
                    if (8 == HitY) { HitY = 0; }
                }
            }
            ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
            k = 0;
            for(i = 0; i < 8; ++i)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr], k,0,32,4,192);
                k += 32;
            }

            LANDING_DRAWFIRE();
            ScreenToFront(MyScreen[AScr]);
            AScr = 1-AScr;
        }
        while (SDIBases < 14);

        for(k = 0; k < 128; ++k)
        {
            Moved[AScr] -= 4;
            if (0  >  Moved[AScr]) { Moved[AScr] += 32; }
            if (30 == Moved[AScr])
            {
                for(i = 0; i < 8; ++i)
                {
                    LandID[i][0] = ID_WATER;
                    for (j = 6; j>=0; --j)
                    {
                        LandID[i][j+1]     = LandID[i][j];
                        LandShield[i][j+1] = LandShield[i][j];
                    }
                }
                if (-1 < HitY)
                {
                    ++HitY;
                    if (8 == HitY) { HitY = 0; }
                }
            }

            ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
            RECTWIN(MyRPort_PTR[AScr],DefaultColor,0,0,255,4);
            LANDING_DRAWFIRE();
            ScreenToFront(MyScreen[AScr]);
            AScr = 1-AScr;
        }
    }

// show summary ....
    BioDrawed += CityDrawed+SDIBaseDrawed+SPHBaseDrawed;
    BioHit    += CityHit   +SDIBaseHit   +SPHBaseHit;

    _s=my_strcpy( s, PathStr[0]);
    _s=my_strcpy(_s, "Paper.pal");
    (void) SETCOLOR(MyScreen[AScr],s);
    (void) my_strcpy(_s-3, "img");
    if (!DISPLAYIMAGE(s,0,0,256,256,5,MyScreen[AScr],0)) { }
    if (SDIBaseDrawed>0) { ActPProjects->data[PROJECT_SDI]          -= (SDIBaseHit*ActPProjects->data[PROJECT_SDI] / SDIBaseDrawed); }
    if (SPHBaseDrawed>0) { ActPProjects->data[PROJECT_SPACEPHALANX] -= (SPHBaseHit*ActPProjects->data[PROJECT_SPACEPHALANX] / SPHBaseDrawed); }
    if ((SDIBaseDrawed>0) && (SPHBaseDrawed>0))
    {
        CityDrawed = CityDrawed*20+SDIBaseDrawed+SPHBaseDrawed;
        CityHit    = CityHit   *20+SDIBaseHit   +SPHBaseHit;
    }
    if (0 == CityHit)
    {
        Percs = 1.0f;
    } else {
        Percs = 1.0f-((double)CityHit/CityDrawed);
    }
    if (0.8f > Percs)
    {
        MyPlanetHeader->Ethno = MyPlanetHeader->PFlags & FLAG_CIV_MASK;
        Save.ImperatorState[ActPlayer-1] = it_round(Save.ImperatorState[ActPlayer-1]*Percs);
    }

    _s=my_strcpy(s, PText[411]);
    *_s++ = ':';
    *_s++ = ' ';
    _s = dez2out(MyPlanetHeader->Population, 0, _s);
    *_s++ = ' ';
    *_s++ = '-';
    *_s++ = '>';
    *_s++ = ' ';
    MyPlanetHeader->Population = it_round(MyPlanetHeader->Population*Percs);
    if ((Percs>0) && (MyPlanetHeader->Population == 0))
    {
        MyPlanetHeader->Population = 1;
    }
    _s = dez2out(MyPlanetHeader->Population, 0, _s);
    *_s++ = ' ';
    *_s++ = '(';
    _s = dez2out(it_round(Percs*100), 0, _s);
    *_s++ = '%';
    *_s++ = ')';
    *_s  =0;
    WRITE(22,40,29,0,MyRPort_PTR[AScr],0,s);

    _s=my_strcpy(s, _PT_Infrastruktur);
    *_s++ = ':';
    *_s++ = ' ';
    _s = dez2out((MyPlanetHeader->Infrastruktur >>1), 0, _s);
    *_s++ = '%';
    *_s++ = ' ';
    *_s++ = '-';
    *_s++ = '>';
    *_s++ = ' ';
    MyPlanetHeader->Infrastruktur -= (CityHit*MyPlanetHeader->Infrastruktur / CityDrawed);
    _s = dez2out((MyPlanetHeader->Infrastruktur >>1), 0, _s);
    *_s++ = '%';
    *_s  =0;
    WRITE(22,55,29,0,MyRPort_PTR[AScr],0,s);

    _s=my_strcpy(s, PText[149]);
    *_s++ = ':';
    *_s++ = ' ';
    _s = dez2out((MyPlanetHeader->Industrie >>1), 0, _s);
    *_s++ = '%';
    *_s++ = ' ';
    *_s++ = '-';
    *_s++ = '>';
    *_s++ = ' ';
    MyPlanetHeader->Industrie -= (CityHit*MyPlanetHeader->Industrie / CityDrawed);
    _s = dez2out((MyPlanetHeader->Industrie >>1), 0, _s);
    *_s++ = '%';
    *_s  =0;
    WRITE(22,70,29,0,MyRPort_PTR[AScr],0,s);

    _s=my_strcpy(s, _PT_Biosphaere);
    *_s++ = ':';
    *_s++ = ' ';
    _s = dez2out((MyPlanetHeader->Biosphaere >>1), 0, _s);
    *_s++ = '%';
    *_s++ = ' ';
    *_s++ = '-';
    *_s++ = '>';
    *_s++ = ' ';
    MyPlanetHeader->Biosphaere -= (BioHit* MyPlanetHeader->Biosphaere / BioDrawed);
    _s = dez2out((MyPlanetHeader->Biosphaere >>1), 0, _s);
    *_s++ = '%';
    *_s  =0;
    WRITE(22,85,29,0,MyRPort_PTR[AScr],0,s);

    _s=my_strcpy(s, PText[413]);
    *_s++ = ':';
    *_s++ = ' ';
    _s = dez2out(Save.ImperatorState[ActPlayer-1], 0, _s);
    *_s++ = ' ';
    (void) my_strcpy(_s, PText[414]);
    WRITE(22,120,29,0,MyRPort_PTR[AScr],0,s);
    ScreenToFront(MyScreen[AScr]);
    WAITLOOP(false);
}

void STARLANDINGEXIT(bool Error, r_PlanetHeader* PPtr, r_ShipHeader* SPtr, uint8 ActSys)
{
    DoClock = false;
    if (NULL != ImgBitMap5.MemA)
    {
        my_FreeMem(ImgBitMap5.MemA, ImgBitMap5.MemL);
        ImgBitMap5.MemA = NULL;
    }
    CLOSEMYSCREENS();
    if (Audio_enable)
    {
        custom.dmacon = BITCLR | DMAF_AUDIO;
    }
    (void) OPENMAINSCREENS();
    (void) INITDESK(0);
    Screen2 = 0;
    if (Error)
    {
        MOVESHIP_SMALLANDING(PPtr,SPtr,ActSys);
    }
    DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
}

void STARLANDING(r_PlanetHeader* PPtr, r_ShipHeader* SPtr, uint8 ActSys)
{
    struct NewScreen SL_NeuScreen = {16,0,256,256,5,0,0,0,CUSTOMSCREEN|SCREENQUIET,NULL,NULL,NULL,NULL};
    int             i;
    uint8           CivVar;
    r_ShipHeader*   MyShipPtr;
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProjects;

    struct TagItem  newTags[] ={{SA_DisplayID,   ScreenModeID_LowRes},
                                {SA_Interleaved, true},
                                {SA_Draggable,   false},
                                {SA_Colors,      (uint32) &ColSpec},
                                {TAG_DONE,0},
                                {TAG_DONE,0},
                                {TAG_DONE,0}};

    if ((LowRes_Width < 256) || (LowRes_Height < 256))
    {
        SL_NeuScreen.Type |= AUTOSCROLL;
    }
    if (Save.SmallLand)
    {
        MOVESHIP_SMALLANDING(PPtr,SPtr,ActSys);
        return;
    }
    MyPlanetHeader = PPtr;
    MyShipPtr = SPtr;
    ActPProjects = MyPlanetHeader->ProjectPtr;
    CLOSEMYSCREENS();
    Display = 100;

    if (!FillITBitMap(&ImgBitMap5, 80,34,5))
    {
        return;
    }

    memcpy( Tags, newTags, 7*sizeof(struct TagItem));

    for(i = 0; i < 2; i++)
    {
        MyScreen[i] = OpenScreenTagList(&SL_NeuScreen, Tags);
        if (NULL == MyScreen[i])
        {
            STARLANDINGEXIT(true,PPtr,SPtr,ActSys);
            return;
        }
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
    }

    SWITCHDISPLAY();

    if (!STARLANDING_INITIMAGES(MyPlanetHeader, MyShipPtr->Weapon))
    {
        STARLANDINGEXIT(true,PPtr,SPtr,ActSys);
        return;
    }

    BltBitMapRastPort((struct BitMap*) &ImgBitMap5,304,0,MyRPort_PTR[0],0,0,1,1,192);
    DefaultColor = ReadPixel(MyRPort_PTR[0],0,0);
    AScr = 1;

    ShipShield = (MyShipPtr->Shield) + (MyShipPtr->Tactical*3) + (MyShipPtr->ShieldBonus);
    ShipWeapon = it_round((MyShipPtr->Weapon/10.0+1.0)*ShipData(MyShipPtr->SType).WeaponPower - MyShipPtr->Tactical);
    CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
    if (0 != CivVar)
    {
        CivVar--; // to shift the Save.TechCosts-array
        GroundWeapon = WEAPON_GUN;
        if (0 == Save.TechCosts[CivVar].data[15]) { GroundWeapon = WEAPON_LASER; }
        if (0 == Save.TechCosts[CivVar].data[24]) { GroundWeapon = WEAPON_PHASER; }
        if (0 == Save.TechCosts[CivVar].data[32]) { GroundWeapon = WEAPON_DISRUPTOR; }
        if (0 == Save.TechCosts[CivVar].data[27]) { GroundWeapon = WEAPON_PTORPEDO; }
    } else {
        GroundWeapon = WEAPON_PTORPEDO;
    }
    GroundWeapon *= 3;
    STARLANDING_LANDING(MyPlanetHeader, MyShipPtr, ActPProjects);
    STARLANDINGEXIT(false,PPtr,SPtr,ActSys);
    if (0 < ShipShield)
    {
        if (0 < (ShipShield - (MyShipPtr->Tactical*3) - MyShipPtr->ShieldBonus))
        {
            MyShipPtr->Shield = ShipShield - (MyShipPtr->Tactical*3) - MyShipPtr->ShieldBonus;
        } else {
            MyShipPtr->Shield = 1;
        }
        ++(MyShipPtr->ShieldBonus);
    } else {
        MyShipPtr->Owner = 0;
    }
}
