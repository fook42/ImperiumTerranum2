#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

//#define FactorSin  ( 0.03998933418663416089) // sin( 0.04);
//#define FactorCos  ( 0.99955003374898754309) // cos( 0.03);
//#define FactorMSin (-0.04997916927067833082) // sin(-0.05);
//#define FactorMCos ( 0.99980000666657775632) // cos(-0.02);

#define FactorSin  ( 0.03998933418663416) // sin( 0.04);
#define FactorCos  ( 0.99920010666097794) // cos( 0.04);
#define FactorMSin (-0.03998933418663416) // sin(-0.04);
#define FactorMCos ( 0.99920010666097794) // cos(-0.04);

#define intFactorSin  ((sint32) 41 )   // =~ 1024*sin( 0.04);
#define intFactorCos  ((sint32) 1023 ) // =~ 1024*cos( 0.04);
#define intFactorMSin ((sint32)-41)    // =~ 1024*sin(-0.04);
#define intFactorMCos ((sint32) 1023 ) // =~ 1024*cos(-0.04);

// will be global variables... waste of memory - maybe we can move them to the heap? ...
#define NUM_VECTOROBJ (13)
VectorObj_t*    VObj[NUM_VECTOROBJ];
extern uint8*   IntroMemA;
extern uint32   IntroMemL;
// ------------------------------

void SETDARKCOLOR(char* FName, r_Col_t* Colors)
{
    uint32  AddrX, AddrEnd, ISize;
    uint8   i;
    uint32* ColorID;
    BPTR    FHandle;

    FHandle = OPENSMOOTH(FName, MODE_OLDFILE);
    if (0 != FHandle)
    {
        (void)  Seek(FHandle, 0, OFFSET_END);
        ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
        (void) Read(FHandle, (APTR) IMemA[0], ISize);
        Close(FHandle);
        AddrX = (uint32) IMemA[0];
        AddrEnd = AddrX + ISize;
        do
        {
            ColorID = (uint32*) AddrX;
            AddrX += 4;
        }
        while ((AddrX < AddrEnd) && (_COLOR_CMAP_TEXT_ != *ColorID));

        if (_COLOR_CMAP_TEXT_ == *ColorID)
        {
            AddrX += 4; // skip "IMPT"-string
            i = 0;
            do
            {
                SetRGB32(MyVPort_PTR[0],i,0,0,0);
                SetRGB32(MyVPort_PTR[1],i,0,0,0);

                Colors[i] = *((r_Col_t*) AddrX);
                AddrX += sizeof(r_Col_t);
                ++i;
            }
            while (AddrX < AddrEnd);
        }
        Colors[31].r = 45;
        Colors[31].g = 45;
        Colors[31].b = 62;
    }
}

// ... fixfloat-round ----------

int ff_round(long int value)
{
    return (((value>>9)+1)>>1);
}

// - ROTATE- subroutines -------------------------------------
void ROTATEpX(VectorObj_t* actObject)
{
    int     i;
    sint32  store;
    for (i = 0; i<actObject->Size1; ++i)
    {
        store = actObject->Y1[i];
        actObject->Y1[i] = (store*intFactorCos - actObject->Z1[i]*intFactorSin)>>10;
        actObject->Z1[i] = (store*intFactorSin + actObject->Z1[i]*intFactorCos)>>10;
    }
    for (i = 0; i<actObject->Size2; ++i)
    {
        store = actObject->Y2[i];
        actObject->Y2[i] = (store*intFactorCos - actObject->Z2[i]*intFactorSin)>>10;
        actObject->Z2[i] = (store*intFactorSin + actObject->Z2[i]*intFactorCos)>>10;
    }
}

