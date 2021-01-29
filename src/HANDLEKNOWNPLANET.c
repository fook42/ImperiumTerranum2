#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

#define PROJECTS_XSIZE  67
#define PROJECTS_YSIZE  66

void WRITECURRENTPROJECT(r_PlanetHeader* MyPlanetHeader)
{
    sint32  l;
    char    s[4];
    char*   _s;

    RECT_RP1(0,56,426,327,448);
    l = 0;
    s[0]=0;
    if (0 != MyPlanetHeader->ProjectID)
    {
        if (0 < MyPlanetHeader->ProjectID)
        {
            _s = Project.data[MyPlanetHeader->ProjectID];
            l = MyPlanetHeader->XProjectPayed*100 / (MyPlanetHeader->XProjectCosts+1);
        } else {
            switch (MyPlanetHeader->ProjectID) {
                case -3:{
                            _s = PText[163];
                            l = MyPlanetHeader->Biosphaere >> 1;
                        }; break;
                case -2:{
                            _s = PText[164];
                            l = MyPlanetHeader->Infrastruktur >> 1;
                        }; break;
                default:{
                            _s = PText[165];
                            l = MyPlanetHeader->Industrie >> 1;
                        }
            }
        }
        WRITE_RP1(191,430,2,(1|WRITE_Center),3, _s);
    }

    RECT_RP1(0,56,399,256,418);
    if (l > 100) { l = 100; }
    if (l < 0)   { l = 0; }
    (void) dez2out(l, 3, s);
    WRITE_RP1(278,402,4,1,1,s);
    RECT_RP1(4,56,399,56+(l*2),418);
}

void WRITEPLANETSTATUS(r_PlanetHeader* MyPlanetHeader, ByteArr42* ActPProjects)
{
    uint8  i, y;
    uint16 xpos;
    char   s[8];


    // draw state of biosphere, infrastructure and industry
    RECT_RP1(4,56,101,56+MyPlanetHeader->Biosphaere,120);
    (void) dez2out((MyPlanetHeader->Biosphaere >> 1), 3, s);
    WRITE_RP1(278,104,4,1,1,s);

    y =  ActPProjects->data[30]              /* Recycling-Anl. */
        +ActPProjects->data[31]              /* Fusions-Kraftwerk */
        +ActPProjects->data[32]              /* Hydro-Kraftwerk */
        +ActPProjects->data[37]              /* intell. Fabrik */
        +ActPProjects->data[42];             /* Wetter-Sat */
    if (y < 5)
    {
        y = 5-y;
        xpos = 59;
        for(i = 0; i < y; ++i)
        {
            // draw skulls ...
            BltBitMapRastPort((struct BitMap*) &ImgBitMap8,320,174,MyRPort_PTR[1],xpos,102,19,18,192);
            xpos += 22;
        }
    }

    RECT_RP1(4,56,150,56+MyPlanetHeader->Infrastruktur,169);
    (void) dez2out((MyPlanetHeader->Infrastruktur >> 1), 3, s);
    WRITE_RP1(278,153,4,1,1,s);

    RECT_RP1(4,56,199,56+MyPlanetHeader->Industrie,218);
    (void) dez2out((MyPlanetHeader->Industrie >> 1), 3, s);
    WRITE_RP1(278,202,4,1,1,s);

    (void) dez2out(MyPlanetHeader->Population, 7, s);
    WRITE_RP1(59,251,4,1,1,s);


    y = 0;   /* Kreativität */
    if (ActPProjects->data[33]>0) { ++y; }
    if (ActPProjects->data[35]>0) { ++y; }
    if (ActPProjects->data[36]>0) { ++y; }
    if (ActPProjects->data[38]>0) { ++y; }
    if (ActPProjects->data[42]>0) { ++y; }
    RECT_RP1(0,56,307,178,330);

    if (y > 0)
    {
        xpos = 56;
        for(i = 0; i < y; ++i)
        {
            // draw lightbulbs ...
            BltBitMapRastPort((struct BitMap*) &ImgBitMap8,320,128,MyRPort_PTR[1],xpos,308,22,22,192);
            xpos += 25;
        }
    }

    y = 0;   /*Produktivität*/
    if (ActPProjects->data[31]>0) { ++y; }
    if (ActPProjects->data[37]>0) { ++y; }
    if (ActPProjects->data[38]>0) { ++y; }
    if (ActPProjects->data[41]>0) { ++y; }
    if (ActPProjects->data[42]>0) { ++y; }
    RECT_RP1(0,56,354,178,376);
    if (y > 0)
    {
        xpos = 56;
        for(i = 0; i < y; ++i)
        {
            // draw coins ...
            BltBitMapRastPort((struct BitMap*) &ImgBitMap8,320,151,MyRPort_PTR[1],xpos,354,22,22,192);
            xpos += 25;
        }
    }
}

