#ifndef _IT2_DEFINES_H_
#define _IT2_DEFINES_H_

#include <hardware/custom.h>
#include "IT2_PText_Defines.h"

#define def_HighRes_Width  (640)
#define def_HighRes_Height (512)
#define def_HighRes_Mode   (0xA9004)
#define def_LowRes_Width   (320)
#define def_LowRes_Height  (256)
#define def_LowRes_Mode    (0xA1000)

#define FLAG_UNKNOWN    0           /* 0x00 */
#define FLAG_KNOWN      0x80        /* 0x80 = 1000 0000 */
#define FLAG_TERRA      20          /* 0x14 = 0001 0100 */
#define FLAG_KLEGAN     8           /* 0x08 = 0000 1000 */
#define FLAG_REMALO     96          /* 0x60 = 0110 0000 */
#define FLAG_CARDAC     10          /* 0x0A = 0000 1010 */
#define FLAG_FERAGI     67          /* 0x43 = 0100 0011 */
#define FLAG_BAROJA     11          /* 0x0B = 0000 1011 */
#define FLAG_VOLKAN     27          /* 0x1B = 0001 1011 */
#define FLAG_OTHER      1           /* 0x01 = 0000 0001 */
#define FLAG_MAQUES     12          /* 0x0C = 0000 1100 */
#define FLAG_CIV_MASK   (FLAG_TERRA | FLAG_KLEGAN | FLAG_REMALO | FLAG_CARDAC | FLAG_FERAGI | FLAG_BAROJA | FLAG_VOLKAN | FLAG_OTHER | FLAG_MAQUES)
                                    /* 0x7F = 0111 1111 */

enum Civilization { CIVVAR_NONE  = 0,
                    CIVVAR_TERRA = 1,
                    CIVVAR_KLEGAN,
                    CIVVAR_REMALO,
                    CIVVAR_CARDAC,
                    CIVVAR_FERAGI,
                    CIVVAR_BAROJA,
                    CIVVAR_VOLKAN,
                    CIVVAR_OTHER,
                    CIVVAR_MAQUES
                    };

#define WFLAG_CEBORC    2
#define WFLAG_DCON      3
#define WFLAG_FIELD     4
#define WFLAG_JAHADR    5

#define MASK_LTRUPPS    0x0f
#define MASK_SIEDLER    0xf0

#define LOGOSIZE        14500
#define LOGOMEMSIZE     (int)(128*128*7/8)

#define RDELAY          1
#define PAUSE           85

#define GFLAG_EXPLORE   1
#define GFLAG_ATTACK    2

#define SHIPFLAG_NONE   0
#define SHIPFLAG_WATER  1

#define LEVEL_DIED      0
#define LEVEL_UNKNOWN   1
#define LEVEL_PEACE     2
#define LEVEL_WAR       3
#define LEVEL_ALLIANZ   4
#define LEVEL_NO_ALLIANZ  5
#define LEVEL_COLDWAR   6

#define TYPE_NOTHING    0
#define TYPE_PLANET     2
#define TYPE_SHIP       4
#define TYPE_STARGATE   16
#define TYPE_WORMHOLE   32
#define SHIPTYPE_FLEET  200
#define SHIPTYPE_STARGATE   21

enum PlanetClasses {
        CLASS_STONES = 0,
        CLASS_GAS,
        CLASS_WATER,
        CLASS_SATURN,
        CLASS_HALFEARTH,
        CLASS_DESERT,
        CLASS_EARTH,
        CLASS_PHANTOM,
        CLASS_ICE,
        CLASS_MAX_TYPES
};

#define WEAPON_GUN       1
#define WEAPON_LASER     3
#define WEAPON_PHASER    5
#define WEAPON_DISRUPTOR 7
#define WEAPON_PTORPEDO  9

enum {  MODE_REFRESH = 0,
        MODE_REDRAW,
        MODE_FLEET,
        MODE_STARGATE,
        MODE_ONCE,
        MODE_ALL,
        MODE_OFFENSIV = 8,
        MODE_DEFENSIV,
        MODE_SHIPS,
        MODE_TERRITORIUM,
        MODE_BELEIDIGUNG,
        MODE_MONEY,
        MODE_FORCE };

#define STATE_ALL_OCC   1
#define STATE_ENEMY     2
#define STATE_TACTICAL  3

enum {  SCREEN_PLANET = 1,
        SCREEN_INVENTION,
        SCREEN_HISCORE,
        SCREEN_TECH };

#define TARGET_POSITION  127
#define TARGET_STARGATE  26
#define TARGET_ENEMY_SHIP  126

