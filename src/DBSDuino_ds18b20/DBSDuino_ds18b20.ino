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

//********************************************************************************************
// Modiflied by FCam1
// Tested on a DieBieSlave device
//********************************************************************************************

#define CUSTOM
#include "DieBieSlaveduino_float.h" // Generated by Easy configurator

#include "EasyCAT.h"                // EasyCAT library to interface the LAN9252
#include <SPI.h>                    // SPI library

#include <OneWire.h>
#include <DallasTemperature.h>


#define UART1_TX PA9
#define UART1_RX PA10
#define LAN_CS      PB12
#define DQ             PB4

OneWire oneWire(DQ);
DallasTemperature sensors(&oneWire);

uint8_t address[8] = {0x28 , 0xFF , 0xB0, 0xA , 0xA3 , 0x15 , 0x1 , 0x43};//DS18b20
uint8_t selected;

HardwareSerial Serial1(UART1_RX, UART1_TX); //rx ,tx

EasyCAT EASYCAT(LAN_CS);                    // EasyCAT istantiation



unsigned long Millis = 0;
unsigned long PreviousMillis = 0;

//---- setup ---------------------------------------------------------------------------------------

void setup()
{
  Serial1.begin(115200);                                             // serial line initialization
  //(used only for debug

//---- DS18b20 -----
  sensors.begin();
  selected = sensors.getDeviceCount();


  if (selected == 0) {
    Serial1.println("sensor not found in setup !");
    return;
  }
  Serial1.println("Sensor found ");

  sensors.setResolution(address, 9);

  Serial1.print("Sensor Resolution: ");
  Serial1.print(sensors.getResolution(address), DEC);
  Serial1.println();

  //---- initialize the EasyCAT board -----

  if (EASYCAT.Init() == true)                                     // initialization
  { // succesfully completed
    Serial1.println ("LAN9252 initialized");                                 //
  }

  else                                                            // initialization failed
  { // the EasyCAT board was not recognized
    Serial1.println ("initialization failed");                       //
    // The most common reason is that the SPI
    // chip select choosen on the board doesn't
    // match the one choosen by the firmware
  }

  delay(100);

}


//---- main loop ----------------------------------------------------------------------------------------

void loop()                                             // In the main loop we must call ciclically the
{
  // EasyCAT task and our application
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
  if (Millis - PreviousMillis >= 10)         // we choose a cycle time of 10 mS
  {
    PreviousMillis = Millis;


    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempC = sensors.getTempC(address);

    if (tempC == DEVICE_DISCONNECTED_C)
    {
      Serial1.println("Error: Could not read temperature data");
      EASYCAT.BufferIn.Cust.InputVariable_float = 0; // Note that Ethercat uses little endian byte order
      return;
    }

    Serial1.print("Temp C: ");
    Serial1.println(tempC);

    EASYCAT.BufferIn.Cust.InputVariable_float = tempC; // Note that Ethercat uses little endian byte order
  }
}



