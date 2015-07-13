/*----------------------------------------------------------------------*/
/* PFatFs for Energia - speed test project  128kB read/write                                               */
/*----------------------------------------------------------------------*/

/*
===============================================================
NOTE:  This version of FatFs revised so that it will compile on
the MSP430F5529.  Use "DIRECT" instead of "DIR" to instantiate
Frank Milburn  12 July 2015
===============================================================
*/ 


#include "SPI.h" 
#include "pfatfs.h"

#define cs_pin  10

unsigned short int bw, br;//, i;
uint32_t transfer=0;
char buffer[192];
int rc;
uint32_t timetest=0;
uint16_t block_size=64;

void setup() {

  pinMode(PUSH2, INPUT_PULLUP);
  Serial.begin(9600);            // initialize the serial terminal
  FatFs.begin(cs_pin,2);       // initialize FatFS library (cspin, divider, module)
}
void die (int pff_err) 		/* Stop with dying message */
                                /* FatFs return value */
{
  Serial.println();
  Serial.print("Failed with rc=");
  Serial.print(pff_err,DEC);
  for (;;) ;
}


/*-----------------------------------------------------------------------*/
/* Program Main                                                          */
/*-----------------------------------------------------------------------*/

void loop()
{
  Serial.println();
  Serial.println("Press button to start...");

  for(uint16_t i=0; i<192;i++){
    buffer[i]='A';
  }
  block_size=64;
  while(digitalRead(PUSH2)==1){
  }
  delay(100);
  while(digitalRead(PUSH2)==0){
  }

//  Serial.println();
//  Serial.println("Create a file to write (test.txt).");
//  delay(100);
//  rc = FatFs.open("test.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
//  if (rc) die(rc);
//  rc = FatFs.lseek(1048576); //create 16MB file
//  if (rc) die(rc);
//  rc=FatFs.sync();
//  if (rc) die(rc);
//  rc = FatFs.close();
//  if (rc) die(rc);



  Serial.println();
  Serial.println("Block size\tRead/Write\tBytes Transferred\tTime Milis\tSpeed");
  for(uint8_t j=1; j<5;j++){


    Serial.print(block_size);
    Serial.print("\t\t");

    rc = FatFs.open("test.txt");//, FA_READ | FA_WRITE);
    if (rc) die(rc);

    Serial.print("Write\t\t");
    delay(100);
    transfer=0;
    FatFs.lseek(0);
    timetest=millis();
    for (int i=0;i<131072/block_size;i++) {
      rc = FatFs.write(buffer, block_size, &bw);
      if (rc || !bw) break;
      transfer+=bw;
    }
    timetest=(millis()-timetest);
    if (rc) die(rc);

    //rc=FatFs.sync();
    rc=FatFs.close();
    if (rc) die(rc);

    Serial.print(transfer);
    Serial.print("\t\t\t");
    Serial.print(timetest,DEC);
    Serial.print("\t\t");
    Serial.print(transfer/timetest);
    Serial.print(".");
    Serial.print((transfer*10/timetest)%10);
    Serial.println(" kB/s");

    delay(100);
    Serial.print(block_size);
    Serial.print("\t\t");
    Serial.print("Read\t\t");

    delay(100);
    transfer=0;
    FatFs.lseek(0);
    timetest=millis();
    for (int i=0;i<131072/block_size;i++) {
      rc = FatFs.read( buffer, block_size, &br); 	/* Read a chunk of file */
      if (rc || !br) break; 			/* Error or end of file */
      for (uint16_t k = 0; k < br; k++){
        if(buffer[k]=='A') transfer++; 		/* Check read data */
      }
    }
    timetest=(millis()-timetest);

    if (rc) die(rc);
    Serial.print(transfer);
    Serial.print("\t\t\t");
    Serial.print(timetest,DEC);
    Serial.print("\t\t");
    Serial.print(transfer/timetest);
    Serial.print(".");
    Serial.print((transfer*10/timetest)%10);
    Serial.println(" kB/s");
    block_size+=64;
  }

  Serial.println();
  Serial.print("Test completed.");

}