void ROTATEpY(VectorObj_t* actObject)
{
    int     i;
    sint32  store;
    for (i = 0; i<actObject->Size1; ++i)
    {
        store = actObject->X1[i];
        actObject->X1[i] = (store*intFactorCos - actObject->Z1[i]*intFactorSin)>>10;
        actObject->Z1[i] = (store*intFactorSin + actObject->Z1[i]*intFactorCos)>>10;
    }
    for (i = 0; i<actObject->Size2; ++i)
    {
        store = actObject->X2[i];
        actObject->X2[i] = (store*intFactorCos - actObject->Z2[i]*intFactorSin)>>10;
        actObject->Z2[i] = (store*intFactorSin + actObject->Z2[i]*intFactorCos)>>10;
    }
}

void ROTATEpZ(VectorObj_t* actObject)
{
    int     i;
    sint32  store;
    for (i = 0; i<actObject->Size1; ++i)
    {
        store = actObject->X1[i];
        actObject->X1[i] = (store*intFactorCos - actObject->Y1[i]*intFactorSin)>>10;
        actObject->Y1[i] = (store*intFactorSin + actObject->Y1[i]*intFactorCos)>>10;
    }
    for (i = 0; i<actObject->Size2; ++i)
    {
        store = actObject->X2[i];
        actObject->X2[i] = (store*intFactorCos - actObject->Y2[i]*intFactorSin)>>10;
        actObject->Y2[i] = (store*intFactorSin + actObject->Y2[i]*intFactorCos)>>10;
    }
}

void ROTATEnX(VectorObj_t* actObject)
{
    int     i;
    sint32  store;
    for (i = 0; i<actObject->Size1; ++i)
    {
        store = actObject->Y1[i];
        actObject->Y1[i] = (store*intFactorMCos - actObject->Z1[i]*intFactorMSin)>>10;
        actObject->Z1[i] = (store*intFactorMSin + actObject->Z1[i]*intFactorMCos)>>10;
    }
    for (i = 0; i<actObject->Size2; ++i)
    {
        store = actObject->Y2[i];
        actObject->Y2[i] = (store*intFactorMCos - actObject->Z2[i]*intFactorMSin)>>10;
        actObject->Z2[i] = (store*intFactorMSin + actObject->Z2[i]*intFactorMCos)>>10;
    }
}

void ROTATEnY(VectorObj_t* actObject)
{
    int     i;
    sint32  store;
    for (i = 0; i<actObject->Size1; ++i)
    {
        store = actObject->X1[i];
        actObject->X1[i] = (store*intFactorMCos - actObject->Z1[i]*intFactorMSin)>>10;
        actObject->Z1[i] = (store*intFactorMSin + actObject->Z1[i]*intFactorMCos)>>10;
    }
    for (i = 0; i<actObject->Size2; ++i)
    {
        store = actObject->X2[i];
        actObject->X2[i] = (store*intFactorMCos - actObject->Z2[i]*intFactorMSin)>>10;
        actObject->Z2[i] = (store*intFactorMSin + actObject->Z2[i]*intFactorMCos)>>10;
    }
}

void ROTATEnZ(VectorObj_t* actObject)
{
    int     i;
    sint32  store;
    for (i = 0; i<actObject->Size1; ++i)
    {
        store = actObject->X1[i];
        actObject->X1[i] = (store*intFactorMCos - actObject->Y1[i]*intFactorMSin)>>10;
        actObject->Y1[i] = (store*intFactorMSin + actObject->Y1[i]*intFactorMCos)>>10;
    }
    for (i = 0; i<actObject->Size2; ++i)
    {
        store = actObject->X2[i];
        actObject->X2[i] = (store*intFactorMCos - actObject->Y2[i]*intFactorMSin)>>10;
        actObject->Y2[i] = (store*intFactorMSin + actObject->Y2[i]*intFactorMCos)>>10;
    }
}
// -----------------------------------------------------------

