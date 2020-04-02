#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CHECKGADS(uint8 GadID)
{
    uint8   i;
    uint16  y;
    char    txt[4];
    // new audio options
    if ((0 == GadID) || (8 == GadID))
    {
        if (Audio_enable)
        {
            MAKEWINBORDER(MyRPort_PTR[1],440,431,506,454, 100, 107, 1);
            MAKEWINBORDER(MyRPort_PTR[1],508,431,574,454,  40,  14, 1);
        } else {
            MAKEWINBORDER(MyRPort_PTR[1],440,431,506,454,  40,  14, 1);
            MAKEWINBORDER(MyRPort_PTR[1],508,431,574,454, 100, 107, 1);
        }
    }
    // ----
    if ((0 == GadID) || (7 == GadID))
    {
        RECT_RP1(0,542,141,568,310);
        txt[0]=Player+'0';
        txt[1]=0;
        WRITE(555,(5-Player)*38+142,40,WRITE_Center,MyRPort_PTR[1],3,txt);
        if (7 == GadID) { return; }
    }
    if ((0 == GadID) || (6 == GadID))
    {
        RECT_RP1(0,447,141,473,310);
        RECT_RP1(0,412,341,508,360);
        if (Level<10)
        {
            txt[0]=Level+'0';
            txt[1]=0;
        } else {
            txt[0]=(Level/10)+'0';
            txt[1]=(Level%10)+'0';
            txt[2]=0;
        }
        WRITE(460,(10-Level)*17+142,40,WRITE_Center,MyRPort_PTR[1],3, txt);
        WRITE(460,              343,40,WRITE_Center,MyRPort_PTR[1],3, PText[Level+491]);
        if (6 == GadID) { return; }
    }
    if (0 == GadID)
    {
        y = 113;
        for (i = 0; i < 5; ++i)
        {
            RECT_RP1(0, 238, y, 388, y+19);
            y += 80;
        }
    } else {
        RECT_RP1(0,238,33+GadID*80,388,52+GadID*80);
    }
    if ((0 == GadID) || (1 == GadID))
    {
        if (Save.PlayMySelf)
        {
            DRAWGAD(80,1);
            WRITE(240,115,40,1,MyRPort_PTR[1],3,PText[505]);
        } else {
            DRAWGAD(80,2);
            WRITE(240,115,40,1,MyRPort_PTR[1],3,PText[506]);
        }
    }
    if ((0 == GadID) || (2 == GadID))
    {
        if (Save.SmallFight)
        {
            DRAWGAD(160,1);
            WRITE(240,195,40,1,MyRPort_PTR[1],3,_PT_aus);
        } else {
            DRAWGAD(160,2);
            WRITE(240,195,40,1,MyRPort_PTR[1],3,_PT_ein);
        }
    }
    if ((0 == GadID) || (3 == GadID))
    {
        if (Save.SmallLand)
        {
            DRAWGAD(240,1);
            WRITE(240,275,40,1,MyRPort_PTR[1],3,PText[505]);
        } else {
            DRAWGAD(240,2);
            WRITE(240,275,40,1,MyRPort_PTR[1],3,"Player");
        }
    }
    if ((0 == GadID) || (4 == GadID))
    {
        if (Save.NoWorm)
        {
            DRAWGAD(320,1);
            WRITE(240,355,40,1,MyRPort_PTR[1],3,PText[505]);
        } else {
            DRAWGAD(320,2);
            WRITE(240,355,40,1,MyRPort_PTR[1],3,"Player");
        }
    }
    if ((0 == GadID) || (5 == GadID))
    {
        if (Save.FastMove)
        {
            DRAWGAD(400,1);
            WRITE(240,435,40,1,MyRPort_PTR[1],3,PText[510]);
        } else {
            DRAWGAD(400,2);
            WRITE(240,435,40,1,MyRPort_PTR[1],3,PText[511]);
        }
    }
}

