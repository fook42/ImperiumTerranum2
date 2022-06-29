#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

#define STARS   15

typedef struct r_SmallShipHeader {
    sint16  ShieldS;
    sint16  WeaponS;
} r_SmallShipHeader;

r_SmallShipHeader   SSHeader[2];

uint16*         BSFSoundMemA[3]; // 2..4 => 0..2
uint32          BSFSoundSize[3]; // 2..4 => 0..2
r_ShipHeader*   ShipPtr1;
r_ShipHeader*   ShipPtr2;
bool            Visible;

/* all these variables are defined as global
    - XTRAROUND initializes, STARFLY uses...
*/
sint16  x[2],y[2];
sint16  Angle[2];
int     sx1[2][2], sy1[2][2], sx2[2][2], sy2[2][2];
uint16  StepCtr;

const sint8 mx[] = {0,1,2,2,2,2,2,1,0,-1,-2,-2,-2,-2,-2,-1};
const sint8 my[] = {-2,-2,-2,-1,0,1,2,2,2,2,2,1,0,-1,-2,-2};

uint8   Rotation;

// BSFSoundSize, BSFSoundMemA
bool LOADSAMPLE(char* FName, const int SID)
{
    LONG    size;
    BPTR    FHandle;

    FHandle = OPENSMOOTH(FName, MODE_OLDFILE, &size);
    if (0 == FHandle)
    {
        return false;
    }
    BSFSoundSize[SID] = (size >> 1);
    (void) Read(FHandle, (APTR) BSFSoundMemA[SID], size);
    Close(FHandle);
    return true;
}

void INITSOUNDNAMES(uint8 SoundID, char* s)
{
    const char* soundnames[]={"Gun","Laser","Phaser","Disruptor","PTorpedo",""};
    uint8   soundnr;
    char*   _s;

    switch (SoundID) {
        case WEAPON_GUN:       soundnr=0; break;
        case WEAPON_LASER:     soundnr=1; break;
        case WEAPON_PHASER:    soundnr=2; break;
        case WEAPON_DISRUPTOR: soundnr=3; break;
        case WEAPON_PTORPEDO:  soundnr=4; break;
        default: { soundnr=5; }
    }
    _s=my_strcpy( s, PathStr[6]);  /* SFX/ */
    _s=my_strcpy(_s, soundnames[soundnr]);
    (void) my_strcpy(_s, ".RAW");
}

// ShipPtr1, ShipPtr2, BSFSoundMemA, BSFSoundSize
bool INITIMAGES()
{
    char    s[60];
    char*   _s1;
    char*   _s2;

    _s1=my_strcpy( s, PathStr[5]);  /* SHIPS/ */
    _s2=my_strcpy(_s1, Project.data[ShipPtr1->SType]);
    (void) my_strcpy(_s2, ".img");
    if (!RAWLOADIMAGE(s,0,32,512,32,4, &ImgBitMap4)) { return false; }

    _s2=my_strcpy(_s1, Project.data[ShipPtr2->SType]);
    (void) my_strcpy(_s2, ".img");
    if (!RAWLOADIMAGE(s,0,64,512,32,4, &ImgBitMap4)) { return false; }

    BSFSoundMemA[0] = (uint16*) IMemA[0];
    INITSOUNDNAMES(ShipPtr1->Weapon, s);
    if (!LOADSAMPLE(s, 0)) { return false; }

    BSFSoundMemA[1] = BSFSoundMemA[0] + BSFSoundSize[0];
    INITSOUNDNAMES(ShipPtr2->Weapon, s);
    if (!LOADSAMPLE(s, 1)) { return false; }

    BSFSoundMemA[2] = BSFSoundMemA[1] + BSFSoundSize[1];
    _s1=my_strcpy(s, PathStr[6]);  /* SFX/ */
    (void) my_strcpy(_s1, "FightSoundDS.RAW");
    if (!LOADSAMPLE(s,2)) { return false; }

    BSFSoundSize[2] /= 2;

    return true;
}

