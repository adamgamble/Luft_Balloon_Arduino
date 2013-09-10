#include <SoftwareSerial.h>
#include <SD.h>

// GPS Setup
#define RXPIN 2
#define TXPIN 3
#define GPSBAUD 4800

String gps_string;

SoftwareSerial uart_gps(RXPIN, TXPIN);

//SD Card stuffs
File myFile;

void setup()
{
  Serial.begin(115200);
  //Sets baud rate of your GPS
  uart_gps.begin(GPSBAUD);
  
  //SD Card Setup
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  
  Serial.println("");  
  Serial.println("       ...waiting for gps lock...           ");
  Serial.println("");
}

void loop()
{
  while(uart_gps.available())     // While there is data on the RX pin...
  {
      char c = uart_gps.read();    // load the data into a variable...
      gps_string.concat(c);
      if (c == '\n') {
        myFile = SD.open("gps.txt", FILE_WRITE);
        if (myFile) {
          Serial.print("Writing to gps.txt...");
          myFile.println(gps_string);
	    // close the file:
          myFile.close();
          Serial.println("done.");
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
        }
        Serial.println(gps_string);
        gps_string = "";
      }
  }
}
