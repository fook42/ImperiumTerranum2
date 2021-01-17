#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSOUNDS()
{
    uint8   slen;
    char    s[50];

    slen = strlen(PathStr[6]);
    memcpy(s, PathStr[6], slen+1);
    strcpy(s+slen, "Blip.RAW");
    LOADSOUND(s, 0);

    strcpy(s+slen, "DestroyDS.RAW");
    LOADSOUND(s, 1);

    strcpy(s+slen, "StargateDS.RAW");
    LOADSOUND(s, 2);
}
