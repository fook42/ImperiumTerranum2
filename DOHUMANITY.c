#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void DOHUMANITY()
{
    uint8   Smallest, Biggest, i, Ende;
    uint32  SmallValue, BigValue;
    sint16  Direction;

    Smallest = 1;
    Biggest = 1;
    SmallValue = 1000000000;
    BigValue = 0;
    if ((Year%2)!=0)
    {
        i = 0;       Ende = (MAXCIVS-2); Direction = 1;     // count upwards
    } else {
        i = MAXCIVS; Ende = 1;           Direction = -1;    // count downwards
    }
    do
    {
        i += Direction;
        if ((Save.WarPower[i-1]>BigValue) && (Save.Bevoelkerung[i-1]>0))
        {
            BigValue = Save.WarPower[i-1];
            Biggest = i;
        }
        if (((Save.WarPower[i-1]<SmallValue) && (Save.Bevoelkerung[i-1]>1000))
                &&((Save.CivPlayer[i-1] == 0) || (Save.PlayMySelf)))
        {
            SmallValue = Save.WarPower[i-1];
            Smallest = i;
        }
    }
    while (i != Ende);
    
    Smallest--;
    Biggest--;
    if ((Save.Bevoelkerung[Smallest]>1000) && ((Save.CivPlayer[Smallest] == 0) || (Save.PlayMySelf)))
    {
        Save.ImperatorState[Biggest] += 2;
        Save.Staatstopf[Biggest]     -= Save.WarPower[Biggest]*25;
        Save.Staatstopf[Smallest]    += Save.WarPower[Biggest]*25;
    }
    i = GETCIVVAR(Save.WorldFlag);
    if ((i>=1) && (i<=7))
    {
        if (Save.Bevoelkerung[Biggest] > (Save.Bevoelkerung[i-1]*3))
        {
            STOPCIVILWAR(0);
        }
    }
    if ((Save.Bevoelkerung[Biggest]>75000) && ((rand()%255) == 0) && (Save.WorldFlag == 0))
    {
        CREATECIVILWAR(Biggest+1);
    }
}
