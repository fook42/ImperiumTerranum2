#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <hardware/cia.h>
#include <hardware/custom.h>
#include <intuition/intuition.h>
#include <utility/tagitem.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "IT2_Types.h"

extern struct Custom custom;

#ifndef _EXTERN_
#define _EXTERN_ extern
_EXTERN_ struct CIA *ciaa;
_EXTERN_ uint8 TechUse1[43];
_EXTERN_ uint8 TechUse2[43];
_EXTERN_ uint8 ProjectNeedsTech[43];
_EXTERN_ uint8 ProjectNeedsProject[43];
_EXTERN_ r_ShipHeader DefaultShip;
_EXTERN_ char _Txt_FontName[];
_EXTERN_ char _Txt_Separator[];
_EXTERN_ struct TextAttr CustomTA[FONTS];
_EXTERN_ struct Image    GadImg1, GadImg2;
#else
struct CIA *ciaa = (struct CIA *) 0xBFE001;

uint8 TechUse1[] = {0,  0,0,0,0,0,  0,1,1,3,4,  3,6,2,8,10,   4,11,13,13,15, 16,18,19,20,21,  12,23,24,24,21,  27,29,25, 2,26,  31,32,34,13,36,  39,38};
uint8 TechUse2[] = {0,  0,0,0,0,0,  0,1,1,3,4,  5,6,7,9,16,  11,12,13,14,17, 20,18,22,20,30,  22,23,24,24,28,  29,33,30,31,31,  32,33,35,13,37,  40,41};
uint8 ProjectNeedsTech[]    = {0,0,0,9,19, 40,41,42,7,11, 15,16,20,21,24, 28,30,25,33,32, 35,31,36,37,38,
                               16,0,0,0,2, 3,4,8,10,12, 13,18,22,23,26, 29,34,39};
uint8 ProjectNeedsProject[] = {0,0,29,0,3, 0,0,6,0,0, 0,25,25,25,25, 25,25,25,25,25, 25,25,25,25,25,
                                0,1,0,0,28, 0,32,0,0,0, 0,0,0,35,0, 34,0,0};
r_ShipHeader DefaultShip = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL};

char _Txt_FontName[] = "StarFont.font\0";
char _Txt_Separator[] = "---\0";

struct TextAttr CustomTA[FONTS]={{(STRPTR) _Txt_FontName,11,0,0}, \
                                 {(STRPTR) _Txt_FontName,14,0,0}, \
                                 {(STRPTR) _Txt_FontName,15,0,0}, \
                                 {(STRPTR) _Txt_FontName,17,0,0}, \
                                 {(STRPTR) _Txt_FontName,24,0,0}};
                                
struct Image    GadImg1 = { 0, 0, 116, 20, 7, NULL , 127, 0, NULL};
struct Image    GadImg2 = { 0, 0, 116, 20, 7, NULL , 127, 0, NULL};
#endif


#ifdef _DEBUG_
_EXTERN_  uint8             PubScreenNum;
#endif

_EXTERN_  bool              Audio_enable;

_EXTERN_  struct TagItem    Tags[7];
_EXTERN_  PenArr            Pens;
_EXTERN_  ColSpecArr        ColSpec;

_EXTERN_  struct Screen*    MyScreen[2];
_EXTERN_  struct NewWindow  NeuWindow;
_EXTERN_  struct Window*    MyWindow[2];
_EXTERN_  struct Screen*    XScreen;
_EXTERN_  struct RastPort*  MyRPort_PTR[2];
_EXTERN_  struct ViewPort*  MyVPort_PTR[2];

_EXTERN_  struct ITBitMap   ImgBitMap4, ImgBitMap7, ImgBitMap8;

// _EXTERN_  struct IntuitionBase* IBase;
// _EXTERN_  struct Process*   Process_Ptr;
// _EXTERN_  BPTR              OldWindow_Ptr;

_EXTERN_  struct Library*   MEDPlayerBase;
    
