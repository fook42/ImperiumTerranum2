#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const char* const Technology[] =
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


void DISPLAYTECH(uint8 TechID)
{
    char    s[50];
    uint8   Depth, CivVar, Offset=0;
    uint16  l;
    APTR    ModC = NULL;

    if (0 == TechID)
    {
        return;
    }
    SWITCHDISPLAY();
    ModC = GETTHESOUND(1);
    RECTWIN(MyRPort_PTR[1],0,0,0,639,511);        // clear the screen

    s[0]=0;
    if      ((TechID>= 1) && (TechID<=18)) { strcpy(s, PathStr[1]); }
    else if ((TechID>=19) && (TechID<=38)) { strcpy(s, PathStr[2]); }
    else if ((TechID>=39) && (TechID<=42)) { strcpy(s, PathStr[3]); }

    if (34 == TechID)
    {
        strcat(s, "selbstSys");
    } else {
        strcat(s, Technology[TechID-1]);
    }
    strcat(s, ".pal");
    Depth = SETCOLOR(MyScreen[1],s);
    if (0 == Depth)
    {
        strcpy(s, PathStr[1]);
        strcat(s, "NoPic.pal");
        Depth = SETCOLOR(MyScreen[1],s);
    }
    strcpy(s+strlen(s)-3, "img");

    if (!DISPLAYIMAGE(s,0,40,320,256,Depth,MyScreen[1],0)) { }

    WRITEWIN(340,50,1,0,MyRPort_PTR[1],3,PText[223]);
    WRITEWIN(340,70,1,0,MyRPort_PTR[1],4,TechnologyL.data[TechID]);
    if (TechUse1[TechID]>0)
    {
        WRITEWIN(340,110,1,0,MyRPort_PTR[1],4,PText[224]);
        WRITEWIN(370,130,1,0,MyRPort_PTR[1],4,TechnologyL.data[TechUse1[TechID]]);
        if (TechUse1[TechID] != TechUse2[TechID])
        {
            WRITEWIN(370,150,1,0,MyRPort_PTR[1],4,TechnologyL.data[TechUse2[TechID]]);
        }
    }
    if (27 != TechID)
    {
        WRITEWIN(340,190,1,0,MyRPort_PTR[1],4,PText[225]);
        l = 0;
        for(CivVar = 1; CivVar < 43; ++CivVar)
        {
            if (TechUse1[CivVar] == TechID) { l      = CivVar; }
            if (TechUse2[CivVar] == TechID) { Offset = CivVar; }
        }
        if (l>0) { WRITEWIN(370,210,1,0,MyRPort_PTR[1],4,TechnologyL.data[l]); }
        if ((l != Offset) && (Offset>=1) && (Offset<=42))
        {
            WRITEWIN(370,230,1,0,MyRPort_PTR[1],4,TechnologyL.data[Offset]);
        }
    }
    l = 320;
    for(CivVar = 1; CivVar < 43; ++CivVar)
    {
        if (ProjectNeedsTech[CivVar] == TechID)
        {
            if       (CivVar < 8)                   { strcpy(s, PText[226]); }
            else if ((CivVar > 7) && (CivVar < 25)) { strcpy(s, PText[227]); }
            else                                    { strcpy(s, PText[228]); }
            strcat(s, "  ");
            strcat(s, Project.data[CivVar]);
            WRITEWIN(20,l,1,0,MyRPort_PTR[1],4,s);
            l += 20;
        }
    }
    Screen2 = SCREEN_INVENTION;
    ScreenToFront(MyScreen[1]);
    if (Save.PlayMySelf)
    {
        delay(PAUSE*2);
    }
    WAITLOOP(Save.PlayMySelf);
    if (NULL != ModC)
    {
        StopPlayer();
        FreePlayer();
        UnLoadModule(ModC);
//        FreeMem(ModC, ModMemL[1]);
    }
    ScreenToFront(MyScreen[0]);
}
