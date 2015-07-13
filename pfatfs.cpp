/*
 * Arduino Wrapper Function Library for FatFs
 * (c) 2010, David Sirkin sirkin@stanford.edu
 *
 * FatFS by ChaN:
 * http://elm-chan.org/fsw/ff/00index_e.html
 */

#include "pfatfs.h"

extern "C" {
#include "utility/diskio.h"
}


FATFS PFFS::fatfs_obj;	/* Working copy of FATFS */
DIRECT PFFS::dir_obj;		/* Working copy of DIR */
//FIL PFFS::fil_obj;		/* Working copy of FIL */
FILINFO PFFS::fno;		/* Working copy of FILINFO */

unsigned char PFFS::CS;	/* Pin to sd card's CS */

uint8_t _clkdivider=4;  //default spi clock 16MHz / 4 = 4MHz

#if _SoftSPI
  uint8_t data;
  uint8_t _SCLK = 7;
  uint8_t _MOSI = 15;
  uint8_t _MISO = 14;  //defaults on HW SPI pins
#endif


/*-----------------------------------------------------------------------*/
/* Constructor                                                           */
/*-----------------------------------------------------------------------*/
/* Don't instantiate from the sketch; it's done at the end of this file. */

PFFS::PFFS() {
}


#if (_USE_WRITE & _USE_PRINT)
size_t PFFS::write(uint8_t character){
	WORD bw;
	pf_write(&character, 1, &bw);   //ok
	return (bw == 1) ? 1 : -1;
}
#endif

/*-------------------------------------------------------------------------

   Local Public Functions (In mm.c and diskio.h)

-------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Chip Select/Deselect Functions for the Card's CS Pin                  */
/*-----------------------------------------------------------------------*/

void PFFS::CS_LOW (void)
{
	digitalWrite(CS, LOW);
}

void PFFS::CS_HIGH (void)
{
	digitalWrite(CS, HIGH);
}

int PFFS::CS_READ (void)
{
	return !digitalRead(CS);
}

/*-----------------------------------------------------------------------*/
/* SPI Functions for diskio driver                  */
/*-----------------------------------------------------------------------*/

uint8_t PFFS::SPI_RECEIVE (void)
{
#if _SoftSPI
//	data = 0;
	digitalWrite(_MOSI, HIGH);
//	
//	if(digitalRead(_MISO)) data+=0x80;	/* bit7 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//	if(digitalRead(_MISO)) data+=0x40;	/* bit6 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//	if(digitalRead(_MISO)) data+=0x20;	/* bit5 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//	if(digitalRead(_MISO)) data+=0x10;	/* bit4 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//	if(digitalRead(_MISO)) data+=0x08;	/* bit3 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//	if(digitalRead(_MISO)) data+=0x04;	/* bit2 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//	if(digitalRead(_MISO)) data+=0x02;	/* bit1 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//	if(digitalRead(_MISO)) data+=0x01;	/* bit0 */
//	digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
//
//	return data;
	return shiftIn(_MISO, _SCLK, MSBFIRST);
#else
	return SPI.transfer( 0xFF );
#endif

}

uint8_t PFFS::SPI_SEND (const uint8_t data)
{
#if _SoftSPI
	//digitalWrite(_MOSI,(data & 0x80));	/* bit7 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	//digitalWrite(_MOSI,(data & 0x40));	/* bit6 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	//digitalWrite(_MOSI,(data & 0x20));	/* bit5 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	//digitalWrite(_MOSI,(data & 0x10));	/* bit4 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	//digitalWrite(_MOSI,(data & 0x08));	/* bit3 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	//digitalWrite(_MOSI,(data & 0x04));	/* bit2 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	//digitalWrite(_MOSI,(data & 0x02));	/* bit1 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	//digitalWrite(_MOSI,(data & 0x01));	/* bit0 */
	//digitalWrite(_SCLK, HIGH);digitalWrite(_SCLK, LOW);
	
	shiftOut(_MOSI, _SCLK, MSBFIRST, data);
	
	return 0; 
#else
	return SPI.transfer( data );
#endif
}

void PFFS::SPI_SET_DIVIDER (const uint8_t _clkdivider)
{
#if _SoftSPI
#else
	SPI.setClockDivider(_clkdivider); 
#endif
}

/*-----------------------------------------------------------------------*/
/* Delay function for diskio driver                                      */
/*-----------------------------------------------------------------------*/

void PFFS::DLY100U (void)
{
	delayMicroseconds(100);
}

/*-----------------------------------------------------------------------*/
/* Setup FS Structures and Register CS Pin                               */
/*-----------------------------------------------------------------------*/
#if _SoftSPI
FRESULT PFFS::begin (
	unsigned char cs_pin,	/* Pin to connect to CS */
	unsigned char SCLK, unsigned char MOSI, unsigned char MISO
)
{
	FRESULT res;
	
	_SCLK = SCLK;
	_MOSI = MOSI;
	_MISO = MISO;
	
	res = begin(cs_pin);
	return res;
}
#endif

