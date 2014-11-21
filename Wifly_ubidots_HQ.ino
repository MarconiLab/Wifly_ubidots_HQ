//Sending data from AI0 to Ubidots with Wifly RN-XV connected to Stalker board
//Library for wifly https://github.com/harlequin-tech/WiFlyHQ
// Arduino       WiFly
//  6    <---->    TX
//  7    <---->    RX
//Author: Rodrigo Carbajales 7/8/14

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFlyHQ.h"

/* Change these to match your WiFi network */
const char mySSID[] = "MarconiLab";
const char myPassword[] = "marconi-lab";
#define AUTH      WIFLY_AUTH_WPA2_PSK     // or WIFLY_AUTH_WPA1, WIFLY_AUTH_WEP, WIFLY_AUTH_OPEN

 //Dots configuration
#define TOKEN          "YRWsXoUOptheqaDFN2T58o65hyHe4m"  //Replace with your TOKEN
#define VARIABLEID1     "538f3bc676254249aec757d9"       //Replace with your variable ID

// Pins' connection
// Arduino       WiFly
//  6    <---->    TX
//  7    <---->    RX
SoftwareSerial wifiSerial(6,7);
WiFly wifly;


char server[] = "things.ubidots.com";
 
int resetTimer = 0; 

char buf[80];

void setup() {
   Serial.begin(115200);
    Serial.println(F("Starting"));
    Serial.print(F("Free memory: "));
    Serial.println(wifly.getFreeMemory(),DEC);

    wifiSerial.begin(9600);
    if (!wifly.begin(&wifiSerial, &Serial)) {
        Serial.println(F("Failed to start wifly"));
	wifly.terminal();
    }

    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	Serial.println(F("Joining network"));
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();
	wifly.save();

	if (wifly.join()) {
	    Serial.println(F("Joined wifi network"));
	} else {
	    Serial.println(F("Failed to join wifi network"));
	    wifly.terminal();
	}
    } else {
        Serial.println(F("Already joined network"));
    }

    wifly.setBroadcastInterval(0);	// Turn off UPD broadcast

    //wifly.terminal();

    Serial.print(F("MAC: "));
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    Serial.print(F("IP: "));
    Serial.println(wifly.getIP(buf, sizeof(buf)));

    wifly.setDeviceID("Wifly-WebServer");

    if (wifly.isConnected()) {
        Serial.println(F("Old connection active. Closing"));
	wifly.close();
    }

    wifly.setProtocol(WIFLY_PROTOCOL_TCP);
    if (wifly.getPort() != 80) {
        wifly.setPort(80);
	/* local port does not take effect until the WiFly has rebooted (2.32) */
	wifly.save();
	Serial.println(F("Set port to 80, rebooting to make it work"));
	wifly.reboot();
	delay(3000);
    }
    Serial.println(F("Ready"));
}

void loop() {
  while (wifly.available()) {
    Serial.write(wifly.read());
 }
  while (Serial.available()) {
    wifly.write(Serial.read());
  } 
     Serial.println("Trying to send data:" + String(analogRead(0)));
     writeUbidots(String(analogRead(0)), VARIABLEID1 );     //Send data in String format to the Ubidots function
     if(resetTimer == 10) {                                          //Restarts if the connection fails 10 times in a row
        asm volatile ("  jmp 0");
     }
  delay(5000); 
}

void writeUbidots(String data, String VARID) {
  String dataString = "{\"value\":"+ data + "}";                  //Prepares the data in JSON format

  if (wifly.open(server, 80)) {                               //If connection is successful, then send this HTTP Request:
    Serial.println("Connected to " + String(VARID));
    wifly.print("POST /api/v1.6/variables/");                    //Specify URL, including the VARIABLE ID
    wifly.print(VARID);
    wifly.println("/values HTTP/1.1");
    wifly.println("Host: things.ubidots.com");
    wifly.print("X-Auth-Token: ");                               //Specify Authentication Token in headers
    wifly.println(TOKEN);
    wifly.println("Content-Type: application/json");
    wifly.print("Content-Length: ");
    wifly.println(dataString.length());
//    wifly.println("Connection: close");
    wifly.println();                                             //End of HTTP headers

    wifly.println(dataString);                                   //This is the actual value to send

    wifly.flush();
    wifly.close();
//    return 1;
  }
  else {                                                          // If the connection wasn't possible, then:
    resetTimer += 1;
    Serial.println("Connection failed");
    Serial.println("Device will restart after 10 failed attempts, so far:"+String(resetTimer)+" attempts.");
    Serial.println("Killing sockets and disconnecting...");
    wifly.flush();
    wifly.close();
//    return 0;
  }
}
