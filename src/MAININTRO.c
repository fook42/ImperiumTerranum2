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
uint8*          IntroMemA = NULL;
uint32          IntroMemL = 0;
VectorObj_t*    VObj[NUM_VECTOROBJ];

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

void INTROEXIT(PLANEPTR MyRastPtr, struct MMD0 *module, uint16** SMemA, uint32* SMemL)
{
    uint8   i;

    SWITCHDISPLAY();
    (void) SetTaskPri(FindTask(NULL),0);
    if (NULL != MyRastPtr)
    {
        FreeRaster(MyRastPtr,640,360);
    }
    for (i = 0; i<2; ++i)
    {
        if (NULL != MyScreen[i])
        {
            CloseScreen(MyScreen[i]);
            MyScreen[i] = NULL;
            MyRPort_PTR[i] = NULL;
            MyVPort_PTR[i] = NULL;
        }
    }
    if (NULL != IntroMemA)
    {
        FreeMem((APTR) IntroMemA, IntroMemL);
    }
    for (i = 0; i<3; ++i)
    {
        if (NULL != SMemA[i])
        {
            FreeMem((APTR) SMemA[i], SMemL[i]);
            SMemA[i] = NULL;
        }
    }
    for (i = 0; i<2; ++i)
    {
        if (NULL != IMemA[i])
        {
            FreeMem((APTR) IMemA[i], IMemL[i]);
            IMemA[i] = NULL;
        }
    }
    if (NULL != module)
    {
        StopPlayer();
        FreePlayer();
        UnLoadModule(module);
    }
    custom.dmacon = BITCLR | DMAF_AUDIO; // 0x000F
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

bool LOADSOUNDS(char* FNamePath, char* FName, uint16** SMemA, uint32* SMemL)
{
    uint8   i;
    uint32  ssize;
    BPTR    FHandle;

    //    INITCHANNELS();
    (void) my_strcpy(FName, "Snd0.RAW");
    for (i = 0; i<3; ++i)
    {
        FName[3] = i+'1';
        FHandle = OPENSMOOTH(FNamePath,MODE_OLDFILE);
        if (0 != FHandle)
        {
            (void)  Seek(FHandle, 0, OFFSET_END);
            ssize = Seek(FHandle, 0, OFFSET_BEGINNING);
            SMemL[i] = ssize;
            SMemA[i] = (uint16*) AllocMem(ssize, MEMF_CHIP | MEMF_CLEAR);
            if (NULL == SMemA[i]) { return false; }
            (void) Read(FHandle, (APTR) SMemA[i], SMemL[i]);
            Close(FHandle);
        } else {
            return false;
        }
    }
    return true;
}

void MAININTRO()
{
    uint16*     SMemA[3];
    uint32      SMemL[3];
    char        s[40];
    char*       _s;
    BPTR        FHandle;
    r_Col_t     Colors[128];
    PLANEPTR    MyRastPtr = NULL;
    struct TmpRas       MyTmpRas;
    struct AreaInfo     MyAI;
    struct MMD0 *SndModulePtr = NULL;
    struct ITBitMap     IntroBitMap;
    struct RastPort*    actRastPort;

    const char* SArr[] = {"",
        "Software & Design",            "Oxygenic",
        "Art Director",                 "Cybertrace",
        "Music by",                     "Ludwig v.Beethoven   N.N. Ikonnikow   Richard Wagner",
        "Special Effects",              "Oxygenic",
        "Credits go to",                "Adam Benjamin   Rikard Cederlund",
        "Jakob Gaardsted   Andy Jones", "George Moore",
        "Surround-Sounds created with", "WaveTracer DS�",
        "Colors in Technicolor�",       "Panaflex� Camera and Lenses by Panavision�"};
    const uint8 FArr[] = {0, 3,4, 3,4, 3,4, 3,4, 3,4, 4,4, 3,4, 3,3};

    r_Coords_t* ShipX;
    r_Coords_t* ShipY;
    r_Coords_t* ShipZ;

    sint16      LEdge[2], TEdge[2];
    uint32      l, ISize;
    double      FacSin, FacCos;
    double      SizeFactor;
    double      Factor;
    uint16      dFactor;
    double      store;
    uint8       i, k;
    uint16      xpos;
    sint16      SXdata0int, SYdata0int;

    //    i = SetTaskPri(FindTask(NULL),120);
    AScr = 0;
    for (i = 0; i<2; ++i) { MyScreen[i] = NULL; IMemA[i] = NULL; MyRPort_PTR[i] = NULL; MyVPort_PTR[i] = NULL; }
    for (i = 0; i<3; ++i) { SMemA[i] = NULL; SMemL[i] = 0; }

    _s=my_strcpy(s,PathStr[7]);
    if (!LOADSOUNDS(s, _s, SMemA, SMemL))
    {
        goto leave_intro;
    }
    for (i = 0; i<2; i++)
    {
        MyScreen[i] = OPENCINEMA(5);
        if (NULL == MyScreen[i])
        {
            goto leave_intro;
        }
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
    }
    ScreenToFront(MyScreen[AScr]);

    if (!FillITBitMap(&IntroBitMap, 80, 183, 5))
    {
        goto leave_intro;
    }
    IMemL[0] = IntroBitMap.MemL;
    IMemA[0] = (uint8*) IntroBitMap.MemA;

    (void) my_strcpy(_s, "Frame0.pal");
    (void) SETCOLOR(MyScreen[0], s);
    (void) SETCOLOR(MyScreen[1], s);

    (void) my_strcpy(_s, "Frame0.img");   // Touchbyte ...
    if (!RAWLOADIMAGE(s,0,0,640,183,5,&IntroBitMap))
    {
        goto leave_intro;
    }
    xpos = 5;
    for (i = 0; i<8; ++i)
    {
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,0,0,MyRPort_PTR[AScr],640-xpos,340,xpos,90,192);
        xpos += 5;
        ScreenToFront(MyScreen[AScr]);
    }
    xpos = 590;
    do
    {
        xpos -= 5;
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap, 0,0,MyRPort_PTR[AScr],xpos   ,340,49,90,192);
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,41,0,MyRPort_PTR[AScr],xpos+49,340, 5,90,192);
        ScreenToFront(MyScreen[AScr]);
        if (LMB_PRESSED)
            { goto leave_intro; }
    }
    while (xpos > 10);
    ClipBlit(MyRPort_PTR[AScr],10,340,MyRPort_PTR[1-AScr],10,340,50,90,192);
    xpos = 5;
    for (i = 0; i<9; ++i)
    {
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,50,0,MyRPort_PTR[AScr],640-xpos,340,xpos,90,192);
        xpos += 5;
        ScreenToFront(MyScreen[AScr]);
    }
    SetAPen(MyRPort_PTR[  AScr],0);
    SetAPen(MyRPort_PTR[1-AScr],0);
    xpos = 595;
    do
    {
        xpos -= 5;
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,50,0,MyRPort_PTR[AScr],xpos,340,41,90,192);
        RectFill(MyRPort_PTR[AScr],xpos+41,340,xpos+50,430);
        ScreenToFront(MyScreen[AScr]);
        if (LMB_PRESSED) { goto leave_intro; }
    }
    while (xpos > 60);

    ClipBlit(MyRPort_PTR[AScr],10,340,MyRPort_PTR[1-AScr],10,340,150,90,192);
    Delay(5);
    for (i = 0; i<2; i++)
    {
        AScr = 1-AScr;
        BltBitMapRastPort((struct BitMap*) &IntroBitMap,0,0,MyRPort_PTR[AScr],10,340,616,91,192);
        ScreenToFront(MyScreen[AScr]);
    }
    if (LMB_PRESSED) { goto leave_intro; }
    Delay(15);
    AScr = 1-AScr;
    BltBitMapRastPort((struct BitMap*) &IntroBitMap,0,90,MyRPort_PTR[AScr],10,337,98,91,192);
    ScreenToFront(MyScreen[AScr]);
    custom.dmacon = BITCLR | DMAF_AUDIO; // 0x000F
    Delay(15);

    SPAddrA = SMemA[0];              SPVolA  = 0;  SPFreqA = 856; SPLengthA = SMemL[0]/4;
    SPAddrB = SMemA[0]+(SMemL[0]/2); SPVolB  = 0;  SPFreqB = 856; SPLengthB = SMemL[0]/4;
    SPAddrC = SMemA[1]+(SMemL[1]/2); SPVolC  = 64; SPFreqC = 380; SPLengthC = 1;
    SPAddrD = SMemA[1];              SPVolD  = 64; SPFreqD = 380; SPLengthD = 1;

    custom.dmacon = BITSET | DMAF_AUD0 | DMAF_AUD1; // 0x8003

    for (i = 1; i<=64; i++)
    {
        SPVolA = i;
        SPVolB = i;
        WaitTOF();
    }

    Delay(25);
    for (i = 0; i<2; i++)
    {
        AScr = 1-AScr;
        SetAPen(MyRPort_PTR[AScr],0);
        RectFill(MyRPort_PTR[AScr],0,335,640,430);
        ScreenToFront(MyScreen[AScr]);
    }

    MyRastPtr = AllocRaster(640,360);
    if (NULL == MyRastPtr)
    {
        goto leave_intro;
    }
    InitTmpRas(&MyTmpRas, MyRastPtr, 21000);
    InitArea(&MyAI, (APTR) IMemA[0], 200);
    MyScreen[0]->RastPort.TmpRas = &MyTmpRas;
    MyScreen[1]->RastPort.TmpRas = &MyTmpRas;
    MyScreen[0]->RastPort.AreaInfo = &MyAI;
    MyScreen[1]->RastPort.AreaInfo = &MyAI;