FRESULT PFFS::begin (
	unsigned char cs_pin	/* Pin to connect to CS */
)
{
	FRESULT res;
	res = begin(cs_pin, _clkdivider);		// begin with def spi divider
	return res;
}


FRESULT PFFS::begin (
	unsigned char cs_pin,	/* Pin to connect to CS */
	uint8_t clkdivider		// parameter for spi clock divider
)
{
	_clkdivider = clkdivider;
	FRESULT res;
	
	CS = cs_pin;
	pinMode(CS, OUTPUT);
#if _SoftSPI
	pinMode(_SCLK, OUTPUT);
	pinMode(_MOSI, OUTPUT);
	pinMode(_MISO, INPUT);
#else
	SPI.begin();
#endif
	attach_cs_pin(CS_LOW, CS_HIGH, CS_READ);
	attach_dly100u(DLY100U);
	attach_SPIdriver(SPI_RECEIVE, SPI_SEND);

	disk_init();
	res = mount(&fatfs_obj);

	SPI_SET_DIVIDER(_clkdivider);
	
#if _USE_DIR
	res = pf_opendir(&dir_obj, (const CHAR *)"/");
#endif

	return res;
}


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS PFFS::disk_init (
	void //unsigned char drv	/* Physical drive number (0) */
)
{
	DSTATUS res;

	SPI_SET_DIVIDER(_clkdivider);
	res = disk_initialize();
	SPI_SET_DIVIDER(_clkdivider);
	
	return res;
}


/*-------------------------------------------------------------------------

   FatFs Module Public Functions (In ff.c and ff.h)

---------------------------------------------------------------------------
   Overloaded methods use working copy of FATFS, DIR or FIL
-------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Mount/Unmount a Locical Drive                                         */
/*-----------------------------------------------------------------------*/

FRESULT PFFS::mount (
	//unsigned char vol,	/* Logical drive number to be mounted/unmounted */
	FATFS *fs			/* Pointer to new file system object (NULL for unmount) */
)
{
	FRESULT res;
	//SPI.setClockDivider(128);
	SPI_SET_DIVIDER(_clkdivider);
	res = pf_mount(fs);  //ok
	//SPI.setClockDivider(_clkdivider);
	SPI_SET_DIVIDER(_clkdivider);
	return res;
}


/*-----------------------------------------------------------------------*/
/* Open or Create a File                                                 */
/*-----------------------------------------------------------------------*/

FRESULT PFFS::open (
	const CHAR *path	/* Pointer to the file name */
	//unsigned char mode	/* Access mode and file open mode flags */
)
{
	return pf_open(path); //ok
}


/*-----------------------------------------------------------------------*/
/* Read File                                                             */
/*-----------------------------------------------------------------------*/
#if _USE_READ
FRESULT PFFS::read (
	void *buff,			/* Pointer to data buffer */
	WORD btr,			/* Number of bytes to read */
	WORD *br			/* Pointer to number of bytes read */
)
{
	return pf_read(buff, btr, br); //ok
}
#endif

#if _USE_WRITE
/*-----------------------------------------------------------------------*/
/* Write File                                                            */
/*-----------------------------------------------------------------------*/

FRESULT PFFS::write (
	const void *buff,	/* Pointer to the data to be written */
	WORD btw,			/* Number of bytes to write */
	WORD *bw			/* Pointer to number of bytes written */
)
{
	return pf_write(buff, btw, bw);   //ok
}


/*-----------------------------------------------------------------------*/
/* Close File                                                            */
/*-----------------------------------------------------------------------*/

FRESULT PFFS::close (
	void				/* Working copy file object will be closed */
)
{
	WORD bw;
	return pf_write(0, 0, &bw);
}	
	
#endif /* _USE_WRITE */

/*-----------------------------------------------------------------------*/
/* Seek File R/W Pointer                                                 */
/*-----------------------------------------------------------------------*/
#if _USE_LSEEK
FRESULT PFFS::lseek (
	DWORD ofs			/* File pointer from top of file */
)
{
	return pf_lseek(ofs);
}
#endif
/*-----------------------------------------------------------------------*/
/* Create a Directroy Object                                             */
/*-----------------------------------------------------------------------*/
#if _USE_DIR
FRESULT PFFS::opendir (
	DIRECT *dj,			/* Pointer to directory object to create */
	const char *path	/* Pointer to the directory path */
)
{
	return pf_opendir(dj, path);
}

FRESULT PFFS::opendir (
	const char *path	/* Pointer to the directory path */
)
{
	return pf_opendir(&dir_obj, path);
}

