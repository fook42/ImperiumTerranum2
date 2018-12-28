#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

enum GROUND {
 ID_WATER     = -1,
 ID_WOOD      = (0<<5),
 ID_GRASS     = (1<<5),
 ID_DESERT    = (2<<5),
 ID_HILLS     = (3<<5),
 ID_SPHALANX  = (4<<5),
 ID_SDI       = (5<<5),
 ID_CITY1     = (6<<5),
 ID_CITY2     = (7<<5),
 ID_LAKE      = (8<<5),
 ID_UFER_OUT  = (9<<5),
 ID_UFER_IN   =(10<<5),
 ID_DESTROYED =(11<<5)
};


ITBitMap    ImgBitMap5;
APTR        SLSoundMemA;
uint32      SLSoundSize;
sint16      ShipShield, ShipWeapon, GroundWeapon;
uint8       AScr, DefaultColor;

int         Moved[2];
sint16      LandID[8][8];
int         LandShield[8][8];
sint16      HitX, HitY, Fired;
uint32      SDIBaseHit, SPHBaseHit, CityHit, BioHit;

bool STARLANDING_INITIMAGES(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr)
{
    //ImgBitMap5 , SLSoundSize, SLSoundMemA
    char    s[40];
    uint8   stringlen;
    BPTR    FHandle;

    stringlen = strlen(PathStr[0]);
    memcpy(s, PathStr[0], stringlen);
    strcpy(s+stringlen, "LandScape");
    stringlen=strlen(s);
    switch (MyPlanetHeader->Class) {
        case CLASS_DESERT:   s[stringlen] = 'D'; break;
        case CLASS_HALFEARTH:s[stringlen] = 'H'; break;
        case CLASS_EARTH:    s[stringlen] = 'M'; break;
        case CLASS_ICE:      s[stringlen] = 'I'; break;
        case CLASS_STONES:   s[stringlen] = 'T'; break;
        case CLASS_WATER:    s[stringlen] = 'W'; break;
        default: { }
    }
    stringlen++;
    strcpy(s+stringlen, ".img");
    if (!RAWLOADIMAGE(s,0,0,640,32,5,&ImgBitMap5))
    {
        return false;
    }

    strcpy(s+stringlen, ".pal");
    (void) SETCOLOR(MyScreen[0],s);
    (void) SETCOLOR(MyScreen[1],s);

    if (Audio_enable)
    {
        stringlen = strlen(PathStr[6]);
        memcpy(s, PathStr[6], stringlen);
        switch (MyShipPtr->Weapon) {
            case WEAPON_GUN:       strcpy(s+stringlen,       "Gun"); break;
            case WEAPON_LASER:     strcpy(s+stringlen,     "Laser"); break;
            case WEAPON_PHASER:    strcpy(s+stringlen,    "Phaser"); break;
            case WEAPON_DISRUPTOR: strcpy(s+stringlen, "Disruptor"); break;
            case WEAPON_PTORPEDO:  strcpy(s+stringlen,  "PTorpedo"); break;
            default: { }
        }
        strcat(s, ".RAW");
        FHandle = OPENSMOOTH(s,MODE_OLDFILE);
        if (0 == FHandle)
        {
            return false;
        }
        (void)        Seek(FHandle, 0, OFFSET_END);
        SLSoundSize = Seek(FHandle, 0, OFFSET_BEGINNING);
        SLSoundMemA = IMemA[0];
        (void) Read(FHandle, SLSoundMemA, SLSoundSize);
        SLSoundSize >>= 1;
        Close(FHandle);
    }
    return true;
}

