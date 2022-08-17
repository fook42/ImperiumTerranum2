#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FUCKSYSTEM(uint8 ActSys)
{
    int             i, j;
    ByteArr42*      ActPPRoject;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    
    for(i = 0; i < SystemHeader[ActSys-1].Planets; i++)
    {
        MyPlanetHeader = &(SystemHeader[ActSys-1].PlanetMemA[i]);

        if (0 != ClassLifeFactor[MyPlanetHeader->Class])
        {
            if (NULL != MyPlanetHeader->ProjectPtr)
            {
                ActPPRoject = (ByteArr42*) MyPlanetHeader->ProjectPtr;
                for(j = 1; j <= 7; j++)
                {
                    if (0 < ActPPRoject->data[j])
                    {
                        Save.ProjectCosts[GETCIVVAR(MyPlanetHeader->PFlags)-1][j] = abs(Year)*j*10;
                    }
                }
                for(j = 8; j <= 42; j++)
                {
                    ActPPRoject->data[j] = 0;
                }
            }
            MyPlanetHeader->Class = CLASS_DESERT;
            MyPlanetHeader->Water = MyPlanetHeader->Size*12;
            MyPlanetHeader->PFlags = 0;
            MyPlanetHeader->Ethno = 0;
            MyPlanetHeader->Population = 0;
            MyPlanetHeader->Biosphaere = 0;
            MyPlanetHeader->Industrie = 0;
            MyPlanetHeader->Infrastruktur = 0;
            MyPlanetHeader->XProjectCosts = 0; 
            MyPlanetHeader->XProjectPayed = 0;
            MyPlanetHeader->ProjectID = 0;
        }
        MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
        while (NULL != MyShipPtr)
        {
            MyShipPtr->Owner = 0;
            MyShipPtr = MyShipPtr->NextShip;
        }
        MyShipPtr = SystemHeader[ActSys-1].FirstShip.NextShip;
        if (NULL != MyShipPtr)
        {
            SystemHeader[ActSys-1].FirstShip.SType = 0;
            do
            {
                MyShipPtr->Owner = 0;
                MyShipPtr = MyShipPtr->NextShip;
            }
            while (NULL != MyShipPtr);
        }
    }
    DRAWSYSTEM(MODE_REDRAW,ActSys,NULL);
    for(i = 1; i < 6; i++)
    {
        SetRGB32(&(MyScreen[0]->ViewPort),0,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF);
        PLAYSOUND(1,1200+rand()%250);
        Delay((rand()%10)+1);
        SetRGB32(&(MyScreen[0]->ViewPort),0,0,0,0);
        Delay((rand()%10)+1);
    }
    Delay(100);
}

void SD_TEXTWINDOW(const char* TextPtr1,const char* TextPtr2,const char* TextPtr3)
{
    struct Window*   STD_Window;
    struct RastPort* RPort_PTR;

    STD_Window=MAKEWINDOWBORDER(50,100,411,81,MyScreen[0]);
    if (NULL != STD_Window)
    {
        RPort_PTR = STD_Window->RPort;

        WRITE(206,10,ActPlayerFlag,WRITE_Center|JAM1,RPort_PTR,3,TextPtr1);
        WRITE(206,30,ActPlayerFlag,WRITE_Center|JAM1,RPort_PTR,3,TextPtr2);
        WRITE(206,50,ActPlayerFlag,WRITE_Center|JAM1,RPort_PTR,3,TextPtr3);
        WAITLOOP(false);
        CloseWindow(STD_Window);
    }
}