void FLY(VectorObj_t* actObject, sint32 Factor)
{
    int     i;
    sint16  VPosX, VPosY;
    VPosX = actObject->PosX;
    VPosY = actObject->PosY;
    for (i = 0; i < actObject->Size1; ++i)
    {
        actObject->X1[i] += (actObject->X1[i] * Factor)>>15;
        actObject->Y1[i] += (actObject->Y1[i] * Factor)>>15;
        actObject->Z1[i] += (actObject->Z1[i] * Factor)>>15;
        if (((VPosX-ff_round(actObject->X1[i]))<0)  || ((VPosX-ff_round(actObject->X1[i]))>639)
         || ((VPosY-ff_round(actObject->Y1[i]))<75) || ((VPosY-ff_round(actObject->Y1[i]))>434))
        {
            actObject->Size1 = 0;
            return;
        }
    }
    for (i = 0; i<actObject->Size2; ++i)
    {
        actObject->X2[i] += (actObject->X2[i] * Factor)>>15;
        actObject->Y2[i] += (actObject->Y2[i] * Factor)>>15;
        actObject->Z2[i] += (actObject->Z2[i] * Factor)>>15;
        if (((VPosX-ff_round(actObject->X2[i]))<0)  || ((VPosX-ff_round(actObject->X2[i]))>639)
         || ((VPosY-ff_round(actObject->Y2[i]))<75) || ((VPosY-ff_round(actObject->Y2[i]))>434))
        {
            actObject->Size1 = 0;
            actObject->Size2 = 0;
            return;
        }
    }
}

