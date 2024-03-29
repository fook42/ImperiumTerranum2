#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

char* SHIPINFO_MAKETEXT(char* s, char* text_PTR)
{
    s=my_strcpy(s, text_PTR);
    *s++=':';
    *s++=' ';
    *s++=' ';
    return s;
}

void SHIPINFO_WRITEDATA(r_ShipHeader* MyShipPtr)
{
    sint16  w1,w2,Col1,Col2,ColX;
    uint8   c1,c2,i;
    double  Step;
    sint16  TactStep;
    char    s[5];
    char*   _s;
    char    Txt_999[] = {"999"};

    TactStep = it_round((MyShipPtr->Tactical * 300) / MyShipPtr->Shield);
    RECT_RP1_C0(386,296,513,310);
    WRITE_RP1(443-TactStep,297,1,1,1,"#");

    Step = (MyShipPtr->Shield / 3);
    if (2 > MyShipPtr->Shield) { Step = 1.0f; }
    ColX = it_round(255*(MyShipPtr->Tactical/Step));
    Col1 = 255+ColX;
    Col2 = 255-ColX;
    for(i = 0; i < 8; ++i)
    {
        if (0 > Col1)      { c1 = 0; }
        else if (255<Col1) { c1 = 255; }
        else               { c1 = Col1; }
        SetRGB32(MyVPort_PTR[1], i+6, c1*(ULONG)0x01010101, 0, 0);
        Col1 -= 30;

        if (0 > Col2)      { c2 = 0; }
        else if (255<Col2) { c2 = 255; }
        else               { c2 = Col2; }
        SetRGB32(MyVPort_PTR[1],i+20, c2*(ULONG)0x01010101, 0, c2*(ULONG)0x01010101);
        Col2 -= 30;
    }
    (void) dez2out(MyShipPtr->Shield + MyShipPtr->Tactical*3, 3, s);
    WRITE_RP1(275,296,5,1,1,s);
    (void) dez2out(it_round((MyShipPtr->Shield+MyShipPtr->Tactical*3.0)/ShipData(MyShipPtr->SType).MaxShield*100.0), 3, s);
    WRITE_RP1(324,296,5,1,1,s);

    w1 = it_round((MyShipPtr->Weapon/10.0+1)*(ShipData(MyShipPtr->SType).WeaponPower-MyShipPtr->Tactical));
    w2 = it_round((MyShipPtr->Weapon/10.0+1)* ShipData(MyShipPtr->SType).WeaponPower);
    (void) dez2out(w1, 3, s);
    WRITE_RP1(522,296,5,1,1,s);
    (void) dez2out(it_round((double)w1/w2*100.0), 3, s);
    WRITE_RP1(571,296,5,1,1,s);

    Step = 111.0 / ShipData(MyShipPtr->SType).MaxMove;
    RECT_RP1_C0(386,427,513,442);
    WRITE_RP1(498-it_round(Step*MyShipPtr->Repair),429,1,1,1,"#");

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
    WRITE_RP1(337,428,5,1,1, _s);

    (void) dez2out((ShipData(MyShipPtr->SType).MaxMove - MyShipPtr->Repair), 3, s);
    WRITE_RP1(522,428,5,1,1,s);
}