// AScr
void STARFLY(uint8 Ship)
{
    uint16  FireFactor;
    sint16  dx,dy,da = 0;

    ++Rotation;
    if (2 < Rotation)
    {
        Rotation = 0;
    }
    RectFill(MyRPort_PTR[AScr],x[Ship]-4,y[Ship]-4,x[Ship]+36,y[Ship]+36);

    x[Ship] += mx[Angle[Ship] >> 5];
    y[Ship] += my[Angle[Ship] >> 5];

    dx = (x[1-Ship]-x[Ship]);
    dy = (y[1-Ship]-y[Ship]);
    if ((dx == 0) && (dy<0))
    {
        da = 0;
    } else if ((dx>0) && (dy<0))
    {
        da = 64;
    } else if ((dx>0) && (dy == 0))
    {
        da = 128;
    } else if ((dx>0) && (dy>0))
    {
        da = 192;
    } else if ((dx == 0) && (dy>0))
    {
        da = 256;
    } else if ((dx<0) && (dy>0))
    {
        da = 320;
    } else if ((dx<0) && (dy == 0))
    {
        da = 384;
    } else if ((dx<0) && (dy<0))
    {
        da = 448;
    }

    if (0 == Rotation)
    {
        StepCtr = 0;
        if ((x[Ship]<15) || (x[Ship]>592))
        {
            StepCtr = 1;
        }
        if ((y[Ship]<15) || (y[Ship]>462))
        {
            StepCtr++;
        }
        if (1 < StepCtr)
        {
            Angle[Ship] += 32;
        } else if ( ((y[Ship]<40)  && (Angle[Ship] >= 0)   && (Angle[Ship] <= 128) && (Ship == 1)) ||
                    ((y[Ship]<90)  && (Angle[Ship] >= 0)   && (Angle[Ship] <= 128) && (Ship == 0)) ||
                    ((x[Ship]>560) && (Angle[Ship] >= 128) && (Angle[Ship] <= 256) && (Ship == 1)) ||
                    ((x[Ship]>510) && (Angle[Ship] >= 128) && (Angle[Ship] <= 256) && (Ship == 0)) ||
                    ((y[Ship]>420) && (Angle[Ship] >= 256) && (Angle[Ship] <= 384) && (Ship == 1)) ||
                    ((y[Ship]>370) && (Angle[Ship] >= 256) && (Angle[Ship] <= 384) && (Ship == 0)) ||
                    ((x[Ship]<40)  && (((Angle[Ship] >= 384) && (Angle[Ship] <= 480)) || (Angle[Ship] == 0)) && (Ship == 1)) ||
                    ((x[Ship]<90)  && (((Angle[Ship] >= 384) && (Angle[Ship] <= 480)) || (Angle[Ship] == 0)) && (Ship == 0)))
        {
            Angle[Ship] += 32;
        } else if ( ((y[Ship]<40)  && (Angle[Ship] >= 384) && (Angle[Ship] <= 480) && (Ship == 1)) ||
                    ((y[Ship]<90)  && (Angle[Ship] >= 384) && (Angle[Ship] <= 480) && (Ship == 0)) ||
                    ((x[Ship]>560) && (Angle[Ship] >= 0)   && (Angle[Ship] <= 96)  && (Ship == 1)) ||
                    ((x[Ship]>510) && (Angle[Ship] >= 0)   && (Angle[Ship] <= 96)  && (Ship == 0)) ||
                    ((y[Ship]>420) && (Angle[Ship] >= 128) && (Angle[Ship] <= 224) && (Ship == 1)) ||
                    ((y[Ship]>370) && (Angle[Ship] >= 128) && (Angle[Ship] <= 224) && (Ship == 0)) ||
                    ((x[Ship]<40)  && (Angle[Ship] >= 256) && (Angle[Ship] <= 352) && (Ship == 1)) ||
                    ((x[Ship]<90)  && (Angle[Ship] >= 256) && (Angle[Ship] <= 352) && (Ship == 0)))
        {
            Angle[Ship] -= 32;
        } else if ((x[Ship] >= (x[1-Ship]-130)) && (x[Ship] <= (x[1-Ship]+130))
                && (y[Ship] >= (y[1-Ship]-130)) && (y[Ship] <= (y[1-Ship]+130)))
        {
            if (0 == Ship)
            {
                Angle[Ship] += 32;
            } else {
                Angle[Ship] -= 32;
            }
        }
        else if ((Angle[Ship] != da) && ((x[Ship] < 200) || (x[Ship] > 440))
                                     && ((y[Ship] < 150) || (y[Ship] > 350))
            && ((x[Ship] > 24) && (x[Ship] < 583) && (y[Ship] > 24) && (y[Ship] < 453)))
        {
            if (((da      >= (Angle[Ship]-224)) && (da       <= Angle[Ship]))
             ||(((da-512) >= (Angle[Ship]-228)) && ((da-512) <= Angle[Ship])))
            {
                Angle[Ship] -= 32;
            } else {
                Angle[Ship] += 32;
            }
        }
        if (((Angle[Ship] < (da-32)) || (Angle[Ship] > (da+32)))
         && (x[Ship] > 49) && (x[Ship] < 591) && (y[Ship] > 49) && (y[Ship] < 463))
        {
            switch (rand()%20) {
                case 0: x[Ship]++; break;
                case 1: x[Ship]--; break;
                case 2: y[Ship]++; break;
                case 3: y[Ship]--; break;
                case 4: Angle[Ship] += 32; break;
                case 5: Angle[Ship] -= 32; break;
                default: { }
            }
        }
        while (Angle[Ship]<0)
        {
            Angle[Ship] += 512;
        }
        Angle[Ship] &= 511;
    }

    SetAPen(MyRPort_PTR[AScr],0);
    if (0 != sx1[Ship][AScr])
    {
        Move(MyRPort_PTR[AScr],sx1[Ship][AScr],sy1[Ship][AScr]);
        Draw(MyRPort_PTR[AScr],sx2[Ship][AScr],sy2[Ship][AScr]);
        sx1[Ship][AScr] = 0;
    }

    BltBitMapRastPort((struct BitMap*) &ImgBitMap4,Angle[Ship],(Ship+1)*32,MyRPort_PTR[AScr],x[Ship],y[Ship],32,32,192);

    if ((Angle[Ship] > (da-33)) && (Angle[Ship] < (da+33))
     && (x[Ship] > (x[1-Ship]-151)) && (x[Ship] < (x[1-Ship]+151))
     && (y[Ship] > (y[1-Ship]-151)) && (y[Ship] < (y[1-Ship]+151)))
    {
        sx1[Ship][AScr] = x[Ship]+16;
        sy1[Ship][AScr] = y[Ship]+16;
        FireFactor = ( abs( sx1[Ship][AScr]-x[1-Ship]+16 ) + abs( sy1[Ship][AScr]-y[1-Ship]+16 ) ) / 3;
        if (75 < FireFactor)
        {
            FireFactor = 75;
        }
        sx2[Ship][AScr] = sx1[Ship][AScr]+ FireFactor * mx[Angle[Ship] >> 5];
        sy2[Ship][AScr] = sy1[Ship][AScr]+ FireFactor * my[Angle[Ship] >> 5];
        if ((sx2[Ship][AScr]<1) || (sx2[Ship][AScr]>(HighRes_Width-1)) || (sy2[Ship][AScr]<1) || (sy2[Ship][AScr]>(HighRes_Height-1)))
        {
            sx1[Ship][AScr] = 0;
        }
    }

    if (Audio_enable)
    {
        SPAddrC = ZeroSound; SPLengthC = 1;
        SPAddrD = ZeroSound; SPLengthD = 1;
    }
}