void GREATEFFECT(uint8 Objects, r_Col_t* Colors, uint16** SMemA, uint32* SMemL)
{
    uint8  Ctr, actFlag;
    int i, j, k;
    sint32  Factor;
    sint16  VPosX, VPosY;
    VectorObj_t* actObject = NULL;
    struct RastPort* RPort_PTR;

    // fade in...
    WaitTOF();
    Factor = 0;
    for (Ctr = 0; Ctr < 50; ++Ctr)
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        Factor += 0x00000029; // 0010 1001 = 1/64 + 1/256 + 1/2048 =~~ 0,0200   >>11 !

        for (i = 1; i < 32; ++i)
        {
            SetRGB32(MyVPort_PTR[AScr],i,   ((Colors[i].r*Factor)<<13) & 0xFF000000,
                                            ((Colors[i].g*Factor)<<13) & 0xFF000000,
                                            ((Colors[i].b*Factor)<<13) & 0xFF000000);
        }
        if (10 == Ctr)
        {
            SPAddrC   = SMemA[1]+(SMemL[1]/2); SPVolC = 64; SPFreqC = 380; SPLengthC = (SMemL[1]/4);
            SPAddrD   = SMemA[1];              SPVolD = 64; SPFreqD = 380; SPLengthD = (SMemL[1]/4);

            custom.dmacon = BITSET | DMAF_AUD2 | DMAF_AUD3; // 0x800C
        } else if (11 == Ctr)
        {
            SPLengthC = 1;
            SPLengthD = 1;
        }
    }

    ScreenToFront(MyScreen[AScr]);
    SetRGB4(MyVPort_PTR[AScr],0,8,8,10);
    WaitTOF();
    WaitTOF();
    SetRGB4(MyVPort_PTR[AScr],0,0,0,0);
    for (i = 1; i < 32; ++i)
    {
        SetRGB32(MyVPort_PTR[AScr],i, Colors[i].r<<24,
                                      Colors[i].g<<24,
                                      Colors[i].b<<24);
    }
    Delay(50);
    // fade out the text below image (col 31) ...
    for (Ctr = 0; Ctr < 50; ++Ctr)
    {
        AScr = 1-AScr;
        Factor -= 0x00000029; // 0010 1001 = 1/64 + 1/256 + 1/2048 =~~ 0,0200
        SetRGB32(MyVPort_PTR[AScr],31,  ((Colors[31].r*Factor)<<13) & 0xFF000000,
                                        ((Colors[31].g*Factor)<<13) & 0xFF000000,
                                        ((Colors[31].b*Factor)<<13) & 0xFF000000);
        ScreenToFront(MyScreen[AScr]);
    }

    for (i = 0; i < Objects; ++i)
    {
        actObject = VObj[i];
        // calculate "center" of each object .. Sum(X)/num of points ... Sum(Y)/num of points
        for (j = 0; j < actObject->Size1; ++j)
        {
            actObject->PosX += actObject->X1[j];
            actObject->PosY += actObject->Y1[j];
        }
        actObject->PosX =     it_round(actObject->PosX / (double) actObject->Size1);
        actObject->PosY = 235+it_round(actObject->PosY / (double) actObject->Size1);

        // PosX, PosY = center of object
        for (j = 0; j < actObject->Size1; ++j)
        {
            actObject->X1[j] = (actObject->PosX+1)-actObject->X1[j];
            actObject->Y1[j] = (actObject->PosY+1)-actObject->Y1[j]-235;
            actObject->X1[j] = (actObject->X1[j])<<10;
            actObject->Y1[j] = (actObject->Y1[j])<<10;
            actObject->Z1[j] = (actObject->Z1[j])<<10;
        }
        for (j = 0; j < actObject->Size2; ++j)
        {
            actObject->X2[j] = (actObject->PosX+1)-actObject->X2[j];
            actObject->Y2[j] = (actObject->PosY+1)-actObject->Y2[j]-235;
            actObject->X2[j] = (actObject->X2[j])<<10;
            actObject->Y2[j] = (actObject->Y2[j])<<10;
            actObject->Z2[j] = (actObject->Z2[j])<<10;
        }
    }

    SPAddrC   = SMemA[2]+(SMemL[2]/2); SPVolC = 64; SPFreqC = 550; SPLengthC = SMemL[2]/4;
    SPAddrD   = SMemA[2];              SPVolD = 64; SPFreqD = 550; SPLengthD = SMemL[2]/4;

    custom.dmacon = BITSET | DMAF_AUD2 | DMAF_AUD3; // 0x800C
    Factor = 0x00000052; // 0101 0010 = 1/512 + 1/2048 + 1/16384 =~ 0,0025
    for (i = 0; (i < 29) && LMB_NOTPRESSED; ++i)
    {
        RPort_PTR = MyRPort_PTR[AScr];
        Factor += 0x00000083; // 1000 0011 = 1/256 + 1/16384 + 1/32768 =~ 0,004
        if (0 == i)
        {
            // reduce all colors ... multiply with 0,625 ...
            for (j = 2; j < 31; ++j)
            {
                SetRGB32(MyVPort_PTR[AScr], j, ((Colors[j].r<<2)+Colors[j].r)<<21,
                                               ((Colors[j].g<<2)+Colors[j].g)<<21,
                                               ((Colors[j].b<<2)+Colors[j].b)<<21);
            }
        } else {
            SetRast(RPort_PTR, 0);
        }

        if (15 < i) { VObj[rand()%13]->Size1 = 0; }

        SetAPen(RPort_PTR, 1);
        for (j = 0; j < Objects; ++j)
        {
            actObject = VObj[j];
            if (0 < actObject->Size1)
            {
                VPosX = actObject->PosX;
                VPosY = actObject->PosY;
                if (1 < j)
                {
                    SetAPen(RPort_PTR,0);
                    if (4 < actObject->Size2)
                    {
                        AreaMove(RPort_PTR,VPosX-ff_round(actObject->X2[3]),VPosY-ff_round(actObject->Y2[3]));
                        AreaDraw(RPort_PTR,VPosX-ff_round(actObject->X2[4]),VPosY-ff_round(actObject->Y2[4]));
                        AreaDraw(RPort_PTR,VPosX-ff_round(actObject->X2[5]),VPosY-ff_round(actObject->Y2[5]));
                    }
                    AreaMove(RPort_PTR,VPosX-ff_round(actObject->X1[3]),VPosY-ff_round(actObject->Y1[3]));
                    AreaDraw(RPort_PTR,VPosX-ff_round(actObject->X1[4]),VPosY-ff_round(actObject->Y1[4]));
                    AreaDraw(RPort_PTR,VPosX-ff_round(actObject->X1[5]),VPosY-ff_round(actObject->Y1[5]));

                    AreaEnd(RPort_PTR);
                    SetAPen(RPort_PTR,1);
                }
                AreaMove(    RPort_PTR,VPosX-ff_round(actObject->X1[0]),VPosY-ff_round(actObject->Y1[0]));
                for (k = 1; k<actObject->Size1; ++k)
                {
                    AreaDraw(RPort_PTR,VPosX-ff_round(actObject->X1[k]),VPosY-ff_round(actObject->Y1[k]));
                }
                AreaEnd(RPort_PTR);
                if (0 < actObject->Size2)
                {
                    AreaMove(    RPort_PTR,VPosX-ff_round(actObject->X2[0]),VPosY-ff_round(actObject->Y2[0]));
                    for (k = 1; k<actObject->Size2; ++k)
                    {
                        AreaDraw(RPort_PTR,VPosX-ff_round(actObject->X2[k]),VPosY-ff_round(actObject->Y2[k]));
                    }
                    AreaEnd(RPort_PTR);
                }
                actFlag = actObject->Flag;
                if (actFlag & ROTATE_PX) { ROTATEpX(actObject); }
                if (actFlag & ROTATE_PY) { ROTATEpY(actObject); }
                if (actFlag & ROTATE_PZ) { ROTATEpZ(actObject); }
                if (actFlag & ROTATE_NX) { ROTATEnX(actObject); }
                if (actFlag & ROTATE_NY) { ROTATEnY(actObject); }
                if (actFlag & ROTATE_NZ) { ROTATEnZ(actObject); }
                FLY(actObject, Factor);
            }
        }
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        if (0 == i)
        {
            SPLengthC = 1;
            SPLengthD = 1;
        }
    }
    for (i = 0; i<2; i++)
    {
        SetRast(MyRPort_PTR[AScr], 0);
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
    }
    custom.dmacon = BITCLR | DMAF_AUD2 | DMAF_AUD3; // 0x000C
}


