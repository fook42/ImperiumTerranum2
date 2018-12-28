#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void FUCKSYSTEM(uint8 ActSys)
{
    uint8           i, j;
    ByteArr42*      ActPPRoject;
    r_PlanetHeader* MyPlanetHeader;
    r_ShipHeader*   MyShipPtr;
    
    for(i = 0; i < SystemHeader[ActSys-1].Planets; i++)
    {
        MyPlanetHeader = &(SystemHeader[ActSys-1].PlanetMemA[i]);

        if ((CLASS_GAS     != MyPlanetHeader->Class)
         && (CLASS_SATURN  != MyPlanetHeader->Class)
         && (CLASS_PHANTOM != MyPlanetHeader->Class))
        {
            if (NULL != MyPlanetHeader->ProjectPtr)
            {
                ActPPRoject = (ByteArr42*) MyPlanetHeader->ProjectPtr;
                for(j = 1; j <= 7; j++)
                {
                    if (0 < ActPPRoject->data[j])
                    {
                        Save.ProjectCosts[GETCIVVAR(MyPlanetHeader->PFlags)-1].data[j] = abs(Year)*j*10;
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
    for(i = 1; i <= 5; i++)
    {
        SetRGB32(&(MyScreen[0]->ViewPort),0,0xFF000000,0xFF000000,0xFF000000);
        PLAYSOUND(2,1200+rand()%250);
        delay((rand()%10)+1);
        SetRGB32(&(MyScreen[0]->ViewPort),0,0,0,0);
        delay((rand()%10)+1);
    }
    delay(100);
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
    uint8           i, j, k;
    bool            system_too_close;
    struct Window*   STD_Window;
    struct RastPort* RPort_PTR;


    // MyShipPtr = ShipPtr;
    if (WFLAG_FIELD == Save.WorldFlag)
    {
        PLAYSOUND(3,420);
        Save.Systems = MAXSYSTEMS;
        Save.WorldFlag = 0;

        STD_Window=MAKEWINDOW(50,100,411,81,MyScreen[0]);
        if (NULL != STD_Window)
        {
            RPort_PTR = STD_Window->RPort;
            MAKEWINBORDER(RPort_PTR,0,0,410,80,12,6,1);
            WRITEWIN(206,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[391]);
            WRITEWIN(206,30,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[392]);
            WRITEWIN(206,50,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[393]);
            WAITLOOP(false);
            CloseWindow(STD_Window);
        }
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
                    PLAYSOUND(3,420);
                    Year = Year-(rand()%250)+75;
                    STD_Window=MAKEWINDOW(50,100,411,81,MyScreen[0]);
                    RPort_PTR = STD_Window->RPort;
                    MAKEWINBORDER(RPort_PTR,0,0,410,80,12,6,1);

                    WRITEWIN(206,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[394]);
                    WRITEWIN(206,30,ActPlayerFlag,WRITE_Center,RPort_PTR,4,_PT_Der_Spieler_wurde);
                    WRITEWIN(206,50,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[396]);
                    for(i = 0; i < Save.Systems; i++)
                    {
                        do
                        {
                            SystemX[i] = 10+(rand()%250)+(rand()%208);
                            SystemY[i] = 10+(rand()%250)+(rand()%240);
                            system_too_close = false;
                            // check if the new system is too close to any previous one..
                            // reduced calculation: dist^2=x^2+y^2 ... x,y = delta x,y
                            // if x+y < 30 -> dist is max. sqrt(550)=~ 23
                            for (j = 0; j < i; j++)
                            {
                                if ((abs(SystemX[i]-SystemX[j])+abs(SystemY[i]-SystemY[j])) < 30)
                                {
                                    system_too_close = true;
                                }
                            }
                        }
                        while (system_too_close);
                    }
                    for(j = 0; j < 7; j++)
                    {
                        for(i = 0;  i < 7;   i++) { Save.WarState[j][i] = LEVEL_COLDWAR; }
                        for(i = 1;  i <= 25; i++) { Save.TechCosts[j].data[i]    = 0; }
                        for(i = 26; i <= 42; i++) { Save.TechCosts[j].data[i]    = i*2500; }
                        for(i = 2;  i <= 7;  i++) { Save.ProjectCosts[j].data[i] = i*100000; }
                        Save.ProjectCosts[j].data[39] = 40000;
                        Save.ActTech[j] = 0;
                    }
                    for(i = 0; i < Save.Systems; i++)
                    {
                        SystemHeader[i].FirstShip.SType = 0;
                        SystemHeader[i].vNS = 0;
                        for(j = 0; j < SystemHeader[i].Planets; j++)
                        {
                            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                            if (0 != MyPlanetHeader->PFlags)
                            {
                                switch(rand()%7) {
                                    case 0: MyPlanetHeader->PFlags = FLAG_TERRA;  break;
                                    case 1: MyPlanetHeader->PFlags = FLAG_KLEGAN; break;
                                    case 2: MyPlanetHeader->PFlags = FLAG_REMALO; break;
                                    case 3: MyPlanetHeader->PFlags = FLAG_CARDAC; break;
                                    case 4: MyPlanetHeader->PFlags = FLAG_BAROJA; break;
                                    case 5: MyPlanetHeader->PFlags = FLAG_VOLKAN; break;
                                    case 6: MyPlanetHeader->PFlags = FLAG_FERAGI; break;
                                    default: {}
                                }
                                MyPlanetHeader->Ethno = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                                MyPlanetHeader->Biosphaere    -= rand()%20;
                                MyPlanetHeader->Infrastruktur -= rand()%20;
                                MyPlanetHeader->Industrie     -= rand()%20;
                                if (NULL != MyPlanetHeader->ProjectPtr)
                                {
                                    ActPPRoject = (ByteArr42*) MyPlanetHeader->ProjectPtr;
                                    ActPPRoject->data[2] = 0;
                                    ActPPRoject->data[39] = 0;
                                    for(k = 1; k <= 42; k++)
                                    {
                                        if (0 < ProjectNeedsTech[k])
                                        {
                                            ActPPRoject->data[k] = 0;
                                        }
                                    }
                                }
                                MyPlanetHeader->ProjectID = -3;
                                MyPlanetHeader->Population = it_round(MyPlanetHeader->Population*0.7);
                                MyShipPtr = MyPlanetHeader->FirstShip.NextShip;
                                while (NULL != MyShipPtr)
                                {
                                    MyShipPtr->Owner = (MyPlanetHeader->PFlags & FLAG_CIV_MASK);
                                    MyShipPtr = MyShipPtr->NextShip;
                                }
                            }
                        }
                    }
                    WAITLOOP(false);
                    CloseWindow(STD_Window);
                } break;
        case 2: {   /* CEBORCS */
                    PLAYSOUND(3,420);
                    CEBORCATTACK(0);
                    STD_Window=MAKEWINDOW(50,100,411,81,MyScreen[0]);
                    if (NULL != STD_Window)
                    {
                        RPort_PTR = STD_Window->RPort;
                        MAKEWINBORDER(RPort_PTR,0,0,410,80,12,6,1);
                        WRITEWIN(206,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,_PT_Der_Spieler_wurde);
                        WRITEWIN(206,30,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[398]);
                        WRITEWIN(206,50,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[399]);
                        WAITLOOP(false);
                        CloseWindow(STD_Window);
                    }

                } break;
        case 3: {   /* DCON-IMPERIUM */
                    PLAYSOUND(3,420);
                    STD_Window=MAKEWINDOW(50,100,411,81,MyScreen[0]);
                    RPort_PTR = STD_Window->RPort;
                    MAKEWINBORDER(RPort_PTR,0,0,410,80,12,6,1);

                    WRITEWIN(206,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,_PT_Der_Spieler_wurde);
                    WRITEWIN(206,30,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[401]);
                    WRITEWIN(206,50,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[402]);
                    while (Year>(-40000))
                    {
                        Year -= (rand()%89) * (rand()%89);
                    }
                    Save.Bevoelkerung[7] = 100;
                    Save.WorldFlag = WFLAG_DCON;
                    for(i = 0; i < (MAXCIVS-1); i++)
                    {
                        Save.WarState[7][i] = LEVEL_PEACE;
                        Save.WarState[i][7] = LEVEL_PEACE;
                    }

                    for(i = 0; i < MAXSYSTEMS; i++)
                    {
                        if ((i+1) != ActSys)
                        {
                            Save.SystemName.data[i][0]='D';
                            Save.SystemName.data[i][1]='c';
                            Save.SystemName.data[i][2]=Nvokal[rand()%NvokalLen];
                            Save.SystemName.data[i][3]=Nkonsonant[rand()%NkonsoLen];
                            Save.SystemName.data[i][4]=Nvokal[rand()%NvokalLen];
                            Save.SystemName.data[i][5]=Nkonsonant[rand()%NkonsoLen];
                            Save.SystemName.data[i][6]=0;
                            // Dc + vokal + kons + vokal + kons 
                            SystemHeader[i].FirstShip.SType = 0;
                            SystemHeader[i].FirstShip.Owner = 0;
                            MyShipPtr = SystemHeader[i].FirstShip.NextShip;
                            while (NULL != MyShipPtr)
                            {
                                MyShipPtr->Owner = FLAG_OTHER;
                                MyShipPtr = MyShipPtr->NextShip;
                            }
                            for(j = 0; j < SystemHeader[i].Planets; j++)
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
                                            if ((ActPPRoject->data[i+1]>0) && (Save.ProjectCosts[7].data[i+1] == 0))
                                            {
                                                ActPPRoject->data[i+1] = 0;
                                            } else {
                                                Save.ProjectCosts[7].data[i+1] = 0;
                                            }
                                        // }
                                    }
                                    MyPlanetHeader->ProjectID = -3;
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

                    for(i = 1; i <= 25; i++)
                    {
                        Save.TechCosts[7].data[i] = 0;
                    }
                    WAITLOOP(false);
                    CloseWindow(STD_Window);
                } break;
        case 4: {   /* ZEIT-ANOMALIE */
                    PLAYSOUND(3,420);
                    Save.WorldFlag = WFLAG_FIELD;
                    STD_Window=MAKEWINDOW(50,100,411,81,MyScreen[0]);
                    if (NULL != STD_Window)
                    {
                        RPort_PTR = STD_Window->RPort;
                        MAKEWINBORDER(RPort_PTR,0,0,410,80,12,6,1);
                        WRITEWIN(206,10,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[404]);
                        WRITEWIN(206,30,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[405]);
                        WRITEWIN(206,50,ActPlayerFlag,WRITE_Center,RPort_PTR,4,PText[406]);
                        WAITLOOP(false);
                        CloseWindow(STD_Window);
                    }
                } break;
        default: {}
    }
    REFRESHDISPLAY();
}
