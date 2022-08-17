#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void CHECKGADS(int GadID)
{
    int     i, y;
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
        RECT_RP1_C0(542,141,568,310);
        txt[0]=Player+'0';
        txt[1]=0;
        WRITE_RP1(555,(5-Player)*38+142,40,WRITE_Center,3,txt);
        if (7 == GadID) { return; }
    }
    if ((0 == GadID) || (6 == GadID))
    {
        RECT_RP1_C0(447,141,473,310);
        RECT_RP1_C0(412,341,508,360);
        if (Level<10)
        {
            txt[0]=Level+'0';
            txt[1]=0;
        } else {
            txt[0]=(Level/10)+'0';
            txt[1]=(Level%10)+'0';
            txt[2]=0;
        }
        WRITE_RP1(460,(10-Level)*17+142,40,WRITE_Center,3, txt);
        WRITE_RP1(460,              343,40,WRITE_Center,3, PText[Level+491]);
        if (6 == GadID) { return; }
    }
    if (0 == GadID)
    {
        y = 113;
        for (i = 0; i < 5; ++i)
        {
            RECT_RP1_C0( 238, y, 388, y+19);
            y += 80;
        }
    } else {
        RECT_RP1_C0(238,33+GadID*80,388,52+GadID*80);
    }
    if ((0 == GadID) || (1 == GadID))
    {
        if (Save.PlayMySelf)
        {
            DRAWGAD(80,1);
            WRITE_RP1(240,115,40,1,3,PText[505]);
        } else {
            DRAWGAD(80,2);
            WRITE_RP1(240,115,40,1,3,PText[506]);
        }
    }
    if ((0 == GadID) || (2 == GadID))
    {
        if (Save.SmallFight)
        {
            DRAWGAD(160,1);
            WRITE_RP1(240,195,40,1,3,_PT_aus);
        } else {
            DRAWGAD(160,2);
            WRITE_RP1(240,195,40,1,3,_PT_ein);
        }
    }
    if ((0 == GadID) || (3 == GadID))
    {
        if (Save.SmallLand)
        {
            DRAWGAD(240,1);
            WRITE_RP1(240,275,40,1,3,PText[505]);
        } else {
            DRAWGAD(240,2);
            WRITE_RP1(240,275,40,1,3,_PTx_Player);
        }
    }
    if ((0 == GadID) || (4 == GadID))
    {
        if (Save.NoWorm)
        {
            DRAWGAD(320,1);
            WRITE_RP1(240,355,40,1,3,PText[505]);
        } else {
            DRAWGAD(320,2);
            WRITE_RP1(240,355,40,1,3,_PTx_Player);
        }
    }
    if ((0 == GadID) || (5 == GadID))
    {
        if (Save.FastMove)
        {
            DRAWGAD(400,1);
            WRITE_RP1(240,435,40,1,3,PText[510]);
        } else {
            DRAWGAD(400,2);
            WRITE_RP1(240,435,40,1,3,PText[511]);
        }
    }
}

void OPTION_REDUCECOSTS(void)
{
    // reduce Project/Tech Costs according to selected Level
    uint8   i, j;
    sint16  Factor;
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
                    Save.TechCosts[j][i]    -= (Save.TechCosts[j][i]   *Factor)>>10;
                    Save.ProjectCosts[j][i] -= (Save.ProjectCosts[j][i]*Factor)>>10;
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
                    Save.TechCosts[j][i]    -= (Save.TechCosts[j][i]   *Factor)>>10;
                    Save.ProjectCosts[j][i] -= (Save.ProjectCosts[j][i]*Factor)>>10;
                }
            }
        }
    }
    // if Level == 5 .. do nothing.. all values are okay.
}

