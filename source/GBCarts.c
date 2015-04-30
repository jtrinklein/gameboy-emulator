/** EMULib Emulation Library *********************************/
/**                                                         **/
/**                        GBCarts.c                        **/
/**                                                         **/
/** This file contains functions to extract information     **/
/** from GameBoy cartridge images. It also includes a list  **/
/** of known manufacturer codes. See GBCarts.h for other    **/
/** cartridge related functions.                            **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1995-2008                 **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/

#include "GBCarts.h"
#include <string.h>
#include <ctype.h>

/** Companies ************************************************/
/** This is a list of known producers and their MakerIDs.   **/
/** It is used by the GB_Maker() function.                  **/
/*************************************************************/
static const struct { unsigned char Code;const char *Name; } Companies[] =
{
  { 0x33,"Nintendo"  },{ 0x79,"Accolade"  },{ 0xA4,"Konami"    },
  { 0x67,"Ocean"     },{ 0x56,"LJN"       },{ 0x99,"ARC"       },
  { 0x01,"Nintendo"  },{ 0x08,"Capcom"    },{ 0x01,"Nintendo"  },
  { 0xBB,"SunSoft"   },{ 0xA4,"Konami"    },{ 0xAF,"Namcot"    },
  { 0x49,"Irem"      },{ 0x9C,"Imagineer" },{ 0xA6,"Kawada"    },
  { 0xB1,"Nexoft"    },{ 0x51,"Acclaim"   },{ 0x60,"Titus"     },
  { 0xB6,"HAL"       },{ 0x33,"Nintendo"  },{ 0x0B,"Coconuts"  },
  { 0x54,"Gametek"   },{ 0x7F,"Kemco"     },{ 0xC0,"Taito"     },
  { 0xEB,"Atlus"     },{ 0xE8,"Asmik"     },{ 0xDA,"Tomy"      },
  { 0xB1,"ASCII"     },{ 0xEB,"Atlus"     },{ 0xC0,"Taito"     },
  { 0x9C,"Imagineer" },{ 0xC2,"Kemco"     },{ 0xD1,"Sofel"     },
  { 0x61,"Virgin"    },{ 0xBB,"SunSoft"   },{ 0xCE,"FCI"       },
  { 0xB4,"Enix"      },{ 0xBD,"Imagesoft" },{ 0x0A,"Jaleco"    },
  { 0xDF,"Altron"    },{ 0xA7,"Takara"    },{ 0xEE,"IGS"       },
  { 0x83,"Lozc"      },{ 0x50,"Absolute"  },{ 0xDD,"NCS"       },
  { 0xE5,"Epoch"     },{ 0xCB,"VAP"       },{ 0x8C,"Vic Tokai" },
  { 0xC2,"Kemco"     },{ 0xBF,"Sammy"     },{ 0xD2,"Quest"     },
  { 0x18,"Hudson Soft"    },{ 0xCA,"Palcom/Ultra" },
  { 0xCA,"Palcom/Ultra"   },{ 0xC5,"Data East" },
  { 0xA9,"Technos Japan"  },{ 0xD9,"Banpresto" },
  { 0x72,"Broderbund"     },{ 0x7A,"Triffix Entertainment" },
  { 0xE1,"Towachiki"      },{ 0x93,"Tsuburava" },
  { 0xC6,"Tonkin House"   },{ 0xCE,"Pony Canyon" },
  { 0x70,"Infogrames"     },{ 0x8B,"Bullet-Proof Software" },
  { 0x55,"Park Place"     },{ 0xEA,"King Records" },
  { 0x5D,"Tradewest"      },{ 0x6F,"ElectroBrain" },
  { 0xAA,"Broderbund"     },{ 0xC3,"SquareSoft" },
  { 0x52,"Activision"     },{ 0x5A,"Bitmap Brothers/Mindscape" },
  { 0x53,"American Sammy" },{ 0x47,"Spectrum Holobyte" },
  { 0x18,"Hudson Soft"    },{ 0x8B,"Bullet-Proof Software" },
  { 0x00,0 }
};

/** TypeNames ************************************************/
/** This is a list of known cartridge type names. It is     **/
/** used by the GB_TypeName() function.                     **/
/*************************************************************/
static const struct { unsigned char Type;const char *Name; } TypeNames[] =
{
  { 0x00,"ROM ONLY" },
  { 0x01,"ROM+MBC1" },
  { 0x02,"ROM+MBC1+RAM" },
  { 0x03,"ROM+MBC1+RAM+Battery" },
  { 0x05,"ROM+MBC2" },
  { 0x06,"ROM+MBC2+Battery" },
  { 0x08,"ROM+RAM" },
  { 0x09,"ROM+RAM+Battery" },
  { 0x0B,"ROM+MMM01" },
  { 0x0C,"ROM+MMM01+RAM" },
  { 0x0D,"ROM+MMM01+RAM+Battery" },
  { 0x0F,"ROM+MBC3+Battery+Timer" },
  { 0x10,"ROM+MBC3+RAM+Battery+Timer" },
  { 0x11,"ROM+MBC3" },
  { 0x12,"ROM+MBC3+RAM" },
  { 0x13,"ROM+MBC3+RAM+Battery" },
  { 0x15,"ROM+MBC4" },
  { 0x16,"ROM+MBC4+RAM" },
  { 0x17,"ROM+MBC4+RAM+Battery" },
  { 0x19,"ROM+MBC5" },
  { 0x1A,"ROM+MBC5+RAM" },
  { 0x1B,"ROM+MBC5+RAM+Battery" },
  { 0x1C,"ROM+MBC5+Rumble" },
  { 0x1D,"ROM+MBC5+RAM+Rumble" },
  { 0x1E,"ROM+MBC5+RAM+Battery+Rumble" },
  { 0x22,"ROM+MBC5+Tilt" },
  { 0xFC,"Nintendo Pocket Camera" },
  { 0xFD,"Bandai TAMA5" },
  { 0xFE,"Hudson HuC-3" },
  { 0xFF,"Hudson HuC-1" },
  { 0x00,0 }
};

