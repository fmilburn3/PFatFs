/*----------------------------------------------------------------------*/
/* Petit FatFs sample project for generic uC  (C)ChaN, 2010             */
/*----------------------------------------------------------------------*/
/* ported to Energia */
/* copy the two files t_read.txt and t_write.txt from the example folder in the root of the sd card*/

/*
===============================================================
NOTE:  This version of FatFs revised so that it will compile on
the MSP430F5529.  Use "DIRECT" instead of "DIR" to instantiate
Frank Milburn  12 July 2015
===============================================================
*/ 


#include "SPI.h" 
#include "pfatfs.h"

#define cs_pin      10             // chip select pin 
#define read_buffer 128             // size (in bytes) of read buffer 

  unsigned short int bw, br;//, i;
  char buffer[read_buffer];
  int rc;
  DIRECT dir;				/* Directory object */
  FILINFO fno;			/* File information object */

void setup() {

         pinMode(PUSH2, INPUT_PULLUP);
         
         Serial.begin(9600);            // initialize the serial terminal
         FatFs.begin(cs_pin);             // initialize FatFS library calls
         }
void die (		/* Stop with dying message */
         int pff_err	/* FatFs return value */
         )
         {
         Serial.println();Serial.print("Failed with rc=");Serial.print(pff_err,DEC);
         for (;;) ;
  }


/*-----------------------------------------------------------------------*/
/* Program Main                                                          */
/*-----------------------------------------------------------------------*/

void loop()
{
        Serial.println();
        Serial.println("Press button to start...");
        while(digitalRead(PUSH2)==1){}
        delay(100);
        while(digitalRead(PUSH2)==0){}
        
        Serial.println();
	Serial.println("Open a test file (t_read.txt).");
        delay(100);
	rc = FatFs.open("T_READ.TXT");
	if (rc) die(rc);

	Serial.println();
        Serial.println("Type the file content.");
        delay(100);
	for (;;) {
		rc = FatFs.read(buffer, sizeof(buffer), &br);	/* Read a chunk of file */
		if (rc || !br) break;			/* Error or end of file */
		for (uint16_t i = 0; i < br; i++)		/* Type the data */
			Serial.print(buffer[i]);
                        delay(100);
	}
	if (rc) die(rc);

        Serial.println();
	Serial.println("Open a file to write (t_write.txt).");
	delay(100);
        rc = FatFs.open("T_WRITE.TXT");
	if (rc) die(rc);

	Serial.println();
        Serial.println("Write a text data. (10 x Hello world!)");
        delay(100);
        bw=0;
        for (uint16_t i=0;i<10;i++) {
		rc = FatFs.write("Hello world!\r\n", 14, &bw);
                if (rc || !bw) break;
	}
	if (rc) die(rc);
    
	rc = FatFs.write(0, 0, &bw);  //Finalize write
	if (rc) die(rc);

     
        delay(100);
        Serial.println();
	Serial.println("Verify the write process (t_write.txt).");
        delay(100);
	rc = FatFs.open("T_WRITE.TXT");
	if (rc) die(rc);

	Serial.println();
        Serial.println("Type the file content.");
        delay(100);
	for (;;) {
		rc = FatFs.read(buffer, sizeof(buffer), &br);	/* Read a chunk of file */
		if (rc || !br) break;			/* Error or end of file */
		for (uint16_t i = 0; i < br; i++)		/* Type the data */
			Serial.print(buffer[i]);
                        delay(100);
        }
	if (rc) die(rc);
        
        Serial.println();
	Serial.println("Open root directory.");
	delay(100);
        rc = FatFs.opendir(&dir, "");
	if (rc) die(rc);

	Serial.println();
        Serial.println("Directory listing...");
        delay(100);
	for (;;) {
		rc = FatFs.readdir(&dir, &fno);	/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR) {Serial.print("<dir>\t"); Serial.println(fno.fname);delay(100);}
		else {Serial.print(fno.fsize);Serial.print("\t"); Serial.println(fno.fname);delay(100);}
	}
	if (rc) die(rc);

        Serial.println();
	Serial.print("Test completed.");
	//for (;;) ;
}