// AScr
void DRAWFIRE()
{
    uint8   Ship;
    sint16  ShieldValue;

    // draw shield-bars at the bottom...
    RECTWIN(MyRPort_PTR[AScr],7,0,500,639,511);
    ShieldValue = SSHeader[0].ShieldS;
    if (ShieldValue > 319)
    {
        ShieldValue = 319;
    }
    RECTWIN(MyRPort_PTR[AScr],14,0,501,ShieldValue,510);
    ShieldValue = SSHeader[1].ShieldS;
    if (ShieldValue > 319)
    {
        ShieldValue = 319;
    }
    RECTWIN(MyRPort_PTR[AScr],15,319,501,319+ShieldValue,510);
    for(Ship = 0; Ship < 2; ++Ship)
    {
        if (0 < sx1[Ship][AScr])
        {
            if (Audio_enable)
            {
                if (0 == Ship)
                {
                    custom.dmacon = BITCLR | DMAF_AUD3;
                    SPAddrD = BSFSoundMemA[0]; SPFreqD = 350; SPLengthD = (UWORD) BSFSoundSize[0]; SPVolD = 50;
                    custom.dmacon = BITSET | DMAF_AUD3;
                } else {
                    custom.dmacon = BITCLR | DMAF_AUD2;
                    SPAddrC = BSFSoundMemA[1]; SPFreqC = 300; SPLengthC = (UWORD) BSFSoundSize[1]; SPVolC = 50;
                    custom.dmacon = BITSET | DMAF_AUD2;
                }
            }
            SetAPen(MyRPort_PTR[AScr],14+Ship);
            Move(MyRPort_PTR[AScr],sx1[Ship][AScr],sy1[Ship][AScr]);
            Draw(MyRPort_PTR[AScr],sx2[Ship][AScr],sy2[Ship][AScr]);
            if ((sx2[Ship][AScr] >= x[1-Ship]) && (sx2[Ship][AScr] < (x[1-Ship]+33))
             && (sy2[Ship][AScr] >= y[1-Ship]) && (sy2[Ship][AScr] < (y[1-Ship]+33)))
            {
                SSHeader[1-Ship].ShieldS -= SSHeader[Ship].WeaponS;
                if (Audio_enable)
                {
                    if (0 == Ship)
                    {
                        custom.dmacon = BITCLR | DMAF_AUD2;
                        SPAddrC = SoundMemA[1]; SPFreqC = 300; SPLengthC = SoundSize[1] / 2; SPVolC = 64;
                        custom.dmacon = BITSET | DMAF_AUD2;
                    } else {
                        custom.dmacon = BITCLR | DMAF_AUD3;
                        SPAddrD = SoundMemA[1]; SPFreqD = 350; SPLengthD = SoundSize[1] / 2; SPVolD = 64;
                        custom.dmacon = BITSET | DMAF_AUD3;
                    }
                }
            }
        }
    }
}

