#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DISPLAYSTATE()
{
    int     punkte;
    char    s[10];
    const char      Txt_Sep[]= "-------\0";
    uint16  posy;
    const uint32    imperator_state = Save.ImperatorState[ActPlayer-1];

    Screen2 = 0;
    SetRGB32(MyVPort_PTR[1],0,         0,         0,0x33333333);
    SetRGB32(MyVPort_PTR[1],1,0xCCCCCCCC,0xCCCCCCCC,0xFFFFFFFF);
    SetRGB32(MyVPort_PTR[1],2,0xCCCCCCCC,         0,0x33333333);

    SetRast(MyRPort_PTR[1], 0); // clear Rastport
    posy = CenterY-216;
    WRITE_RP1(CenterX,posy,1,WRITE_Center,4,_PT_HighscoreImpstatus);

    posy += 50;
    WRITE_RP1(CenterX-220,posy,1,0,3,_PT_erreichtePunktzahl);
    (void) dez2out(imperator_state, 0, s);
    WRITE_RP1(CenterX+220,posy,2,WRITE_Right,1,s);

    posy += 30;
    WRITE_RP1(CenterX-220,posy,1,0,3,_PT_Levelbonus);
    punkte = it_round(imperator_state*((double)Level/30.0));
    (void) dez2out(punkte, 0, s);
    WRITE_RP1(CenterX+220,posy,2,WRITE_Right,1,s);

    posy += 30;
    WRITE_RP1(CenterX-220,posy,1,0,3,_PT_Spieldauer);
    if (0 < Year)
    {
        s[0] = '-';
        (void) dez2out(Year, 0, s+1);
        punkte -= Year;
    } else {
        s[0] = '0';
        s[1] = 0;
    }
    WRITE_RP1(CenterX+220,posy,2,WRITE_Right,1, s);
    posy += 20;
    WRITE_RP1(CenterX+220,posy,2,WRITE_Right,1, Txt_Sep);
    posy += 30;
    WRITE_RP1(CenterX-220,posy,1,0,3,_PT_Gesamtpunktzahl);
    punkte += imperator_state;
    (void) dez2out(punkte, 0, s);
    WRITE_RP1(CenterX+220,posy,2,WRITE_Right,1,s);

    posy += 30;
    WRITE_RP1(CenterX+220,posy,2,WRITE_Right,1,Txt_Sep);
    WRITE_RP1(CenterX+220,posy+5,2,WRITE_Right,1,Txt_Sep);
    ScreenToFront(MyScreen[1]);
    do
    {
        Delay(RDELAY);
    }
    while (LMB_NOTPRESSED && RMB_NOTPRESSED);
    PLAYSOUND(0,300);
    do
    {
        Delay(RDELAY);
    }
    while ((LMB_PRESSED || RMB_PRESSED) && (!Bool_var));
}
