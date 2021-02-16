#ifndef _IT2_DEFINES_H_
#define _IT2_DEFINES_H_

#include <hardware/custom.h>
#include "IT2_PText_Defines.h"

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

enum { CIVVAR_NONE  = 0,
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

#define TYPE_PLANET     2
#define TYPE_SHIP       4
#define TYPE_STARGATE   16
#define TYPE_WORMHOLE   32
#define SHIPTYPE_FLEET  200
#define SHIPTYPE_STARGATE   21

#define CLASS_STONES    0   /*T*/
#define CLASS_GAS       1   /*G*/
#define CLASS_WATER     2   /*W*/
#define CLASS_SATURN    3   /*S*/
#define CLASS_HALFEARTH 4   /*H*/
#define CLASS_DESERT    5   /*D*/
#define CLASS_EARTH     6   /*M*/
#define CLASS_PHANTOM   7   /*P*/
#define CLASS_ICE       8   /*I*/
#define CLASS_MAX_TYPES 9

#define WEAPON_GUN       1
#define WEAPON_LASER     3
#define WEAPON_PHASER    5
#define WEAPON_DISRUPTOR 7
#define WEAPON_PTORPEDO  9

#define MODE_REFRESH    0
#define MODE_REDRAW     1
#define MODE_FLEET      2
#define MODE_STARGATE   3
#define MODE_ONCE       4
#define MODE_ALL        5
#define MODE_OFFENSIV   8
#define MODE_DEFENSIV   9
#define MODE_SHIPS      10
#define MODE_TERRITORIUM  11
#define MODE_BELEIDIGUNG  12
#define MODE_MONEY      13
#define MODE_FORCE      14

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

#define SPAddrA      custom.aud[0].ac_ptr
#define SPLengthA    custom.aud[0].ac_len
#define SPFreqA      custom.aud[0].ac_per
#define SPVolA       custom.aud[0].ac_vol
#define SPDataA      custom.aud[0].ac_dat

#define SPAddrB      custom.aud[1].ac_ptr
#define SPLengthB    custom.aud[1].ac_len
#define SPFreqB      custom.aud[1].ac_per
#define SPVolB       custom.aud[1].ac_vol
#define SPDataB      custom.aud[1].ac_dat

#define SPAddrC      custom.aud[2].ac_ptr
#define SPLengthC    custom.aud[2].ac_len
#define SPFreqC      custom.aud[2].ac_per
#define SPVolC       custom.aud[2].ac_vol
#define SPDataC      custom.aud[2].ac_dat

#define SPAddrD      custom.aud[3].ac_ptr
#define SPLengthD    custom.aud[3].ac_len
#define SPFreqD      custom.aud[3].ac_per
#define SPVolD       custom.aud[3].ac_vol
#define SPDataD      custom.aud[3].ac_dat

// example to access array with offset
#define SHIPDATA_OFFSET     8
#define ShipData(...)       ShipData[(__VA_ARGS__) - SHIPDATA_OFFSET]

#endif
