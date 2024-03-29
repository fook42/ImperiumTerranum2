const char __ver[] = "$VER: ImperiumTerranum 2.820 (22.08.2022)\0";

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

#define SCREENREQTITLE_High "HIGHRES 640x512 screenmode"
#define SCREENREQTITLE_Low  "LOWRES 320x256 screenmode"

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"


BOOL getScreenmode(char* RequestTitle, int ReqWidth, int ReqHeight, ULONG orgScreenmodeID, ULONG* newScreenmodeID, UWORD* newScreenWidth, UWORD* newScreenHeight)
{
    struct ScreenModeRequester* gSm_SMRequester;
    struct TagItem RequesterTags[]={{ASLSM_DoWidth, false},
                                    {ASLSM_DoHeight, false},
                                    {ASLSM_DoDepth, true},
                                    {ASLSM_InitialDisplayID, orgScreenmodeID},
                                    {ASLSM_InitialDisplayWidth, (ULONG) ReqWidth},
                                    {ASLSM_InitialDisplayHeight, (ULONG) ReqHeight},
                                    {ASLSM_InitialDisplayDepth, (ULONG) 8},
                                    {ASLSM_MinDepth, (ULONG) 8},
                                    {ASLSM_MaxDepth, (ULONG) 8},
                                    {ASLSM_TitleText, (ULONG) RequestTitle},
                                    {TAG_DONE,0},
                                    {TAG_END,0} };
    bool    rc = false;
    *newScreenmodeID = orgScreenmodeID;
    *newScreenWidth  = ReqWidth;
    *newScreenHeight = ReqHeight;

    gSm_SMRequester = AllocAslRequest(ASL_ScreenModeRequest, RequesterTags);

    if (NULL != gSm_SMRequester)
    {
        rc = AslRequest( (APTR) gSm_SMRequester, NULL );
        if (rc)
        {
            *newScreenmodeID = gSm_SMRequester->sm_DisplayID;
            *newScreenWidth  = (UWORD) gSm_SMRequester->sm_DisplayWidth;
            *newScreenHeight = (UWORD) gSm_SMRequester->sm_DisplayHeight;
        }
        FreeAslRequest( gSm_SMRequester );
    }
    return rc;
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
            {
                CurrentDir(wbarg->wa_Lock);
            }
 
            if (!getMyScreenToolTypes( wbarg->wa_Name, &ScreenModeID_HighRes, &ScreenModeID_LowRes ))
            {
                (void) getScreenmode(SCREENREQTITLE_High, 640, 512, 0xA9004, &ScreenModeID_HighRes, &HighRes_Width, &HighRes_Height);
                (void) getScreenmode(SCREENREQTITLE_Low,  320, 256, 0xA1000, &ScreenModeID_LowRes,  &LowRes_Width,  &LowRes_Height);
                (void) setMyScreenToolTypes( wbarg->wa_Name, ScreenModeID_HighRes, ScreenModeID_LowRes );
            }
        } else {
            /* started from CLI ... open the screenmode-requesters anyway */
            (void) getScreenmode(SCREENREQTITLE_High, 640, 512, 0xA9004, &ScreenModeID_HighRes, &HighRes_Width, &HighRes_Height);
            (void) getScreenmode(SCREENREQTITLE_Low,  320, 256, 0xA1000, &ScreenModeID_LowRes,  &LowRes_Width,  &LowRes_Height);
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
