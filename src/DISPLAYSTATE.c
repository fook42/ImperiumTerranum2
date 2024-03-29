#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DISPLAYSTATE()
{
    int     punkte;
    char    s[10];
    char    Txt_Sep[]= "-------\0";

    --ActPlayer; // to shift the array

    Screen2 = 0;
    SetRGB32(MyVPort_PTR[1],0,         0,         0,0x33333333);
    SetRGB32(MyVPort_PTR[1],1,0xCCCCCCCC,0xCCCCCCCC,0xFFFFFFFF);
    SetRGB32(MyVPort_PTR[1],2,0xCCCCCCCC,         0,0x33333333);

    SetRast(MyRPort_PTR[1], 0); // clear Rastport
    WRITE_RP1(320,40,1,WRITE_Center,4,_PT_HighscoreImpstatus);

    WRITE_RP1(100,130,1,0,3,_PT_erreichtePunktzahl);
    (void) dez2out(Save.ImperatorState[ActPlayer], 0, s);
    WRITE_RP1(540,130,2,WRITE_Right,1,s);

    WRITE_RP1(100,160,1,0,3,_PT_Levelbonus);
    punkte = it_round(Save.ImperatorState[ActPlayer]*(Level/30.0));
    (void) dez2out(punkte, 0, s);
    WRITE_RP1(540,160,2,WRITE_Right,1,s);

    WRITE_RP1(100,190,1,0,3,_PT_Spieldauer);
    if (0 < Year)
    {
        s[0] = '-';
        (void) dez2out(Year, 0, s+1);
    } else {
        s[0] = '0';
        s[1] = 0;
    }
    WRITE_RP1(540,190,2,WRITE_Right,1, s);

    WRITE_RP1(540,210,2,WRITE_Right,1, Txt_Sep);

    WRITE_RP1(100,240,1,0,3,_PT_Gesamtpunktzahl);
    punkte += Save.ImperatorState[ActPlayer];
    if (0 < Year)
    {
        punkte -= Year;
    }
    (void) dez2out(punkte, 0, s);
    WRITE_RP1(540,240,2,WRITE_Right,1,s);

    WRITE_RP1(540,270,2,WRITE_Right,1,Txt_Sep);
    WRITE_RP1(540,275,2,WRITE_Right,1,Txt_Sep);
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
    ++ActPlayer; // to shift the array
}