int MAININTRO_PART2(uint16** SMemA, uint32* SMemL)
{
    char        s[40];
    char*       _s;
    r_Col_t     Colors[128];
    PLANEPTR    MyRastPtr = NULL;
    struct TmpRas       MyTmpRas;
    struct AreaInfo     MyAI;
    uint8       i;


    MyRastPtr = AllocRaster(640,360);
    if (NULL == MyRastPtr)
    {
        return -1;
    }
    InitTmpRas(&MyTmpRas, MyRastPtr, 21000);
    InitArea(&MyAI, (APTR) IMemA[0], 200);
    
    MyRPort_PTR[0]->TmpRas = &MyTmpRas;
    MyRPort_PTR[1]->TmpRas = &MyTmpRas;
    MyRPort_PTR[0]->AreaInfo = &MyAI;
    MyRPort_PTR[1]->AreaInfo = &MyAI;

/**** new .. alloc mem for vectorObj .. free this later ***/
    IntroMemL = sizeof(VectorObj_t)*NUM_VECTOROBJ;
    IntroMemA = (uint8*) AllocMem(IntroMemL, MEMF_ANY | MEMF_CLEAR);
    if (NULL == IntroMemA)
    {
        return -1;
    }
    for (i = 0; i<NUM_VECTOROBJ; ++i)
    {
        VObj[i] = (VectorObj_t*) (IntroMemA + i*sizeof(VectorObj_t));
    }
/**** new .. */

    _s = my_strcpy(s,PathStr[7]);
    /*****************************************************************************/
    /* TOUCHBYTE SOFTWARE PRESENTS */
    (void) my_strcpy(_s, "Frame1.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame1.img");
    if (!DISPLAYIMAGE(s,0,235,640,37,5,MyScreen[AScr],0)) { return -1; }
    WRITE(320,285,31,WRITE_Center,MyRPort_PTR[AScr],4,"PRESENTS");
    WaitTOF();
    ClipBlit(MyRPort_PTR[AScr],0,235,MyRPort_PTR[1-AScr],0,235,640,75,192);

    /* T */
    *VObj[0] = (VectorObj_t){0,0,ROTATE_PY|ROTATE_PX,
        6,4,0,{38, 9, 9, 9, 9,38},{ 2, 2,10,10, 2, 2},{1,1,1,3,3,3},
        {18,18,18,18, 0, 0},{11,36,36,11, 0, 0},{1,1,3,3,0,0}};
    /* U */
    *VObj[1] = (VectorObj_t){0,0,ROTATE_PX|ROTATE_PZ,
        6,6,0,{91,80,80,80,80,91},{1,1,26,26,1,1},{1,1,1,3,3,3},
        {111,100,100,100,100,111},{1,1,24,24,1,1},{1,1,1,3,3,3}};
    /* H */
    *VObj[2] = (VectorObj_t){0,0,ROTATE_PY|ROTATE_NZ,
        6,6,0,{163,152,152,152,152,163},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {184,173,173,173,173,184},{2,2,12,12,2,2},{1,1,1,3,3,3}};
    /* B */
    *VObj[3] = (VectorObj_t){0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{209,187,187,187,187,209},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {205,187,187,187,187,205},{15,15,36,36,15,15},{1,1,1,3,3,3}};
    /* Y */
    *VObj[4] = (VectorObj_t){0,0,ROTATE_PZ|ROTATE_NX,
        6,6,0,{229,217,229,229,217,229},{2,2,19,19,2,2},{1,1,1,3,3,3},
        {254,242,236,236,242,254},{2,2,12,12,2,2},{1,1,1,3,3,3}};
    /* T */
    *VObj[5] = (VectorObj_t){0,0,ROTATE_NX|ROTATE_NY,
        6,4,0,{285,256,256,256,256,285},{2,2,9,9,1,1},{1,1,1,3,3,3},
        {266,266,266,266,0,0},{11,36,36,11,0,0},{1,1,3,3,0,0}};
    /* E */
    *VObj[6] = (VectorObj_t){0,0,ROTATE_PX|ROTATE_PY,
        6,6,0,{318,289,289,289,289,318},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {315,289,289,289,289,315},{14,14,36,36,14,14},{1,1,1,3,3,3}};
    /* F */
    *VObj[7] = (VectorObj_t){0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{440,410,410,410,410,440},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {437,410,410,410,410,437},{14,14,36,36,14,14},{1,1,1,3,3,3}};
    /* T */
    *VObj[8] = (VectorObj_t){0,0,ROTATE_NZ|ROTATE_NY,
        6,4,0,{472,443,443,443,443,472},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {452,452,452,452,0,0},{11,36,36,11,0,0},{1,1,3,3,0,0}};
    /* W */
    *VObj[9] = (VectorObj_t){0,0,ROTATE_PX|ROTATE_PZ,
        6,6,0,{486,475,486,486,475,486},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {506,497,492,492,497,506},{2,2,16,16,2,2},{1,1,1,3,3,3}};
    /* A */
    *VObj[10] = (VectorObj_t){0,0,ROTATE_PX|ROTATE_PY,
        6,0,0,{547,536,521,521,536,547},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
    /* R */
    *VObj[11] = (VectorObj_t){0,0,ROTATE_PX|ROTATE_PZ,
        6,6,0,{589,565,565,565,565,589},{3,2,10,10,2,3},{1,1,1,3,3,3},
        {576,565,565,565,565,576},{16,16,36,36,16,16},{1,1,1,3,3,3}};
    /* E */
    *VObj[12] = (VectorObj_t){0,0,ROTATE_NX|ROTATE_NY,
        6,6,0,{630,601,601,601,601,630},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {627,601,601,601,601,627},{14,14,36,36,14,14},{1,1,1,3,3,3}};

    if (LMB_PRESSED) { return 1; }
    GREATEFFECT(13, Colors, SMemA, SMemL);
    if (LMB_PRESSED) { return 1; }

    /*****************************************************************************/
    /* A VIRTUAL WORLDS PRODUCTION */
    (void) my_strcpy(_s, "Frame2.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame2.img");
    if (!DISPLAYIMAGE(s,0,235,640,37,5,MyScreen[AScr],0)) { return -1; }
    WRITE(320,205,31,WRITE_Center,MyRPort_PTR[AScr],4,"A");
    WRITE(320,285,31,WRITE_Center,MyRPort_PTR[AScr],4,"PRODUCTION");
    WaitTOF();
    ClipBlit(MyRPort_PTR[AScr],0,200,MyRPort_PTR[1-AScr],0,200,640,100,192);

    /* V */
    *VObj[0] = (VectorObj_t) {0,0,ROTATE_PY|ROTATE_PX,
        6,6,0,{98,86,101,101,86,98},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {126,114,106,106,114,126},{2,2,23,23,2,2},{1,1,1,3,3,3}};
    /* I */
    *VObj[1] = (VectorObj_t) {0,0,ROTATE_PX|ROTATE_PZ,
        6,0,0,{140,129,129,129,129,140},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
    /* R */
    *VObj[2] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{167,143,143,143,143,167},{3,2,10,10,2,3},{1,1,1,3,3,3},
        {154,143,143,143,143,154},{16,16,36,36,16,16},{1,1,1,3,3,3}};
    /* T */
    *VObj[3] = (VectorObj_t) {0,0,ROTATE_PY|ROTATE_NZ,
        6,4,0,{206,177,177,177,177,206},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {186,186,186,186,0,0},{11,36,36,11,0,0},{1,1,3,3,0,0}};
    /* U */
    *VObj[4] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{220,209,209,209,209,220},{2,2,27,27,2,2},{1,1,1,3,3,3},
        {240,229,229,229,229,240},{2,2,25,25,2,2},{1,1,1,3,3,3}};
    /* A */
    *VObj[5] = (VectorObj_t) {0,0,ROTATE_PZ|ROTATE_NX,
        6,0,0,{266,255,240,240,255,266},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
    /* L */
    *VObj[6] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_NY,
        6,4,0,{295,284,284,284,284,295},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {311,296,296,311,0,0},{26,26,26,26,0,0},{1,1,3,3,0,0}};
    /* W */
    *VObj[7] = (VectorObj_t) {0,0,ROTATE_PX|ROTATE_PY,
        6,6,0,{335,324,335,335,324,335},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {355,346,341,341,346,355},{2,2,16,16,2,2},{1,1,1,3,3,3}};
    /* R */
    *VObj[8] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{441,417,417,417,417,441},{3,2,10,10,2,3},{1,1,1,3,3,3},
        {428,417,417,417,417,428},{16,16,36,36,16,16},{1,1,1,3,3,3}};
    /* L */
    *VObj[9] = (VectorObj_t) {0,0,ROTATE_NZ|ROTATE_NY,
        6,4,0,{464,453,453,453,453,464},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {480,465,465,480,0,0},{26,26,26,26,0,0},{1,1,3,3,0,0}};
    /* D */
    *VObj[10] = (VectorObj_t) {0,0,ROTATE_PX|ROTATE_PZ,
        6,0,0,{505,483,483,483,483,505},{3,2,36,36,2,3},{1,1,1,3,3,3},
        {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};

    if (LMB_PRESSED) { return 1; }
    GREATEFFECT(11, Colors, SMemA, SMemL);
    if (LMB_PRESSED) { return 1; }

    /*****************************************************************************/
    /* IMPERIUM TERRANUM */
    (void) my_strcpy(_s, "Frame3.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame3.img");
    if (!DISPLAYIMAGE(s,0,235,640,37,5,MyScreen[AScr],0)) { return -1; }
    WaitTOF();
    ClipBlit(MyRPort_PTR[AScr],0,235,MyRPort_PTR[1-AScr],0,235,640,37,192);

    /* I */
    *VObj[0] = (VectorObj_t) {0,0,ROTATE_PY|ROTATE_PX,
        6,6,0,{48,37,37,37,37,48},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {95,84,73,73,84,95},{2,2,18,18,2,2},{1,1,1,3,3,3}};
    /* M */
    *VObj[1] = (VectorObj_t) {0,0,ROTATE_PX|ROTATE_PZ,
        6,0,0,{62,51,51,51,51,62},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
    /* E */
    *VObj[2] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{161,132,132,132,132,161},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {158,132,132,132,132,158},{14,14,36,36,14,14},{1,1,1,3,3,3}};
    /* I */
    *VObj[3] = (VectorObj_t) {0,0,ROTATE_PY|ROTATE_NZ,
        6,0,0,{211,200,200,200,200,211},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
    /* U */
    *VObj[4] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{225,214,214,214,214,225},{2,2,27,27,2,2},{1,1,1,3,3,3},
        {245,234,234,234,234,245},{2,2,25,25,2,2},{1,1,1,3,3,3}};
    /* M */
    *VObj[5] = (VectorObj_t) {0,0,ROTATE_PZ|ROTATE_NX,
        6,6,0,{259,248,248,248,248,259},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {292,281,270,270,281,292},{2,2,18,18,2,2},{1,1,1,3,3,3}};
    /* T */
    *VObj[6] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_NY,
        6,4,0,{338,309,309,309,309,338},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {318,318,318,318,0,0},{21,36,36,11,0,0},{1,1,3,3,0,0}};
    /* E */
    *VObj[7] = (VectorObj_t) {0,0,ROTATE_PX|ROTATE_PY,
        6,6,0,{370,341,341,341,341,370},{2,2,10,10,2,2},{1,1,1,3,3,3},
        {367,341,341,341,341,367},{14,14,36,36,14,14},{1,1,1,3,3,3}};
    /* R */
    *VObj[8] = (VectorObj_t) {0,0,ROTATE_NX|ROTATE_PZ,
        6,6,0,{433,409,409,409,409,433},{3,2,10,10,2,3},{1,1,1,3,3,3},
        {420,409,409,409,409,420},{16,16,36,36,16,16},{1,1,1,3,3,3}};
    /* A */
    *VObj[9] = (VectorObj_t) {0,0,ROTATE_NZ|ROTATE_NY,
        6,0,0,{471,460,445,445,460,471},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
    /* N */
    *VObj[10] = (VectorObj_t) {0,0,ROTATE_PX|ROTATE_PZ,
        6,6,0,{500,489,489,489,489,500},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {521,510,510,510,510,521},{2,2,17,17,2,2},{1,1,1,3,3,3}};
    /* M */
    *VObj[11] = (VectorObj_t) {0,0,ROTATE_PX|ROTATE_PZ,
        6,6,0,{569,558,558,558,558,569},{2,2,36,36,2,2},{1,1,1,3,3,3},
        {602,591,580,580,591,602},{2,2,18,18,2,2},{1,1,1,3,3,3}};

    if (LMB_PRESSED) { return 1; }
    GREATEFFECT(12, Colors, SMemA, SMemL);
    if (LMB_PRESSED) { return 1; }

    /*****************************************************************************/
    // cleanup...
    FreeRaster(MyRastPtr,640,360);
    MyRastPtr = NULL;

    if (NULL != IntroMemA)
    {
        FreeMem((APTR) IntroMemA, IntroMemL);
    }

    if (NULL != IMemA[0])
    {
        FreeMem((APTR) IMemA[0], IMemL[0]);
        IMemA[0] = NULL;
    }
    for (i = 0; i<2; i++)
    {
        if (NULL != MyScreen[i])
        {
            CloseScreen(MyScreen[i]);
        }
        MyScreen[i] = OPENCINEMA(7);
        if (NULL == MyScreen[i])
        {
            return -1;
        }
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
    }

    return 0;
}