/**** new .. alloc mem for vectorObj .. free this later ***/
    IntroMemL = sizeof(VectorObj_t)*NUM_VECTOROBJ;
    IntroMemA = (uint8*) AllocMem(IntroMemL, MEMF_ANY | MEMF_CLEAR);
    if (NULL == IntroMemA)
    {
        goto leave_intro;
    }
    for (i = 0; i<NUM_VECTOROBJ; ++i)
    {
        VObj[i] = (VectorObj_t*) (IntroMemA + i*sizeof(VectorObj_t));
    }
/**** new .. */

    /*****************************************************************************/
    /* TOUCHBYTE SOFTWARE PRESENTS */
    (void) my_strcpy(_s, "Frame1.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame1.img");
    if (!DISPLAYIMAGE(s,0,235,640,37,5,MyScreen[AScr],0)) { goto leave_intro; }
    WRITE(320,285,31,WRITE_Center,MyRPort_PTR[AScr],4,"PRESENTS");
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

    if (LMB_PRESSED) { goto leave_intro; }
    GREATEFFECT(13, Colors, SMemA, SMemL);
    if (LMB_PRESSED) { goto leave_intro; }

    /*****************************************************************************/
    /* A VIRTUAL WORLDS PRODUCTION */
    (void) my_strcpy(_s, "Frame2.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame2.img");
    if (!DISPLAYIMAGE(s,0,235,640,37,5,MyScreen[AScr],0)) { goto leave_intro; }
    WRITE(320,205,31,WRITE_Center,MyRPort_PTR[AScr],4,"A");
    WRITE(320,285,31,WRITE_Center,MyRPort_PTR[AScr],4,"PRODUCTION");
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

    if (LMB_PRESSED) { goto leave_intro; }
    GREATEFFECT(11, Colors, SMemA, SMemL);
    if (LMB_PRESSED) { goto leave_intro; }

    /*****************************************************************************/
    /* IMPERIUM TERRANUM */
    (void) my_strcpy(_s, "Frame3.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame3.img");
    if (!DISPLAYIMAGE(s,0,235,640,37,5,MyScreen[AScr],0)) { goto leave_intro; }
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

    if (LMB_PRESSED) { goto leave_intro; }
    GREATEFFECT(12, Colors, SMemA, SMemL);
    if (LMB_PRESSED) { goto leave_intro; }

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
            goto leave_intro;
        }
        MyRPort_PTR[i] = &(MyScreen[i]->RastPort);
        MyVPort_PTR[i] = &(MyScreen[i]->ViewPort);
    }
    /*****************************************************************************/
    // prepare next effect...
    IMemL[0] = 201856;
    IMemL[1] = 21000;
    for (i = 0; i<2; ++i)
    {
        IMemA[i] = (uint8*) AllocMem(IMemL[i], MEMF_CHIP);
        if (NULL == IMemA[i])
        {
            goto leave_intro;
        }
    }

    SPAddrC = SMemA[1]+(SMemL[1]/2)+6500; SPVolC = 64; SPFreqC = 380; SPLengthC = (SMemL[1]-13000)/4;
    SPAddrD = SMemA[1]+6500;              SPVolD = 64; SPFreqD = 380; SPLengthD = (SMemL[1]-13000)/4;

    custom.dmacon = BITSET | DMAF_AUD2 | DMAF_AUD3; // 0x800C
    WaitTOF();

    SPLengthD = 1;
    SPLengthC = 1;

    i = 255;
    do
    {
        SetRGB32(MyVPort_PTR[1], 0, i<<24, i<<24, i<<24);
        i = i-5;
        WaitTOF();
    }
    while (i > 10);
    SetRGB32(MyVPort_PTR[1],0,0,0,0);
    custom.dmacon = BITCLR | DMAF_AUD2 | DMAF_AUD3; // 0x000C
    if (LMB_PRESSED)
        { goto leave_intro; }

    (void) my_strcpy(_s, "MOD.Intro");
    FHandle = OPENSMOOTH(s,MODE_OLDFILE);
    if (0 != FHandle)
    {
        Close(FHandle);
        StopPlayer();
        FreePlayer();
        SndModulePtr = LoadModule(s);   // module will be loaded directly from disk
        if (GETMIDIPLAYER(SndModulePtr))
        {
            PlayModule(SndModulePtr);
        }
    }

    (void) my_strcpy(_s, "Frame4.pal");
    SETDARKCOLOR(s, Colors);
    (void) my_strcpy(_s, "Frame4.img");       // terrain with stars...
    if (!DISPLAYIMAGE(s,0,75,640,360,7,MyScreen[1],0))
    {
        goto leave_intro;
    }
    ClipBlit(MyRPort_PTR[1],0,75,MyRPort_PTR[0],0,75,640,360,192);

    (void) my_strcpy(_s, "Frame5.img");       // little earth-image..

    FHandle = OPENSMOOTH(s,MODE_OLDFILE);
    if (0 != FHandle)
    {
        (void)  Seek(FHandle, 0, OFFSET_END);
        ISize = Seek(FHandle, 0, OFFSET_BEGINNING);
        (void) Read(FHandle,(APTR) (IMemA[0]+IMemL[0]-ISize-250), ISize);
        UNPACK(IMemA[0],IMemA[0]+IMemL[0]-ISize-250,20160,0);
        Close(FHandle);
    }

    struct BitMap MyBitMap = { 20, 144, 1, 7, 0, \
                            {(PLANEPTR) (IMemA[0]),      (PLANEPTR) (IMemA[0]+2880), (PLANEPTR) (IMemA[0]+5760), \
                             (PLANEPTR) (IMemA[0]+8640), (PLANEPTR) (IMemA[0]+11520),(PLANEPTR) (IMemA[0]+14400), \
                             (PLANEPTR) (IMemA[0]+17280), NULL }};

    // fade in....
    dFactor = 0;
    k = 20;
    do
    {
        ScreenToFront(MyScreen[AScr]);
        AScr=1-AScr;
        dFactor += 0xCD; // 11001101 =~ 0,05  >>12
        for (i = 1; i<128; ++i)
        {
            SetRGB32(MyVPort_PTR[AScr], i, (Colors[i].r*dFactor)<<12,
                                           (Colors[i].g*dFactor)<<12,
                                           (Colors[i].b*dFactor)<<12);
        }
        --k;
    }
    while (0 != k);

    // set all colours to the max. values
    for (i = 1; i<128; ++i)
    {
        SetRGB32(MyVPort_PTR[1-AScr], i, Colors[i].r<<24, Colors[i].g<<24, Colors[i].b<<24);
        SetRGB32(MyVPort_PTR[  AScr], i, Colors[i].r<<24, Colors[i].g<<24, Colors[i].b<<24);
    }
    ScrollRaster(MyRPort_PTR[AScr],0,-4,0,75,639,434);

    for (i = 0; i<6; ++i)
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ScrollRaster(actRastPort,0,-8,0,75,639,434);
        ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
        if (LMB_PRESSED)
            { goto leave_intro; }
    }

    for (i = 0; i<35; ++i)
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ScrollRaster(actRastPort,0,-8,0,75,639,434);
        ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
        BltBitMapRastPort(&MyBitMap,0,136-i*4,actRastPort,380,75,160,8,192);
        if (LMB_PRESSED)
            { goto leave_intro; }
    }

    ScreenToFront(MyScreen[AScr]);
    AScr = 1-AScr;
    actRastPort = MyRPort_PTR[AScr];
    ScrollRaster(actRastPort,0,-8,0,75,639,434);
    ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
    BltBitMapRastPort(&MyBitMap,0,0,actRastPort,380,79,160,4,192);

    for (i = 0; i<14; ++i)
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ScrollRaster(actRastPort,0,-8,0,75,639,434);
        ClipBlit(actRastPort,0,270,actRastPort,0,75,640,8,192);
        if (LMB_PRESSED)
            { goto leave_intro; }
    }

    Delay(10);
    if (LMB_PRESSED)
        { goto leave_intro; }

    SetRast(actRastPort, 0); // clear Rastport
    ScreenToFront(MyScreen[AScr]);
    AScr = 1-AScr;
    actRastPort = MyRPort_PTR[AScr];
    SetRast(actRastPort, 0); // clear Rastport

    (void) my_strcpy(_s, "Frame6.pal");
    (void) SETCOLOR(MyScreen[  AScr],s);
    (void) SETCOLOR(MyScreen[1-AScr],s);
    SetRGB4(MyVPort_PTR[AScr],127,9,9,11);   SetRGB4(MyVPort_PTR[1-AScr],127,9,9,11);
    SetRGB4(MyVPort_PTR[AScr],126,11,11,15); SetRGB4(MyVPort_PTR[1-AScr],126,11,11,15);
    SetRGB4(MyVPort_PTR[AScr],125,15,0,15);  SetRGB4(MyVPort_PTR[1-AScr],125,15,0,15);
    SetRGB4(MyVPort_PTR[AScr],124,13,0,0);   SetRGB4(MyVPort_PTR[1-AScr],124,15,2,2);
    SetRGB4(MyVPort_PTR[AScr],123,14,14,15); SetRGB4(MyVPort_PTR[1-AScr],123,14,14,15);

    (void) my_strcpy(_s, "Frame6.img");
    if (!DISPLAYIMAGE(s,0,75,640,360,7,MyScreen[AScr],0)) { goto leave_intro; }
    MyBitMap = (struct BitMap) { 80, 360, 1, 7, 0, \
                            {(PLANEPTR) (IMemA[0]),       (PLANEPTR) (IMemA[0]+28800), (PLANEPTR) (IMemA[0]+57600), \
                             (PLANEPTR) (IMemA[0]+86400), (PLANEPTR) (IMemA[0]+115200),(PLANEPTR) (IMemA[0]+144000), \
                             (PLANEPTR) (IMemA[0]+172800)}};
    BltBitMapRastPort(&MyBitMap,0,0,MyRPort_PTR[1-AScr],0,75,640,360,192);

    MyRastPtr = AllocRaster(640,360);
    if (NULL == MyRastPtr)
    {
        goto leave_intro;
    }
    InitTmpRas(&MyTmpRas,MyRastPtr,21000);
    InitArea(&MyAI, (APTR) IMemA[1], 200);
    MyScreen[0]->RastPort.TmpRas = &MyTmpRas;
    MyScreen[1]->RastPort.TmpRas = &MyTmpRas;
    MyScreen[0]->RastPort.AreaInfo = &MyAI;
    MyScreen[1]->RastPort.AreaInfo = &MyAI;

