/*---------------------------------------------------------------------------/
/ Petit FatFs Configuration Options
/
/ CAUTION! Do not forget to make clean the project after any changes to
/ the configuration options.
/
/----------------------------------------------------------------------------*/

#ifndef _PFFCONF
#define _PFFCONF

#define _SoftSPI	0   /* 1:Enable included bitbang SPI driver */

#define _USE_PRINT  0   /* 1:Enable PRINT class functions */

#define	_USE_READ	1	/* 1:Enable pf_read() */

#define	_USE_DIR	1	/* 1:Enable pf_opendir() and pf_readdir() */

#define	_USE_LSEEK	1	/* 1:Enable pf_lseek() */

#define	_USE_WRITE	1	/* 1:Enable pf_write() */

#define	_USE_STRFUNC	2	/* 0:Disable or 1-2:Enable */
/* To enable string functions, set _USE_STRFUNC to 1 or 2. */

#define _FS_FAT12	0	/* 1:Enable FAT12 support */

#define _FS_FAT32	1	/* 1:Enable FAT32 support */

#define	_CODE_PAGE	1
/* Defines which code page is used for path name. Supported code pages are:
/  932, 936, 949, 950, 437, 720, 737, 775, 850, 852, 855, 857, 858, 862, 866,
/  874, 1250, 1251, 1252, 1253, 1254, 1255, 1257, 1258 and 1 (ASCII only).
/  SBCS code pages except for 1 requiers a case conversion table. This
/  might occupy 128 bytes on the RAM on some platforms, e.g. avr-gcc. */

#define _WORD_ACCESS	0
/* The _WORD_ACCESS option defines which access method is used to the word
/  data in the FAT structure.
/
/   0: Byte-by-byte access. Always compatible with all platforms.
/   1: Word access. Do not choose this unless following condition is met.
/
/  When the byte order on the memory is big-endian or address miss-aligned
/  word access results incorrect behavior, the _WORD_ACCESS must be set to 0.
/  If it is not the case, the value can also be set to 1 to improve the
/  performance and code efficiency. */


/* End of configuration options. Do not change followings without care.     */
/*--------------------------------------------------------------------------*/

#endif //_PFFCONF
