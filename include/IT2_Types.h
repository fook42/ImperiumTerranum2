#ifndef _IT2_TYPES_H_
#define _IT2_TYPES_H_

#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/medplayer.h>

#include <math.h>
#include <hardware/custom.h>
#include <hardware/cia.h>
#include <graphics/gfxmacros.h>
#include <utility/tagitem.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "IT2_Defines.h"

typedef UBYTE  uint8;
typedef UWORD  uint16;
typedef ULONG  uint32;
typedef BYTE   sint8;
typedef WORD   sint16;
typedef LONG   sint32;

typedef struct TagArr { struct TagItem data[7]; } TagArr;

typedef struct PenArr { uint16      data[13]; } PenArr;

typedef struct ColSpecArr { sint16  data[5]; } ColSpecArr;

typedef struct StrArr11         { char  data[12][15];           } StrArr11;
typedef struct StrArrMAXPLANETS { char  data[MAXPLANETS+1][6];  } StrArrMAXPLANETS;

typedef struct ByteArr42 { uint8    data[43]; } ByteArr42; // range 0..42

typedef struct ITBitMap {
    uint16          BytesPerRow, Rows;
    uint8           Flags, Depth;
    uint16          pad;
    PLANEPTR        PPtr[8];
    APTR            MemA;
    uint32          MemL;
} ITBitMap;

typedef struct r_ShipData {
    uint8           MaxLoad, MaxShield, MaxMove, WeaponPower;
} r_ShipData;

typedef struct r_ShipHeader {
    uint8           Age, SType, Owner, Flags, ShieldBonus, Ladung, Fracht;
    sint8           PosX, PosY;
    uint8           Shield, Weapon, Repair;
    sint8           Moving, Source, Target, Tactical;
    struct r_ShipHeader*    TargetShip;
    struct r_ShipHeader*    BeforeShip;
    struct r_ShipHeader*    NextShip;
} r_ShipHeader;

typedef struct r_PlanetHeader {
    uint8           Class, Size, PFlags, Ethno;
    char            PName[16];
    float           PosX, PosY;
    uint32          Population, Water;
    uint8           Biosphaere, Infrastruktur, Industrie;
    sint32          XProjectCosts, XProjectPayed;
    sint8           ProjectID;
    r_ShipHeader    FirstShip;
    struct ByteArr42*       ProjectPtr;
} r_PlanetHeader;

typedef struct r_SystemHeader {
    r_PlanetHeader* PlanetMemA;
    uint8           State;
    r_ShipHeader    FirstShip;
    uint8           Planets, vNS, SysOwner;
} r_SystemHeader;

typedef struct r_HiScore {
    char    Player[8][HISCORE_NAMELEN];
    uint8   CivVar[8];
    uint32  Points[8];
} r_HiScore;

typedef struct r_WormHole {
    uint8   System[2];
    sint8   PosX[2], PosY[2];
    uint8   CivKnowledge[MAXCIVS+1];    // added 1 byte to fill up 16 bytes (=compatible with original IT2)
} r_WormHole;


typedef struct r_Col {
    uint8   r, g, b;
} r_Col_t;

typedef struct worldcolors_t {
    uint32  r, g, b;
} worldcolors_t;

typedef struct r_Save {
    uint8       WarState[MAXCIVS][MAXCIVS];
    uint8       LastWarState[MAXCIVS][MAXCIVS];
    sint32      Staatstopf[MAXCIVS];
    uint32      Bevoelkerung[MAXCIVS];
    uint32      WarPower[MAXCIVS];
    uint32      MaxWarPower[MAXCIVS];
    uint32      ImperatorState[MAXCIVS];
    uint32      SSMoney[MAXCIVS][MAXCIVS];
    sint32      TechCosts[MAXCIVS][43];
    sint32      ProjectCosts[MAXCIVS][43];
    sint8       ActTech[MAXCIVS+1];
    uint8       GlobalFlags[MAXCIVS+1];
    sint8       GSteuer[MAXCIVS+1];
    uint8       JSteuer[MAXCIVS+1];
    uint8       stProject[MAXCIVS+1];
    uint8       SService[MAXCIVS+1];
    uint8       Military[MAXCIVS+1];
    bool        PlayMySelf, SmallFight, SmallLand, FastMove, NoWorm;
    uint8       WorldFlag, Systems, CivilWar;
    char        SystemName[MAXSYSTEMS][12];
    uint8       CivPlayer[MAXCIVS];
} r_Save;
/*
 * originaler Code..+ Array-Grenzen
type r_Save=record
       WarState,LastWarState                            :array [1..MAXCIVS,1..MAXCIVS] of byte;
       Staatstopf,Bevölkerung,WarPower,MaxWarPower,
       ImperatorState                                   :array [1..MAXCIVS] of long;
       SSMoney                                          :array [1..MAXCIVS,1..MAXCIVS] of long;
       TechCosts,ProjectCosts                           :array [1..MAXCIVS] of LongArr42;
       ActTech,GlobalFlags,GSteuer,JSteuer              :array [1..MAXCIVS] of byte;
       stProject,SService,Military                      :array [1..MAXCIVS] of byte;
       PlayMySelf,SmallFight,SmallLand,FastMove,NoWorm  :boolean;
       WorldFlag,SYSTEMS,CivilWar                       :byte
       SystemName                                       :StrArr;
       CivPlayer                                        :array [1..MAXCIVS] of byte;
end;
*/
typedef struct VectorObj {
    sint16  PosX, PosY;
    uint8   Flag, Size1, Size2, __spare;    // spare added to align the structure
    sint32  X1[6],Y1[6],Z1[6],X2[6],Y2[6],Z2[6];
} VectorObj_t;

#endif
