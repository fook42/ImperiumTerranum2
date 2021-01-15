const char __ver[] = "$VER: ImperiumTerranum 2.801c (01.01.2021)\0";

#include <stdio.h>
#include <stdlib.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/medplayer.h>
#include <proto/icon.h>
#include <proto/asl.h>  // for screenmode-requester
#include <dos/dostags.h>
#include <workbench/startup.h>

#define AUTOCON "CON:20/20/500/120/mycon/auto/close/wait"
#define _EXTERN_

#define SCREENREQTITLE_High "HIGHRES (min. 640x512) screenmode"
#define SCREENREQTITLE_Low  "LOWRES (min. 320x256) screenmode"

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

uint8 getMyScreenToolTypes(BYTE* waName)
{
    uint8 status = 0;
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
                ScreenModeID_HighRes = string2hex(s);
                status |= 1;
            }
            s = (char*) FindToolType(toolarray, "LowScreenMode");
            if (NULL != s)
            {
                ScreenModeID_LowRes = string2hex(s);
                status |= 2;
            }
            FreeDiskObject(myIcon);
        }
    }
    return status;
}


ULONG getScreenmode(char* RequestTitle, int ReqWidth, int ReqHeight, ULONG orgScreenmodeID)
{
    struct ScreenModeRequester* gSm_SMRequester;
    struct TagItem RequesterTags[]={{ASLSM_DoWidth, true},
                                    {ASLSM_DoHeight, true},
                                    {ASLSM_DoDepth, true},
                                    {ASLSM_InitialDisplayID, orgScreenmodeID},
                                    {ASLSM_InitialDisplayWidth, (ULONG) ReqWidth},
                                    {ASLSM_InitialDisplayHeight, (ULONG) ReqHeight},
                                    {ASLSM_InitialDisplayDepth, (ULONG) 8},
                                    {ASLSM_TitleText, (ULONG) RequestTitle},
                                    {TAG_DONE,0},
                                    {TAG_END,0} };
    bool    rc;
    ULONG   newScreenmodeID = orgScreenmodeID;

    gSm_SMRequester = AllocAslRequest(ASL_ScreenModeRequest, RequesterTags);

    if (NULL != gSm_SMRequester)
    {
        rc = AslRequest( (APTR) gSm_SMRequester, NULL );
        if (rc)
        {
            newScreenmodeID = gSm_SMRequester->sm_DisplayID;
        }
        FreeAslRequest( gSm_SMRequester );
    }
    return newScreenmodeID;
}


int main(int argc,char** argv)
{
    struct Process   *proc;
    struct WBStartup *WBMsg = NULL;
    struct WBArg     *wbarg = NULL;
    bool             prCLI;
    BPTR             CLIout;
    int              rc = RETURN_OK;

    proc = (struct Process *)FindTask(NULL);

    prCLI = proc->pr_CLI ? true : false;
    if (!prCLI)
    {
        WBMsg = (struct WBStartup*) argv;
    }

    CLIout = prCLI ? Output() : Open(AUTOCON,MODE_OLDFILE);
    if (CLIout)
    {
        SelectOutput(CLIout);
        //--------------------------------------------------
        MEDPlayerBase = OpenLibrary((CONST_STRPTR) "medplayer.library", (ULONG) 0);
        if(!MEDPlayerBase)
        {
            puts("Can't open medplayer.library!\n");
        }

        /* get icon -> tooltypes to detect screenmodes.. if started from WB */
        if (!prCLI)
        {
            wbarg = WBMsg->sm_ArgList;

            /* if there's a directory lock for this wbarg, CD there */
            if((wbarg->wa_Lock) && (*wbarg->wa_Name))
                CurrentDir(wbarg->wa_Lock);
 
            if (3 != getMyScreenToolTypes( wbarg->wa_Name ))
            {
                ScreenModeID_HighRes = getScreenmode(SCREENREQTITLE_High, 640, 512, 0xA9004);
                ScreenModeID_LowRes  = getScreenmode(SCREENREQTITLE_Low,  320, 256, 0xA1000);
            }
        }

        MAIN_FNC();
        rc = RETURN_OK;

        if(MEDPlayerBase)
        {
            StopPlayer();
            FreePlayer();
            CloseLibrary(MEDPlayerBase);
        }
        //--------------------------------------------------
        if (!prCLI)
        {
            Close(CLIout);
        }

    } else {
        rc = RETURN_FAIL;
    }

    return rc;
}
