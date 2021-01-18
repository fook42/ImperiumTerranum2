#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void VERHANDLUNG_INIT(uint8 CivFlag, char* CivStr, uint8 VI_Mode)
{
    MAKEWINBORDER(MyRPort_PTR[0],30,80,480,230,12,6,0);
    WRITE_RP0(256,90,CivFlag,WRITE_Center,3,CivStr);
    if (1 == VI_Mode)
    {
        DrawImage(MyRPort_PTR[0],&GadImg1, 60,200);
        DrawImage(MyRPort_PTR[0],&GadImg1,330,200);
        WRITE_RP0(118,202,0,WRITE_Center,3,_PT_Annehmen);
        WRITE_RP0(388,202,0,WRITE_Center,3,_PT_Ablehnen);
    }
}

void VERHANDLUNG(uint8 CivFlag, uint8 Mode)
{
    typedef struct TextArr4 {
        char*   data[4];
    } TextArr4;

    TextArr4    TArr4;
    char        s[56], s2[56];
    char*       CivStr = NULL;
    char*       _s;
    int         len1;
    uint16      ypos;
    uint8       XSystem, XTech, CivVar, i;
    uint32      XCosts;

    CivVar = GETCIVVAR(CivFlag);
    if ((WFLAG_CEBORC == Save.WorldFlag) && (FLAG_OTHER == CivFlag)) { return; }
    if (FLAG_MAQUES == CivFlag) { return; }
    if (CIVVAR_NONE == CivVar)  { return; }

    if (((MODE_OFFENSIV == Mode)
      || (MODE_DEFENSIV == Mode))
        && (Save.CivPlayer[CivVar] != 0))
    {
        Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_WAR;
        Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_WAR;
        return;
    }
    switch (CivFlag) {
        case FLAG_TERRA:  CivStr = PText[247]; break;
        case FLAG_KLEGAN: CivStr = PText[248]; break;
        case FLAG_REMALO: CivStr = PText[249]; break;
        case FLAG_CARDAC: CivStr = PText[250]; break;
        case FLAG_FERAGI: CivStr = PText[251]; break;
        case FLAG_BAROJA: CivStr = PText[252]; break;
        case FLAG_VOLKAN: CivStr = PText[253]; break;
        case FLAG_OTHER:{
                            switch (Save.WorldFlag) {
                                case WFLAG_DCON:    CivStr = PText[254]; break;
                                case WFLAG_JAHADR:  CivStr = PText[255]; break;
                                default:            CivStr = PText[256];
                            }
                        } break;
        default:    return;
    }
    XSystem = 0;
    for(i = 0; i < Save.Systems; ++i)
    {
        if ((SystemFlags[0][i] & FLAG_CIV_MASK) == ActPlayerFlag)
        {
            XSystem = i+1;
        }
    }
    XTech = 0;
    for (i = 42; i>0; --i)
    {
        if (   (Save.TechCosts[CivVar-1].data[i] > 0)
            && (Save.TechCosts[ActPlayer-1].data[i] <= 0))
        {
            XTech = i;
        }
    }
    XCosts = abs(Year) * 46;
    if ((CIVVAR_OTHER == CivVar) && (WFLAG_JAHADR == Save.WorldFlag))
    {
        if (Save.WarState[ActPlayer-1][7] != LEVEL_WAR)
        {
            VERHANDLUNG_INIT(CivFlag, CivStr, 1);
            i = 2*(1+(Save.WarPower[7] / Save.WarPower[ActPlayer-1]));
            if (MODE_FORCE == Mode)
            {
                strcpy(s, PText[260]);
                len1 = strlen(s);
                s[len1++]=' ';
                _s = GETCIVNAME(ActPlayer);
                strcpy(s+len1, _s);
                len1+= strlen(_s);
                s[len1++]=' ';
                strcpy(s+len1, PText[231]);
                WRITE_RP0(256,110,FLAG_OTHER,WRITE_Center,3,s);
                WRITE_RP0(256,130,FLAG_OTHER,WRITE_Center,3,PText[261]);
                WRITE_RP0(256,150,FLAG_OTHER,WRITE_Center,3,PText[262]);
            } else {
                WRITE_RP0(256,110,FLAG_OTHER,WRITE_Center,3,PText[263]);
                WRITE_RP0(256,130,FLAG_OTHER,WRITE_Center,3,PText[264]);
                WRITE_RP0(256,150,FLAG_OTHER,WRITE_Center,3,PText[265]);
            }
            if (0 == Save.JSteuer[ActPlayer-1])
            {
                strcpy(s, PText[266]);
                len1 = strlen(s);
                s[len1]=' ';
                _s = dez2out(i,0,s+len1+1);
                *_s++='%';
                *_s++=' ';
                strcpy(_s, PText[267]);
                _s = s;
            } else {
                _s = PText[268];
            }
            WRITE_RP0(256,170,FLAG_OTHER,WRITE_Center,3, _s);
            while (LMB_PRESSED) { };
            do
            {
                Delay(RDELAY);
            }
            while (LMB_NOTPRESSED || (MouseY(0)<200) || (MouseY(0)>220)
                                  || (((MouseX(0)<60)  || (MouseX(0)>176))
                                   && ((MouseX(0)<330) || (MouseX(0)>446))));
            RECT_RP0_C0(30,250,480,360);
            if ((MouseX(0)>=60) && (MouseX(0)<=176))
            {
                KLICKGAD(60,200);
                RECT_RP0_C0(30,80,480,230);
                if ((0   < Save.JSteuer[ActPlayer-1]) &&
                    (100 > Save.JSteuer[ActPlayer-1]))
                {
                    ++(Save.JSteuer[ActPlayer-1]);
                } else {
                    Save.JSteuer[ActPlayer-1] = i;
                }
                Save.WarState[7][ActPlayer-1] = LEVEL_PEACE;
                Save.WarState[ActPlayer-1][7] = LEVEL_PEACE;
                PEACEINFO(ActPlayer,8,ActPlayerFlag,FLAG_OTHER);
                return;
            }
            KLICKGAD(330,200);
            VERHANDLUNG_INIT(CivFlag, CivStr, 0);
            WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[270]);
            WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[271]);
            strcpy(s, PText[272]);
            strcat(s, " ");
            strcat(s, GETCIVADJ(ActPlayer));
            strcat(s, " ");
            strcat(s, PText[273]);
            WRITE_RP0(256,150,CivFlag,WRITE_Center,3,s);
            WAITLOOP(false);
            RECT_RP0_C0(30,80,480,230);
            Save.JSteuer[ActPlayer-1] = 0;
            Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_WAR;
            Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_WAR;
            return;
        } else if ((Save.WarPower[7]*2) > Save.WarPower[ActPlayer-1])
        {
            if (0 == (Year % 6))
            {
                VERHANDLUNG_INIT(CivFlag, CivStr, 1);
                WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[275]);
                WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[276]);
                WRITE_RP0(256,150,CivFlag,WRITE_Center,3,PText[277]);
                i = 2*(1+(Save.WarPower[7] / Save.WarPower[ActPlayer-1]));
                if (i>53) { i = 53; }

                len1 = strlen(PText[278]);
                memcpy(s, PText[278], len1);
                s[len1++]=' ';
                _s = dez2out(i, 0, s+len1);
                *_s++='%';
                *_s++=' ';
                strcpy(_s, PText[279]);

                WRITE_RP0(256,170,CivFlag,WRITE_Center,3,s);
                do
                {
                    Delay(RDELAY);
                }
                while (LMB_NOTPRESSED || (MouseY(0)<200) || (MouseY(0)>220)
                                      || (((MouseX(0)<60)  || (MouseX(0)>176))
                                       && ((MouseX(0)<330) || (MouseX(0)>446))));
                if ((MouseX(0)>59) && (MouseX(0)<177))
                {
                    KLICKGAD(60,200);
                    RECT_RP0_C0(30,80,480,230);
                    Save.JSteuer[ActPlayer-1] = i;
                    Save.WarState[7][ActPlayer-1] = LEVEL_PEACE;
                    Save.WarState[ActPlayer-1][7] = LEVEL_PEACE;
                    PEACEINFO(ActPlayer,8,ActPlayerFlag,FLAG_OTHER);
                    return;
                }
                KLICKGAD(330,200);
                RECT_RP0_C0(30,80,480,230);
                return;
            } else if (MODE_FORCE == Mode)
            {
                VERHANDLUNG_INIT(CivFlag, CivStr, 0);
                WRITE_RP0(256,115,CivFlag,WRITE_Center,3,PText[280]);
                WAITLOOP(false);
                RECT_RP0_C0(30,80,480,230);
                return;
            }
        } else if ((Save.WarPower[7]*2) > Save.WarPower[ActPlayer-1])
        {
            return;
        }
    }
    if (MODE_BELEIDIGUNG == Mode)
    {
        VERHANDLUNG_INIT(CivFlag, CivStr, 0);
        WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[282]);
        WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[283]);
        WRITE_RP0(256,150,CivFlag,WRITE_Center,3,PText[284]);
        WAITLOOP(false);
        RECT_RP0_C0(30,80,480,230);
        Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_WAR;
        Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_WAR;
        return;
    } else if ((MODE_OFFENSIV == Mode) ||
               (MODE_MONEY    == Mode) ||
               (MODE_FORCE    == Mode))
    {
        if (Save.WarState[CivVar-1][ActPlayer-1] != LEVEL_WAR)
        {
            VERHANDLUNG_INIT(CivFlag, CivStr, 1);
            if (MODE_MONEY != Mode)
            {
                Save.ImperatorState[ActPlayer-1] -= 50;
            }
            strcpy(s2, PText[285]);
            strcat(s2, " ");
            strcat(s2, GETCIVADJ(ActPlayer));
            strcat(s2, " ");
            strcat(s2, PText[286]);
            TArr4 = (TextArr4) {{s2, PText[287], NULL, NULL}};
            if (MODE_MONEY == Mode)
            {
                len1 = strlen(PText[290]);
                memcpy(s, PText[290], len1);
                s[len1++] = ' ';
                _s = dez2out(XCosts, 0, s+len1);
                *_s++=',';
                *_s++=' ';
                strcpy(_s, PText[291]);

                len1 = strlen(PText[292]);
                memcpy(s2, PText[292], len1);
                s2[len1++] = ' ';
                strcpy(s2+len1, GETCIVADJ(ActPlayer));

                TArr4 = (TextArr4) {{PText[293], s, s2, PText[294]}};
            } else if ((Save.WarPower[ActPlayer-1] < (Save.WarPower[CivVar-1]*3)) && (XSystem>0))
            {
                TArr4.data[2] = PText[295];
                strcpy(s, PText[174]);
                strcat(s, " ");
                strcat(s, Save.SystemName.data[XSystem-1]);
                strcat(s, ".");
                TArr4.data[3] = s;
                SYSINFO(XSystem,ActPlayerFlag);
            } else if ((Save.WarPower[ActPlayer-1] < (Save.WarPower[CivVar-1]*4)) && (XTech>0))
            {
                if ((XTech==12) || (XTech==13) || (XTech==15) || (XTech==18)
                  ||(XTech==20) || (XTech==23) || (XTech==24) || (XTech==28)
                  ||(XTech==30) || (XTech==33) || (XTech==36))
                {
                    strcpy(s, PText[296]);
                } else if ((XTech==3) || (XTech==17) || (XTech==21) || (XTech==25) || (XTech==31))
                {
                    strcpy(s, PText[297]);
                } else {
                    strcpy(s, PText[298]);
                }
                strcat(s, " ");
                strcat(s, TechnologyL.data[XTech]);
                strcat(s, ".");
                TArr4.data[2] = PText[299];
                TArr4.data[3] = s;
            } else {
                (void) dez2out(XCosts, 0, s);
                TArr4.data[2] = PText[300];
                TArr4.data[3] = s;
            }
            ypos = 110;
            for(i = 0; i < 4; ++i)
            {
                WRITE_RP0(256, ypos,CivFlag,WRITE_Center,3,TArr4.data[i]);
                ypos += 20;
            }
            do
            {
                Delay(RDELAY);
            }
            while (LMB_NOTPRESSED || (MouseY(0)<200) || (MouseY(0)>220)
                                  || (((MouseX(0)<60) || (MouseX(0)>176))
                                   && ((MouseX(0)<330)|| (MouseX(0)>446))));
            RECT_RP0_C0(30,250,480,360);  // clear SYSINFO-Area
            if ((MouseX(0)>59) && (MouseX(0)<177))
            {
                KLICKGAD(60,200);
                RECT_RP0_C0(30,80,480,230);
                if ((Save.WarPower[ActPlayer-1] < (Save.WarPower[CivVar-1]*2)) && (XSystem>0))
                {
                    SYSTEMTOENEMY(XSystem-1,CivFlag,ActPlayerFlag);
                } else if ((Save.WarPower[ActPlayer-1] < (Save.WarPower[CivVar-1]*3)) && (XTech>0))
                {
                    Save.TechCosts[XTech-1].data[CivVar] = 0;
                } else {
                    Save.Staatstopf[CivVar-1]    += XCosts;
                    Save.Staatstopf[ActPlayer-1] -= XCosts;
                }
                Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_PEACE;
                Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_PEACE;
                PEACEINFO(ActPlayer,CivVar,ActPlayerFlag,CivFlag);
            } else {
                KLICKGAD(330,200);
                VERHANDLUNG_INIT(CivFlag, CivStr, 0);
                WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[305]);
                WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[306]);
                strcpy(s, PText[298]);
                strcat(s, " ");
                strcat(s, GETCIVNAME(ActPlayer));
                strcat(s, " ");
                strcat(s, PText[307]);
                WRITE_RP0(256,150,CivFlag,WRITE_Center,3,s);
                WAITLOOP(false);
                RECT_RP0_C0(30,80,480,230);
                Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_WAR;
                Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_WAR;
            }
            return;
        }
        if ((Save.WarState[CivVar-1][ActPlayer-1] == LEVEL_WAR) && ((Save.WarPower[CivVar-1]*2) < Save.WarPower[ActPlayer-1])
            && ((0 == (Year % 6)) || (MODE_FORCE == Mode)))
        {
            VERHANDLUNG_INIT(CivFlag, CivStr, 1);
            WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[308]);
            WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[309]);
            do
            {
                Delay(RDELAY);
            }
            while (LMB_NOTPRESSED || (MouseY(0)<200) || (MouseY(0)>220)
                                  || (((MouseX(0)<60)  || (MouseX(0)>176))
                                   && ((MouseX(0)<330) || (MouseX(0)>446))));
            if ((MouseX(0)>=60) && (MouseX(0)<=176))
            {
                KLICKGAD(60,200);
                RECT_RP0_C0(30,80,480,250);
                Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_PEACE;
                Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_PEACE;
                PEACEINFO(ActPlayer,CivVar,ActPlayerFlag,CivFlag);
                return;
            }
            KLICKGAD(330,200);
            RECT_RP0_C0(30,80,480,250);
        } else if (MODE_FORCE == Mode)
        {
            MAKEWINBORDER(MyRPort_PTR[0],85,120,425,200,12,6,0);
            strcpy(s, PText[310]);
            strcat(s, " ");
            strcat(s, GETCIVNAME(ActPlayer));
            WRITE_RP0(256,140,CivFlag,WRITE_Center,3,s);
            WRITE_RP0(256,165,CivFlag,WRITE_Center,3,PText[311]);
            WAITLOOP(false);
            RECT_RP0_C0(85,120,425,200);
            REFRESHDISPLAY();
        }
    } else if ((MODE_DEFENSIV    == Mode) ||
               (MODE_TERRITORIUM == Mode))
    {
        if (0 == (rand()%3)) { return; }
        if (0 != XSystem)
        {
            VERHANDLUNG_INIT(CivFlag, CivStr, 1);
            if (MODE_TERRITORIUM == Mode)
            {
                /*  verurteilt die Verletzung seines Hoheitsgebietes
                    und verlangt als Entschädigung für diese Aggression
                    die bedingungslose Übergabe des
                */
                WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[315]);
                WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[316]);
                WRITE_RP0(256,150,CivFlag,WRITE_Center,3,PText[317]);
            } else {
                /*  weist auf die militärische Überlegenheit seiner
                    Zivilisation hin und verlangt als Vorbedingung
                    für einen Friedensvertrag die Übergabe des
                */
                WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[318]);
                WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[319]);
                WRITE_RP0(256,150,CivFlag,WRITE_Center,3,PText[320]);
            }
            strcpy(s, GETCIVADJ(ActPlayer));
            strcat(s, " ");
            strcat(s, PText[321]);      // "n Systems"
            strcat(s, " ");
            strcat(s, Save.SystemName.data[XSystem-1]);
            strcat(s, ".");
            WRITE_RP0(256,170,CivFlag,WRITE_Center,3,s);
            SYSINFO(XSystem,ActPlayerFlag);
            do
            {
                Delay(RDELAY);
            }
            while (LMB_NOTPRESSED || (MouseY(0)<200) || (MouseY(0)>220)
                                  || (((MouseX(0)<60)  || (MouseX(0)>176))
                                   && ((MouseX(0)<330) || (MouseX(0)>446))));
            RECT_RP0_C0(30,250,480,360); // clear SYSINFO-Area
            if ((MouseX(0)>59) && (MouseX(0)<177))
            {
                KLICKGAD(60,200);
                RECT_RP0_C0(30,80,480,230);
                SYSTEMTOENEMY(XSystem-1,CivFlag,ActPlayerFlag);
                Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_PEACE;
                Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_PEACE;
                if ((rand()%100)<25)
                {
                    XSystem = 0;
                } else {
                    PEACEINFO(ActPlayer,CivVar,ActPlayerFlag,CivFlag);
                    return;
                }
            } else {
                KLICKGAD(330,200);
                RECT_RP0_C0(30,80,480,230);
                if (LEVEL_WAR == Save.WarState[CivVar-1][ActPlayer-1])
                {
                    return;
                }
            }
            VERHANDLUNG_INIT(CivFlag, CivStr, 0);
            if (MODE_TERRITORIUM == Mode)
            {
                WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[325]);
                WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[326]);
                strcpy(s, "den Krieg gegen die ");
                strcat(s, GETCIVNAME(ActPlayer));
                strcat(s, "!");
                WRITE_RP0(256,150,CivFlag,WRITE_Center,3,s);
            } else {
                WRITE_RP0(256,110,CivFlag,WRITE_Center,3,PText[327]);
                WRITE_RP0(256,130,CivFlag,WRITE_Center,3,PText[328]);
                strcpy(s, PText[329]);
                strcat(s, " ");
                strcat(s, GETCIVADJ(ActPlayer));
                strcat(s, PText[330]);
                WRITE_RP0(256,150,CivFlag,WRITE_Center,3,s);
            }
            WAITLOOP(false);
            RECT_RP0_C0(30,80,480,230);
            Save.WarState[CivVar-1][ActPlayer-1] = LEVEL_WAR;
            Save.WarState[ActPlayer-1][CivVar-1] = LEVEL_WAR;
        }
    }
}
