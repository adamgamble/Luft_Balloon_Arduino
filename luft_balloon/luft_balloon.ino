#include <SoftwareSerial.h>
#include <SD.h>

// GPS Setup
#define GPSBAUD 4800

String gps_string;

SoftwareSerial uart_gps(0, 1);

//SD Card stuffs
File myFile;

//GPRS Setup
SoftwareSerial GPRS(7, 8);
String url;

void setup()
{
  Serial.begin(115200);
  //Sets baud rate of ygprsour GPS
  uart_gps.begin(GPSBAUD);
  GPRS.begin(19200);
  
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
        post_data();
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

void post_data() {
  Serial.println("post_data called");
  GPRS.println("AT+CSQ");
  delay(100);
  ShowSerialData();
  
  GPRS.println("AT+CGATT?");
  delay(100);
  ShowSerialData();
  Serial.println("1st");
 
  GPRS.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR, the connection type is using gprs
  delay(1000);
  ShowSerialData();
  Serial.println("2nd");
 
  GPRS.println("AT+SAPBR=3,1,\"APN\",\"wap.cingular\"");//setting the APN, the second need you fill in your local apn server
  delay(4000);
  ShowSerialData();
  Serial.println("3rd");
 
  GPRS.println("AT+SAPBR=1,1");//setting the SAPBR, for detail you can refer to the AT command mamual
  delay(2000);
  Serial.println("4th");
 
 ShowSerialData();
  GPRS.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000); 
  Serial.println("5th");
  ShowSerialData();
  url = "AT+HTTPPARA=\"URL\",\"luft-balloon.herokuapp.com\\";
  //url.concat(gps_string);
  url.concat("\"");
  Serial.println("URL:");
  Serial.println(url);
  
  GPRS.println(url);// setting the httppara, the second parameter is the website you want to access
  delay(1000);
  ShowSerialData();
 
  GPRS.println("AT+HTTPACTION=0");//submit the request 
  delay(10000);//the delay is very important, the delay time is base on the return from the website, if the return datas are very large, the time required longer.
  //while(!gprs.available());
 ShowSerialData();
  GPRS.println("AT+HTTPREAD");// read the data from the website you access
  delay(300);
 ShowSerialData();
  GPRS.println("");
  delay(100);
  ShowSerialData();
}

void ShowSerialData()
{
  Serial.println("ShowSerialData() called");
  while(GPRS.available()!=0) {
    Serial.println("ShowSerialData()1 called");
    Serial.write(GPRS.read());
  }
}
