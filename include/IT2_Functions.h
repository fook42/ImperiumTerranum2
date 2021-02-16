#ifndef _FUNC_INC_H_
#define _FUNC_INC_H_

#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/medplayer.h>
#include <proto/icon.h>
#include "IT2_Types.h"


BOOL getMyScreenToolTypes(BYTE* waName, ULONG* Hires_ID_PTR, ULONG* Lowres_ID_PTR);
BOOL setMyScreenToolTypes(BYTE* waName, ULONG Hires_ID, ULONG Lowres_ID);

int it_round(double x);
char* dez2out(int value, int digits, char* dest);
char* float2out(double fvalue, int dezdigits, int flodigits, char* dest);
char* hex2out(int dez, int digits, char* dest);
ULONG string2hex(char* in_string);
char* my_strcpy(char* destination, const char* source);

bool FillITBitMap(struct ITBitMap* ITBMap, const int BytesPerRow, const int Rows, const int Depth);

uint8 GETCIVFLAG(const int CivVar);
uint8 GETCIVVAR(const int CivFlag);
char* GETCIVADJ(int CivVar);
char* GETCIVNAME(const int CivVar);
char* GETMYADJ(const int CivFlag, bool* DconDone);

void AUTOSHIPTRAVEL(uint8 ActSys, uint8 Mode, r_ShipHeader* ShipPtr);
void AUTOVERHANDLUNG(const int CivFlag1, const int CivFlag2, const int Mode);
bool AUTOWATERTRANSPORT(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr, uint8 ActSys);

void BOX(struct RastPort* RPort, const int Left, const int Top, const int Right, const int Bottom);

// >>> DISKMENU
uint8 BIGSHIPFIGHT(r_ShipHeader* Ship1, r_ShipHeader* Ship2, uint8 Mode, uint8 ActSys);
uint8 STARTBIGSHIPFIGHT(r_ShipHeader* Ship1, r_ShipHeader* Ship2, uint8 Mode, uint8 ActSys);
// <<<

void CEBORCATTACK(uint8 CA_ActPlayerFlag);
void CHECKPLANET(r_PlanetHeader* MyPlanetHeader);
void CHECKPROJECTS(r_PlanetHeader* PlanetPtr, uint8 NewOwner);
bool CHECKSUN(r_ShipHeader* MyShipPtr);
void CLEARINTUITION();
void CLICKRECT(struct RastPort* RPort, int Left, int Top, int Right, int Bottom, const int Color);

void CLOCK();
void CLOSEMYSCREENS();
void CREATECIVILWAR(int CivVar);
void CREATEJAHADR(const int CJ_ActPlayer);
bool CREATEMAQUESSHIP(uint8 SysID, uint8 ShipID);
void CREATENEWSYSTEM(const int ActSys, const int CivVar, const int minHomePlanets);
void CREATEPANIC(r_PlanetHeader* PPtr, uint8 ActSys, uint8 PlanetNum);
bool READPATHS();
void CREATEPATHS();
void DECREASE(uint8 ActSys, r_PlanetHeader* PPtr);
// >>> DISKMENU
void LOADSHIPS(r_ShipHeader* ShipPtr, BPTR FHandle);
void SAVESHIPS(r_ShipHeader* ShipPtr, BPTR FHandle);
void GETSAVENAME(char* Title, char* SName);
void ENCODEDATA();
void DECODEDATA();
void NOMEMMESSAGE();
bool DISKMENU(uint8 Auto);
// <<<
bool DISPLAYIMAGE(char* Fn, const int LEdge, const int TEdge, const int Width, const int Height, const int Depth, struct Screen* DI_Screen, const int CacheNum);
void DISPLAYLOGO(const int DL_ActPlayer, const int LEdge, const int TEdge);
void DISPLAYSTATE();
void DISPLAYTECH(const int TechID);
void DOHUMANITY();
void DOINFLATION(uint8 DI_ActPlayer);
void DOSSIER();
void DOWORMHANDLING(r_ShipHeader* MyShipPtr, uint8 ActSys, bool Visible);
void DRAWGAD(uint16 GadID, uint16 GadSelect);
// >>> DRAWMOVINGSHIP
void FASTREFRESH(sint16 OldX, sint16 OldY, r_ShipHeader* XCludeShip);
void DRAWMOVINGSHIP(r_ShipHeader* MyShipPtr, uint8 ActSys);
// <<<
void DRAWRECT(const int x, const int y, const r_ShipHeader* MyShipPtr, const int color);

