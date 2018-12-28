#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSOUNDS()
{
    int     l;
    char    s[50];

    l = strlen(PathStr[6]);
    memcpy(s, PathStr[6], l+1);
    strcpy(s+l, "Blip.RAW");
    LOADSOUND(s, 0);

    strcpy(s+l, "DestroyDS.RAW");
    LOADSOUND(s, 1);

    strcpy(s+l, "StargateDS.RAW");
    LOADSOUND(s, 2);
}