void LANDING_DRAWFIRE()
{
    // r:  ImgBitMap5, AScr, Moved[], ShipWeapon
    // rw: Fired, HitX, HitY, LandID[][], LandShield[][], SDIBaseHit, SPHBaseHit, CityHit, BioHit

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
        BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[HitX][HitY],0,MyRPort_PTR[AScr],HitX*32,HitY*32-Moved[AScr],32,32,192);
        if (608 == LandID[HitX][HitY])
        {
            if (0 < Moved[1-AScr])
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[HitX][HitY],0,MyRPort_PTR[1-AScr],HitX*32,HitY*32-Moved[1-AScr],32,32,192);
            } else {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[HitX][HitY],0,MyRPort_PTR[1-AScr],HitX*32,HitY*32-32,32,32,192);
            }
            HitX = -1;
            HitY = -1;
        }
    }
    if (0 < Fired)
    {
        Fired--;
    } else if ((LMB_PRESSED) && (MouseY(AScr) >= 65) && (MouseY(AScr) <= 382))
    {
        Fired = 7;
        if (Audio_enable)
        {
            custom.dmacon = BITCLR | DMAF_AUD2 | DMAF_AUD3;
            SPAddrC = SLSoundMemA; SPFreqC = 350; SPLengthC = SLSoundSize; SPVolC = 50;
            SPAddrD = SLSoundMemA; SPFreqD = 300; SPLengthD = SLSoundSize; SPVolD = 50;
            custom.dmacon = BITSET | DMAF_AUD2 | DMAF_AUD3;
        }
        HitX = (MouseX(AScr) - 32) >> 5;
        HitY = ((MouseY(AScr) / 2) + Moved[AScr]) >> 5;
        LandShield[HitX][HitY] -= ShipWeapon;
        if ((((ID_SDI  ==LandID[HitX][HitY])|| (ID_SPHALANX==LandID[HitX][HitY])) && (0 > LandShield[HitX][HitY]))
           || (ID_WOOD ==LandID[HitX][HitY])|| (ID_GRASS==LandID[HitX][HitY]) || (ID_DESERT==LandID[HitX][HitY])
           || (ID_HILLS==LandID[HitX][HitY])|| (ID_CITY2==LandID[HitX][HitY]) || (ID_CITY1 ==LandID[HitX][HitY]))
        {
            if       (ID_SDI      == LandID[HitX][HitY])  { SDIBaseHit++; }
            else if  (ID_SPHALANX == LandID[HitX][HitY])  { SPHBaseHit++; }
            else if ((ID_CITY1    == LandID[HitX][HitY])
                  || (ID_CITY2    == LandID[HitX][HitY])) { CityHit++; }
            else                                          { BioHit++; }
            PLAYSOUND(2,800);
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

    char    s[80];
    char*   _s;
    int     slen;

    for(i = 0; i < 2; i++)
    {
        RECTWIN(MyRPort_PTR[i],DefaultColor,0,0,255,240);
        RECTWIN(MyRPort_PTR[i],12,0,244,255,255);
        if (255 > ShipShield)
        {
            RECTWIN(MyRPort_PTR[i],0,ShipShield,245,254,254);
        }
    }
    for(j = 0; j < 8; j++)
    {
        for(i = 0; i < 8; i++)
        {
            LandID[j][i] = ID_WATER;
            LandShield[j][i] = 0;
        }
        LandID[j][0] = ID_UFER_IN;
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
    if (CityComp>30)
    {
        CityComp = 30;
    }

    do
    {
        /* MOVE UFER_IN */
        SDIBases++;
        Moved[AScr] -= 4;
        ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
        for(i = 0; i < 8; i++)
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr],i*32,0,32,4,192);
        }
        LANDING_DRAWFIRE();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
    }
    while (SDIBases < 14);

    SDIBases = (ActPProjects->data[34] / 10)-1;
    SPHBases = (ActPProjects->data[40] / 10)-1;

    do
    {
        /* MOVE LANDSCAPE AND BATTLE */
        Moved[AScr] -= 4;
        if (0 > Moved[AScr]) { Moved[AScr] += 32; }
        ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
        SetRGB32(MyVPort_PTR[AScr],0,0,0,0);
        if (30 == Moved[AScr])
        {
            for(i = 0; i <= 7; i++)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0], 0,MyRPort_PTR[AScr],i*32,2,32,2,192);
            }
            for(i = 0; i <= 7; i++)
            {
                if ((ID_SDI == LandID[i][7]) || (ID_SPHALANX == LandID[i][7]))
                {
                    SetRGB32(MyVPort_PTR[AScr],0,0xFF000000,0xFF000000,0xFF000000);
                    PLAYSOUND(2,400);
                    ShipShield -= GroundWeapon;
                    if (0 > ShipShield) { ShipShield = 0; }
                    if ((0 <= ShipShield) && (255 >= ShipShield))
                    {
                        for(k = 0; k < 2; k++)
                        {
                            RECTWIN(MyRPort_PTR[k],0,ShipShield,245,254,254);
                        }
                    }
                }
                for (j = 6; j>=0; j--)
                {
                    LandID[i][j+1]     = LandID[i][j];
                    LandShield[i][j+1] = LandShield[i][j];
                }
            }
            for(i = 0; i <= 7; i++)
            {
                LandShield[i][0] = 0;
                if ((0 == (rand()%5)) || (CityDrawed>CityComp))
                {
                    LandID[i][0] = (rand()%9)*32;
                    switch (LandID[i][0]) {
                        case ID_SDI: if (SDIBases<0)
                                    {
                                        LandID[i][0] = ID_CITY1;
                                    } else {
                                        SDIBases--;
                                        SDIBaseDrawed++;
                                        LandShield[i][0] = 18;
                                    } break;
                        case ID_SPHALANX: if (SPHBases<0)
                                    {
                                        LandID[i][0] = ID_CITY2;
                                    } else {
                                        SPHBases--;
                                        SPHBaseDrawed++;
                                        LandShield[i][0] = 72;
                                    } break;
                        default: { }
                    }
                    if ((ID_CITY1 == LandID[i][0]) || (ID_CITY2 == LandID[i][0]))
                    {
                        CityDrawed++;
                    } else if ((ID_WOOD == LandID[i][0]) || (ID_HILLS == LandID[i][0]))
                    {
                        BioDrawed++;
                    }
                } else {
                    if ((190 > MyPlanetHeader->Biosphaere) && (5 > (rand()%100)))
                    {
                        LandID[i][0] = 608; // 288+320 = ID_UFER_IN+ID_UFER_OUT
                    } else {
                        LandID[i][0] = (rand()%3) << 5; // 0,32,64
                        BioDrawed++;
                    }
                }
            }
            for(i = 0; i <= 7; i++)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],30,MyRPort_PTR[AScr],i*32,0,32,2,192);
            }
            if (HitY>-1)
            {
                HitY++;
                if (8 == HitY) { HitY = 0; }
            }
        } else {
            for(i = 0; i <= 7; i++)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr],i*32,0,32,4,192);
            }
        }
        LANDING_DRAWFIRE();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        if ((SDIBases<0) && (SPHBases<0))
        {
            SDIBases--;
        }
    }
    while (((ShipShield > 0) && ((SDIBases>=(-160)) || (CityDrawed<=CityComp))) || (AScr != 1));

    if (ShipShield<0)   /* DESTROYED SHIP */
    {
        MyShipPtr->Owner = 0;
        if (Audio_enable)
        {
            custom.dmacon = BITCLR | DMAF_AUDIO;
        }
        SetRGB32(MyVPort_PTR[0],0,0xFF000000,0xFF000000,0xFF000000);
        RECT(MyScreen[0],0,0,0,255,255);
        PLAYSOUND(2,1000);
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

        for(i = 1; i <= 20; i++)
        {
            ScreenToFront(MyScreen[AScr]);
            AScr = 1-AScr;
            WaitTOF();
        }
    } else {    /* MOVE UFER_OUT */
        if ((Moved[AScr]-4)>0)
        {
            do
            {
                Moved[AScr] -= 4;
                ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
                for(i = 0; i < 8; i++)
                {
                    BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr],i*32,0,32,4,192);
                }
                LANDING_DRAWFIRE();
                ScreenToFront(MyScreen[AScr]);
                AScr = 1-AScr;
            }
            while ((Moved[AScr]-4) >= 0);
        }

        SDIBases = 0;

        Moved[AScr] += 28;
        ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
        for(i = 0; i < 8; i++)
        {
            BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0], 0,MyRPort_PTR[AScr],i*32,2,32,2,192);

            for (j = 6; j>=0; j--)
            {
                LandID[i][j+1]     = LandID[i][j];
                LandShield[i][j+1] = LandShield[i][j];
            }
            LandID[i][0] = ID_UFER_OUT;

            BltBitMapRastPort((struct BitMap*) &ImgBitMap5, ID_UFER_OUT,30,MyRPort_PTR[AScr],i*32,0,32,2,192);
        }
        if (HitY>-1)
        {
            HitY++;
            if (HitY == 8) { HitY = 0; }
        }

        LANDING_DRAWFIRE();
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        do
        {
            SDIBases++;
            Moved[AScr] -= 4;
            if (Moved[AScr]<0) { Moved[AScr] += 32; }
            if (Moved[AScr] == 30)
            {
                for(i = 0; i < 8; i++)
                {
                    for (j = 6; j>=0; j--)
                    {
                        LandID[i][j+1]     = LandID[i][j];
                        LandShield[i][j+1] = LandShield[i][j];
                    }
                    LandID[i][0] = ID_WATER;
                }
                if (HitY>-1)
                {
                    HitY++;
                    if (HitY == 8) { HitY = 0; }
                }
            }
            ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
            for(i = 0; i <= 7; i++)
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap5,LandID[i][0],Moved[AScr],MyRPort_PTR[AScr],i*32,0,32,4,192);
            }
            LANDING_DRAWFIRE();
            ScreenToFront(MyScreen[AScr]);
            AScr = 1-AScr;
        }
        while (SDIBases < 14);

        for(k = 1; k <= 128; k++)
        {
            Moved[AScr] -= 4;
            if (Moved[AScr]<0) { Moved[AScr] += 32; }
            if (Moved[AScr] == 30)
            {
                for(i = 0; i < 8; i++)
                {
                    LandID[i][0] = ID_WATER;
                    for (j = 6; j>=0; j--)
                    {
                        LandID[i][j+1]     = LandID[i][j];
                        LandShield[i][j+1] = LandShield[i][j];
                    }
                }
                if (HitY>-1)
                {
                    HitY++;
                    if (HitY == 8) { HitY = 0; }
                }
            }
            ScrollRaster(MyRPort_PTR[AScr],0,-4,0,0,255,240);
            RECT(MyScreen[AScr],DefaultColor,0,0,255,4);
            LANDING_DRAWFIRE();
            ScreenToFront(MyScreen[AScr]);
            AScr = 1-AScr;
        }
    }

