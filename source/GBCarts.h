/** EMULib Emulation Library *********************************/
/**                                                         **/
/**                        GBCarts.h                        **/
/**                                                         **/
/** This file contains functions to extract information     **/
/** from GameBoy cartridge images. Also see GBCarts.c.      **/
/**                                                         **/
/** Copyright (C) Marat Fayzullin 1995-2008                 **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/
#ifndef GBCARTS_H
#define GBCARTS_H
#ifdef __cplusplus
extern "C" {
#endif

#define GB_CRC(Header)       (((int)Header[0x14E]<<8)+Header[0x14F])
#define GB_ROMSize(Header)   (GB_ROMBanks(Header)<<14)
#define GB_CMP(Header)       Header[0x14D]
#define GB_Version(Header)   Header[0x14C]
#define GB_TypeID(Header)    Header[0x147]
#define GB_ValidType(Header) (GB_Type(Header)!=0)
#define GB_Developer(Header) (GB_CMP(Header)==0xE7)
#define GB_Japanese(Header)  (Header[0x14A]==0x00)
#define GB_OnlyColor(Header) (Header[0x143]==0xC0)
#define GB_WithColor(Header) (Header[0x143]==0x80)
#define GB_SuperGB(Header)   (Header[0x146]==0x03)
#define GB_ColorGB(Header)   (GB_OnlyColor(Header)||GB_WithColor(Header))
#define GB_Camera(Header)    (GB_TypeID(Header)==0xFC)
#define GB_Tilt(Header)      (GB_TypeID(Header)==0x22)
#define GB_Timer(Header)     ((GB_TypeID(Header)>=0x0F)&&(GB_TypeID(Header)<=0x10))
#define GB_Rumble(Header)    ((GB_TypeID(Header)>=0x1C)&&(GB_TypeID(Header)<=0x1E))
#define GB_MBC1(Header)      ((GB_TypeID(Header)>=0x01)&&(GB_TypeID(Header)<=0x03))
#define GB_MBC2(Header)      ((GB_TypeID(Header)>=0x05)&&(GB_TypeID(Header)<=0x06))
#define GB_MMM01(Header)     ((GB_TypeID(Header)>=0x0B)&&(GB_TypeID(Header)<=0x0D))
#define GB_MBC3(Header)      ((GB_TypeID(Header)>=0x0F)&&(GB_TypeID(Header)<=0x13))
#define GB_MBC4(Header)      ((GB_TypeID(Header)>=0x15)&&(GB_TypeID(Header)<=0x17))
#define GB_MBC5(Header)      ((GB_TypeID(Header)>=0x19)&&(GB_TypeID(Header)<=0x22))
#define GB_HuC1(Header)      (GB_TypeID(Header)==0xFF)
#define GB_HuC3(Header)      (GB_TypeID(Header)==0xFE)

/** GB_MakerID() *********************************************/
/** Cartridge producer ID code.                             **/
/*************************************************************/
unsigned char GB_MakerID(const unsigned char *Header);

/** GB_ROMBanks() ********************************************/
/** Size of on-cartridge ROM in 16kB pages.                 **/
/*************************************************************/
int GB_ROMBanks(const unsigned char *Header);

/** GB_RAMSize() *********************************************/
/** Size of on-cartridge RAM in bytes.                      **/
/*************************************************************/
int GB_RAMSize(const unsigned char *Header);

/** GB_Name() ************************************************/
/** Extract and truncate cartridge name. Returns a pointer  **/
/** to the internal buffer!                                 **/
/*************************************************************/
char *GB_Name(const unsigned char *Header);

/** GB_Maker() ***********************************************/
/** Try to figure out producer's name. Returns name or NULL **/
/** if no producer is known for cartridge's GB_MakerID().   **/
/*************************************************************/
const char *GB_Maker(const unsigned char *Header);

/** GB_Type() ************************************************/
/** Returns the textual name of the cartridge MBC type or   **/
/** NULL for unknown types.                                 **/
/*************************************************************/
const char *GB_Type(const unsigned char *Header);

/** GB_RealCMP() *********************************************/
/** Calculate complement byte of a cartridge.               **/
/*************************************************************/
unsigned char GB_RealCMP(const unsigned char *Header);

/** GB_RealCRC() *********************************************/
/** Calculate CRC of a cartridge.                           **/
/*************************************************************/
unsigned short GB_RealCRC(const unsigned char *Data);

/** GB_Battery() *********************************************/
/** Return 1 if cartridge has a battery, 0 otherwise.       **/
/*************************************************************/
unsigned char GB_Battery(const unsigned char *Header);

#ifdef __cplusplus
}
#endif
#endif /* GBCARTS_H */
