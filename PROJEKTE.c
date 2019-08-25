#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PROJEKTE()
{
    uint8   i,k,CivVar;
    uint16  j, xpos;
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

    xpos = 172;
    for (i = 1; i<4; ++i)
    {
        WRITE(xpos,14,12,WRITE_Right,RPort_PTR,0,Project.data[i]);
        xpos += 167;
        YPos[i] = 40;
    }
    for (i = 0; i < Save.Systems; ++i)
    {
        if (NULL != SystemHeader[i].PlanetMemA)
        {
            for (j = 0; j<SystemHeader[i].Planets; ++j)
            {
                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                if (0 != (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
                {
                    CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
                    if ((Save.SSMoney[ActPlayer-1][CivVar-1] > (Save.WarPower[CivVar-1]*39)) || (CivVar == ActPlayer))
                    {
                        xpos = 172;
                        for (k = 1; k<4; ++k)
                        {
                            if (0 != (MyPlanetHeader->ProjectPtr->data[k]) && (500 > YPos[k]))
                            {
                                WRITE(xpos,YPos[k],GETCIVFLAG(CivVar),WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                                YPos[k] += 11;
                            }
                            xpos += 167;
                        }
                    }
                }
            }
        }
    }
    j = 0;
    for (i = 1; i<4; ++i)
    {
        if (YPos[i]>j)
        {
            j = YPos[i];
        }
    }
    j += 15;
    if (500 > j)
    {
        SetAPen(RPort_PTR,12);
        Move(RPort_PTR, 10,j-10);
        Draw(RPort_PTR,500,j-10);

        for (i = 1; i<8; ++i)
        {
            YPos[i] = j;
        }
        xpos = 130;
        for (i = 4; i<8; ++i)
        {
            WRITE(xpos,YPos[i],12,WRITE_Right,RPort_PTR,0,Project.data[i]);
            xpos += 125;
            YPos[i] += 15;
        }

        for (i = 0; i < Save.Systems; ++i)
        {
            if (NULL != SystemHeader[i].PlanetMemA)
            {
                for (j = 0; j<SystemHeader[i].Planets; ++j)
                {
                    MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                    if (0 != (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
                    {
                        CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
                        if ((Save.SSMoney[ActPlayer-1][CivVar-1] > (Save.WarPower[CivVar-1]*39)) || (CivVar == ActPlayer))
                        {
                            xpos = 130;
                            for (k = 4; k<8; ++k)
                            {
                                if ((0 != MyPlanetHeader->ProjectPtr->data[k]) && (500 > YPos[k]))
                                {
                                    WRITE(xpos,YPos[k],GETCIVFLAG(CivVar),WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                                    YPos[k] += 11;
                                }
                                xpos += 125;
                            }
                        }
                    }
                }
            }
        }
    }
    j = 0;
    for (i = 1; i<8; ++i)
    {
        if (YPos[i]>j)
        {
            j = YPos[i];
        }
    }
    j += 30;
    if (500 > j)
    {
        SetAPen(RPort_PTR,12);
        Move(RPort_PTR, 10,j-25);
        Draw(RPort_PTR,500,j-25);
        WRITE(255,j-15,12,WRITE_Center,RPort_PTR,0,PText[698]);
        for (i = 4; i<8; ++i)
        {
            YPos[i] = j;
        }
        for (i = 0; i < Save.Systems; ++i)
        {
            if (NULL != SystemHeader[i].PlanetMemA)
            {
                for (j = 0; j<SystemHeader[i].Planets; ++j)
                {
                    MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                    if (0 != (MyPlanetHeader->PFlags & FLAG_CIV_MASK))
                    {
                        CivVar = GETCIVVAR(MyPlanetHeader->PFlags);
                        if ((Save.SSMoney[ActPlayer-1][CivVar-1] > (Save.WarPower[CivVar-1]*39)) || (CivVar == ActPlayer))
                        {
                            k = MyPlanetHeader->ProjectID;
                            if ((3 < k) && (8 > k) && (500 > YPos[k]))
                            {
                                WRITE((k-4)*125+130,YPos[k],GETCIVFLAG(CivVar),WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                                YPos[k] += 11;
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