#define INFLATION       ((double) 1.03)

#define MODULES         4

#define MAXSYSTEMS      25
#define MAXMAQUES       5
#define MAXHOLES        3

#define SOUNDS          4
#define CACHES          4
#define IMAGES          3
#define GADGETS         3
#define FONTS           5

#define MAXPLANETS      11
#define MAXCIVS         9

#define PATHS           11

#define MAXPMONEY       102000L
#define MAXSTR          900

#define ROTATE_PX       (1<<0)
#define ROTATE_PY       (1<<1)
#define ROTATE_PZ       (1<<2)
#define ROTATE_NX       (1<<3)
#define ROTATE_NY       (1<<4)
#define ROTATE_NZ       (1<<5)

#define _COLOR_CMAP_TEXT_ (0x434D4150)

#define HISCORE_NAMELEN (20)

#define MAX_VECTORS     (200)

#define DISKMENU_PROMPT     (0)
#define DISKMENU_LOADGAME   (1)
#define DISKMENU_SAVEGAME   (2)
// fakefunctions

#define MAKEBORDER(A,B,C,D,E,F,G,H)  MAKEWINBORDER(&((A)->RastPort),B,C,D,E,F,G,H)
#define MAKECENTERWINDOW(A,B,C)      MAKEWINDOWBORDER(Area_CenterX-((A)/2), Area_CenterY-((B)/2),A,B,C)
#define RECT(A,B,C,D,E,F)            RECTWIN(&((A)->RastPort),B,C,D,E,F)

// WRITE-Defines
#define WRITE_Left    0x00
#define WRITE_Center  0x10
#define WRITE_Right   0x20
#define WRITE_Shadow  0x40

// Hardware related defines ...
#define RData        custom.potinp
#define LData        ciaa->ciapra

#define LMB_PRESSED     (0 == (LData & 0x0040))
#define LMB_NOTPRESSED  (0 != (LData & 0x0040))
#define RMB_PRESSED     (0 == (RData & 0x0400))
#define RMB_NOTPRESSED  (0 != (RData & 0x0400))

#define MouseX(...)     MyScreen[(__VA_ARGS__)]->MouseX
#define MouseY(...)     MyScreen[(__VA_ARGS__)]->MouseY

#define WinMouseX(...)  MyWindow[(__VA_ARGS__)]->MouseX
#define WinMouseY(...)  MyWindow[(__VA_ARGS__)]->MouseY

#define SPAddr(...)      custom.aud[(__VA_ARGS__)].ac_ptr
#define SPLength(...)    custom.aud[(__VA_ARGS__)].ac_len
#define SPFreq(...)      custom.aud[(__VA_ARGS__)].ac_per
#define SPVol(...)       custom.aud[(__VA_ARGS__)].ac_vol
#define SPData(...)      custom.aud[(__VA_ARGS__)].ac_dat

#define SWITCHDISPLAY(...) custom.dmacon=(BITCLR|DMAF_RASTER)

// example to access array with offset
#define SHIPDATA_OFFSET     8
#define ShipData(...)       ShipData[(__VA_ARGS__) - SHIPDATA_OFFSET]

enum Projects {
        PROJECT_CLEAR_BIOPHERE  = -3,
        PROJECT_REPAIR_INFRA    = -2,
        PROJECT_REPAIR_INDUSTRY = -1,
        PROJECT_NONE            =  0,
        PROJECT_SPACEDOCK       = 25,
        PROJECT_SETTLERS,       // 26
        PROJECT_LANDINGTROOPS,  // 27
        PROJECT_CONT_UNION,     // 28
        PROJECT_GLOBAL_UNION,   // 29
        PROJECT_RECYCLINGPLANT, // 30
        PROJECT_FUSIONPOWER,    // 31
        PROJECT_HYDROPOWER,     // 32
        PROJECT_PART_ACCEL,     // 33
        PROJECT_SDI,            // 34
        PROJECT_INTERNET,       // 35
        PROJECT_VIRT_UNIVERSITY,// 36
        PROJECT_INT_PLANT,      // 37
        PROJECT_INFO_HIGHWAY,   // 38
        PROJECT_VON_NEUMANN,    // 39
        PROJECT_SPACEPHALANX,   // 40
        PROJECT_MICROIDS,       // 41
        PROJECT_WEATHERSTATION, // 42
        PROJECT_NOMORE          // 43
};

#define AreaFillVectors (200)
#define IntroAreaSize (AreaFillVectors*5)

#endif
