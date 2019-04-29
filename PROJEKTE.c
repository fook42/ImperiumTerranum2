#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PROJEKTE()
{
    uint8   i,k,CivVar;
    uint16  j;
    uint16  YPos[8];
    r_PlanetHeader* MyPlanetHeader;
	struct Window* PRJ_Window;
	struct RastPort* RPort_PTR;

	PRJ_Window=MAKEWINDOW(0,1,512,511,MyScreen[0]);
	if (NULL == PRJ_Window)
	{
		return;
	}
	RPort_PTR = PRJ_Window->RPort;
    MAKEWINBORDER(RPort_PTR,0,0,511,510,12,6,1);

    for (i = 1; i<4; i++)
    {
        WRITE(i*167+5,14,12,WRITE_Right,RPort_PTR,0,Project.data[i]);
        YPos[i] = 40;
    }
    for (i = 0; i < Save.Systems; i++)
    {
        if (NULL != SystemHeader[i].PlanetMemA)
        {
            for (j = 0; j<SystemHeader[i].Planets; j++)
            {
                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) != 0)
                {
                    CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
                    if ((Save.SSMoney[ActPlayer-1][CivVar-1] > Save.WarPower[CivVar-1]*39) || (CivVar == ActPlayer))
                    {
                        for (k = 1; k<4; k++)
                        {
                            if ((MyPlanetHeader->ProjectPtr->data[k] != 0) && (YPos[k]<500))
                            {
                                WRITE(k*167+5,YPos[k],GETCIVFLAG(CivVar),WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                                YPos[k] += 11;
                            }
                        }
                    }
                }
            }
        }
    }
    j = 0;
    for (i = 1; i<4; i++)
    {
        if (YPos[i]>j)
        {
            j = YPos[i];
        }
    }
    j += 15;
    if (j<500)
    {
        for (i = 1; i<8; i++)
        {
            YPos[i] = j;
        }
        SetAPen(RPort_PTR,12);
        Move(RPort_PTR, 10,YPos[1]-10);
        Draw(RPort_PTR,500,YPos[1]-10);
        for (i = 4; i<8; i++)
        {
            WRITE((i-3)*125+5,YPos[i],12,WRITE_Right,RPort_PTR,0,Project.data[i]);
            YPos[i] += 15;
        }

        for (i = 0; i < Save.Systems; i++)
        {
            if (NULL != SystemHeader[i].PlanetMemA)
            {
                for (j = 0; j<SystemHeader[i].Planets; j++)
                {
                    MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                    if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) != 0)
                    {
                        CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
                        if ((Save.SSMoney[ActPlayer-1][CivVar-1]>Save.WarPower[CivVar-1]*39) || (CivVar == ActPlayer))
                        {
                            for (k = 4; k<=7; k++)
                            {
                                if ((MyPlanetHeader->ProjectPtr->data[k] != 0) && (YPos[k]<500))
                                {
                                    WRITE((k-3)*125+5,YPos[k],GETCIVFLAG(CivVar),WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                                    YPos[k] += 11;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    j = 0;
    for (i = 1; i<8; i++)
    {
        if (YPos[i]>j)
        {
            j = YPos[i];
        }
    }
    j += 30;
    if (j<500)
    {
        for (i = 4; i<8; i++)
        {
            YPos[i] = j;
        }
        SetAPen(RPort_PTR,12);
        Move(RPort_PTR, 10,YPos[4]-25);
        Draw(RPort_PTR,500,YPos[4]-25);
        WRITE(255,YPos[4]-15,12,WRITE_Center,RPort_PTR,0,PText[698]);
        for (i = 0; i < Save.Systems; i++)
        {
            if (NULL != SystemHeader[i].PlanetMemA)
            {
                for (j = 0; j<SystemHeader[i].Planets; j++)
                {
                    MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                    if ((MyPlanetHeader->PFlags & FLAG_CIV_MASK) != 0)
                    {
                        CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
                        if ((Save.SSMoney[ActPlayer-1][CivVar-1]>Save.WarPower[CivVar-1]*39) || (CivVar == ActPlayer))
                        {
                            if ((MyPlanetHeader->ProjectID>=4) && (MyPlanetHeader->ProjectID<=7) && (YPos[MyPlanetHeader->ProjectID]<500))
                            {
                                WRITE((MyPlanetHeader->ProjectID-3)*125+5,YPos[MyPlanetHeader->ProjectID],GETCIVFLAG(CivVar),WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                                YPos[MyPlanetHeader->ProjectID] += 11;
                            }
                        }
                    }
                }
            }
        }
    }
    WAITLOOP(false);
	CloseWindow(PRJ_Window);
}