void WRITEPROJECTSSTATUS(r_PlanetHeader* MyPlanetHeader, ByteArr42* ActPProjects)
{
    uint8   i;
    uint16  x, y;
    char    s[4];

    // clear right project icon area
    RECT_RP1(0,359,92,639,511);
    x = 0;
    y = 0;
    for(i = 0; i < 7; ++i)
    {
        // did we finished this project here?
        if (Save.ProjectCosts[ActPlayer-1].data[i+1] <= 0)
        {
            // was this project invented at this planet first?
            if (ActPProjects->data[i+1]>0)
            {
                RECT_RP1(4,360+x,92+y,427+x,159+y);
            }
            // copy project icon bitmap to screen
            BltBitMapRastPort((struct BitMap*) &ImgBitMap8,i*64,0,MyRPort_PTR[1],362+x,94+y,64,64,192);
            x += PROJECTS_XSIZE;
            if ((3*PROJECTS_XSIZE) < x)
            {
                // next row
                x = 0;
                y += PROJECTS_YSIZE;
            }
        }
    }

    for(i = 25; i < 43; ++i)
    {
        if ((ActPProjects->data[i]>0) || ((i == 39) && (Save.ProjectCosts[ActPlayer-1].data[i] <= 0)))
        {
            if        ((i>24) && (i<28))
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap8,(i-18)*64,  0,MyRPort_PTR[1],362+x,94+y,64,64,192);
            } else if ((i>27) && (i<38))
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap8,(i-28)*64, 64,MyRPort_PTR[1],362+x,94+y,64,64,192);
            } else
            {
                BltBitMapRastPort((struct BitMap*) &ImgBitMap8,(i-38)*64,128,MyRPort_PTR[1],362+x,94+y,64,64,192);
            }
            if ((26 == i) || (27 == i))
            {
                /* settlers and landingtroops */
                (void) dez2out(ActPProjects->data[i], 3, s);
                WRITE_RP1(376+x,141+y,4,0,1,s);
            } else if ((34 == i) || (40 == i))
            {
                /* vNeuMann and space-Phalanx */
                (void) dez2out(ActPProjects->data[i], 3, s);
                WRITE_RP1(367+x,141+y,4,0,1,s);
            }
            x += PROJECTS_XSIZE;
            if ((3*PROJECTS_XSIZE) < x)
            {
                x = 0;
                y += PROJECTS_YSIZE;
            }
            if ((6*PROJECTS_YSIZE) == y) { return; }
        }
    }
}