void OPTION_MULTIPLAYER(void)
{
    uint8   i, j, btx;
    uint16  offset;
    char    s[60];
    char*   _s1;
    char*   _s2;
    bool    b;

    // randomly exchange one StarSystemName with the first one (Solar-system)
    i = 1+(rand()%(MAXSYSTEMS-1));
    (void) my_strcpy(s, Save.SystemName[i]);
    (void) my_strcpy(Save.SystemName[i], Save.SystemName[0]);
    (void) my_strcpy(Save.SystemName[0], s);

    // pick the civilization names for each player...
    _s1=my_strcpy(s, _PTx_Player);
    *_s1++ = ' ';
    (void) my_strcpy(_s1+2, PText[520]);
    for (i = 0; i < Player; ++i)
    {
        SWITCHDISPLAY();
        INITMENU();
        *_s1     = i + '1';
        *(_s1+1) = ' ';
        WRITE_RP1(320,50,40,WRITE_Center,3,s);
        btx = 7;
        if (0 == i)
        {
            btx = 1;
            Save.CivPlayer[0] = 0;
        }
        *(_s1+1) = 0;
        offset = 100;
        for (j = 0; j < btx; ++j)
        {
            if (0 == Save.CivPlayer[j])
            {
                MAKEWINBORDER(MyRPort_PTR[1],100, offset, 540, offset+30,14,40,0);
                _s2 = GETCIVNAME(j+1);
            } else {
                RECT_RP1_C0( 100, offset, 540, offset+30);
                *_s1 = Save.CivPlayer[j]+'0';
                _s2 = s;
            }
            WRITE_RP1(320, offset+8,40,WRITE_Center,3, _s2);
            offset += 50;
        }

        b = false;
        ScreenToFront(MyScreen[1]);
        do
        {
            Delay(RDELAY);
            if (LMB_PRESSED && (MouseX(1)>=100) && (MouseX(1)<=540))
            {
                offset = 100;
                for (j = 0; j < btx; ++j)
                {
                    if ((MouseY(1)>=offset) && (MouseY(1)<=(offset+30)) && (0 == Save.CivPlayer[j]))
                    {
                        b = true;
                        Save.CivPlayer[j] = i+1;
                        CLICKRECT(MyRPort_PTR[1], 100, offset, 540, offset+30, 40);
                        PLAYERJINGLE(j);
                    }
                    offset += 50;
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
    WRITE_RP1(320,100,40,WRITE_Center,3,PText[521]);
    offset = 123;
    for (i = 0; i < 5; ++i)
    {
        MAKEWINBORDER(MyRPort_PTR[1], offset, 200, offset+40, 240,14,40,0);
        s[0] = i+'1';
        s[1] = 0;
        WRITE_RP1(offset+20,213,40,WRITE_Center,3,s);
        offset += 88;
    }

    ScreenToFront(MyScreen[1]);
    HomePlanets = 99;
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED && (MouseY(1)>=200) && (MouseY(1)<=240)) 
        {
            offset = 123;
            for (i = 0; i < 5; ++i)
            {
                if ((MouseX(1)>offset) && (MouseX(1)<(offset+40)))
                {
                    HomePlanets = i;
                    break;
                }
                offset += 88;
            }
        }
    }
    while (5 < HomePlanets);

    ++HomePlanets;

    CLICKRECT(MyRPort_PTR[1],offset,200,offset+40,240,40);

    OPTION_REDUCECOSTS();

    ScreenToFront(XScreen);
}


void OPTIONMENU(int Mode)
{
    int     i, y;
    const sint32 bitmap_srcCord[5][2] = { {384,448},{384,448},{384,512},{384,448},{576,512} };
    const char*  button_txt[5] = {_PTx_Player, PText[514], PText[515], PText[516], PText[517]};

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
    WRITE_RP1(320,55,40,WRITE_Center,3,PText[513]);

    // draw left 5*2 images
    y = 80;
    for (i = 0; i < 5; ++i)
    {
        MAKEWINBORDER(MyRPort_PTR[1], 236, y+31, 390, y+54, 14, 40, 1);
        BltBitMapRastPort((struct BitMap *) &ImgBitMap8, bitmap_srcCord[i][0], 128, MyRPort_PTR[1], 60, y, 64, 64, 192);
        BltBitMapRastPort((struct BitMap *) &ImgBitMap8, bitmap_srcCord[i][1], 128, MyRPort_PTR[1],150, y, 64, 64, 192);
        WRITE_RP1(240,y+10,40,0,3,button_txt[i]);
        y += 80;
    }

    WRITE_RP1(460,100,40,WRITE_Center,3,"Level");
    if (1 == Mode)
    {
        WRITE_RP1(455,120,40,0,1,"I");
        WRITE_RP1(455,320,40,0,1,"J");
    }
    MAKEWINBORDER(MyRPort_PTR[1],445,139,475,312,14,40,1);
    MAKEWINBORDER(MyRPort_PTR[1],410,339,510,362,14,40,1);

    WRITE_RP1(555,100,40,WRITE_Center,3,_PTx_Player);
    if (1 == Mode)
    {
        WRITE_RP1(550,120,40,0,1,"I");
        WRITE_RP1(550,320,40,0,1,"J");
    }
    MAKEWINBORDER(MyRPort_PTR[1],540,139,570,312,14,40,1);

    // new .. audio-option!!!
    WRITE_RP1(507,410,40,WRITE_Center,3,"Audio");
    RECT_RP1_C0(442,431,504,454);
    WRITE_RP1(473,435,40,WRITE_Center,3,_PT_ein);
    RECT_RP1_C0(510,431,572,454);
    WRITE_RP1(541,435,40,WRITE_Center,3,_PT_aus);
    // ----

    CHECKGADS(0);

    ScreenToFront(MyScreen[1]);
    do
    {
        Delay(RDELAY);
        if (LMB_PRESSED)
        {
            PLAYSOUND(0,300);
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
    PLAYSOUND(0,300);

    ScreenToFront(XScreen);

    if ((1 < Player) || (0 == Save.CivPlayer[0]))
    {
        MultiPlayer = true;
    } else {
        MultiPlayer = false;
        Save.CivPlayer[0] = 1;
    }
}
