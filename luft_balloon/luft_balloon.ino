/*
  6-8-10
  Aaron Weiss
  SparkFun Electronics
  
  Example GPS Parser based off of arduiniana.org TinyGPS examples.
  
  Parses NMEA sentences from an EM406 running at 4800bps into readable 
  values for latitude, longitude, elevation, date, time, course, and 
  speed. 
  
  For the SparkFun GPS Shield. Make sure the switch is set to DLINE.
  
  Once you get your longitude and latitude you can paste your 
  coordinates from the terminal window into Google Maps. Here is the 
  link for SparkFun's location.  
  http://maps.google.com/maps?q=40.06477,+-105.20997
  
  Uses the NewSoftSerial library for serial communication with your GPS, 
  so connect your GPS TX and RX pin to any digital pin on the Arduino, 
  just be sure to define which pins you are using on the Arduino to 
  communicate with the GPS module. 
  
  REVISIONS:
  1-17-11 
    changed values to RXPIN = 2 and TXPIN = to correspond with
    hardware v14+. Hardware v13 used RXPIN = 3 and TXPIN = 2.
  
*/ 

// In order for this sketch to work, you will need to download 
// TinyGPS library from arduiniana.org and put them 
// into the hardware->libraries folder in your ardiuno directory.
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#include <SD.h>

// Define which pins you will use on the Arduino to communicate with your 
// GPS. In this case, the GPS module's TX pin will connect to the 
// Arduino's RXPIN which is pin 3.
#define RXPIN 2
#define TXPIN 3
//Set this value equal to the baud rate of your GPS
#define GPSBAUD 4800
String gps_string;
// Create an instance of the TinyGPS object
TinyGPS gps;
// Initialize the NewSoftSerial library to the pins you defined above
SoftwareSerial uart_gps(RXPIN, TXPIN);
File myFile;
// This is where you declare prototypes for the functions that will be 
// using the TinyGPS library.
void getgps(TinyGPS &gps);

// In the setup function, you need to initialize two serial ports; the 
// standard hardware serial port (Serial()) to communicate with your 
// terminal program an another serial port (NewSoftSerial()) for your 
// GPS.
void setup()
{
  // This is the serial rate for your terminal program. It must be this 
  // fast because we need to print everything before a new sentence 
  // comes in. If you slow it down, the messages might not be valid and 
  // you will likely get checksum errors.
  Serial.begin(115200);
  //Sets baud rate of your GPS
  uart_gps.begin(GPSBAUD);
  
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  
  Serial.println("");
  Serial.println("GPS Shield QuickStart Example Sketch v12");
  Serial.println("       ...waiting for lock...           ");
  Serial.println("");
}

// This is the main loop of the code. All it does is check for data on 
// the RX pin of the ardiuno, makes sure the data is valid NMEA sentences, 
// then jumps to the getgps() function.
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
