#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"

const char* const PName_def[8][11] =
    {  {"Tacoron","Delosiab","Dhelax","Lakusid","Rosod",
        "Munaxx", "Zehketa", "Yamos", "Jake",   "Emeneres",
        "Andmun"},
       {0,0,0,0,0,0,0,0,0,0,0},
       {"Darukis","Akaneb", "Kleganeb","Sedokub","Khalakeb",
        "Dasekab","Redaseb","Vedekab", "Damakeb","Khetomer",
        "Zhalakab"},
       {"Akusum", "Celadum","Remalus","Dianum","Ecanum",
        "Jamaris","Lorius", "Noratum","Tamarus","Pekateg",
        "Raletis"},
       {"Inarett",  "Ferraccud","Cardaccia","Latecca",  "Ooro",
        "Reedameec","Soorov",   "Zalee",    "Zaalasena","Veda",
        "Sodoora"},
       {"Deragi",  "Sodaki",  "Feragini","Zolati", "Plauti",
        "Olsatebi","Munakati","Rastoki", "Mosavli","Berali",
        "Dresadai"},
       {"Bos","Dak","Bajo","Zeda","Co",
        "Zud","Fec","Paq", "Loa", "Zate",
        "Wid"},
       {"Wolaron","Unareus", "Volkan","Ataneia","Peratham",
        "Musacem","Geratios","Paluae","Idakel", "Amirima",
        "Ressan"}};

const char* const ProjectShips[] =
    {   "Wostok",    "Mir",      "Spaceshuttle","Starwing",
        "Galaxy",    "Catamaran","Quasar",      "Destroyer",
        "Voyager",   "Deltawing","Tristars",    "Voyager2",
        "Warbird",   "Stargate", "Voyager3",    "Pulsar",
        "Starburner" };

const LongArr42 techcosts =
    {{  0,    2010, 2510, 3020, 13020,3030, 4530,  4040, 5040, 6550,
        7050, 7060, 9560, 11070,11070,12580,13080,15090, 15100,16100,
        17100,18200,29200,21300,21800,22900,23400,24500, 26000,26600,
        27600,29200,30200,33300,33800,35400,38900,40500, 55000,15000,
        55000,85000,135000}};

const LongArr42 projcosts =
    {{ 0,  92010, 184020, 276030,
        368040, 561050, 664060,
        795000,
        15080,17090,22100,25110,
        28120,31130,34140,37150,
        39160,43170,49180,55190,
        63200,70210,78220,86230,
        96240,
        45250,12260,17270,45280,
        50290, 19300, 55310,
        35320, 50330, 55340,
        30350, 25360, 25370,
        50380, 35390,130400,
        70410, 80420}};


