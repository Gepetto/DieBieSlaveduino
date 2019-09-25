//********************************************************************************************
//                                                                                           *
// AB&T Tecnologie Informatiche - Ivrea Italy                                                *
// http://www.bausano.net                                                                    *
// https://www.ethercat.org/en/products/791FFAA126AD43859920EA64384AD4FD.htm                 *
//                                                                                           *
//********************************************************************************************

//********************************************************************************************
//                                                                                           *
// This software is distributed as an example, "AS IS", in the hope that it could            *
// be useful, WITHOUT ANY WARRANTY of any kind, express or implied, included, but            *
// not limited,  to the warranties of merchantability, fitness for a particular              *
// purpose, and non infringiment. In no event shall the authors be liable for any            *
// claim, damages or other liability, arising from, or in connection with this software.     *
//                                                                                           *
//********************************************************************************************

/*
      Modified by FCam1
      Slave send only example 
     command :  st-flash write /tmp/arduino_build_799212/DBSDuino_V1.ino.bin  0x08000000
*/


//---- AB&T EasyCAT shield application example V.2_0 -------------------------------------------


#define CUSTOM
#include "DieBieSlaveduino.h"

#include "EasyCAT.h"                // EasyCAT library to interface the LAN9252
#include <SPI.h>                    // SPI library



#define UART1_TX PA9
#define UART1_RX PA10
#define LAN_CS PB12

HardwareSerial Serial1(UART1_RX, UART1_TX); //rx ,tx

EasyCAT EASYCAT(LAN_CS);                    // EasyCAT istantiation

// The constructor allow us to choose the pin used for the EasyCAT SPI chip select
// Without any parameter pin 9 will be used

// We can choose between:
// 8, 9, 10, A5, 6, 7

// On the EasyCAT board the SPI chip select is selected through a bank of jumpers

// (The EasyCAT board REV_A allows only pins 8, 9, 10 through 0 ohm resistors)

//EasyCAT EASYCAT(8);              // example:
// pin 8 will be used as SPI chip select
// The chip select chosen by the firmware must match the setting on the board

//---- global variables ---------------------------------------------------------------------------
unsigned long Millis = 0;
unsigned long PreviousMillis = 0;

int i = 0;

//output
uint8_t sineA[73] =
{ 128 , 139 , 150 , 160 , 171 , 181 , 191 , 201 , 209 , 218 , 225 ,
  232 , 238 , 243 , 247 , 251 , 253 , 255 , 255 , 255 , 253 , 251 ,
  247 , 243 , 238 , 232 , 225 , 218 , 209 , 201 , 191 , 181 , 171 ,
  160 , 150 , 139 , 128 , 116 , 105 , 95  , 84  , 74  , 64  , 54  , 46  ,
  37  , 30  , 23  , 17  , 12  , 8   , 4   , 2   , 0   , 0   , 0   , 2   , 4   , 8   ,
  12  , 17  , 23  , 30  , 37  , 46  , 54  ,
  64  , 74  , 84  , 95  , 105 , 116 , 127
};

//---- setup ---------------------------------------------------------------------------------------

void setup()
{
  Serial1.begin(115200);                                             // serial line initialization

  Serial1.println ("\nEasyCAT - Generic EtherCAT slave\n");          // print the banner


  //---- initialize the EasyCAT board -----

  if (EASYCAT.Init() == true)                     // initialization
  { // succesfully completed
    Serial1.println ("initialized");
  }

  else                                                            // initialization failed
  { // the EasyCAT board was not recognized
    Serial1.println ("initialization failed");
    Serial1.println ("Verify the link (SPI) between the stm32 and the lan9252 ");
    // The most common reason is that the SPI
    // chip select choosen on the board doesn't
    // match the one choosen by the firmware
  }
}


void loop()                                             // In the main loop we must call ciclically the
{ // EasyCAT task and our application
  //
  // This allows the bidirectional exachange of the data
  // between the EtherCAT master and our application
  //
  // The EasyCAT cycle and the Master cycle are asynchronous
  //

  EASYCAT.MainTask();                                   // execute the EasyCAT task

  Application();                                        // user applications
}



//---- user application ------------------------------------------------------------------------------

void Application ()
{
  Millis = millis();                                    // As an example for this application
  if (Millis - PreviousMillis >= 10)                    // we choose a cycle time of 10 mS
  {
    PreviousMillis = Millis;                            //

    i = i + 1;
    if (i > 72) i = 0;

    Serial1.println (sineA[i]);

    //Structure :
    //    float       input_float;
    //    uint16_t    input_16;
    //    int16_t     input_int16;
    //    int8_t      input_int8;
    //    uint8_t     input_8;

    float myinput_float;

    EASYCAT.BufferIn.Cust.input_float = ReverseFloat(1.0); //big endian
    EASYCAT.BufferIn.Cust.input_16 = (0x14 >> 8) | (0x14 << 8); //big endian
    EASYCAT.BufferIn.Cust.input_int16 = (0x1E >> 8) | (0x1E << 8);//big endian
    EASYCAT.BufferIn.Cust.input_int8 = 0x28;
    EASYCAT.BufferIn.Cust.input_8 = sineA[i];
  }
}



float ReverseFloat( const float inFloat )
{
  float retVal;
  char *floatToConvert = ( char* ) & inFloat;
  char *returnFloat = ( char* ) & retVal;

  // swap the bytes into a temporary buffer
  returnFloat[0] = floatToConvert[3];
  returnFloat[1] = floatToConvert[2];
  returnFloat[2] = floatToConvert[1];
  returnFloat[3] = floatToConvert[0];

  return retVal;
}



