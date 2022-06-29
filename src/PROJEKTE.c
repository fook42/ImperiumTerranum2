#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PROJEKTE()
{
    int         i,j,k,CivVar;
    int         XPos, YPos[8];
    r_PlanetHeader*     MyPlanetHeader;
	struct Window*      PRJ_Window;
	struct RastPort*    RPort_PTR;

	PRJ_Window=MAKEWINDOWBORDER(0,0,Area_Width,Area_Height,MyScreen[0]);
	if (NULL == PRJ_Window)
	{
		return;
	}
	RPort_PTR = PRJ_Window->RPort;


    XPos = 172;
    for (i = 1; i<4; ++i)
    {
        WRITE(XPos,14,12,WRITE_Right,RPort_PTR,0,Project.data[i]);
        XPos += 167;
        YPos[i] = 40;
    }
    --ActPlayer;
    for (i = 0; i < Save.Systems; ++i)
    {
        if (NULL == SystemHeader[i].PlanetMemA) { continue; }

        for (j = 0; j<SystemHeader[i].Planets; ++j)
        {
            MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
            if (0 == (MyPlanetHeader->PFlags & FLAG_CIV_MASK)) { continue; }

            CivVar = GETCIVVAR(MyPlanetHeader->PFlags)-1;
            if ((Save.SSMoney[ActPlayer][CivVar] > (Save.WarPower[CivVar]*39)) || (CivVar == ActPlayer))
            {
                XPos = 172;
                for (k = 1; k<4; ++k)
                {
                    if (0 != (MyPlanetHeader->ProjectPtr->data[k]) && (500 > YPos[k]))
                    {
                        WRITE(XPos,YPos[k],MyPlanetHeader->PFlags,WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                        YPos[k] += 11;
                    }
                    XPos += 167;
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
    if ((Area_Height-12) > j)
    {
        SetAPen(RPort_PTR,12);
        Move(RPort_PTR,           10,j-10);
        Draw(RPort_PTR,Area_Width-10,j-10);

        for (i = 1; i<8; ++i)
        {
            YPos[i] = j;
        }
        XPos = 130;
        for (i = 4; i<8; ++i)
        {
            WRITE(XPos,YPos[i],12,WRITE_Right,RPort_PTR,0,Project.data[i]);
            XPos += 125;
            YPos[i] += 15;
        }

        for (i = 0; i < Save.Systems; ++i)
        {
            if (NULL == SystemHeader[i].PlanetMemA) { continue; }

            for (j = 0; j<SystemHeader[i].Planets; ++j)
            {
                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                if (0 == (MyPlanetHeader->PFlags & FLAG_CIV_MASK)) { continue; }

                CivVar = GETCIVVAR(MyPlanetHeader->PFlags)-1;
                if ((Save.SSMoney[ActPlayer][CivVar] > (Save.WarPower[CivVar]*39)) || (CivVar == ActPlayer))
                {
                    XPos = 130;
                    for (k = 4; k<8; ++k)
                    {
                        if ((0 != MyPlanetHeader->ProjectPtr->data[k]) && (500 > YPos[k]))
                        {
                            WRITE(XPos,YPos[k],MyPlanetHeader->PFlags,WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                            YPos[k] += 11;
                        }
                        XPos += 125;
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
    if ((Area_Height-12) > j)
    {
        SetAPen(RPort_PTR,12);
        Move(RPort_PTR,           10,j-25);
        Draw(RPort_PTR,Area_Width-10,j-25);
        WRITE(Area_CenterX,j-15,12,WRITE_Center,RPort_PTR,0,PText[698]);
        for (i = 4; i<8; ++i)
        {
            YPos[i] = j;
        }
        for (i = 0; i < Save.Systems; ++i)
        {
            if (NULL == SystemHeader[i].PlanetMemA) { continue; }

            for (j = 0; j<SystemHeader[i].Planets; ++j)
            {
                MyPlanetHeader = &(SystemHeader[i].PlanetMemA[j]);
                if (0 == (MyPlanetHeader->PFlags & FLAG_CIV_MASK)) { continue; }

                CivVar = GETCIVVAR(MyPlanetHeader->PFlags)-1;
                if ((Save.SSMoney[ActPlayer][CivVar] > (Save.WarPower[CivVar]*39)) || (CivVar == ActPlayer))
                {
                    k = MyPlanetHeader->ProjectID;
                    if ((3 < k) && (8 > k) && ((Area_Height-12) > YPos[k]))
                    {
                        WRITE((k-4)*125+130,YPos[k],MyPlanetHeader->PFlags,WRITE_Right,RPort_PTR,0,MyPlanetHeader->PName);
                        YPos[k] += 11;
                    }
                }
            }
        }
    }
    ++ActPlayer;
    WAITLOOP(false);
	CloseWindow(PRJ_Window);
}
