char Error_Text[] = "ERROR\0";

#include "IT2_Defines.h"
#include "IT2_Types.h"
#include "IT2_Vars.h"
#include "IT2_Functions.h"
/* -------------------------------------------------------- */

uint8 CIVFLAGs[]={0,           FLAG_TERRA,  FLAG_KLEGAN, FLAG_REMALO, FLAG_CARDAC,
                  FLAG_FERAGI, FLAG_BAROJA, FLAG_VOLKAN, FLAG_OTHER,  FLAG_MAQUES};

uint8 CIVADJs[] ={0, 115, 116, 117, 118, 119, 120, 121};
uint8 CIVADJs8[]={122, 123};


uint8 GETCIVFLAG(uint8 CivVar)
{
    if ((0 < CivVar) && ((sizeof(CIVFLAGs)/sizeof(CIVFLAGs[0])) > CivVar))
    {
        return CIVFLAGs[CivVar];
    } else {
        return CIVFLAGs[0];
    }
}

/* -------------------------------------------------------- */
uint8 GETCIVVAR(uint8 CivFlag)
{
    uint8 Result;

    switch (CivFlag & FLAG_CIV_MASK) {
        case FLAG_TERRA:   Result = 1; break;
        case FLAG_KLEGAN:  Result = 2; break;
        case FLAG_REMALO:  Result = 3; break;
        case FLAG_CARDAC:  Result = 4; break;
        case FLAG_FERAGI:  Result = 5; break;
        case FLAG_BAROJA:  Result = 6; break;
        case FLAG_VOLKAN:  Result = 7; break;
        case FLAG_OTHER:   Result = 8; break;
        case FLAG_MAQUES:  Result = 9; break;
        default:           Result = 0;
    }
    return Result;
}
/* -------------------------------------------------------- */

char* GETCIVADJ(uint8 CivVar)
{

    if (0 < CivVar)
    {
        if ((sizeof(CIVADJs)/sizeof(CIVADJs[0])) > CivVar)
        {
            return PText[CIVADJs[CivVar]];
        } else {
            CivVar -= (sizeof(CIVADJs)/sizeof(CIVADJs[0]))-1;
            if ((CivVar>0) && (CivVar < (sizeof(CIVADJs8)/sizeof(CIVADJs8[0]))))
            {
                return PText[CIVADJs8[CivVar]];
            } else {
                return GETCIVADJ(GETCIVVAR(Save.WorldFlag));
            }
        }
    } else {
        return Error_Text;
    }
}
/* -------------------------------------------------------- */
char* GETCIVNAME(uint8 CivVar)
{
   char* Result;
   switch (CivVar) {
      case 1: Result = _PT_Terraner;  break;
      case 2: Result = _PT_Kleganer;  break;
      case 3: Result = _PT_Remalonen; break;
      case 4: Result = _PT_Cardaccer; break;
      case 5: Result = _PT_Feragis;   break;
      case 6: Result = _PT_Barojaner; break;
      case 7: Result = _PT_Volkanier; break;
      case 8: switch (Save.WorldFlag) {
                case WFLAG_CEBORC: Result = _PT_Ceborcs;   break;
                case WFLAG_DCON:   Result = _PT_Dconianer; break;
                case WFLAG_JAHADR: Result = _PT_JaHadr;    break;
                default: Result = GETCIVNAME(GETCIVVAR(Save.WorldFlag));
              }; break;
      case 9: Result = _PT_Die_Maques; break;
      default: Result = (char*) &Error_Text;
   }
   return Result;
}

/* -------------------------------------------------------- */
char* GETMYADJ(uint8 CivFlag, bool* DconDone)
{
    char* Result;
    switch (CivFlag) {
        case FLAG_TERRA:  Result = PText[540]; break;
        case FLAG_KLEGAN: Result = PText[541]; break;
        case FLAG_REMALO: Result = PText[542]; break;
        case FLAG_CARDAC: Result = PText[543]; break;
        case FLAG_FERAGI: Result = PText[544]; break;
        case FLAG_BAROJA: Result = PText[545]; break;
        case FLAG_VOLKAN: Result = PText[546]; break;
        case FLAG_OTHER:  switch (Save.WorldFlag) {
                            case WFLAG_CEBORC: Result = PText[547]; break;
                            case WFLAG_DCON:   {
                                                Result = PText[548];
                                                *DconDone = true;
                                                }; break;
                            case WFLAG_JAHADR: Result = PText[549]; break;
                            default: Result = GETMYADJ(Save.WorldFlag, DconDone);
                          }; break;
        case FLAG_MAQUES: Result = PText[550]; break;
        default: Result = (char*) &Error_Text;
    }
    return Result;
}