_EXTERN_  r_WormHole        MyWormHole[MAXHOLES];  // shift done..
_EXTERN_  StrArr11          PNames[MAXCIVS-1];  // range 0..MAXCIVS-2
_EXTERN_  sint16            SystemX[MAXSYSTEMS];  // new range 0..MAXSYSTEMS-1
_EXTERN_  sint16            SystemY[MAXSYSTEMS];  // new range 0..MAXSYSTEMS-1
_EXTERN_  uint8             SystemFlags[MAXCIVS][MAXSYSTEMS]; // new range: 0..MAXCIVS-1, 0..MAXSYSTEMS-1
_EXTERN_  r_SystemHeader    SystemHeader[MAXSYSTEMS];  // new range 0..MAXSYSTEMS-1

_EXTERN_  StrArr42          TechnologyL, Project;

_EXTERN_  UWORD*        SoundMemA[SOUNDS]; // new range 0..SOUNDS-1
_EXTERN_  UWORD         SoundSize[SOUNDS]; // new range 0..SOUNDS-1
_EXTERN_  uint8*        CacheMemA[CACHES];
_EXTERN_  uint32        CacheMemL[CACHES];
_EXTERN_  uint8*        LogoMemA[MAXCIVS-1];
_EXTERN_  uint8*        LogoSMemA[MAXCIVS-1];
_EXTERN_  uint32        LogoSMemL[MAXCIVS-1];
_EXTERN_  UWORD*        ZeroSound;
_EXTERN_  uint8*        IMemA[IMAGES];
_EXTERN_  uint32        IMemL[IMAGES];
_EXTERN_  uint8*        ModMemA[MODULES];
_EXTERN_  uint32        ModMemL[MODULES];
_EXTERN_  char*         PathStr[PATHS];

_EXTERN_  r_ShipData    ShipData[18]; // offset 8 -> range 0..17 = 8..25

_EXTERN_  sint32        AllCreative[MAXCIVS];
_EXTERN_  uint32        Militaerausgaben[MAXCIVS];
_EXTERN_  uint32        Verschrottung[MAXCIVS];
_EXTERN_  uint8         LastDisplay[MAXCIVS+1];
_EXTERN_  uint8         Warnung[MAXCIVS+1];
_EXTERN_  uint8*        TextMemA;
_EXTERN_  uint8*        PathMemA;
_EXTERN_  uint32        TextMemL, PathMemL, HelpID, MaquesShips;
_EXTERN_  sint32        Year;
_EXTERN_  sint16        OffsetX, OffsetY, OldX, OldY;

_EXTERN_  uint8         Screen2, LastSystem, Level;

_EXTERN_  uint8         ActPlayer, ActPlayerFlag, Display;
_EXTERN_  uint8         HomePlanets;

_EXTERN_  void*         ObjPtr;
_EXTERN_  uint8         ObjType;

// _EXTERN_  struct Gadget     DKnopf[GADGETS+1];
// _EXTERN_  struct IntuiText  DKnopfTx[GADGETS+1];
// _EXTERN_  void*         MTBase;
// _EXTERN_  void*         MDBBase;
_EXTERN_  bool          WBench, Bool_var, Valid; // , IMemID;
_EXTERN_  bool          DoClock, MultiPlayer, Informed;
_EXTERN_  r_HiScore     HiScore;

_EXTERN_  struct r_PlanetHeader* GetPlanet[MAXCIVS]; // new range 0..MAXCIVS-1
_EXTERN_  uint8                  GetPlanetSys[MAXCIVS]; // new range 0..MAXCIVS-1
_EXTERN_  bool          vNSonde[MAXCIVS]; // new range 0..MAXCIVS-1
_EXTERN_  char*         PText[MAXSTR+1];

_EXTERN_  r_Save        Save;

// added ...
_EXTERN_  sint16        MOVESHIP_x;
_EXTERN_  sint16        MOVESHIP_y;
_EXTERN_  sint16        MOVESHIP_ToX;
_EXTERN_  sint16        MOVESHIP_ToY;
_EXTERN_  sint16        MOVESHIP_FromX;
_EXTERN_  sint16        MOVESHIP_FromY;

extern struct Custom custom;