// show summary ....
    BioDrawed += CityDrawed+SDIBaseDrawed+SPHBaseDrawed;
    BioHit    += CityHit   +SDIBaseHit   +SPHBaseHit;

    strcpy(s, PathStr[0]);
    strcat(s, "Paper.pal");
    (void) SETCOLOR(MyScreen[AScr],s);
    strcpy(s+strlen(s)-4, ".img");
    if (!DISPLAYIMAGE(s,0,0,256,256,5,MyScreen[AScr],0)) { }
    if (SDIBaseDrawed>0) { ActPProjects->data[34] -= (SDIBaseHit*ActPProjects->data[34] / SDIBaseDrawed); }
    if (SPHBaseDrawed>0) { ActPProjects->data[40] -= (SPHBaseHit*ActPProjects->data[40] / SPHBaseDrawed); }
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

    slen=strlen(PText[411]);
    memcpy(s, PText[411], slen);
    s[slen++]=':';
    s[slen++]=' ';
    _s = dez2out(MyPlanetHeader->Population, 0, s+slen);
    *_s++=' ';
    *_s++='-';
    *_s++='>';
    *_s++=' ';
    MyPlanetHeader->Population = it_round(MyPlanetHeader->Population*Percs);
    if ((Percs>0) && (MyPlanetHeader->Population <= 0))
    {
        MyPlanetHeader->Population = 1;
    }
    _s = dez2out(MyPlanetHeader->Population, 0, _s);
    *_s++=' ';
    *_s++='(';
    _s = dez2out(it_round(Percs*100), 0, _s);
    *_s++='%';
    *_s++=')';
    *_s  =0;
    WRITE(22,40,29,0,MyScreen[AScr],1,s);

    slen=strlen(PText[152]);
    memcpy(s, PText[152], slen);
    s[slen++]=':';
    s[slen++]=' ';
    _s = dez2out((MyPlanetHeader->Infrastruktur >>1), 0, s+slen);
    *_s++='%';
    *_s++=' ';
    *_s++='-';
    *_s++='>';
    *_s++=' ';
    MyPlanetHeader->Infrastruktur -= (CityHit*MyPlanetHeader->Infrastruktur / CityDrawed);
    _s = dez2out((MyPlanetHeader->Infrastruktur >>1), 0, _s);
    *_s++='%';
    *_s  =0;
    WRITE(22,55,29,0,MyScreen[AScr],1,s);

    slen=strlen(PText[149]);
    memcpy(s, PText[149], slen);
    s[slen++]=':';
    s[slen++]=' ';
    _s = dez2out((MyPlanetHeader->Industrie >>1), 0, s+slen);
    *_s++='%';
    *_s++=' ';
    *_s++='-';
    *_s++='>';
    *_s++=' ';
    MyPlanetHeader->Industrie -= (CityHit*MyPlanetHeader->Industrie / CityDrawed);
    _s = dez2out((MyPlanetHeader->Industrie >>1), 0, _s);
    *_s++='%';
    *_s  =0;
    WRITE(22,70,29,0,MyScreen[AScr],1,s);

    slen=strlen(PText[151]);
    memcpy(s, PText[151], slen);
    s[slen++]=':';
    s[slen++]=' ';
    _s = dez2out((MyPlanetHeader->Biosphaere >>1), 0, s+slen);
    *_s++='%';
    *_s++=' ';
    *_s++='-';
    *_s++='>';
    *_s++=' ';
    MyPlanetHeader->Biosphaere -= (BioHit* MyPlanetHeader->Biosphaere / BioDrawed);
    _s = dez2out((MyPlanetHeader->Biosphaere >>1), 0, _s);
    *_s++='%';
    *_s  =0;
    WRITE(22,85,29,0,MyScreen[AScr],1,s);

    slen=strlen(PText[413]);
    memcpy(s, PText[413], slen);
    s[slen++]=':';
    s[slen++]=' ';
    _s = dez2out(Save.ImperatorState[ActPlayer-1], 0, s+slen);
    *_s++=' ';
    strcpy(_s, PText[414]);
    WRITE(22,120,29,0,MyScreen[AScr],1,s);
    ScreenToFront(MyScreen[AScr]);
    WAITLOOP(false);
}

void STARLANDINGEXIT(bool Error, r_PlanetHeader* PPtr, r_ShipHeader* SPtr, uint8 ActSys)
{
    DoClock = false;
    if (NULL != ImgBitMap5.MemA)
    {
        FreeMem(ImgBitMap5.MemA, ImgBitMap5.MemL);
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
    struct NewScreen    SL_NeuScreen = {16,0,256,256,5,0,0,0,CUSTOMSCREEN+SCREENQUIET,NULL,NULL,NULL,NULL};
    int             i;
    uint8           CivVar;
    r_ShipHeader*   MyShipPtr;
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProjects;

    struct TagItem  newTags[] ={{SA_DisplayID,   (0x1000+HelpID)},
                                {SA_Interleaved, true},
                                {SA_Draggable,   false},
                                {SA_Colors,      (uint32) &ColSpec},
                                {TAG_DONE,0},
                                {TAG_DONE,0},
                                {TAG_DONE,0}};

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

    if (!STARLANDING_INITIMAGES(MyPlanetHeader, MyShipPtr))
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
        MyShipPtr->ShieldBonus++;
    } else {
        MyShipPtr->Owner = 0;
    }
}