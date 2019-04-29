#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

BPTR OPENSMOOTH(char* FName, uint32 FMode)
{
    char    DName[50];
    BPTR    FHandle;
    int     i;
    struct NewScreen    OS_Screen = {180,0,280,40,1,0,0,HIRES+LACE,CUSTOMSCREEN+SCREENQUIET,NULL,NULL,NULL,NULL};
    struct Screen*      OS_XScreen;

    FHandle = Open((CONST_STRPTR) FName, FMode);   
    if (0 == FHandle)
    {
        do
        {
            delay(RDELAY);
        }
        while (LMB_PRESSED);

        DName[0] = 0;
        i = 0;
        while ((':' != FName[i]) && (0 != FName[i]) && (39 > i))
        {
            DName[i] = FName[i];
            i++;
        }
        DName[i] = 0;
        
        if (':' == FName[i])
        {
            INITSTDTAGS();
            OS_XScreen = OpenScreenTagList(&OS_Screen, Tags);

            if (NULL == OS_XScreen) { return (BPTR)0; }

            SetRGB32(&(OS_XScreen->ViewPort), 1, 0xBB000000, 0xBB000000, 0xFF000000);
            ScreenToBack(OS_XScreen);

            WRITE(140, 0,1,WRITE_Center,&(OS_XScreen->RastPort),3, "Bitte Disk");
            strcpy(DName+i, " einlegen!");
            WRITE(140,20,1,WRITE_Center,&(OS_XScreen->RastPort),3, DName);

            i = 0;
            while ((0 == FHandle) && (3 > i))
            {
                i++;
                ScreenToFront(OS_XScreen);

                do
                {
                    delay(6);
                    FHandle = Open((CONST_STRPTR) FName, FMode);
                }
                while ((LMB_NOTPRESSED) && (0 == FHandle));
                ScreenToBack(OS_XScreen);

                do
                {
                    delay(6);
                }
                while (LMB_PRESSED);
                if (0 == FHandle)
                {
                    FHandle = Open((CONST_STRPTR) FName, FMode);
                }
            }
            CloseScreen(OS_XScreen);
        }
    }
    return FHandle;
}
