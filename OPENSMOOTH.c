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

    // try to open the file ...
    FHandle = Open((CONST_STRPTR) FName, FMode);
    if (0 == FHandle)
    {
        // if the file-open was NOT successful, we need to access the right disk!
        do
        {
            Delay(RDELAY);
        }
        while (LMB_PRESSED);

        // extract the volume/diskname (if any) from the given filename
        DName[0] = 0;
        i = 0;
        while ((':' != FName[i]) && (0 != FName[i]) && (48 > i))
        {
            DName[i] = FName[i];
            ++i;
        }
        DName[i] = 0;

        // the only part left from the filename should start with the volume-seperator ":"
        if (':' == FName[i])
        {
            // open a screen, the informs about the missing disk
            INITSTDTAGS();
            OS_XScreen = OpenScreenTagList(&OS_Screen, Tags);

            // if we can not open a screen, we fail at this point - no file will be opened
            if (NULL == OS_XScreen) { return (BPTR)0; }

            // setup a screen colour (light blue) and put the screen to the back
            SetRGB32(&(OS_XScreen->ViewPort), 1, 0xBB000000, 0xBB000000, 0xFF000000);
            ScreenToBack(OS_XScreen);

            WRITE(140, 0,1,WRITE_Center,&(OS_XScreen->RastPort),3, "Bitte Disk");
            strcpy(DName+i, " einlegen!");
            WRITE(140,20,1,WRITE_Center,&(OS_XScreen->RastPort),3, DName);

            i = 0;
            while ((0 == FHandle) && (3 > i))
            {
                ++i;
                // show the "insert disk"-screen
                ScreenToFront(OS_XScreen);

                // now repeat to open the file...wait some time inbetween..
                //  stop when the file was opened successfully or left mouse button clicked
                do
                {
                    Delay(6);
                    FHandle = Open((CONST_STRPTR) FName, FMode);
                }
                while ((LMB_NOTPRESSED) && (0 == FHandle));
                ScreenToBack(OS_XScreen);

                // wait to release the left mouse button
                do
                {
                    Delay(6);
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