/**** new .. alloc mem for r_Coords .. free this later ***/
    IntroMemL = sizeof(r_Coords_t)*3;
    IntroMemA = (uint8*) AllocMem(IntroMemL, MEMF_ANY | MEMF_CLEAR);
    if (NULL == IntroMemA)
    {
        goto leave_intro;
    }
    ShipX = (r_Coords_t*) (IntroMemA);
    ShipY = (r_Coords_t*) (IntroMemA+sizeof(r_Coords_t));
    ShipZ = (r_Coords_t*) (IntroMemA+2*sizeof(r_Coords_t));

    *ShipX = (r_Coords_t) {{ 390, 264,252,186,186,192,256,290,354,360,360,294,282, 252, 256, 290,294, 282,294,294, 258,265,273,281,288, 185,185,190,190, 356,356,361,361,0,0,0,0,0,0,0,0}};
    *ShipY = (r_Coords_t) {{ 315, 174,275,250,188,225,234,234,225,188,250,275,174, 275, 243, 243,275, 174,275,275, 269,342,269,342,269, 187,172,172,187, 187,172,172,187,0,0,0,0,0,0,0,0}};
    *ShipZ = (r_Coords_t) {{-2.5,  -5, -1, -5, -5, -5, -1, -1, -5, -5, -5, -1, -5,  -1,-1.1,-1.1, -1,  -5, -8, -1,  -2, -2, -2, -2, -2,  -5, -5, -5, -5,  -5, -5, -5, -5,0,0,0,0,0,0,0,0}};
