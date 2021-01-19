#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

void INITSOUNDS()
{
    char    s[50];
    char*   _s;

    _s=my_strcpy(s, PathStr[6]);
    (void)my_strcpy(_s, "Blip.RAW");
    LOADSOUND(s, 0);

    (void)my_strcpy(_s, "DestroyDS.RAW");
    LOADSOUND(s, 1);

    (void)my_strcpy(_s, "StargateDS.RAW");
    LOADSOUND(s, 2);
}
