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
    uint16  l;
    APTR    ModC = NULL;

    if (0 == TechID)
    {
        return;
    }
    SWITCHDISPLAY();
    ModC = GETTHESOUND(0);
    SetRast(MyRPort_PTR[1], 0); // clear the screen

    if      ((0  < TechID) && (19 > TechID)) { textid = 1; }
    else if ((18 < TechID) && (39 > TechID)) { textid = 2; }
    else if ((38 < TechID) && (43 > TechID)) { textid = 3; }

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
    if (!DISPLAYIMAGE(s,0,40,320,256,Depth,MyScreen[1],0)) { }

    WRITE_RP1(340,50,1,0,2,PText[223]);
    WRITE_RP1(340,70,1,0,3,TechnologyName[TechID]);
    if (0 < TechUse1[TechID])
    {
        WRITE_RP1(340,110,1,0,3,PText[224]);
        WRITE_RP1(370,130,1,0,3,TechnologyName[TechUse1[TechID]]);
        if (TechUse1[TechID] != TechUse2[TechID])
        {
            WRITE_RP1(370,150,1,0,3,TechnologyName[TechUse2[TechID]]);
        }
    }
    if (27 != TechID)
    {
        WRITE_RP1(340,190,1,0,3,PText[225]);
        l = 0;
        for(CivVar = 1; CivVar < 43; ++CivVar)
        {
            if (TechUse1[CivVar] == TechID) { l      = CivVar; }
            if (TechUse2[CivVar] == TechID) { Offset = CivVar; }
        }
        if (0 < l) { WRITE_RP1(370,210,1,0,3,TechnologyName[l]); }
        if ((l != Offset) && (0 < Offset) && (43 > Offset))
        {
            WRITE_RP1(370,230,1,0,3,TechnologyName[Offset]);
        }
    }
    l = 320;
    for(CivVar = 1; CivVar < 43; ++CivVar)
    {
        if (ProjectNeedsTech[CivVar] == TechID)
        {
            textid = 228;
            if       (8 > CivVar)                   { textid = 226; }
            else if ((7 < CivVar) && (25 > CivVar)) { textid = 227; }
            _s=my_strcpy(s, PText[textid]);

            *_s++ = ' ';
            *_s++ = ' ';
            (void) my_strcpy(_s, ProjectName[CivVar]);
            WRITE_RP1(20,l,1,0,3,s);
            l += 20;
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