void DRAWSTARS(const int Mode);
void DRAWSYSTEM(const int Mode, int ActSys, r_ShipHeader* ActShipPtr);
void ENDSEQUENZ(sint16 Mode);
bool EXISTSPLANET(uint8 CivVar, uint8 ActSys, uint8 Mode);
void FADESTARGATE(uint8 SysID, uint8 ActSys, r_ShipHeader* MyShipPtr);
void FINANZEN();
void FINDENEMYOBJECT(uint8 ActSys, r_ShipHeader* ShipPtr);
void FINDENEMYSYSTEM(uint8 ActSys, uint8 CivVar, r_ShipHeader* ShipPtr);
bool FINDMAQUESSHIP(uint8 ActSys, r_ShipHeader* MyShipPtr);
uint8 FINDMONEYPLANET(uint8 CivFlag, uint8 CivVar, char* s);
uint8 FINDNEXTPLANET(uint8 ActSys, r_ShipHeader* ShipPtr);
bool FINDOBJECT(const int ActSys, const int GetX, const int GetY, void* ExcludeObj);
uint8 FIRSTMENU();
void FORSCHUNG();
void FREESYSTEMMEMORY();
void GAMEEXIT();
void GETPLAYERNAME(uint8 GPN_ActPlayer, char* PName);
UWORD GETRAWCODE();
APTR GETTHESOUND(const int MID);
uint8 GOTONEXTPLANET(uint8 ActSys, r_ShipHeader* MyShipPtr);
uint8 GOTONEXTSYSTEM(uint8 ActSys, r_ShipHeader* ShipPtr);
void GOTOPEACE(int CivVar1, int CivVar2);
void GOTOWAR(int CivFlag1, int CivFlag2);
// >>> HANDLEKNOWNPLANET
void WRITECURRENTPROJECT(r_PlanetHeader* MyPlanetHeader);
void WRITEPLANETSTATUS(r_PlanetHeader* MyPlanetHeader,ByteArr42* ActPProjects);
void WRITEPROJECTSSTATUS(r_PlanetHeader* MyPlanetHeader, ByteArr42* ActPProjects);
void HANDLEKNOWNPLANET(uint8 ActSys, uint8 Mode, r_PlanetHeader* PlanetPtr);
// <<<
// >>> HANDLESYSTEM
uint8 HANDLESYSTEM_DRAWSHIPS(sint8 Mode, uint8 stSys, uint8* PSys, r_ShipHeader** ShipPos);
uint8 DRAWPLANETS(uint8 CivFlag, uint8 stSys, uint8* PSys, uint8* PNum, uint8* PCol);
void SEARCHOBJECT(uint8* ActSys);
void HANDLESYSTEM(uint8* ActSys, r_ShipHeader* ShipPtr);
// <<<
// >>> HIGHSCORE
void CREATEHIGHSCORE(char* HScoreFileName);
uint8 GETCOLOR(const int Col);
void HIGHSCORE();
// <<<
void INFORMUSER();
bool INITDESK(const int Mode);
bool INITLANG();
void INITMENU();
void INITSCREEN(const int Mode);
void INITSOUNDS();
bool INITSTARS();
void INITSTDTAGS();
void INITVARS();
void KILLFLEET(r_ShipHeader* MyShipPtr);
void KLICKGAD(const int x, const int y);
void LINKSHIP(r_ShipHeader* SPtr, r_ShipHeader* TPtr, uint8 Mode);
void LINKTOORBIT(r_PlanetHeader* MyPlanetHeader, r_ShipHeader* MyShipPtr, const int ActSys);
// >>> LOADMOD
void LOADMOD(char* FName, int MID);
LONG GETMIDIPLAYER(struct MMD0 *module);
// <<<
void LOADSOUND(char* FName, const int SoundID);
// >>> MAININTRO
void MAININTRO();
void SETDARKCOLOR(char* FName, r_Col_t* Colors);
// void INTROEXIT(PLANEPTR MyRastPtr, struct MMD0 *module);
void INTROEXIT(PLANEPTR MyRastPtr, struct MMD0 *module, uint16** SMemA, uint32* SMemL);
bool LOADSOUNDS(char* FNamePath, char* FName, uint16** SMemA, uint32* SMemL);
int ff_round(long int value);
void ROTATEpX(VectorObj_t* actObject);
void ROTATEpY(VectorObj_t* actObject);
void ROTATEpZ(VectorObj_t* actObject);
void ROTATEnX(VectorObj_t* actObject);
void ROTATEnY(VectorObj_t* actObject);
void ROTATEnZ(VectorObj_t* actObject);
void FLY(VectorObj_t* actObject, sint32 Factor);
void GREATEFFECT(uint8 Objects, r_Col_t* Colors, uint16** SMemA, uint32* SMemL);
// <<<
void MAIN_FNC();
// >>> MILITAER
//void DRAWDATA(uint8 BSet);
void MILITAER();
// <<<
// >>> MOVESHIP
void MOVESHIP(uint8 ActSys, r_ShipHeader* ShipPtr, bool Visible);
void MOVESHIP_DRAWSCENE(uint8 ActSys, r_ShipHeader* MyShipPtr);
void MOVESHIP_EXPLODE(uint8 ActSys, r_ShipHeader* MyShipPtr);
void MOVESHIP_SMALLANDING(r_PlanetHeader* PPtr, r_ShipHeader* SPtr, uint8 ActSys);
// <<<
struct Screen* OPENCINEMA(uint8 Depth);
bool OPENMAINSCREENS();
BPTR OPENSMOOTH(const char* FName, const uint32 FMode);
// >>> OPTIONMENU
void CHECKGADS(int GadID);
void OPTION_REDUCECOSTS(void);
void OPTION_MULTIPLAYER(void);
void OPTIONMENU(int Mode);
// <<<
// >>> ORBITINFO
int DRAWSHIPS(int i, char (*ShipNames)[15], r_ShipHeader* MyShipPtr);
void SETFLEETPOSITION(uint8 ActSys, r_ShipHeader* StShipPtr, r_ShipHeader* MyShipPtr);
void ORBITINFO(r_ShipHeader* StShipPtr, char* ReqText, uint8 ActSys, sint8 XPosX, sint8 XPosY);
// <<<
void PEACEINFO(int CivVar1, int CivVar2, int CivFlag1, int CivFlag2);
bool PLANETHANDLING(uint8 ActSys, r_ShipHeader* _MyShipPtr);
void PLANETINFO(const int ActSys);
void PLAYERHIGHSCORE(uint8 PHS_ActPlayer);
void PLAYERJINGLE(const int JingleID);
void PLAYSOUND(const int SoundID, const int SoundRate);
void PRINTGLOBALINFOS(uint8 PGI_ActPlayer);
void PROJEKTE();
void PUMPUPTHELEVEL();
void QUOTEPICARD();
bool RAWLOADIMAGE(char* FName, const int LEdge, const int TEdge, const int Width, const int Height, const int Depth, ITBitMap* DestBitMap);
// ***** void RECT(struct Screen* XScreen, uint16 Color, uint16 LEdge, uint16 TEdge, uint16 REdge, uint16 BEdge);
void REFRESHDISPLAY();
void REFRESHSHIPS(r_ShipHeader* ShipPtr, const int SysID, const int Mode);
void REGIERUNG();
void REMOVEPLAYER(const int RP_ActPlayer);
void REQUEST(const char* text1, const char* text2, const int color1, const int color2);
void ROTATEPLANETS(uint8 ActSys);
int SETCOLOR(struct Screen* SC_Screen, char* FName);
bool SETNEWPLANETOWNER(r_PlanetHeader* MyPlanetHeader, r_PlanetHeader* OldPlanet, sint16* TheProject, uint8 ActSys, char** s1, char** s2, bool* PlanetLose);
void SETWORLDCOLORS();
// >>> SHIPINFO
void SHIPINFO_WRITEDATA(r_ShipHeader* MyShipPtr);
void SHIPINFO(uint8 ActSys);
// <<<
void SMALLGAMEEXIT();
// >>> STARDESASTER
void FUCKSYSTEM(uint8 ActSys);
void STARDESASTER(uint8 ActSys, r_ShipHeader* ShipPtr);
// <<<
void STARLANDING(r_PlanetHeader* PPtr, r_ShipHeader* SPtr, uint8 ActSys);
// >>> STARTROTATEPLANETS
//// void CREATEINFOBOX();
void CREATEINFOBOX(struct Window** CIB_Window, struct RastPort** CIB_RPort_PTR);
void STARTROTATEPLANETS();
// <<<
void STATISTIK();
void STOPCIVILWAR(uint8 EndText);
void SUPPORTCIVI(uint32 XValue);
void SWITCHDISPLAY();
void SYSINFO(int SysID, uint8 ThePlayerFlag);
void SYSTEMINFO(uint8 ActSys);
void SYSTEMTOENEMY(uint8 ActSys, uint8 NewOwner, uint8 OldOwner);
bool TAKETECH(uint8 CivFlag1, uint8 CivFlag2);
void UNPACK(uint8* Anfang, uint8* PackedD, const int Laenge, const int Mode);
// >>> VERHANDLUNG ---
void VERHANDLUNG_INIT(int CivFlag, char* CivStr, int Mode);
void VERHANDLUNG(int CivFlag, int Mode);
// <<<
void WAITLOOP(bool no_wait);
// >>> WORMHOLE ---
void WORMHOLE_LOADSOUND(char* FName, const int SoundID);
void WORMHOLE_INITSOUNDS();
bool WORMHOLE_INITIMAGES();
void TRAVEL();
bool SMALLWORMFLIGHT(r_ShipHeader* MyShipPtr);
bool WORMEXIT(bool _WORMEXIT, r_ShipHeader* MyShipPtr, uint8 ActSys);
bool WORMFLIGHT(r_ShipHeader* ShipPtr, uint8 ActSys);
bool WORMHOLE(r_ShipHeader* ShipPtr, uint8 ActSys);
// <<<
void WRITE(const int IPosX, const int IPosY, const int Color, const int DMode, struct RastPort* RPort, const int TA, const char* WText);
void WRITEGALAXYDATA(uint8 ActSys, uint8 ShipMaxMove);

//--------------------------------
void RECT_RP0_C0(const int LEdge, const int TEdge, const int REdge, const int BEdge);
void RECT_RP1(const int Color, const int LEdge, const int TEdge, const int REdge, const int BEdge);
void RECTWIN(struct RastPort* RPort, const int Color, const int LEdge, const int TEdge, const int REdge, const int BEdge);
void MAKEWINBORDER(struct RastPort* RPort, const int LEdge, const int TEdge, const int REdge, const int BEdge, const int Col1, const int Col2, const int Darken);
void KLICKWINGAD(struct RastPort* RPort, const int x, const int y);
struct Window* MAKEWINDOW(const int LEdge, const int TEdge, const int SizeX, const int SizeY, struct Screen* onScreen);
void WRITE_RP0(const int IPosX, const int IPosY, const int Color, const int DMode, const int TA, const char* WText);
void WRITE_RP1(const int IPosX, const int IPosY, const int Color, const int DMode, const int TA, const char* WText);

#endif
