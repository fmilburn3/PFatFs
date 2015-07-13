/*
 * Arduino Wrapper Function Library for FatFs
 * (c) 2010, David Sirkin sirkin@stanford.edu
 *
 * FatFS by ChaN:
 * http://elm-chan.org/fsw/ff/00index_e.html
 */

 
#ifndef PFFS_h
#define PFFS_h

#include "Energia.h"
#include "Print.h"


#if _SoftSPI < 1
#include "../SPI/SPI.h"
#endif

extern "C"
{
#include "utility/pff.h"
#include "utility/diskio.h"
#include "utility/integer.h"
#include "pffconf.h"
}

#if (_USE_WRITE & _USE_PRINT)
class PFFS : public Print 
#else
class PFFS
#endif
{
  private:
  	static FATFS fatfs_obj;
  	static DIRECT dir_obj;
  	//static FIL fil_obj;
  	static FILINFO fno;
  	
  	static unsigned char CS;
  	
  	static void CS_LOW (void);
  	static void CS_HIGH (void);
	static int CS_READ (void);
	static void DLY100U(void);
	static uint8_t SPI_RECEIVE (void);
	static uint8_t SPI_SEND (const uint8_t);
	static void SPI_SET_DIVIDER (const uint8_t);

  
  public:

	
	PFFS();

	/*--------------------------------------------------------------*/
	/* Functions for Local Disk Control                             */
	/*--------------------------------------------------------------*/
#if _SoftSPI
	FRESULT begin (unsigned char,unsigned char,unsigned char,unsigned char);
#endif
	FRESULT begin (unsigned char);
	FRESULT begin (unsigned char, uint8_t);
	DSTATUS disk_init (void);
    
	/*--------------------------------------------------------------*/
	/* Functions in the FatFs Module Application Interface          */
	/*--------------------------------------------------------------*/
    /* Overloaded methods use working copy of FATFS, DIR or FIL     */
    
	FRESULT mount (FATFS*);						/* Mount/Unmount a logical drive */
	FRESULT open (const char*);					/* Open a file */
#if _USE_READ
	FRESULT read (void*, WORD, WORD*);			/* Read data from the open file */
#endif	
#if _USE_LSEEK
	FRESULT lseek (DWORD);						/* Move file pointer of the open file */
#endif
#if _USE_DIR	
	FRESULT opendir (DIRECT*, const char*);		/* Open a directory */
	FRESULT opendir (const char*);
	FRESULT opendir (void);
	FRESULT readdir (DIRECT*, FILINFO*);			/* Read a directory item from the open directory */
	FRESULT readdir (FILINFO*);
	FRESULT readdir (void);
#endif
#if _USE_WRITE		
	FRESULT write (const void*, WORD, WORD*);	/* Write data to the open file */
	FRESULT close (void);						/* finalize write the open file */
#if _USE_PRINT
	virtual size_t write(uint8_t);
	using Print::write;
#endif
#endif	
#if _USE_STRFUNC
#if _USE_READ
	CHAR* gets (CHAR*, WORD);							/* Get a string from the file */
#endif
#if _USE_WRITE	
	int fputc (CHAR);									/* Put a character to the file */
	int puts (const CHAR*);								/* Put a string to the file */
	int printf (const CHAR*, ...);						/* Put a formatted string to the file */
#endif
#ifndef EOF
#define EOF -1
#endif /* EOF */
#endif
};

extern PFFS FatFs;

#endif
