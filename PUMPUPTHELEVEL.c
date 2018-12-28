#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void PUMPUPTHELEVEL()
{
    bool    b = false;
    uint8   i,j;

    do
    {
        switch (rand()%5) {
            case 0: if ((Save.WorldFlag==0) || (Save.WorldFlag==WFLAG_CEBORC))
                    {
                        CEBORCATTACK(ActPlayerFlag);
                        b = true;
                    } break;
            case 1: if ((Save.WorldFlag==0) || (Save.WorldFlag==WFLAG_JAHADR))
                    {
                        for(i = 0; i < (MAXCIVS-2); i++)
                        {
                            if ((i != (ActPlayer-1)) && (!b) && (Save.WarPower[i]>Save.WarPower[ActPlayer-1]))
                            {
                                CREATEJAHADR(i);
                                b = true;
                            }
                        }
                    } break;
            case 2: {
                        for(i = 0; i < (MAXCIVS-2); i++)
                        {
                            for(j = 0; j < (MAXCIVS-2); j++)
                            {
                                if ((Save.WarState[i][j] != LEVEL_DIED) && (Save.WarState[j][i] != LEVEL_DIED))
                                {
                                    if ((i != (ActPlayer-1)) && (j != (ActPlayer-1))
                                            && (Save.CivPlayer[i] == 0) && (Save.CivPlayer[j] == 0))
                                    {
                                        Save.WarState[i][j] = LEVEL_PEACE;
                                        Save.WarState[j][i] = LEVEL_PEACE;
                                        Save.Staatstopf[i] += 5000;
                                        Save.Staatstopf[j] += 5000;
                                    } else if (i != j)  {
                                        Save.WarState[i][j] = LEVEL_COLDWAR;
                                        Save.WarState[j][i] = LEVEL_COLDWAR;
                                    }
                                }
                            }
                        }
                        b = true;
                    } break;
            case 3: {
                        for(i = 0; i < 25; i++)
                        {
                            while (!CREATEMAQUESSHIP(i,(rand()%5)+19));
                        }
                        b = true;
                    } break;
            case 4: if ((Save.WorldFlag == 0) || (Save.WorldFlag == ActPlayerFlag))
                    {
                        CREATECIVILWAR(ActPlayer);
                        b = true;
                    } break;
            default: { }
        }
    }
    while (!b);
    REQUEST(PText[388],PText[389],12,12);
}