// AScr
void KILLSHIP(uint8 Ship, uint8 Phase)
{
    if (0 == Phase)
    {
        WaitTOF();
        PLAYSOUND(1,1100);
        BltBitMapRastPort((struct BitMap*) &ImgBitMap4,0,0,MyRPort_PTR[AScr],x[Ship],y[Ship],32,32,192);
        WaitTOF();
    } else if (16 == Phase)
    {
        RectFill(MyRPort_PTR[AScr],x[Ship]-3,y[Ship]-3,x[Ship]+36,y[Ship]+36);
    } else {
        RectFill(MyRPort_PTR[AScr],x[Ship]-3,y[Ship]-3,x[Ship]+36,y[Ship]+36);
        BltBitMapRastPort((struct BitMap*) &ImgBitMap4,Phase*32,0,MyRPort_PTR[AScr],x[Ship],y[Ship],32,32,192);
        if (Audio_enable)
        {
            SPAddrA   = ZeroSound; SPLengthA = 1;
            SPAddrB   = ZeroSound; SPLengthB = 1;
        }
    }
}

void XTRAROUND()
{
    uint16  StarX[STARS];
    uint16  StarY[STARS];
    uint8   NowDie;
    time_t  t;
    int     i, j;

    srand((unsigned) time(&t));
    Rotation = 0;
    for(i = 0; i < STARS; ++i)
    {
        StarX[i] = ((rand()%(639 / STARS))+1)*STARS;
        StarY[i] = ((rand()%(505 / STARS))+1)*STARS;
    }
    sx1[0][0] = 0;
    sx1[1][0] = 0;
    sx1[0][1] = 0;
    sx1[1][1] = 0;
    x[1] = 520; y[1] = 100; Angle[1] = (rand()%17)*32;
    x[0] = 120; y[0] = 412; Angle[0] = (rand()%17)*32;
    StepCtr = 1;
    SetRast(MyRPort_PTR[0], 0);
    SetRast(MyRPort_PTR[1], 0);
    if (Audio_enable)
    {
        custom.dmacon = BITCLR | DMAF_AUDIO;
        SPAddrA = BSFSoundMemA[2];                 SPFreqA = 450; SPLengthA = (UWORD) BSFSoundSize[2]; SPVolA = 40;
        SPAddrB = BSFSoundMemA[2]+BSFSoundSize[2]; SPFreqB = 450; SPLengthB = (UWORD) BSFSoundSize[2]; SPVolB = 40;
        custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1;
    }
    // lets fight .. until one of the shields is <0
    do
    {
        AScr = 1-AScr;
        SetAPen(MyRPort_PTR[AScr],12);
        for(i = 0; i < STARS; ++i)
        {
            WritePixel(MyRPort_PTR[AScr],StarX[i],StarY[i]);
        }
        SetAPen(MyRPort_PTR[AScr],0);
        STARFLY(0);
        STARFLY(1);
        DRAWFIRE();
        ScreenToFront(MyScreen[AScr]);
    }
    while ((0 <= SSHeader[0].ShieldS) && (0 <= SSHeader[1].ShieldS));

    if (0 > SSHeader[0].ShieldS) { NowDie = 0; }
    if (0 > SSHeader[1].ShieldS) { NowDie = 1; }
    for(i = 0; i < 2; ++i)
    {
        if (0 != sx1[NowDie][i])
        {
            SetAPen(MyRPort_PTR[i], 0);
            Move(MyRPort_PTR[i], sx1[NowDie][i], sy1[NowDie][i]);
            Draw(MyRPort_PTR[i], sx2[NowDie][i], sy2[NowDie][i]);
            sx1[NowDie][i] = 0;
        }
    }
    AScr = 1-AScr;
    SetAPen(MyRPort_PTR[AScr],0);
    if (0 >= SSHeader[1-NowDie].ShieldS)
    {
        SSHeader[1-NowDie].ShieldS = 1;
    }
    KILLSHIP(NowDie,0);
    STARFLY(1-NowDie);
    ScreenToFront(MyScreen[AScr]);
    for(i = 1; i < 17; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            AScr = 1-AScr;
            SetAPen(MyRPort_PTR[AScr],0);
            KILLSHIP(NowDie, i);
            STARFLY(1-NowDie);
            ScreenToFront(MyScreen[AScr]);
        }
    }
    for(j = 0; j < 35; ++j)
    {
        AScr = 1-AScr;
        SetAPen(MyRPort_PTR[AScr],12);
        for(i = 0; i < STARS; ++i)
        {
            WritePixel(MyRPort_PTR[AScr],StarX[i],StarY[i]);
        }
        SetAPen(MyRPort_PTR[AScr],0);
        STARFLY(1-NowDie);
        ScreenToFront(MyScreen[AScr]);
    }
    if (Audio_enable)
    {
        custom.dmacon = BITCLR | DMAF_AUDIO;
    }
}

