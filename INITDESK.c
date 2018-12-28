#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

bool INITDESK(uint8 Mode)
{
    uint32  l;
    char    s[50];
    BPTR    FHandle;

    l = strlen(PathStr[0]);
    memcpy(s, PathStr[0], l+1);
    strcpy(s+l, "Desk.pal");
    (void) SETCOLOR( MyScreen[0], s);
  
    strcpy(s+l, "Desk.img");
    if (!DISPLAYIMAGE( s, 512, 0, 128, 512, 7, MyScreen[0], 0)) {  return false; }
    SETWORLDCOLORS();
    WRITEWIN(576,156,45,WRITE_Center, MyRPort_PTR[0], 4, _PT_Jahr);
    WRITEWIN(576,207,45,WRITE_Center, MyRPort_PTR[0], 4, PText[146]);
    WRITEWIN(576,258,45,WRITE_Center, MyRPort_PTR[0], 4, PText[147]);

    l = strlen(PathStr[5]);
    memcpy(s, PathStr[5], l+1);

    strcpy(s+l, "DeskImages.img");
    if (!RAWLOADIMAGE(s,0,32,608,32,4,&ImgBitMap4)) { return false; }

    if (1 == Mode)
    {
        l = strlen(PathStr[0]);
        memcpy(s, PathStr[0], l+1);

        strcpy(s+l, "DeskImages.img");
        if (!RAWLOADIMAGE(s,0,0,416, 32,7,&ImgBitMap7)) { return false; }

        strcpy(s+l, "ProjectIcons.img");
        if (!RAWLOADIMAGE(s,0,0,640,192,8,&ImgBitMap8)) { return false; }

        IMemL[1] = 4480;
        IMemA[1] = AllocMem( IMemL[1], MEMF_CHIP+MEMF_CLEAR );
        if (NULL == IMemA[1]) { return false; }

        strcpy(s+l, "DeskGads.img");
        FHandle = OPENSMOOTH( s, MODE_OLDFILE);       /*Planets/Deskgads.img*/
        if (0 == FHandle)
        {
            FreeMem( IMemA[1], IMemL[1] );
            IMemA[1] = NULL;
            return false;
        }
        (void) Read( FHandle, IMemA[1], IMemL[1]);
        Close( FHandle );
        GadImg1.ImageData = IMemA[1];
        GadImg2.ImageData = IMemA[1]+2240;

        l = strlen(PathStr[4]);
        memcpy(s, PathStr[4], l+1);

        strcpy(s+l, "MOD.Invention");
        LOADMOD( s, 1 );
        strcpy(s+l, "MOD.War");
        LOADMOD( s, 2 );
        strcpy(s+l, "MOD.Tech");
        LOADMOD( s, 3 );
        strcpy(s+l, "MOD.Bad");
        LOADMOD( s, 4 );

        l = strlen(PathStr[7]);
        memcpy(s, PathStr[7], l+1);

        strcpy(s+l, "Worm.img");
        if (!RAWLOADIMAGE(s,0,32,512,32,7,&ImgBitMap7)) { return false; }

        strcpy(s+l, "XPlode.img");
        if (!RAWLOADIMAGE(s,0, 0,512,32,4,&ImgBitMap4)) { return false; }
    }
    return true;
}