void STARDESASTER(uint8 ActSys, r_ShipHeader* ShipPtr)
{
    r_ShipHeader*   MyShipPtr;
    r_PlanetHeader* MyPlanetHeader;
    ByteArr42*      ActPPRoject;
    const char      Nvokal[] = {"aeiou"};
    const char      Nkonsonant[] = {"bcdfghjklmnpqrstvwxyz"};
    const uint8     NvokalLen = sizeof(Nvokal)-1;
    const uint8     NkonsoLen = sizeof(Nkonsonant)-1;
    int             i, j, k;
    bool            system_too_close;

    const uint8 PFlags[] = {FLAG_TERRA, FLAG_KLEGAN, FLAG_REMALO, FLAG_CARDAC, FLAG_BAROJA, FLAG_VOLKAN, FLAG_FERAGI};

    // MyShipPtr = ShipPtr;
    if (WFLAG_FIELD == Save.WorldFlag)
    {
        PLAYSOUND(2,420);
        Save.Systems = MAXSYSTEMS;
        Save.WorldFlag = 0;

        SD_TEXTWINDOW(PText[391], PText[392], PText[393]);
        return;
    }
    Save.ImperatorState[ActPlayer-1] -= 500;
    if (0 != Save.WorldFlag)
    {
        FUCKSYSTEM(ActSys);
        return;
    }
    Save.GSteuer[8] = 0;

    switch(rand()%5) {
        case 0: FUCKSYSTEM(ActSys); break;  /* SONNENSYS ZERSTÖRT */
        case 1: {        /* NEUVERTEILUNG DER KRÄFTE */
                    PLAYSOUND(2,420);
                    Year = Year-(rand()%250)+75;

                    for(i = 0; i < Save.Systems; i++)
                    {
                        do
                        {
                            SystemX[i] = 10+(rand()%458);
                            SystemY[i] = 10+(rand()%490);
                            system_too_close = false;
                            // check if the new system is too close to any previous one..
                            // reduced calculation: dist^2=x^2+y^2 ... x,y = delta x,y
                            // if x+y < 30 -> dist is max. sqrt(550)=~ 23
                            for (j = 0; j < i; j++)
                            {
                                if ((abs(SystemX[i]-SystemX[j])+abs(SystemY[i]-SystemY[j])) < 30)
                                {
                                    system_too_close = true;
                                    break;
                                }
                            }
                        }
                        while (system_too_close);
                    }
                    for(j = 0; j < 7; ++j)
                    {
                        for(i = 0;  i < 7;  ++i) { Save.WarState[j][i] = LEVEL_COLDWAR; }
                        for(i = 1;  i < 26; ++i) { Save.TechCosts[j][i]    = 0; }
                        for(i = 26; i < 43; ++i) { Save.TechCosts[j][i]    = i*2500; }
                        for(i = 2;  i < 8;  ++i) { Save.ProjectCosts[j][i] = i*100000; }
                        Save.ProjectCosts[j][39] = 40000;
                        Save.ActTech[j] = 0;
                    }
                    for(i = 0; i < Save.Systems; ++i)
                    {
                        SystemHeader[i].FirstShip.SType = 0;
                        SystemHeader[i].vNS = 0;
                        for(j = 0; j < SystemHeader[i].Planets; ++j)
                        {
                            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                            if (0 == MyPlanetHeader->PFlags) { continue; }  // only inhabited planets get a new owner

                            MyPlanetHeader->PFlags = PFlags[rand()%7];
                            MyPlanetHeader->Ethno = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                            MyPlanetHeader->Biosphaere    -= rand()%20;
                            MyPlanetHeader->Infrastruktur -= rand()%20;
                            MyPlanetHeader->Industrie     -= rand()%20;
                            if (NULL != MyPlanetHeader->ProjectPtr)
                            {
                                ActPPRoject = (ByteArr42*) MyPlanetHeader->ProjectPtr;
                                ActPPRoject->data[2] = 0;
                                ActPPRoject->data[PROJECT_VON_NEUMANN] = 0;
                                for(k = 1; k < PROJECT_NOMORE; ++k)
                                {
                                    if (0 < ProjectNeedsTech[k])
                                    {
                                        ActPPRoject->data[k] = 0;
                                    }
                                }
                            }
                            MyPlanetHeader->ProjectID = PROJECT_CLEAR_BIOPHERE;
                            MyPlanetHeader->Population = it_round(MyPlanetHeader->Population*0.7);
                            MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                            while (NULL != MyShipPtr)
                            {
                                MyShipPtr->Owner = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                                MyShipPtr = MyShipPtr->NextShip;
                            }
                        }
                    }
                    SD_TEXTWINDOW(PText[394], _PT_Der_Spieler_wurde, PText[396]);
                } break;
        case 2: {   /* CEBORCS */
                    PLAYSOUND(2,420);
                    CEBORCATTACK(0);
                    SD_TEXTWINDOW(_PT_Der_Spieler_wurde, PText[398], PText[399]);
                } break;
        case 3: {   /* DCON-IMPERIUM */
                    PLAYSOUND(2,420);
                    while (Year>(-40000))
                    {
                        Year -= (rand()%89) * (rand()%89);
                    }
                    Save.Bevoelkerung[7] = 100;
                    Save.WorldFlag = WFLAG_DCON;
                    for(i = 0; i < (MAXCIVS-1); ++i)
                    {
                        Save.WarState[7][i] = LEVEL_PEACE;
                        Save.WarState[i][7] = LEVEL_PEACE;
                    }

                    for(i = 0; i < MAXSYSTEMS; ++i)
                    {
                        if ((i+1) != ActSys)
                        {
                            Save.SystemName[i][0]='D';
                            Save.SystemName[i][1]='c';
                            Save.SystemName[i][2]=Nvokal[rand()%NvokalLen];
                            Save.SystemName[i][3]=Nkonsonant[rand()%NkonsoLen];
                            Save.SystemName[i][4]=Nvokal[rand()%NvokalLen];
                            Save.SystemName[i][5]=Nkonsonant[rand()%NkonsoLen];
                            Save.SystemName[i][6]=0;
                            // Dc + vokal + kons + vokal + kons 
                            SystemHeader[i].FirstShip.SType = 0;
                            SystemHeader[i].FirstShip.Owner = 0;
                            MyShipPtr = SystemHeader[i].FirstShip.NextShip;
                            while (NULL != MyShipPtr)
                            {
                                MyShipPtr->Owner = FLAG_OTHER;
                                MyShipPtr = MyShipPtr->NextShip;
                            }
                            for(j = 0; j < SystemHeader[i].Planets; ++j)
                            {
                                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                                MyPlanetHeader->PName[0]='D';
                                MyPlanetHeader->PName[1]='c';
                                MyPlanetHeader->PName[2]=Nvokal[rand()%NvokalLen];
                                MyPlanetHeader->PName[3]=Nkonsonant[rand()%NkonsoLen];
                                MyPlanetHeader->PName[4]=0;
                                // Dc + vokal + kons
                                if (0 != MyPlanetHeader->PFlags)
                                {
                                    MyPlanetHeader->PFlags = FLAG_OTHER;
                                    MyPlanetHeader->Ethno  = FLAG_OTHER;
                                    MyPlanetHeader->Biosphaere    = 200;
                                    MyPlanetHeader->Infrastruktur = 190;
                                    MyPlanetHeader->Industrie     = 180;
                                    if (NULL != MyPlanetHeader->ProjectPtr)
                                    {
                                        ActPPRoject = (ByteArr42*) MyPlanetHeader->ProjectPtr;
                                        // for(k = 0; k < 7; k++)
                                        // {
                                            // ...todo ... 7* same comparison ? ... ProjectPtr->data[i] makes no sense.. 
                                            if ((0 < ActPPRoject->data[i+1]) && (0 == Save.ProjectCosts[7][i+1]))
                                            {
                                                ActPPRoject->data[i+1] = 0;
                                            } else {
                                                Save.ProjectCosts[7][i+1] = 0;
                                            }
                                        // }
                                    }
                                    MyPlanetHeader->ProjectID = PROJECT_CLEAR_BIOPHERE;
                                    MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                                    while (NULL != MyShipPtr)
                                    {
                                        MyShipPtr->Owner = FLAG_OTHER;
                                        MyShipPtr = MyShipPtr->NextShip;
                                    }
                                }
                            }
                        }
                    }

                    for(i = 1; i < 26; ++i)
                    {
                        Save.TechCosts[7][i] = 0;
                    }
                    SD_TEXTWINDOW(_PT_Der_Spieler_wurde, PText[401], PText[402]);
                } break;
        case 4: {   /* ZEIT-ANOMALIE */
                    PLAYSOUND(2,420);
                    Save.WorldFlag = WFLAG_FIELD;
                    SD_TEXTWINDOW(PText[404], PText[405], PText[406]);
                } break;
        default: {}
    }
    REFRESHDISPLAY();
}