void OPTION_REDUCECOSTS(void)
{
    // reduce Project/Tech Costs according to selected Level
    uint8   i, j;
    sint32  Factor;
    const sint16 LevelFactors[] = { 0xE2, 0xA9, 0x71, 0x38, 0x00, 0x44, 0x88, 0xCC, 0x110 ,0x154 };
    // Levels 0..5 = 0.220, 0.165, 0.110, 0.055, 0
    // Levels 6..10= 0.066, 0.133, 0.200, 0.266, 0.332  ... >> 10!
    Factor = LevelFactors[Level];

    if (5 > Level)
    {
        // Levels 1..4 -> reduce our own costs
        for (j = 0; j < MAXCIVS; ++j)
        {
            if (0 != Save.CivPlayer[j])
            {
                for (i = 1; i<43; ++i)
                {
                    Save.TechCosts[j].data[i]    -= (Save.TechCosts[j].data[i]   *Factor)>>10;
                    Save.ProjectCosts[j].data[i] -= (Save.ProjectCosts[j].data[i]*Factor)>>10;
                }
            }
        }
    } else if (5 < Level)
    {
        // Levels 5..10 -> reduce enemies' costs
        for (j = 0; j < MAXCIVS; ++j)
        {
            if (0 == Save.CivPlayer[j])
            {
                for (i = 1; i<43; ++i)
                {
                    Save.TechCosts[j].data[i]    -= (Save.TechCosts[j].data[i]   *Factor)>>10;
                    Save.ProjectCosts[j].data[i] -= (Save.ProjectCosts[j].data[i]*Factor)>>10;
                }
            }
        }
    }
    // if Level == 5 .. do nothing.. all values are okay.
}

void OPTION_MULTIPLAYER(void)
{
    uint8   i, j, btx;
    uint16  y;
    char    s[40];
    char*   _s;
    int     stringlen;
    bool    b;

    // randomly exchange one StarSystemName with the first one (Solar-system)
    i = 1+(rand()%(MAXSYSTEMS-1));
    strcpy(s, Save.SystemName.data[i]);
    strcpy(Save.SystemName.data[i], Save.SystemName.data[0]);
    strcpy(Save.SystemName.data[0], s);

    // pick the civilization names for each player...
    for (i = 0; i < Player; ++i)
    {
        SWITCHDISPLAY();
        INITMENU();
        strcpy(s, "Player 1 ");
        stringlen = strlen(s);
        s[stringlen-2] = i + '1';
        strcpy(s+stringlen, PText[520]);
        WRITE(320,50,40,WRITE_Center,MyRPort_PTR[1],3,s);
        btx = 7;
        if (0 == i)
        {
            btx = 1;
            Save.CivPlayer[0] = 0;
        }
        y = 100;
        for (j = 0; j < btx; ++j)
        {
            if (0 == Save.CivPlayer[j])
            {
                MAKEWINBORDER(MyRPort_PTR[1],100, y, 540, y+30,14,40,0);
                _s = GETCIVNAME(j+1);
            } else {
                RECT_RP1(0, 100, y, 540, y+30);
                strcpy(s, "Player 0");
                s[strlen(s)-1] = Save.CivPlayer[j]+'0';
                _s = s;
            }
            WRITE(320, y+8,40,WRITE_Center,MyRPort_PTR[1],3, _s);
            y += 50;
        }

        b = false;
        ScreenToFront(MyScreen[1]);
        do
        {
            Delay(RDELAY);
            if (LMB_PRESSED && (MouseX(1)>=100) && (MouseX(1)<=540))
            {
                y = 100;
                for (j = 0; j < btx; ++j)
                {
                    if ((MouseY(1)>=y) && (MouseY(1)<=(y+30)) && (0 == Save.CivPlayer[j]))
                    {
                        b = true;
                        Save.CivPlayer[j] = i+1;
                        CLICKRECT(MyRPort_PTR[1], 100, y, 540, y+30, 40);
                        PLAYERJINGLE(j+1);
                    }
                    y += 50;
                }

                while (LMB_PRESSED) {};
            }
        }
        while (!b);
        for (j = 0; j < MAXCIVS; j++)
        {
            if ((Save.CivPlayer[i] == Save.CivPlayer[j]) && (i != j))
            {
                Save.CivPlayer[j] = 0;
            }
        }
        Delay(10);
    }

    // pick the number of home-planets (1..5) **************
    SWITCHDISPLAY();
    INITMENU();
    WRITE(320,100,40,WRITE_Center,MyRPort_PTR[1],3,PText[521]);
    y = 123;
    for (i = 0; i < 5; ++i)
    {
        MAKEWINBORDER(MyRPort_PTR[1], y, 200, y+40, 240,14,40,0);
        s[0] = i+'1';
        s[1] = 0;
        WRITE(y+20,213,40,WRITE_Center,MyRPort_PTR[1],3,s);
        y += 88;
    }

    ScreenToFront(MyScreen[1]);
    HomePlanets = 0;
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED && (MouseY(1)>=200) && (MouseY(1)<=240)) 
        {
            HomePlanets = (uint8) ((MouseX(1)-35) / 88);
        }
    }
    while ((1 > HomePlanets) || (5 < HomePlanets));

    CLICKRECT(MyRPort_PTR[1],HomePlanets*88+35,200,HomePlanets*88+75,240,40);