uint8 SMALLSHIPFIGHT(r_ShipHeader* ShipPtr1, r_ShipHeader* ShipPtr2, r_ShipHeader* ShipPtr02, uint8 Mode, uint8 ActSys)
{
    volatile sint32 Shield1,Shield2;
    int     Fire1,Fire2;
    time_t  t;

    srand((unsigned) time(&t));
    Shield1 = ShipPtr1->Shield + ShipPtr1->ShieldBonus + ShipPtr1->Tactical*3;
    do
    {
        Shield2 = ShipPtr2->Shield + ShipPtr2->ShieldBonus + ShipPtr2->Tactical*3;
        Fire1 = it_round((double) (ShipPtr1->Weapon/10.0+1) * ShipData(ShipPtr1->SType).WeaponPower) - ShipPtr1->Tactical;
        Fire2 = it_round((double) (ShipPtr2->Weapon/10.0+1) * ShipData(ShipPtr2->SType).WeaponPower) - ShipPtr2->Tactical;
        do
        {
            if (rand()&1)
            {
                Shield1 -= Fire2;
            } else {
                Shield2 -= Fire1;
            }
        }
        while ((0 < Shield1) && (0 < Shield2));
        if (1 > Shield1)
        {
            if (0 < (Shield2 - ShipPtr2->ShieldBonus - ShipPtr2->Tactical*3))
            {
                ShipPtr2->Shield = Shield2 - ShipPtr2->ShieldBonus - ShipPtr2->Tactical*3;
            } else {
                ShipPtr2->Shield = 1;
            }
            if (ShipPtr1->SType > ShipPtr2->SType)
            {
                ShipPtr2->ShieldBonus += (ShipPtr1->SType - ShipPtr2->SType);
            } else {
                ++(ShipPtr2->ShieldBonus);
            }
            ShipPtr1->Owner = 0;
            return 1;

        } else {
            if (0 < (Shield1 - ShipPtr1->ShieldBonus - ShipPtr1->Tactical*3))
            {
                ShipPtr1->Shield = Shield1 - ShipPtr1->ShieldBonus - ShipPtr1->Tactical*3;
            } else {
                ShipPtr1->Shield = 1;
            }
            if (ShipPtr2->SType > ShipPtr1->SType)
            {
                ShipPtr1->ShieldBonus += (ShipPtr2->SType - ShipPtr1->SType);
            } else {
                ++(ShipPtr1->ShieldBonus);
            }
            ShipPtr1->PosX = MOVESHIP_FromX;
            ShipPtr1->PosY = MOVESHIP_FromY;

            ShipPtr2->Owner = 0;
            if ((MODE_ALL == Mode) || (MODE_FLEET == Mode))
            {
                if ((0 != (SystemFlags[ActPlayer-1][ActSys-1] & FLAG_KNOWN))
                        || (ShipPtr1->Owner == ActPlayerFlag)
                        || (ShipPtr2->Owner == ActPlayerFlag))
                {
                    if (MODE_ALL == Mode)
                    {
                        if (Visible) { PLAYSOUND(1,900); }
                        Delay(20);
                    } else {
                        ShipPtr2->PosX = ShipPtr02->PosX;
                        ShipPtr2->PosY = ShipPtr02->PosY;
                        if (Visible) { MOVESHIP_EXPLODE(ActSys,ShipPtr2); }
                    }
                }
                do
                {
                    ShipPtr2 = ShipPtr2->NextShip;
                }
                while ((NULL != ShipPtr2) && (0 == ShipPtr2->Owner));
            }
        }
    }
    while ((NULL != ShipPtr2) && (MODE_ONCE != Mode));
    return 2;
}