void HANDLEKNOWNPLANET(uint8 ActSys, uint8 Mode, r_PlanetHeader* PlanetPtr)
{
    sint32  PMoney, PProd;
    char    NewProject[26][30];             // 1..25
    uint32  ProjectRounds[26];              // 1..25
    sint16  ProjectNum[26], ProjectType[26];// 1..25
    int     i, j;
    sint32  l;
    uint8   blink;
    uint8   Ships, btx, RawCode;
    bool    DoIt, b;
    char    s[60];
    char*   _s;
    struct Window*   HKP_Window;
    struct RastPort* RPort_PTR;

    r_ShipHeader*   MyShipPtr;
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPProjects;

    SWITCHDISPLAY();
    MyPlanetHeader = PlanetPtr;

    INITSCREEN(SCREEN_PLANET);
    ActPProjects = MyPlanetHeader->ProjectPtr;
    j=0;
    for(i = 0; i < 3; ++i)
    {
        RECT_RP1(0,56,101+j,256,120+j);
        j+=49;
    }
    RECT_RP1(0,0,0,639,90);

    _s=my_strcpy(s, _PT_System);
    *_s++ = ':';
    *_s++ = ' ';
    (void) my_strcpy(_s, Save.SystemName.data[ActSys-1]);
    WRITE_RP1(5,5,1,1,3,s);

    _s=my_strcpy(s, _PT_Planet);
    *_s++ = ':';
    *_s++ = ' ';
    (void) my_strcpy(_s, MyPlanetHeader->PName);
    WRITE_RP1(5,25,1,1,3,s);

    _s=my_strcpy(s, _PT_Klasse);
    *_s++ = ':';
    *_s++ = ' ';
    switch ( MyPlanetHeader->Class) {
        case CLASS_DESERT    : _s=my_strcpy(_s, "D (ca. 60% "); break;
        case CLASS_HALFEARTH : _s=my_strcpy(_s, "H (ca. 80% "); break;
        case CLASS_EARTH     : _s=my_strcpy(_s, "M (ca. 95% "); break;
        case CLASS_ICE       : _s=my_strcpy(_s, "I (ca. 60% "); break;
        case CLASS_STONES    : _s=my_strcpy(_s, "T (ca. 75% "); break;
        case CLASS_WATER     : _s=my_strcpy(_s, "W (ca. 60% "); break;
        default: { }
    }
    _s=my_strcpy(_s, _PT_Besiedlung);
    *_s++ = ')';
    *_s = 0;
    WRITE_RP1(5,45,1,1,3,s);

    _s=my_strcpy(s, _PT_Groesse);
    *_s++ = ':';
    *_s++ = ' ';
    _s = float2out( (MyPlanetHeader->Size/10.0), 0, 2, _s);
    (void) my_strcpy(_s, PText[171]);
    WRITE_RP1(5,65,1,1,3,s);

    if (Save.ActTech[ActPlayer-1]>0)
    {
        _s=my_strcpy(s, PText[172]);
        *_s++ = ':';
        *_s++ = ' ';
        (void) my_strcpy(_s, TechnologyL.data[Save.ActTech[ActPlayer-1]]);
        WRITE_RP1(275,5,1,1,3,s);
    } else {
        WRITE_RP1(275,5,1,1,3,PText[173]);
    }

    if (NULL != MyPlanetHeader->FirstShip.NextShip)
    {
        i = 0;
        MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
        do
        {
            if (0 != MyShipPtr->Owner) { ++i; }
            MyShipPtr = MyShipPtr->NextShip;
        }
        while (NULL != MyShipPtr);

        _s=my_strcpy(s, PText[175]);
        *_s++ = ':';
        *_s++ = ' ';
        (void) dez2out(i, 0, _s);
        WRITE_RP1(275,45,1,1,3,s);
    }
    if (MyPlanetHeader->Ethno != (MyPlanetHeader->PFlags & ActPlayerFlag))
    {
        _s=my_strcpy(s, PText[176]);
        *_s++ = ' ';
        (void) my_strcpy(_s, GETCIVNAME(GETCIVVAR(MyPlanetHeader->Ethno)));
        WRITE_RP1(275,65,1,1,3,s);
    }

    PProd = 11+(ActPProjects->data[31]+ActPProjects->data[37]+
                ActPProjects->data[38]+ActPProjects->data[41]+
                ActPProjects->data[42])*6;
    PMoney = it_round(PProd*(double)(MyPlanetHeader->Infrastruktur+MyPlanetHeader->Industrie+MyPlanetHeader->Population)/17.0)+1;
    while (PMoney>MAXPMONEY)
    {
        PMoney = it_round(PMoney*0.95);
    }

    WRITEPLANETSTATUS(MyPlanetHeader, ActPProjects);
    WRITECURRENTPROJECT(MyPlanetHeader);
    WRITEPROJECTSSTATUS(MyPlanetHeader, ActPProjects);

    ScreenToFront(MyScreen[1]);

    blink = 0;
    do
    {
        Delay(RDELAY);
        if (1 == Mode)
        {
            blink++;
            if (20 == blink)
            {
                WRITE_RP1(278,402,0,1,1, "100");
            } else if (40 == blink)
            {
                WRITE_RP1(278,402,4,1,1, "100");
                blink = 0;
            }
        }

        RawCode = GETRAWCODE();

        if (LMB_PRESSED)
        {
            if ((MouseX(1) >  55) && (MouseX(1) < 188)
              &&(MouseY(1) > 454) && (MouseY(1) < 481))
            {
                CLICKRECT(MyRPort_PTR[1],55,455,186,481,2);
                Mode = 0;
                j = 1;
                if (MyPlanetHeader->Biosphaere<200)
                {
                    (void) my_strcpy(NewProject[j], PText[163]);
                    ProjectRounds[j] = (200-MyPlanetHeader->Biosphaere)*PMoney / 9;
                    ProjectNum[j] = -3;
                    ProjectType[j] = 1;
                    j++;
                }
                if (MyPlanetHeader->Infrastruktur<200)
                {
                    (void) my_strcpy(NewProject[j], PText[164]);
                    ProjectRounds[j] = (200-MyPlanetHeader->Infrastruktur)*PMoney / 9;
                    ProjectNum[j] = -2;
                    ProjectType[j] = 1;
                    j++;
                }
                if (MyPlanetHeader->Industrie<200)
                {
                    (void) my_strcpy(NewProject[j], PText[165]);
                    ProjectRounds[j] = (200-MyPlanetHeader->Industrie)*PMoney / 9;
                    ProjectNum[j] = -1;
                    ProjectType[j] = 1;
                    j++;
                }
                Ships = 0;
                for (i = 42; i>0; --i)
                {
                    DoIt = false;
                    /* Technologie vorhanden */
                    if ((Save.TechCosts[ActPlayer-1].data[ProjectNeedsTech[i]] <= 0)
                     && ((ActPProjects->data[ProjectNeedsProject[i]]>0)
                        || (Save.ProjectCosts[ActPlayer-1].data[ProjectNeedsProject[i]] <= 0)))
                    {
                        /* nötiges Projekt vorhanden */
                        if ((((0 < i) && (8 > i)) || (39 == i)) && (Save.ProjectCosts[ActPlayer-1].data[i]>0))
                        {
                            DoIt = true;
                        }
                        /* Großprojekt noch nicht gebaut */
                        if (((25 == i) || ((27 < i) && (39 > i)) || ((39 < i) && (43 > i)))
                            && (0 == ActPProjects->data[i]))
                        {
                            DoIt = true;
                        }
                        /* Projekt noch nicht gebaut */
                        if (((7 < i) && (25 > i)) || (26 == i) || (27 == i))
                        {
                            DoIt = true;
                        }
                        /* sonstige Projekte, können mehrfach gebaut werden */
                    }
                    if (DoIt)
                    {
                        ProjectRounds[j] = Save.ProjectCosts[ActPlayer-1].data[i];
                        (void) my_strcpy(NewProject[j], Project.data[i]);
                        ProjectNum[j] = i;
                        if ((7 < i) && (25 > i))
                        {
                            if (3 < Ships)
                            {
                                --j;
                            } else {
                                ++Ships;
                                ProjectType[j] = 3;
                                ProjectRounds[j] += it_round(ProjectRounds[j]*(Save.Military[ActPlayer-1]/100.0));
                            }
                        }
                        if ((0 < i) && (8 > i))
                        {
                            ProjectType[j] = 2;
                        }
                        if (24 < i)
                        {
                            ProjectType[j] = 4;
                        }
                        ++j;
                    }
                }
                --j;
//                Img = (struct Image) {0,0,384,407,7,IMemA[0],127,0,NULL};
                // clear right project-area to print the list of available Projects to build
                RECT_RP1(0,360,92,639,511);
                WRITE_RP1(365,474,4,1,3,PText[177]);
                for(i = 1; i <= j; ++i)
                {
                    WRITE_RP1(365,78+i*16,ProjectType[i],1,2,NewProject[i]);
                    if (0 < ProjectNum[i])
                    {
                        l = ProjectRounds[i]-MyPlanetHeader->XProjectPayed;
                    } else {
                        l = ProjectRounds[i];
                    }
                    l = (l / PMoney ) +1;
                    if (1 > l) { l = 1; }
                    (void) dez2out(l, 7 ,s);
                    WRITE_RP1(575,78+i*16,ProjectType[i],1,2,s);
                }
                btx = 1;
                MyPlanetHeader->ProjectID = 0;
                do
                {
                    Delay(RDELAY);
                    if (MouseX(1) > 365)
                    {
                        i = (MouseY(1)-78) / 16;
                        if ((i != btx) && (i>=1) && (i<=j))
                        {
                            btx = i;
                            for(i = 1; i <= j; ++i)
                            {
                                if (i != btx)
                                {
                                    WRITE_RP1(365,78+i*16,ProjectType[i],1,2,NewProject[i]);
                                } else {
                                    WRITE_RP1(365,78+i*16,ProjectType[i],5,2,NewProject[i]);
                                }
                            }
                        }

                        if (LMB_PRESSED)
                        {
                            PLAYSOUND(0,300);
                            if (btx>0) { MyPlanetHeader->ProjectID = ProjectNum[btx]; }
                        }
                    }
                }
                while ((0 == MyPlanetHeader->ProjectID) && RMB_NOTPRESSED);
                if (RMB_PRESSED)
                {
                    /* cancel any project-work .. */
                    MyPlanetHeader->ProjectID = 0;
                    PLAYSOUND(0,300);
                } else {
                    /* new project selected .. */
                    MyPlanetHeader->XProjectCosts = ProjectRounds[btx];
                    if (MyPlanetHeader->ProjectID>0)
                    {
                        (void) my_strcpy(s, Project.data[MyPlanetHeader->ProjectID]);
                    }
                }
                WRITECURRENTPROJECT(MyPlanetHeader);
                WRITEPROJECTSSTATUS(MyPlanetHeader, ActPProjects);
            } else if ((MouseX(1) > 197) && (MouseX(1) < 329)
                    && (MouseY(1) > 454) && (MouseY(1) < 481)
                    && (MyPlanetHeader->ProjectID>0))
            {
                CLICKRECT(MyRPort_PTR[1],197,455,328,481,2);

                HKP_Window=MAKEWINDOW(183,186,274,138,MyScreen[1]);
                RPort_PTR = HKP_Window->RPort;
                MAKEWINBORDER(RPort_PTR,0,0,273,137,2,32,1);

                WRITE(136, 11,2,WRITE_Center,RPort_PTR,3,PText[178]);
                l = MyPlanetHeader->XProjectCosts-MyPlanetHeader->XProjectPayed;
                if (l < 0) { l = 0; }
                (void) dez2out(l, 0, s);
                WRITE(136, 32,4,WRITE_Center,RPort_PTR,1,s);
                WRITE(136, 53,2,WRITE_Center,RPort_PTR,3,PText[179]);
                (void) dez2out(Save.Staatstopf[ActPlayer-1], 0, s);
                WRITE(136, 74,4,WRITE_Center,RPort_PTR,1,s);
                MAKEWINBORDER(RPort_PTR, 10,100,131,125,2,32,1);
                WRITE( 70,106,2,WRITE_Center,RPort_PTR,3,_PT_Kaufen);
                MAKEWINBORDER(RPort_PTR,141,100,263,125,2,32,1);
                WRITE(202,106,2,WRITE_Center,RPort_PTR,3,"Abbruch");
                b = false;
                do
                {
                    Delay(RDELAY);
                    if (LMB_PRESSED
                        && (HKP_Window->MouseX >  9) && (HKP_Window->MouseX < 132)
                        && (HKP_Window->MouseY > 99) && (HKP_Window->MouseY < 126))
                    {
                        /* Kaufen */
                        CLICKRECT(RPort_PTR, 10,100,131,125,2);
                        b = true;
                        l = MyPlanetHeader->XProjectCosts-MyPlanetHeader->XProjectPayed;
                        if (l < 0) { l = 0; }
                        if (l >= Save.Staatstopf[ActPlayer-1])
                        {
                            MyPlanetHeader->XProjectPayed += Save.Staatstopf[ActPlayer-1];
                            Save.Staatstopf[ActPlayer-1] = 0;
                        } else {
                            MyPlanetHeader->XProjectPayed = MyPlanetHeader->XProjectCosts;
                            Save.Staatstopf[ActPlayer-1] -= l;
                        }
                    }
                    if (RMB_PRESSED || (LMB_PRESSED
                        && (HKP_Window->MouseX > 140) && (HKP_Window->MouseX < 264)
                        && (HKP_Window->MouseY >  99) && (HKP_Window->MouseY < 126)))
                    {
                        /* Abbruch */
                        CLICKRECT(RPort_PTR,141,100,263,125,2);
                        b = true;
                    }
                }
                while (!b);
                CloseWindow(HKP_Window);
                WRITECURRENTPROJECT(MyPlanetHeader);
            }
        }
    }
    while (RMB_NOTPRESSED && (64 != RawCode) && (67 != RawCode) && (68 != RawCode));
    PLAYSOUND(0,300);
    CLEARINTUITION();
    ScreenToFront(MyScreen[0]);
}
