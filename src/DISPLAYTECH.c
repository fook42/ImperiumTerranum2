#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const char* const TechnologyFileName[] =
        {   "Halbleiter",              "Informatik",
            "Recycling",               "Kernfusion",
            "Carbonfasern",            "Keramik-Technologie",
            "Mikroelektronik",         "Fotovoltaik",
            "Gentechnik",              "Teilchenphysik",
            "Raumstations-Technologie","Supraleiter",
            "Mikrocomputer",           "selbstwachsende Strukturen",
            "Hochenergie-Laser",       "Raumdock-Technologie",
            "Triravit",                "Cyberspace",
            "Bio-Elektronik",          "Ionenantrieb",
            "Duravit",                 "kuenstliche Intelligenz",
            "Bio-Computer",            "Phaser",
            "Duranium",                "Androiden",
            "Raumholografie",          "Impulsantrieb",
            "Materiedestruktion",      "Sol-Antrieb",
            "Beamen",                  "Disruptoren",
            "Warp-Antrieb",            "selbstorganisierende Systeme",
            "Tiefschlaf-Technologie",  "Transwarp",
            "Protonentorpedos",        "Bewusstseins-Transformation",
            "Chaostheorie",            "Energiestrukturen",
            "Weltformel",              "energetische Intelligenz" };


void DISPLAYTECH(const int TechID)
{
    char    s[50];
    char*   _s;
    int     Depth, CivVar, Offset=0, textid=0;
    uint16  l, posy;
    APTR    ModC = NULL;

    if ((0 >= TechID) || (42 < TechID))
    {
        return;
    }
    SWITCHDISPLAY();
    ModC = GETTHESOUND(0);
    SetRast(MyRPort_PTR[1], 0); // clear the screen

    if      (19 > TechID) { textid = 1; }
    else if (39 > TechID) { textid = 2; }
    else                  { textid = 3; }

    _s = my_strcpy(s, PathStr[textid]);

    if (34 == TechID)
    {
        _s=my_strcpy(_s, "selbstSys");
    } else {
        _s=my_strcpy(_s, TechnologyFileName[TechID-1]);
    }
    _s=my_strcpy(_s, ".pal");
    Depth = SETCOLOR(MyScreen[1],s);
    if (0 == Depth)
    {
        _s=my_strcpy(my_strcpy( s, PathStr[1]), "NoPic.pal");
        Depth = SETCOLOR(MyScreen[1],s);
    }
    (void) my_strcpy(_s-3, "img");
    posy = CenterY-216;
    if (!DISPLAYIMAGE(s,CenterX-320,posy,320,256,Depth,MyScreen[1],MyRPort_PTR[1],0)) { }

    WRITE_RP1(CenterX+20,posy+10,1,0,2,PText[223]);
    WRITE_RP1(CenterX+20,posy+30,1,0,3,TechnologyName[TechID]);
    if (0 < TechUse1[TechID])
    {
        WRITE_RP1(CenterX+20,posy+70,1,0,3,PText[224]);
        WRITE_RP1(CenterX+50,posy+90,1,0,3,TechnologyName[TechUse1[TechID]]);
        if (TechUse1[TechID] != TechUse2[TechID])
        {
            WRITE_RP1(CenterX+50,posy+110,1,0,3,TechnologyName[TechUse2[TechID]]);
        }
    }
    if (27 != TechID)
    {
        WRITE_RP1(CenterX+20,posy+150,1,0,3,PText[225]);
        l = 0;
        for(CivVar = 1; CivVar < 43; ++CivVar)
        {
            if (TechUse1[CivVar] == TechID) { l      = CivVar; }
            if (TechUse2[CivVar] == TechID) { Offset = CivVar; }
        }
        if (0 < l) { WRITE_RP1(CenterX+50,posy+170,1,0,3,TechnologyName[l]); }
        if ((l != Offset) && (0 < Offset) && (43 > Offset))
        {
            WRITE_RP1(CenterX+50,posy+190,1,0,3,TechnologyName[Offset]);
        }
    }
    posy += 280;
    for(CivVar = 1; CivVar < 43; ++CivVar)
    {
        if (ProjectNeedsTech[CivVar] == TechID)
        {
            if       (8 > CivVar) { textid = 226; }
            else if (25 > CivVar) { textid = 227; }
            else                  { textid = 228; }

            _s=my_strcpy(s, PText[textid]);

            *_s++ = ' ';
            *_s++ = ' ';
            (void) my_strcpy(_s, ProjectName[CivVar]);
            WRITE_RP1(CenterX-300,posy,1,0,3,s);
            posy += 20;
        }
    }
    Screen2 = SCREEN_INVENTION;
    ScreenToFront(MyScreen[1]);
    if (Save.PlayMySelf)
    {
        Delay(PAUSE*2);
    }
    WAITLOOP(Save.PlayMySelf);
    if (NULL != ModC)
    {
        StopPlayer();
        UnLoadModule(ModC);
        FreePlayer();
    }
    ScreenToFront(MyScreen[0]);
}
