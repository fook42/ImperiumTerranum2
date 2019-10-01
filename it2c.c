const char __ver[] = "$VER: ImperiumTerranum 2.8c (12.09.19)\0";

#include <stdio.h>
#include <stdlib.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/medplayer.h>
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

ULONG getScreenmode(char* RequestTitle, ULONG orgScreenmodeID)
{
    struct ScreenModeRequester* gSm_SMRequester;
    struct TagItem RequesterTags[]={{ASLSM_DoWidth, true},
                                    {ASLSM_DoHeight, true},
                                    {ASLSM_DoDepth, true},
                                    {ASLSM_InitialDisplayID, orgScreenmodeID},
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

int main(void)
{
    struct Process   *proc;
    struct WBStartup *WBMsg = NULL;
    bool             prCLI;
    BPTR             CLIout;
    int              rc = 0;

    proc    = (struct Process *)FindTask(NULL);

    prCLI = proc->pr_CLI ? true : false;
    if (!prCLI)
    {
        WaitPort(&proc->pr_MsgPort);
        WBMsg = (struct WBStartup *)GetMsg(&proc->pr_MsgPort);
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

        ScreenModeID_HighRes = getScreenmode(SCREENREQTITLE_High, 0xA9004);

        ScreenModeID_LowRes  = getScreenmode(SCREENREQTITLE_Low,  0xA1000);

        MAIN_FNC();
        rc = 0;

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
    if (WBMsg)
    {
        Forbid();
        ReplyMsg(&WBMsg->sm_Message);
    }

    return rc;
}