void BIGFIGHTEXIT(uint8 Mode, uint8 ActSys, uint8* BIGSHIPFIGHT)
{
    DoClock = false;
    CLOSEMYSCREENS();
    (void) OPENMAINSCREENS();
    (void) INITDESK(0);
    Screen2 = 0;
    if ((SSHeader[0].ShieldS>0) && (SSHeader[1].ShieldS>0))
    {
        *BIGSHIPFIGHT = SMALLSHIPFIGHT(ShipPtr1,ShipPtr2,NULL,Mode,ActSys);
    }
    DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    ScreenToFront(MyScreen[0]);
}

uint8 BIGSHIPFIGHT(r_ShipHeader* Ship1, r_ShipHeader* Ship2, uint8 Mode, uint8 ActSys)
{
    uint8   _BIGSHIPFIGHT = 2;
    struct NewScreen BSF_NeuScreen = {0,0,640,512,4,0,0,HIRES+LACE,CUSTOMSCREEN|SCREENQUIET, NULL,NULL,NULL,NULL};
    const struct worldcolors_t BSF_Colors[]= {
                                        {         0,         0,         0},{0xDDDDDDDD,0xDDDDDDDD,0xFFFFFFFF},
                                        {0xCCCCCCCC,0xCCCCCCCC,0xEEEEEEEE},{0xBBBBBBBB,0xBBBBBBBB,0xDDDDDDDD},
                                        {0xAAAAAAAA,0xAAAAAAAA,0xCCCCCCCC},{0x99999999,0x99999999,0xBBBBBBBB},
                                        {0x88888888,0x88888888,0xAAAAAAAA},{0xFFFFFFFF,         0,         0},
                                        {0x77777777,0x55555555,0x55555555},{0xFFFFFFFF,0xFFFFFFFF,         0},
                                        {0xFFFFFFFF,0x22222222,0xCCCCCCCC},{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
                                        {0x11111111,0x11111111,0xFFFFFFFF} };

    const struct worldcolors_t ShipColors[] = { {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF},
                                                {0x66666666,0x66666666,0xFFFFFFFF},
                                                {0xFFFFFFFF,         0,         0},
                                                {         0,0xFFFFFFFF,0x11111111},
                                                {0xFFFFFFFF,0xFFFFFFFF,         0},
                                                {0xBABABABA,0x8B8B8B8B,0x48484848},
                                                {0xFFFFFFFF,         0,0xB0B0B0B0},
                                                {0x77777777,0x77777777,0x77777777},
                                                {         0,0xFFFFFFFF,0xFFFFFFFF}};
    uint8   l;
    uint8   i, j;

    ShipPtr1 = Ship1;
    ShipPtr2 = Ship2;
    if (SHIPTYPE_FLEET == Ship2->SType)
    {
        ShipPtr2 = Ship2->TargetShip;
        Mode = MODE_FLEET;
    }

    while ((NULL != ShipPtr2) && (0 == ShipPtr2->Owner))
    {
        ShipPtr2 = ShipPtr2->NextShip;
    }
    if (NULL == ShipPtr2)
    {
        return _BIGSHIPFIGHT;
    }
    if ((0 != Save.CivPlayer[GETCIVVAR(ShipPtr1->Owner)-1])
     || (0 != Save.CivPlayer[GETCIVVAR(ShipPtr2->Owner)-1])
     || (Display == ActSys))
    {
        Visible = true;
    } else {
        Visible = false;
    }
    if (((ShipPtr1->Owner != ActPlayerFlag) && (ShipPtr2->Owner != ActPlayerFlag))
     || ((0 == Save.CivPlayer[GETCIVVAR(ShipPtr1->Owner)-1]) && (0 == Save.CivPlayer[GETCIVVAR(ShipPtr2->Owner)-1]))
     || (Save.SmallFight) || (SHIPTYPE_FLEET == Ship2->SType))
    {
        l = SMALLSHIPFIGHT(ShipPtr1,ShipPtr2,Ship2,Mode,ActSys);
        if ((1 == l) && (Visible))
        {
            MOVESHIP_EXPLODE(ActSys,ShipPtr1);
        } else if ((2 == l) && (MODE_FLEET != Mode) && (Visible))
        {
            MOVESHIP_EXPLODE(ActSys,ShipPtr2);
        }
        if (SHIPTYPE_FLEET == Ship2->SType)
        {
            ShipPtr2 = Ship2->TargetShip;
            do
            {
                if (0 == ShipPtr2->Owner)
                {
                    Ship2->TargetShip = ShipPtr2->NextShip;
                    if (NULL != ShipPtr2->NextShip)
                    {
                        ShipPtr2->NextShip->BeforeShip = Ship2;
                    }
                    LINKSHIP(ShipPtr2,Ship2,0);
                }
                ShipPtr2 = Ship2->TargetShip;
            }
            while ((NULL != ShipPtr2) && (0 == ShipPtr2->Owner));
            if (NULL == ShipPtr2)
            {
                Ship2->Owner = 0;
            } else if (NULL == ShipPtr2->NextShip)
            {
                Ship2->Owner = 0;
                Ship2->SType = 8;
                ShipPtr2->PosX = Ship2->PosX;
                ShipPtr2->PosY = Ship2->PosY;
                LINKSHIP(ShipPtr2,Ship2,0);
            }
            REFRESHDISPLAY();
        }
        return l;
    }
    SSHeader[0].ShieldS = 1;
    SSHeader[1].ShieldS = 1;
    CLOSEMYSCREENS();
    Display = 100;
    for(i = 0; i < 2; ++i)
    {
        MyScreen[i] = OpenScreenTagList(&BSF_NeuScreen, Tags);
        if (NULL != MyScreen[i])
        {
            MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
            MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
            for (j=0; j<13; ++j)
            {
                SetRGB32(MyVPort_PTR[i], j+1, BSF_Colors[j].r, BSF_Colors[j].g, BSF_Colors[j].b);
            }
        }
    }
    SWITCHDISPLAY();
    if ((NULL == MyScreen[0]) || (NULL == MyScreen[1]))
    {
        BIGFIGHTEXIT(Mode, ActSys, &_BIGSHIPFIGHT);
        return _BIGSHIPFIGHT;
    }
    AScr = 1;
//    do
//    {
    if (!INITIMAGES())
    {
        BIGFIGHTEXIT(Mode, ActSys, &_BIGSHIPFIGHT);
        return _BIGSHIPFIGHT;
    }
    do
    {
        SSHeader[0] = (r_SmallShipHeader) {(ShipPtr1->Shield + ShipPtr1->ShieldBonus + ShipPtr1->Tactical*3)*2,
                                            it_round((double)(ShipPtr1->Weapon/10.0+1.0)*ShipData(ShipPtr1->SType).WeaponPower)-ShipPtr1->Tactical};
        SSHeader[1] = (r_SmallShipHeader) {(ShipPtr2->Shield + ShipPtr2->ShieldBonus + ShipPtr2->Tactical*3)*2,
                                            it_round((double)(ShipPtr2->Weapon/10.0+1.0)*ShipData(ShipPtr2->SType).WeaponPower)-ShipPtr2->Tactical};
        for(i = 0; i < 2; ++i)
        {
            switch (ShipPtr1->Owner) {
                case FLAG_TERRA:  j = 1; break;
                case FLAG_KLEGAN: j = 2; break;
                case FLAG_REMALO: j = 3; break;
                case FLAG_CARDAC: j = 4; break;
                case FLAG_FERAGI: j = 5; break;
                case FLAG_BAROJA: j = 6; break;
                case FLAG_VOLKAN: j = 7; break;
                case FLAG_OTHER:  j = 8; break;
                default:          j = 0;
            }
            SetRGB32(MyVPort_PTR[i], 14, ShipColors[j].r, ShipColors[j].g, ShipColors[j].b);

            switch (ShipPtr2->Owner) {
                case FLAG_TERRA:  j = 1; break;
                case FLAG_KLEGAN: j = 2; break;
                case FLAG_REMALO: j = 3; break;
                case FLAG_CARDAC: j = 4; break;
                case FLAG_FERAGI: j = 5; break;
                case FLAG_BAROJA: j = 6; break;
                case FLAG_VOLKAN: j = 7; break;
                case FLAG_OTHER:  j = 8; break;
                default:          j = 0;
            }
            SetRGB32(MyVPort_PTR[i], 15, ShipColors[j].r, ShipColors[j].g, ShipColors[j].b);
        }
        XTRAROUND();
        if (SSHeader[0].ShieldS < 0)
        {
            _BIGSHIPFIGHT = 1;
            if (((SSHeader[1].ShieldS / 2) - ShipPtr2->ShieldBonus - ShipPtr2->Tactical*3) > 0)
            {
                ShipPtr2->Shield = (SSHeader[1].ShieldS / 2) - ShipPtr2->ShieldBonus - ShipPtr2->Tactical*3;
            } else {
                ShipPtr2->Shield = 1;
            }
            if (ShipPtr1->SType > ShipPtr2->SType)
            {
                ShipPtr2->ShieldBonus += (ShipPtr1->SType - ShipPtr2->SType);
            } else {
                ++(ShipPtr2->ShieldBonus);
            }
            ShipPtr1->Owner = 0;
        } else {
            _BIGSHIPFIGHT = 2;

            if (((SSHeader[0].ShieldS / 2) - ShipPtr1->ShieldBonus - ShipPtr1->Tactical*3) > 0)
            {
                ShipPtr1->Shield = (SSHeader[0].ShieldS / 2) - ShipPtr1->ShieldBonus - ShipPtr1->Tactical*3;
            } else {
                ShipPtr1->Shield = 1;
            }
            if (ShipPtr2->SType > ShipPtr1->SType)
            {
                ShipPtr1->ShieldBonus += (ShipPtr2->SType - ShipPtr1->SType);
            } else {
                ++(ShipPtr1->ShieldBonus);
            }
            ShipPtr1->PosX = MOVESHIP_FromX;
            ShipPtr1->PosY = MOVESHIP_FromY;

            ShipPtr2->Owner = 0;
            if (MODE_ALL == Mode)
            {
                do
                {
                    ShipPtr2 = ShipPtr2->NextShip;
                }
                while ((NULL != ShipPtr2) && (0 == ShipPtr2->Owner));
            }
        }
    }
    while ((NULL != ShipPtr2) && (MODE_ONCE != Mode) && (0 != ShipPtr1->Owner));

    BIGFIGHTEXIT(Mode, ActSys, &_BIGSHIPFIGHT);
    return _BIGSHIPFIGHT;
}

uint8 STARTBIGSHIPFIGHT(r_ShipHeader* Ship1, r_ShipHeader* Ship2, uint8 Mode, uint8 ActSys)
{
    r_ShipHeader*   MyShipPtr;
    uint8   Result;
    bool    SFight, b_tmp;

    SFight = true;
    if (SHIPTYPE_FLEET == Ship1->SType)
    {
        b_tmp = SFight;
        SFight = Save.SmallFight;
        Save.SmallFight = b_tmp;
        MyShipPtr = Ship1->TargetShip;
        do
        {
            MyShipPtr->PosX = Ship1->PosX;
            MyShipPtr->PosY = Ship1->PosY;
            Result = BIGSHIPFIGHT(MyShipPtr,Ship2,Mode,ActSys);
            if (0 == MyShipPtr->Owner)
            {
                Ship1->TargetShip = MyShipPtr->NextShip;
                if (NULL != MyShipPtr->NextShip)
                {
                    MyShipPtr->NextShip->BeforeShip = Ship1;
                }
                LINKSHIP(MyShipPtr,Ship1,0);
            }
            MyShipPtr = Ship1->TargetShip;
        }
        while ((NULL != MyShipPtr) && (2 != Result));
        if (NULL == MyShipPtr)
        {
            Ship1->Owner = 0;
        } else if (NULL == MyShipPtr->NextShip)
        {
            Ship1->Owner = 0;
            Ship1->SType = 8;
            LINKSHIP(MyShipPtr,Ship1,0);
        }
        b_tmp = SFight;
        SFight = Save.SmallFight;
        Save.SmallFight = b_tmp;
    } else {
        return BIGSHIPFIGHT(Ship1,Ship2,Mode,ActSys);
    }
    return Result;
}

