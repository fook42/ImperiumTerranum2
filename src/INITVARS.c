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

/*ShipData:    (MaxLoad,MaxShield,MaxMove,WeaponPower) */
const r_ShipData shipDataDef[] =
    {   { 3,  7, 1, 1},   /* Wostok,       7*/
        { 4,  5, 1, 1},   /* Mir,         11*/
        { 5, 16, 2, 5},   /* Spaceshuttle,15*/
        { 6, 22, 3, 4},   /* Starwing,    16*/
        { 9, 36, 6, 5},   /* Galaxy,      20*/
        {10, 49, 6, 6},   /* Catamaran,   24*/
        {11, 57, 8, 7},   /* Quasar,      21*/
        { 9, 64, 7, 9},   /* Destroyer    28*/
        {10, 79,12,10},   /* Voyager 1,   25*/
        {15, 93,10,11},   /* Delta-Wing,  30*/
        { 9,107,16,12},   /* Tristars,    33*/
        {19,121,14,14},   /* Voyager 2,   32*/
        {22,129,21,15},   /* Warbird,     35*/
        { 4, 60,20,12},   /* Stargate,    31*/
        {25,136,19,16},   /* Voyager 3,   38*/
        {28,149,27,18},   /* Pulsar,      37*/
        {33,160,24,20}    /* Starburner,  36*/
    };

const sint32 techcosts[] =
    {   0,    2010, 2510, 3020, 13020,3030, 4530,  4040, 5040, 6550,
        7050, 7060, 9560, 11070,11070,12580,13080,15090, 15100,16100,
        17100,18200,29200,21300,21800,22900,23400,24500, 26000,26600,
        27600,29200,30200,33300,33800,35400,38900,40500, 55000,15000,
        55000,85000,135000};

const sint32 projcosts[] =
    {   0,     92010,184020,276030,368040,561050,664060,795000,
        15080, 17090, 22100, 25110, 28120, 31130, 34140, 37150,         // ships costs 0..7
        39160, 43170, 49180, 55190, 63200, 70210, 78220, 86230, 96240,  // ships costs 8..17
        45250, 12260, 17270, 45280, 50290, 19300, 55310,
        35320, 50330, 55340,
        30350, 25360, 25370,
        50380, 35390,130400,
        70410, 80420};


void INITVARS()
{
    int i,j;

    memset(&Pens, 0, sizeof(PenArr));

    memset(&ImgBitMap4, 0, sizeof(ITBitMap));
    memset(&ImgBitMap7, 0, sizeof(ITBitMap));
    memset(&ImgBitMap8, 0, sizeof(ITBitMap));

    for (i = 0; i< SOUNDS;  ++i ) { SoundMemA[i] = NULL; }
    for (i = 0; i< IMAGES;  ++i ) { IMemA[i]     = NULL; }
    for (i = 0; i< MODULES; ++i ) { ModMemA[i]   = NULL; }
    for (i = 0; i< CACHES;  ++i ) { CacheMemA[i] = NULL; }

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

    memset(&Save, 0, sizeof(Save)); // saves time to clean all values in Save-struct

    for (i = 0;  i<25; ++i ) { (void) my_strcpy(Save.SystemName.data[i], PText[i+1] ); }

    for (i = 0;  i<42; ++i ) { (void) my_strcpy(TechnologyL.data[i+1], PText[i+30]); }

    for (i = 1;  i<8;  ++i )            { (void) my_strcpy(Project.data[i], PText[i+74]); }
    for (i = 8;  i<25; ++i )            { (void) my_strcpy(Project.data[i], ProjectShips[i-8]); }
    for (i = 25; i<PROJECT_NOMORE; ++i ){ (void) my_strcpy(Project.data[i], PText[i+60]); }

    for (i = 0; i < MAXCIVS; ++i)        // TODO ... added due to shift of Save-array
    {
        Warnung[i] = 0;
        LastDisplay[i+1] = 0; // TODO ... arrays should also be shifted..

        AllCreative[i] = 1;
        Verschrottung[i] = 0;
        Militaerausgaben[i] = 0;
        GetPlanet[i] = NULL;
        GetPlanetSys[i] = 0;
        vNSonde[i] = false;
        Save.GlobalFlags[i] = GFLAG_EXPLORE;

        memcpy(Save.TechCosts[i],    techcosts, sizeof(Save.TechCosts[i]) * sizeof(Save.TechCosts[i][1]));
        memcpy(Save.ProjectCosts[i], projcosts, sizeof(Save.ProjectCosts[i]) * sizeof(Save.ProjectCosts[i][1]));
        Save.ImperatorState[i] = 2500;
        Save.Bevoelkerung[i] = 5;
        Save.GSteuer[i] = 10;
        for (j = 0; j < MAXCIVS; ++j)
        {
            Save.WarState[i][j] = LEVEL_UNKNOWN;
            Save.LastWarState[i][j] = LEVEL_UNKNOWN;
        }
    }
    Save.WarState[0][0] = LEVEL_PEACE;
    Save.CivPlayer[0] = 1;

    Save.Systems = MAXSYSTEMS;
    for (i = 0; i < MAXSYSTEMS; ++i)
    {
        SystemHeader[i].PlanetMemA = NULL;
    }

    memcpy(ShipData, shipDataDef, sizeof(shipDataDef));

    for (i = 0; i<(MAXCIVS-1); ++i)
    {
        LogoMemA[i] = NULL;
        LogoSMemA[i] = NULL;
        LogoSMemL[i] = 0;
        for (j = 0; j<11; ++j)
        {
            (void) my_strcpy(PNames[i].data[j], PName_def[i][j]);
        }
    }

    // fill in the names of solar-system from language-file...
    for (i = 0; i<9; ++i) { (void) my_strcpy( PNames[1].data[i], PText[105+i]); }

    OffsetX = 0;
    OffsetY = 0;
    OldX = 0;
    OldY = 0;
    LastSystem = 1;
}
