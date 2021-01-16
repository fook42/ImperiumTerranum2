/* Icon handling */

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

BOOL getMyScreenToolTypes(BYTE* waName, ULONG* Hires_ID_PTR, ULONG* Lowres_ID_PTR)
{
    BOOL status = false;
    STRPTR *toolarray;
    struct DiskObject* myIcon;
    char* s;
    if (NULL != waName)
    {
        myIcon = GetDiskObject(waName);
        if (NULL != myIcon)
        {
            toolarray = myIcon->do_ToolTypes;
            s = (char*) FindToolType(toolarray, "BigScreenMode");
            if (NULL != s)
            {
                *Hires_ID_PTR = string2hex(s);
                s = (char*) FindToolType(toolarray, "LowScreenMode");
                if (NULL != s)
                {
                    *Lowres_ID_PTR = string2hex(s);
                    status = true;
                }
            }
            FreeDiskObject(myIcon);
        }
    }
    return status;
}


BOOL setMyScreenToolTypes(BYTE* waName, ULONG Hires_ID, ULONG Lowres_ID)
{
    BOOL status = false;
    struct DiskObject* myIcon;
    UBYTE **oldTooltypes;
    UBYTE BigScreenTooltype[] = "BigScreenMode=0x00000000\0";
    UBYTE LowScreenTooltype[] = "LowScreenMode=0x00000000\0";
    UBYTE *newTooltypes[] = {BigScreenTooltype, LowScreenTooltype, NULL};
    if (NULL != waName)
    {
        myIcon = GetDiskObject(waName);
        if (NULL != myIcon)
        {
            oldTooltypes = myIcon->do_ToolTypes;

            (void) hex2out(Hires_ID, 8,BigScreenTooltype+sizeof(BigScreenTooltype)-10);
            (void) hex2out(Lowres_ID,8,LowScreenTooltype+sizeof(LowScreenTooltype)-10);
            myIcon->do_ToolTypes = newTooltypes;

            if (PutDiskObject(waName, myIcon))
            {
                status = true;
            }

            myIcon->do_ToolTypes = oldTooltypes;
            FreeDiskObject(myIcon);
        }
    }
    return status;
}