void SHIPINFO(uint8 ActSys, r_ShipHeader* MyShipPtr)
{
    char    s[50];
    char*   _s;
    char*   _s2;
    APTR    ModC = NULL;

    if (( 8 > MyShipPtr->SType)
     || (24 < MyShipPtr->SType)
     || (ActPlayerFlag != MyShipPtr->Owner))
    {
        return;
    }
    ModC = GETTHESOUND(2);

    INITSCREEN(SCREEN_TECH);
    _s=my_strcpy( s, PathStr[5]);
    _s=my_strcpy(_s,ProjectName[MyShipPtr->SType]);
    (void) my_strcpy(_s, "Tech.img");
    if (!DISPLAYIMAGE(s,6,7,256,498,5,MyScreen[1],0)) { }

    WRITE_RP1(290,15,1,0,3,ProjectName[MyShipPtr->SType]);
    s[0] = '-'; s[1] = ' ';
    _s2=s+2;
    _s = SHIPINFO_MAKETEXT(_s2, PText[190]);
    switch (MyShipPtr->Weapon) {
        case WEAPON_GUN:       (void) my_strcpy(_s, PText[185]); break;
        case WEAPON_LASER:     (void) my_strcpy(_s, PText[186]); break;
        case WEAPON_PHASER:    (void) my_strcpy(_s, PText[187]); break;
        case WEAPON_DISRUPTOR: (void) my_strcpy(_s, PText[188]); break;
        case WEAPON_PTORPEDO:  (void) my_strcpy(_s, PText[189]); break;
        default: (void) dez2out(MyShipPtr->Weapon, 0, _s);
    }
    WRITE_RP1(290,42,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[191]);
    (void) dez2out(it_round((double)ShipData(MyShipPtr->SType).WeaponPower*(MyShipPtr->Weapon/10+1)), 0, _s);
    WRITE_RP1(290,62,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[192]);
    (void) dez2out(ShipData(MyShipPtr->SType).MaxShield, 0, _s);
    WRITE_RP1(290,82,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[193]);
    (void) dez2out(MyShipPtr->ShieldBonus, 0, _s);
    WRITE_RP1(290,102,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[194]);
    _s = float2out( ((double) MyShipPtr->Shield / ShipData(MyShipPtr->SType).MaxShield*100.0), 0, 2, _s);
    *_s++ = ' ';
    *_s++ = '%';
    *_s  =0;
    WRITE_RP1(290,122,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[195]);
    (void) dez2out(ShipData(MyShipPtr->SType).MaxMove, 0, _s);
    WRITE_RP1(290,142,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[196]);
    (void) dez2out( ((MyShipPtr->Ladung & MASK_SIEDLER) / 16), 0, _s);
    WRITE_RP1(290,162,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[197]);
    (void) dez2out( (MyShipPtr->Ladung & MASK_LTRUPPS), 0, _s);
    WRITE_RP1(290,182,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[198]);
    _s = dez2out(it_round((double) MyShipPtr->Fracht / ShipData(MyShipPtr->SType).MaxLoad*100.0), 0, _s);
    (void) my_strcpy(_s, "% belegt");
    WRITE_RP1(290,202,1,0,2,s);

    _s = SHIPINFO_MAKETEXT(_s2, PText[199]);
    if (200 > MyShipPtr->Age)
    {
        (void) dez2out(Year-(MyShipPtr->Age), 0, _s);
    } else {
        *_s++ = ' ';
        (void) my_strcpy(_s, PText[200]);
    }
    WRITE_RP1(290,222,1,0,2,s);

    SHIPINFO_WRITEDATA(MyShipPtr);
    ScreenToFront(MyScreen[1]);
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(0,300);
            if ((454<MouseY(1)) && (481>MouseY(1)))
            {
                if ((379<MouseX(1)) && (411>MouseX(1))
                        && (MyShipPtr->Repair < ShipData(MyShipPtr->SType).MaxMove))
                {
                    ++(MyShipPtr->Repair);
                    SHIPINFO_WRITEDATA(MyShipPtr);
                }
                else if ((489<MouseX(1)) && (526>MouseX(1))
                        && (MyShipPtr->Repair > 0))
                {
                    --(MyShipPtr->Repair);
                    SHIPINFO_WRITEDATA(MyShipPtr);
                }
            }
            else if ((314<MouseY(1)) && (346>MouseY(1)))
            {
                if ((379<MouseX(1)) && (411>MouseX(1))
                        && (   MyShipPtr->Tactical  < (ShipData(MyShipPtr->SType).WeaponPower-2))
                        && ((3*MyShipPtr->Tactical) < (MyShipPtr->Shield-2)))
                {
                    ++(MyShipPtr->Tactical);
                    SHIPINFO_WRITEDATA(MyShipPtr);
                }
                else if ((489<MouseX(1)) && (526>MouseX(1))
                        && (   MyShipPtr->Tactical  > -(ShipData(MyShipPtr->SType).WeaponPower-2))
                        && ((3*MyShipPtr->Tactical) > -(MyShipPtr->Shield-2)))

                {
                    --(MyShipPtr->Tactical);
                    SHIPINFO_WRITEDATA(MyShipPtr);
                }
            }
            while (LMB_PRESSED)
            { };
        }
    }
    while (RMB_NOTPRESSED);

    if (NULL != ModC)
    {
        StopPlayer();
        UnLoadModule(ModC);
    }
    PLAYSOUND(0,300);
    ScreenToFront(MyScreen[0]);
}

