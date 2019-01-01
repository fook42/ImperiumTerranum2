#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void SHIPINFO_WRITEDATA(r_ShipHeader* MyShipPtr)
{
    sint16  w1,w2,Col1,Col2;
    uint8   c1,c2,i;
    double  Step;
    sint16  TactStep;
    char    s[5];
    char*   _s;
    char    Txt_999[] = {"999"};

    TactStep = it_round((MyShipPtr->Tactical * 300) / MyShipPtr->Shield);
    RECTWIN(MyRPort_PTR[1],0,386,296,513,310);
    WRITEWIN(443-TactStep,297,1,1,MyRPort_PTR[1],2,"#");

    Step = (MyShipPtr->Shield / 3);
    if (0 == it_round(Step)) { Step = 1.0f; }
    Col1 = it_round( (MyShipPtr->Tactical+Step)*(255/Step));
    Col2 = it_round(-(MyShipPtr->Tactical-Step)*(255/Step));
    for(i = 0; i < 8; i++)
    {
        if (0 > Col1)      { c1 = 0; }
        else if (255<Col1) { c1 = 255; }
        else               { c1 = Col1; }
        SetRGB32(MyVPort_PTR[1], i+6, c1<<24, 0, 0);
        Col1 -= 30;

        if (0 > Col2)      { c2 = 0; }
        else if (255<Col2) { c2 = 255; }
        else               { c2 = Col2; }
        SetRGB32(MyVPort_PTR[1],i+20, c2<<24, 0, c2<<24);
        Col2 -= 30;
    }
    (void) dez2out(MyShipPtr->Shield + MyShipPtr->Tactical*3, 3, s);
    WRITEWIN(275,296,5,1,MyRPort_PTR[1],2,s);
    (void) dez2out(it_round((MyShipPtr->Shield+MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, s);
    WRITEWIN(324,296,5,1,MyRPort_PTR[1],2,s);

    w1 = it_round((MyShipPtr->Weapon/10.0+1)*(ShipData(MyShipPtr->SType).WeaponPower-MyShipPtr->Tactical));
    w2 = it_round((MyShipPtr->Weapon/10.0+1)* ShipData(MyShipPtr->SType).WeaponPower);
    (void) dez2out(w1, 3, s);
    WRITEWIN(522,296,5,1,MyRPort_PTR[1],2,s);
    (void) dez2out(it_round((double)w1/w2*100.0), 3, s);
    WRITEWIN(571,296,5,1,MyRPort_PTR[1],2,s);

    Step = 111.0 / ShipData(MyShipPtr->SType).MaxMove;
    RECTWIN(MyRPort_PTR[1],0,386,427,513,442);
    WRITEWIN(498-it_round(Step*MyShipPtr->Repair),429,1,1,MyRPort_PTR[1],2,"#");

    if (0 == MyShipPtr->Repair)
    {
        _s = Txt_999;
    } else if (MyShipPtr->Shield == ShipData(MyShipPtr->SType).MaxShield)
    {
        _s = _Txt_Separator;
    } else {
        (void) dez2out(((ShipData(MyShipPtr->SType).MaxShield-MyShipPtr->Shield) / MyShipPtr->Repair), 3, s);
        _s=(char*) &s;
    }
    WRITEWIN(337,428,5,1,MyRPort_PTR[1],2, _s);

    (void) dez2out((ShipData(MyShipPtr->SType).MaxMove - MyShipPtr->Repair), 3, s);
    WRITEWIN(522,428,5,1,MyRPort_PTR[1],2,s);
}

void SHIPINFO(uint8 ActSys)
{
    char    s[50];
    char*   _s;
    char    Txt_Separate[]={":  "};
    const int _Txt_Separate_len = (sizeof(Txt_Separate)/sizeof(Txt_Separate[0]))-1;
    uint8   l,l1,l2;
    r_ShipHeader*   MyShipPtr;
    APTR    ModC = NULL;
    uint32  ModL;

    MyShipPtr = ObjPtr;
    if (( 8 > MyShipPtr->SType)
     || (24 < MyShipPtr->SType)
     || (ActPlayerFlag != MyShipPtr->Owner))
    {
        return;
    }
    ModC = GETTHESOUND(3);
    ModL = ModMemL[3];

    INITSCREEN(SCREEN_TECH);
    l1=strlen(PathStr[5]);
    memcpy(s, PathStr[5], l1);
    l2=strlen(Project.data[MyShipPtr->SType]);
    memcpy(s+l1, Project.data[MyShipPtr->SType], l2);
    strcpy(s+l1+l2, "Tech.img");
    if (!DISPLAYIMAGE(s,6,7,256,498,5,MyScreen[1],0)) { }

    WRITEWIN(290,15,1,0,MyRPort_PTR[1],4,Project.data[MyShipPtr->SType]);
    s[0]='-'; s[1]=' ';
    l=strlen(PText[190]);
    memcpy(s+2, PText[190], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    switch (MyShipPtr->Weapon) {
        case WEAPON_GUN:       strcpy(s+l, PText[185]); break;
        case WEAPON_LASER:     strcpy(s+l, PText[186]); break;
        case WEAPON_PHASER:    strcpy(s+l, PText[187]); break;
        case WEAPON_DISRUPTOR: strcpy(s+l, PText[188]); break;
        case WEAPON_PTORPEDO:  strcpy(s+l, PText[189]); break;
        default: (void) dez2out(MyShipPtr->Weapon, 0, s+l);
    }  
    WRITEWIN(290,42,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[191]);
    memcpy(s+2, PText[191], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    (void)dez2out(it_round((double)ShipData(MyShipPtr->SType).WeaponPower*(MyShipPtr->Weapon/10+1)), 0, s+l);
    WRITEWIN(290,62,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[192]);
    memcpy(s+2, PText[192], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    (void)dez2out(ShipData(MyShipPtr->SType).MaxShield, 0, s+l);
    WRITEWIN(290,82,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[193]);
    memcpy(s+2, PText[193], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    (void)dez2out(MyShipPtr->ShieldBonus, 0, s+l);
    WRITEWIN(290,102,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[194]);
    memcpy(s+2, PText[194], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    _s = float2out( ((double) MyShipPtr->Shield / ShipData(MyShipPtr->SType).MaxShield*100.0), 0, 2, s+l);
    *_s++=' ';
    *_s++='%';
    *_s  =0;
    WRITEWIN(290,122,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[195]);
    memcpy(s+2, PText[195], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    (void)dez2out(ShipData(MyShipPtr->SType).MaxMove, 0, s+l);
    WRITEWIN(290,142,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[196]);
    memcpy(s+2, PText[196], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    (void)dez2out( ((MyShipPtr->Ladung & MASK_SIEDLER) / 16), 0, s+l);
    WRITEWIN(290,162,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[197]);
    memcpy(s+2, PText[197], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    (void)dez2out( (MyShipPtr->Ladung & MASK_LTRUPPS), 0, s+l);
    WRITEWIN(290,182,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[198]);
    memcpy(s+2, PText[198], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    _s = dez2out(it_round((double) MyShipPtr->Fracht / ShipData(MyShipPtr->SType).MaxLoad*100.0), 0, s+l);
    strcpy(_s, "% belegt");
    WRITEWIN(290,202,1,0,MyRPort_PTR[1],3,s);

    l=strlen(PText[199]);
    memcpy(s+2, PText[199], l);
    memcpy(s+2+l, Txt_Separate, _Txt_Separate_len);
    l+=2+_Txt_Separate_len;
    if (MyShipPtr->Age<200)
    {
        (void)dez2out(Year-(MyShipPtr->Age), 0, s+l);
    } else {
        s[l++]=' ';
        strcpy(s+l, PText[200]);
    }
    WRITEWIN(290,222,1,0,MyRPort_PTR[1],3,s);

    SHIPINFO_WRITEDATA(MyShipPtr);
    ScreenToFront(MyScreen[1]);
    do
    {
        delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(1,300);
            if ((MouseY(1)>=455) && (MouseY(1)<=480))
            {
                if ((MouseX(1)>=380) && (MouseX(1)<=410)
                        && (MyShipPtr->Repair < ShipData(MyShipPtr->SType).MaxMove))
                {
                    MyShipPtr->Repair++;
                }
                if ((MouseX(1)>=490) && (MouseX(1)<=525)
                        && (MyShipPtr->Repair > 0))
                {
                    MyShipPtr->Repair--;
                }
                SHIPINFO_WRITEDATA(MyShipPtr);
            }
            if ((MouseY(1)>=315) && (MouseY(1)<=345))
            {
                if ((MouseX(1)>=380) && (MouseX(1)<=410)
                        && (MyShipPtr->Tactical < (ShipData(MyShipPtr->SType).WeaponPower-2))
                        && ((3*MyShipPtr->Tactical) < (MyShipPtr->Shield-2)))
                {
                    MyShipPtr->Tactical++;
                }
                if ((MouseX(1)>=490) && (MouseX(1)<=525)
                        && ((-3*MyShipPtr->Tactical) < (MyShipPtr->Shield-2))
                        && ((-MyShipPtr->Tactical) < (ShipData(MyShipPtr->SType).WeaponPower-2)))
                {
                    MyShipPtr->Tactical--;
                }
                SHIPINFO_WRITEDATA(MyShipPtr);
            }
        }
    }
    while (RMB_NOTPRESSED);

    if (NULL != ModC)
    {
        StopPlayer();
        FreeMem(ModC,ModL);
    }
    PLAYSOUND(1,300);
    ScreenToFront(MyScreen[0]);
}