/**** new .. */

    for (i = 1; i<41; ++i)
    {
        ShipX->data[i] = 273-ShipX->data[i];
        ShipY->data[i] = 257-ShipY->data[i];
    }

    // rotate around x-axis
    FacSin = sin(1.35);
    FacCos = cos(1.35);
    for (i = 1; i<41; ++i)
    {
        store = ShipY->data[i];
        ShipY->data[i] = store*FacCos - ShipZ->data[i]*FacSin;
        ShipZ->data[i] = store*FacSin + ShipZ->data[i]*FacCos;
    }

    // rotate around y-axis
    FacSin = sin(-0.44);
    FacCos = cos(-0.44);
    for (i = 1; i<41; ++i)
    {
        store = ShipX->data[i];
        ShipX->data[i] = store*FacCos - ShipZ->data[i]*FacSin;
        ShipZ->data[i] = store*FacSin + ShipZ->data[i]*FacCos;
    }

    // rotate around z-axis
    FacSin = sin(-0.08);
    FacCos = cos(-0.08);
    for (i = 1; i<41; ++i)
    {
        store = ShipX->data[i];
        ShipX->data[i] = store*FacCos - ShipY->data[i]*FacSin;
        ShipY->data[i] = store*FacSin + ShipY->data[i]*FacCos;
    }
    if (LMB_PRESSED)
        { goto leave_intro; }

    Factor     = 0.0074;
    SizeFactor = 0.009;
    LEdge[0] = 0;
    TEdge[0] = 75;
    LEdge[1] = 0;
    TEdge[1] = 75;
    l = 1;
    ISize = 0;
    do
    {
        ScreenToFront(MyScreen[AScr]);
        AScr = 1-AScr;
        actRastPort = MyRPort_PTR[AScr];
        ShipX->data[0] = ShipX->data[0]-Factor-Factor-0.22;
        ShipY->data[0] = ShipY->data[0]-Factor;
        SXdata0int = it_round(ShipX->data[0]);
        SYdata0int = it_round(ShipY->data[0]);

        BltBitMapRastPort(&MyBitMap,LEdge[AScr],TEdge[AScr]-75,actRastPort,LEdge[AScr],TEdge[AScr],160,80,192);
        BltBitMapRastPort(&MyBitMap,20,145,actRastPort,20,220,600,55,192);

        LEdge[AScr] = SXdata0int-90;
        if (LEdge[AScr]<0) { LEdge[AScr] = 0; }

        TEdge[AScr] = SYdata0int-60;
        if (TEdge[AScr]<75) { TEdge[AScr] = 75; }

        store = ShipX->data[0];
        for (i = 1; i<41; ++i)
        {
            if (store < (ShipX->data[i]*SizeFactor))
            {
                FacSin = 1+ ((abs(store-ShipX->data[i]*SizeFactor))/store);
                ShipX->data[i] = store/SizeFactor;
                ShipY->data[i] = ShipY->data[i]/FacSin;
            }
        }

        SetAPen(actRastPort,124);
        /*** Antrieb ***/
        AreaMove(actRastPort,SXdata0int-it_round(ShipX->data[20]*SizeFactor),SYdata0int-it_round(ShipY->data[20]*SizeFactor));
        for (k = 21; k<25; ++k) { AreaDraw(actRastPort,SXdata0int-it_round(ShipX->data[k]*SizeFactor),SYdata0int-it_round(ShipY->data[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        SetAPen(actRastPort,126);
        /*** BodenPlatte hell ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX->data[13]*SizeFactor),SYdata0int -it_round(ShipY->data[13]*SizeFactor));
        for (k = 14; k<17; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX->data[k]*SizeFactor),SYdata0int -it_round(ShipY->data[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        /*** Seitenwand ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX->data[17]*SizeFactor),SYdata0int -it_round(ShipY->data[17]*SizeFactor));
        for (k = 18; k<20; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX->data[k]*SizeFactor),SYdata0int -it_round(ShipY->data[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        SetAPen(actRastPort,127);
        /*** BodenPlatte ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX->data[1]*SizeFactor),SYdata0int -it_round(ShipY->data[1]*SizeFactor));
        for (k = 2; k<13; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX->data[k]*SizeFactor),SYdata0int -it_round(ShipY->data[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        SetAPen(actRastPort,125);
        /*** Waffen ***/
        AreaMove(actRastPort,SXdata0int -it_round(ShipX->data[25]*SizeFactor),SYdata0int -it_round(ShipY->data[25]*SizeFactor));
        for (k = 26; k<29; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX->data[k]*SizeFactor),SYdata0int -it_round(ShipY->data[k]*SizeFactor)); }
        AreaEnd(actRastPort);
        AreaMove(actRastPort,SXdata0int -it_round(ShipX->data[29]*SizeFactor),SYdata0int -it_round(ShipY->data[29]*SizeFactor));
        for (k = 30; k<33; ++k) { AreaDraw(actRastPort,SXdata0int -it_round(ShipX->data[k]*SizeFactor),SYdata0int -it_round(ShipY->data[k]*SizeFactor)); }
        AreaEnd(actRastPort);

        SetAPen(actRastPort,0);
        RectFill(actRastPort,0,75,0,200);
        if (1 < ISize)
        {
            WRITE(320,220,123,WRITE_Center,actRastPort,FArr[l  ],SArr[l  ]);
            WRITE(320,245,123,WRITE_Center,actRastPort,FArr[l+1],SArr[l+1]);
        }
        ++ISize;
        if (55 < ISize)
        {
            l += 2;
            ISize = 0;
        }
        Factor     *= 1.018;
        SizeFactor *= 1.0137;
    }
    while ((Factor<3.2) && (SizeFactor<3.2) && LMB_NOTPRESSED);
    if (LMB_PRESSED)
        { goto leave_intro; }

    if (16 > l)
    {
        do
        {
            ScreenToFront(MyScreen[AScr]);
            AScr = 1-AScr;
            BltBitMapRastPort(&MyBitMap,0,0,MyRPort_PTR[AScr],0,75,640,360,192);
            if (0 < ISize)
            {
                WRITE(320,220,123,WRITE_Center,MyRPort_PTR[AScr],FArr[l  ],SArr[l  ]);
                WRITE(320,245,123,WRITE_Center,MyRPort_PTR[AScr],FArr[l+1],SArr[l+1]);
            }
            ++ISize;
            if (20 < ISize)
            {
                l += 2;
                ISize = 0;
            }
        }
        while ((17 > l) && LMB_NOTPRESSED);
    }

leave_intro:
    INTROEXIT(MyRastPtr, SndModulePtr, SMemA, SMemL);
}
