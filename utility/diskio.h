/*-----------------------------------------------------------------------
/  PFF - Low level disk interface modlue include file    (C)ChaN, 2009
/-----------------------------------------------------------------------*/

#ifndef _DISKIO

#ifdef __cplusplus
extern "C" {
#endif

#include "integer.h"


/* Status of Disk Functions */
typedef BYTE	DSTATUS;


/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Function succeeded */
	RES_ERROR,		/* 1: Disk error */
	RES_NOTRDY,		/* 2: Not ready */
	RES_PARERR		/* 3: Invalid parameter */
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */

void attach_cs_pin (void (*cs_low)(void), void (*cs_high)(void), int (*cs_read)(void));
void attach_dly100u (void (*dly100u)(void));
void attach_SPIdriver(BYTE (*spi_receive)(void), BYTE (*spi_send)(BYTE));//, void (*spi_set_divider)(BYTE));

DSTATUS disk_initialize ();
DRESULT disk_readp (BYTE*, DWORD, WORD, WORD);
DRESULT disk_writep (const BYTE*, DWORD);

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */

/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK			0x08	/* Block addressing */

#define _DISKIO
#ifdef __cplusplus
}
#endif
#endif