/** GB_MakerID() *********************************************/
/** Cartridge producer ID code.                             **/
/*************************************************************/
unsigned char GB_MakerID(const unsigned char *Header)
{
  static const char *Digits = "0123456789ABCDEF";
  unsigned char ID;
  char *P;

  ID=Header[0x14B];
  if(ID==0x33)
  {
    P=strchr(Digits,(char)toupper(Header[0x144]));
    if(P) ID=(P-Digits)<<4; else return(0x33);
    P=strchr(Digits,(char)toupper(Header[0x145]));
    if(P) ID|=P-Digits; else return(0x33);
  }

  return(ID);
}

/** GB_ROMBanks() ********************************************/
/** Size of on-cartridge ROM in 16kB pages.                 **/
/*************************************************************/
int GB_ROMBanks(const unsigned char *Header)
{
  int J;

  /* Check upper nibble (optional if <>0) */
  J=Header[0x148]>>4;
  J=J? (2<<J):0;

  /* Check lower nibble (obligatory) */
  J+=2<<(Header[0x148]&0x0F);

  /* Truncate and return result */
  return(J);
}

/** GB_RAMSize() *********************************************/
/** Size of on-cartridge RAM in bytes.                      **/
/*************************************************************/
int GB_RAMSize(const unsigned char *Header)
{
  static const int Sizes[6] = { 0,2048,8192,32768,131072,65536 };

  switch(GB_TypeID(Header))
  {
    case 0x02: case 0x03: case 0x08: case 0x09:
    case 0x0D: case 0x0F: case 0x10: case 0x12:
    case 0x13: case 0x16: case 0x17: case 0x1A:
    case 0x1B: case 0x1D: case 0x1E: case 0xFC:
      return(Header[0x149]<=5? Sizes[Header[0x149]]:0);

    case 0x06: /* MBC2 handles 512 bytes of RAM */
      return(512);

    case 0xFE: /* HuC3 has 32kB of RAM */
    case 0xFF: /* HuC1 has 32kB of RAM */
      return(32768);
  }

  return(0);
}

/** GB_Name() ************************************************/
/** Extract and truncate cartridge name. Returns a pointer  **/
/** to the internal buffer!                                 **/
/*************************************************************/
char *GB_Name(const unsigned char *Header)
{
  static char Buf[32];
  int J,C;

  for(J=0;(J<16)&&Header[J+0x134];J++)
  {
    C=Header[J+0x134];
    Buf[J]=(C>0x20)&&(C<0x80)? C:' ';
  }
  Buf[J]='\0';
  return(Buf);
}

/** GB_Type() ************************************************/
/** Returns the textual name of the cartridge MBC type or   **/
/** NULL for unknown types.                                 **/
/*************************************************************/
const char *GB_Type(const unsigned char *Header)
{
  int Type,J;

  Type=GB_TypeID(Header);
  for(J=0;TypeNames[J].Name;J++)
    if(Type==TypeNames[J].Type) return(TypeNames[J].Name);
  return(0);
}

/** GB_Maker() ***********************************************/
/** Try to figure out producer's name. Returns name or NULL **/
/** if no producer is known for cartridge's GB_MakerID().   **/
/*************************************************************/
const char *GB_Maker(const unsigned char *Header)
{
  int Code,J;

  Code=GB_MakerID(Header);
  for(J=0;Companies[J].Name;J++)
    if(Code==Companies[J].Code) return(Companies[J].Name);
  return(0);
}

/** GB_RealCMP() *********************************************/
/** Calculate complement byte of a cartridge.               **/
/*************************************************************/
unsigned char GB_RealCMP(const unsigned char *Header)
{
  unsigned char R;
  int J;

  for(J=0x134,R=25;J<0x14D;J++) R+=Header[J];
  return(0x100-R);
}

/** GB_RealCRC() *********************************************/
/** Calculate CRC of a cartridge.                           **/
/*************************************************************/
unsigned short GB_RealCRC(const unsigned char *Data)
{
  unsigned short R;
  int J,Length;

  Length=GB_ROMSize(Data);R=0;
  for(J=0;J<Length;J++) R+=Data[J];
  return(R-Data[0x14E]-Data[0x14F]);
}

/** GB_Battery() *********************************************/
/** Return 1 if cartridge has a battery, 0 otherwise.       **/
/*************************************************************/
unsigned char GB_Battery(const unsigned char *Header)
{
  static const unsigned char BatteryTypes[12] =
  { 0x03,0x06,0x09,0x0D,0x0F,0x10,0x13,0x17,0x1B,0x1E,0xFC,0xFF };

  return(memchr(BatteryTypes,GB_TypeID(Header),12)!=0);
}