FRESULT PFFS::opendir (
	void				/* Working copies of directory object & path */
)
{
	return pf_opendir(&dir_obj, (const char *)"/");   //ok
}


/*-----------------------------------------------------------------------*/
/* Read Directory Entry in Sequense                                      */
/*-----------------------------------------------------------------------*/

FRESULT PFFS::readdir (
	DIRECT *dj,			/* Pointer to the open directory object */
	FILINFO *fno		/* Pointer to file information to return */
)
{
	return pf_readdir(dj, fno);
}

FRESULT PFFS::readdir (
	FILINFO *fno		/* Pointer to file information to return */
)
{
	return pf_readdir(&dir_obj, fno);
}

FRESULT PFFS::readdir (
	void				/* Working copies of directory object & file */
)
{
	return pf_readdir(&dir_obj, NULL);
}
#endif

#if _USE_STRFUNC
#include <stdarg.h>
/*-----------------------------------------------------------------------*/
/* Get a string from the file                                            */
/*-----------------------------------------------------------------------*/
#if _USE_READ
CHAR* PFFS::gets (
	CHAR* buff,			/* Pointer to the string buffer to read */
	WORD len				/* Size of string buffer */
)
{
	return pf_gets(buff, len);
}
#endif
#if _USE_WRITE
/*-----------------------------------------------------------------------*/
/* Put a character to the file                                           */
/*-----------------------------------------------------------------------*/
/* Since putc is also a macro, use fputc to avoid any parameter conflict */

int PFFS::fputc (
	CHAR chr				/* A character to be output */
)
{
	return pf_putc(chr);
}

/*-----------------------------------------------------------------------*/
/* Put a string to the file                                              */
/*-----------------------------------------------------------------------*/

int PFFS::puts (
	const CHAR* str		/* Pointer to the string to be output */
)
{
	return pf_puts(str);
}


/*-----------------------------------------------------------------------*/
/* Put a formatted string to the file                                    */
/*-----------------------------------------------------------------------*/

int PFFS::printf (
	const CHAR* str,	/* Pointer to the format string */
	...					/* Optional arguments... */
)
{
	va_list arp;
	UCHAR c, f, r;
	ULONG val;
	char s[16];
	int i, w, res, cc;


	va_start(arp, str);

	for (cc = res = 0; cc != EOF; res += cc) {
		c = *str++;
		if (c == 0) break;			/* End of string */
		if (c != '%') {				/* Non escape cahracter */
			cc = fputc(c);
			if (cc != EOF) cc = 1;
			continue;
		}
		w = f = 0;
		c = *str++;
		if (c == '0') {				/* Flag: '0' padding */
			f = 1; c = *str++;
		}
		while (c >= '0' && c <= '9') {	/* Precision */
			w = w * 10 + (c - '0');
			c = *str++;
		}
		if (c == 'l') {				/* Prefix: Size is long int */
			f |= 2; c = *str++;
		}
		if (c == 's') {				/* Type is string */
			cc = puts(va_arg(arp, char*));
			continue;
		}
		if (c == 'c') {				/* Type is character */
			cc = fputc(va_arg(arp, int));
			if (cc != EOF) cc = 1;
			continue;
		}
		r = 0;
		if (c == 'd') r = 10;		/* Type is signed decimal */
		if (c == 'u') r = 10;		/* Type is unsigned decimal */
		if (c == 'X') r = 16;		/* Type is unsigned hexdecimal */
		if (r == 0) break;			/* Unknown type */
		if (f & 2) {				/* Get the value */
			val = (ULONG)va_arg(arp, long);
		} else {
			val = (c == 'd') ? (ULONG)(long)va_arg(arp, int) : (ULONG)va_arg(arp, unsigned int);
		}
		/* Put numeral string */
		if (c == 'd') {
			if (val & 0x80000000) {
				val = 0 - val;
				f |= 4;
			}
		}
		i = sizeof(s) - 1; s[i] = 0;
		do {
			c = (UCHAR)(val % r + '0');
			if (c > '9') c += 7;
			s[--i] = c;
			val /= r;
		} while (i && val);
		if (i && (f & 4)) s[--i] = '-';
		w = sizeof(s) - 1 - w;
		while (i && i > w) s[--i] = (f & 1) ? '0' : ' ';
		cc = puts(&s[i]);
	}

	va_end(arp);
	return (cc == EOF) ? cc : res;
}
#endif /*_USE_WRITE*/
#endif /*_USE_STRFUNC*/


/*-----------------------------------------------------------------------*/
/* Instantiate PFFS for Use by Arduino                                    */
/*-----------------------------------------------------------------------*/

PFFS FatFs = PFFS(); 		/* Ceate usable instance */