//    Delay(20);
//    SWITCHDISPLAY();

    OPTION_REDUCECOSTS();

/*
    while (RMB_NOTPRESSED) {};
    while (RMB_PRESSED) {};
*/
    ScreenToFront(XScreen);
}


void OPTIONMENU(uint8 Mode)
{
    uint8   i;
    uint16  y;
    sint32  bitmap_srcCord[5][2] = { {384,448},{384,448},{384,512},{384,448},{576,512} };

    SWITCHDISPLAY();
	INITMENU();

    // count initiated human/civ-players
    Player = 0;
    for (i = 0; i < 7; ++i)
    {
        if (0 != Save.CivPlayer[i])
        {
            ++Player;
        }
    }
    WRITE(320,55,40,WRITE_Center,MyRPort_PTR[1],3,PText[513]);

    // draw left 5*2 images
    y = 80;
    for (i = 0; i < 5; ++i)
    {
        MAKEWINBORDER(MyRPort_PTR[1], 236, y+31, 390, y+54, 14, 40, 1);
        BltBitMapRastPort((struct BitMap *) &ImgBitMap8, bitmap_srcCord[i][0], 128, MyRPort_PTR[1], 60, y, 64, 64, 192);
        BltBitMapRastPort((struct BitMap *) &ImgBitMap8, bitmap_srcCord[i][1], 128, MyRPort_PTR[1],150, y, 64, 64, 192);
        y += 80;
    }

    WRITE(240, 90,40,0,MyRPort_PTR[1],3,"Player");
    WRITE(240,170,40,0,MyRPort_PTR[1],3,PText[514]);
    WRITE(240,250,40,0,MyRPort_PTR[1],3,PText[515]);
    WRITE(240,330,40,0,MyRPort_PTR[1],3,PText[516]);
    WRITE(240,410,40,0,MyRPort_PTR[1],3,PText[517]);

    WRITE(460,100,40,WRITE_Center,MyRPort_PTR[1],3,"Level");
    if (1 == Mode)
    {
        WRITE(455,120,40,0,MyRPort_PTR[1],1,"I");
        WRITE(455,320,40,0,MyRPort_PTR[1],1,"J");
    }
    MAKEWINBORDER(MyRPort_PTR[1],445,139,475,312,14,40,1);
    MAKEWINBORDER(MyRPort_PTR[1],410,339,510,362,14,40,1);

    WRITE(555,100,40,WRITE_Center,MyRPort_PTR[1],3,"Player");
    if (1 == Mode)
    {
        WRITE(550,120,40,0,MyRPort_PTR[1],1,"I");
        WRITE(550,320,40,0,MyRPort_PTR[1],1,"J");
    }
    MAKEWINBORDER(MyRPort_PTR[1],540,139,570,312,14,40,1);

    // new .. audio-option!!!
    WRITE(507,410,40,WRITE_Center,MyRPort_PTR[1],3,"Audio");
    RECT_RP1(0,442,431,504,454);
    WRITE(473,435,40,WRITE_Center,MyRPort_PTR[1],3,_PT_ein);
    RECT_RP1(0,510,431,572,454);
    WRITE(541,435,40,WRITE_Center,MyRPort_PTR[1],3,_PT_aus);
    // ----

    CHECKGADS(0);

    ScreenToFront(MyScreen[1]);
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(1,300);
            if ((MouseX(1)>=440) && (MouseX(1)<=574) && (MouseY(1)>=431) && (MouseY(1)<=454))
            {
                if      (MouseX(1) < 507) { Audio_enable = true; }
                else if (MouseX(1) > 507) { Audio_enable = false; }
                CHECKGADS(8);
            } else if ((1 == Mode) && (MouseX(1)>=550) && (MouseX(1)<=580)
                                   && (MouseY(1)>=120) && (MouseY(1)<=330))
            {
                if      ((MouseY(1) < 141) && (Player<5)) { ++Player; }
                else if ((MouseY(1) > 309) && (Player>1)) { --Player; }
                CHECKGADS(7);
            } else if ((1 == Mode) && (MouseX(1)>=455) && (MouseX(1)<=485)
                                   && (MouseY(1)>=120) && (MouseY(1)<=330))
            {
                if      ((MouseY(1) < 141) && (Level<10)) { ++Level; }
                else if ((MouseY(1) > 309) && (Level> 1)) { --Level; }
                CHECKGADS(6);
            } else {
                if ((MouseY(1)>=80) && (MouseY(1)<=144))
                {
                    if      ((MouseX(1)>= 60) && (MouseX(1)<=124)) { Save.PlayMySelf = true; }
                    else if ((MouseX(1)>=150) && (MouseX(1)<=214)) { Save.PlayMySelf = false; }
                    CHECKGADS(1);
                }
                
                if ((MouseY(1)>=160) && (MouseY(1)<=224))
                {
                    if      ((MouseX(1)>= 60) && (MouseX(1)<=124)) { Save.SmallFight = true; }
                    else if ((MouseX(1)>=150) && (MouseX(1)<=214)) { Save.SmallFight = false; }
                    CHECKGADS(2);
                }
                
                if ((MouseY(1)>=240) && (MouseY(1)<=304))
                {
                    if      ((MouseX(1)>= 60) && (MouseX(1)<=124)) { Save.SmallLand = true; }
                    else if ((MouseX(1)>=150) && (MouseX(1)<=214)) { Save.SmallLand = false; }
                    CHECKGADS(3);
                }
                
                if ((MouseY(1)>=320) && (MouseY(1)<=384))
                {
                    if      ((MouseX(1)>= 60) && (MouseX(1)<=124)) { Save.NoWorm = true; }
                    else if ((MouseX(1)>=150) && (MouseX(1)<=214)) { Save.NoWorm = false; }
                    CHECKGADS(4);
                }
                
                if ((MouseY(1)>=400) && (MouseY(1)<=464))
                {
                    if      ((MouseX(1)>= 60) && (MouseX(1)<=124)) { Save.FastMove = true; }
                    else if ((MouseX(1)>=150) && (MouseX(1)<=214)) { Save.FastMove = false; }
                    CHECKGADS(5);
                }
            }
            while(LMB_PRESSED) {};
        }
    }
    while (RMB_NOTPRESSED);

    while (RMB_PRESSED) {};
    PLAYSOUND(1,300);

    ScreenToFront(XScreen);

    if ((Player>1) || (Save.CivPlayer[0] == 0))
    {
        MultiPlayer = true;
    } else {
        MultiPlayer = false;
        Save.CivPlayer[0] = 1;
    }
}