void INITVARS()
{
    int i,j;

    memset(&Pens, 0, sizeof(PenArr));

    for (i = 0; i< SOUNDS;  i++ ) { SoundMemA[i] = NULL; }
    for (i = 0; i<=IMAGES;  i++ ) { IMemA[i]     = NULL; }
    for (i = 0; i< MODULES; i++ ) { ModMemA[i]   = NULL; }
    for (i = 1; i<=CACHES;  i++ ) { CacheMemA[i] = NULL; }

    memset(&ImgBitMap4, 0, sizeof(ITBitMap));
    memset(&ImgBitMap7, 0, sizeof(ITBitMap));
    memset(&ImgBitMap8, 0, sizeof(ITBitMap));

    MyScreen[0] = NULL;
    MyScreen[1] = NULL;
    Screen2 = 0;
    PathMemA = NULL;
    ZeroSound = NULL;
    Year = 1899;
    Level = 5;
    Display = 100;
    ActPlayer = 1;
    ActPlayerFlag = FLAG_TERRA;
    HomePlanets = 1;
    MaquesShips = 0;
    Valid = true;
    DoClock = false;
    Audio_enable = true;
    Save.PlayMySelf = false;
    Save.NoWorm     = false;
    Save.SmallLand  = false;
    Save.SmallFight = false;
    Save.FastMove   = false;
    Save.WorldFlag  = 0;
    Save.CivilWar   = 0;

    for (i = 1;  i<=25; i++ ) { strcpy(Save.SystemName.data[i-1], PText[i] ); }

    memcpy(&Save.TechCosts[0],    &techcosts, sizeof(LongArr42));
    memcpy(&Save.ProjectCosts[0], &projcosts, sizeof(LongArr42));

    for (i = 1;  i<43; i++ ) { strncpy(TechnologyL.data[i], PText[29+i], 30); }

    for (i = 1;  i<8;  i++ ) { strncpy(Project.data[i], PText[i+74], 30); }
    for (i = 8;  i<25; i++ ) { strcpy (Project.data[i], ProjectShips[i-8]); }
    for (i = 25; i<43; i++ ) { strncpy(Project.data[i], PText[i+60], 30); }

    for (i = 1; i<= MAXCIVS; i++)   // TODO ... arrays should also be shifted..
    {
        Warnung[i] = 0;
        LastDisplay[i] = 0;
    }
    for (i = 0; i < MAXCIVS; i++)        // TODO ... added due to shift of Save-array
    {
        AllCreative[i] = 1;
        Verschrottung[i] = 0;
        Militaerausgaben[i] = 0;
        GetPlanet[i] = NULL;
        GetPlanetSys[i] = 0;
        vNSonde[i] = false;
        Save.CivPlayer[i] = 0;
        Save.GlobalFlags[i] = GFLAG_EXPLORE;
        Save.MaxWarPower[i] = 0;
        Save.SService[i] = 0;
        Save.Military[i] = 0;
        Save.TechCosts[i] = Save.TechCosts[0];
        Save.ProjectCosts[i] = Save.ProjectCosts[0];
        Save.ImperatorState[i] = 2500;
        Save.Staatstopf[i] = 0;
        Save.Bevoelkerung[i] = 5;
        Save.WarPower[i] = 0;
        Save.ActTech[i] = 0;
        Save.GSteuer[i] = 10;
        Save.JSteuer[i] = 0;
        Save.stProject[i] = 0;
        for (j = 0; j < MAXCIVS; j++)
        {
            Save.SSMoney[i][j] = 0;
            Save.WarState[i][j] = LEVEL_UNKNOWN;
            Save.LastWarState[i][j] = LEVEL_UNKNOWN;
        }
        Save.WarState[i][7] = LEVEL_DIED;
        Save.WarState[i][8] = LEVEL_DIED;
    }
    Save.WarState[0][0] = LEVEL_PEACE;
    Save.CivPlayer[0] = 1;

    Save.Systems = MAXSYSTEMS;
    for (i = 0; i < MAXSYSTEMS; i++)
    {
        SystemHeader[i].PlanetMemA = NULL;
    }

    /*ShipData:    MaxLoad,MaxShield,MaxMove,WeaponPower*/
    ShipData(8)  = (r_ShipData) { 3,  7, 1, 1};   /* Wostok,       7*/
    ShipData(9)  = (r_ShipData) { 4,  5, 1, 1};   /* Mir,         11*/
    ShipData(10) = (r_ShipData) { 5, 16, 2, 5};   /* Spaceshuttle,15*/
    ShipData(11) = (r_ShipData) { 6, 22, 3, 4};   /* Starwing,    16*/
    ShipData(12) = (r_ShipData) { 9, 36, 6, 5};   /* Galaxy,      20*/
    ShipData(13) = (r_ShipData) {10, 49, 6, 6};   /* Catamaran,   24*/
    ShipData(14) = (r_ShipData) {11, 57, 8, 7};   /* Quasar,      21*/
    ShipData(15) = (r_ShipData) { 9, 64, 7, 9};   /* Destroyer    28*/
    ShipData(16) = (r_ShipData) {10, 79,12,10};   /* Voyager 1,   25*/
    ShipData(17) = (r_ShipData) {15, 93,10,11};   /* Delta-Wing,  30*/
    ShipData(18) = (r_ShipData) { 9,107,16,12};   /* Tristars,    33*/
    ShipData(19) = (r_ShipData) {19,121,14,14};   /* Voyager 2,   32*/
    ShipData(20) = (r_ShipData) {22,129,21,15};   /* Warbird,     35*/
    ShipData(21) = (r_ShipData) { 4, 60,20,12};   /* Stargate,    31*/
    ShipData(22) = (r_ShipData) {25,136,19,16};   /* Voyager 3,   38*/
    ShipData(23) = (r_ShipData) {28,149,27,18};   /* Pulsar,      37*/
    ShipData(24) = (r_ShipData) {33,160,24,20};   /* Starburner,  36*/
    /* Maximal:                  --,---,27,-                    */

    for (i = 0; i<(MAXCIVS-1); i++)
    {
        LogoMemA[i] = NULL;
        LogoSMemA[i] = NULL;
        LogoSMemL[i] = 0;
        for (j = 0; j<11; j++)
        {
            strcpy(PNames[i].data[j], PName_def[i][j]);
        }
    }
    for (i = 0; i<9; i++) { strcpy( PNames[1].data[i], PText[105+i]); }

    OffsetX = 0;
    OffsetY = 0;
    OldX = 0;
    OldY = 0;
    LastSystem = 1;
}
