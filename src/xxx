#include <Arduino.h>
#include "IRremote.hpp"
#include "Audio.h"
//#include <WebServer_WT32_ETH01.h>
#include "WiFi.h"
#include "WiFiMulti.h"

WiFiServer server(80);
const char* ssid = "ggg";
const char* password = "0987654321";
//String ssid =     "ggg";
//String password = "0987654321";

// Digital I/O used
#define I2S_DOUT      12  //12 na WTH32
#define I2S_BCLK      14
#define I2S_LRC       15
//#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial
//#define _ETHERNET_WEBSERVER_LOGLEVEL_       0  // Debug Level from 0 to 4
uint16_t last_command = 0;
uint8_t volume = 14;
const int irReceiverPin = 4;

Audio audio;
WiFiMulti wifiMulti;

const long timeoutTime = 2000;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
String header;
String output26State = "off";
String output27State = "off";

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}


void setup() {
    Serial.begin(115200);
    delay(10);
    initWiFi();
    server.begin();
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(volume); // 0...21
    IrReceiver.begin(irReceiverPin , DISABLE_LED_FEEDBACK);
//    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
//    audio.connecttohost("http://somafm.com/wma128/missioncontrol.asx"); //  asx
//    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac"); //  128k aac
//    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3
      audio.connecttohost("https://stream.radio357.pl");
}

void loop()
{
    WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
             // Display the HTML web page
             client.println("<!DOCTYPE html><html>");
             client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
             client.println("<link rel=\"icon\" href=\"data:,\">");
             // CSS to style the on/off buttons 
             // Feel free to change the background-color and font-size attributes to fit your preferences
             client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
             client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
             client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
             client.println(".button2 {background-color: #555555;}</style></head>");
             
             // Web Page Heading
             client.println("<body><h1>ESP32 Web Server</h1>");
             
             // Display current state, and ON/OFF buttons for GPIO 26  
             client.println("<p>GPIO 26 - State " + output26State + "</p>");
             // If the output26State is off, it displays the ON button       
             if (output26State=="off") {
               client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
             } else {
               client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
             } 
                
             // Display current state, and ON/OFF buttons for GPIO 27  
             client.println("<p>GPIO 27 - State " + output27State + "</p>");
             // If the output27State is off, it displays the ON button       
             if (output27State=="off") {
               client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
             } else {
               client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
             }
             client.println("</body></html>");
             
             // The HTTP response ends with another blank line
             client.println();
             // Break out of the while loop
             break;
           } else { // if you got a newline, then clear currentLine
             currentLine = "";
           }
         } else if (c != '\r') {  // if you got anything else but a carriage return character,
           currentLine += c;      // add it to the end of the currentLine
         }
       }
     }
     // Clear the header variable
     header = "";
     // Close the connection
     client.stop();
   }
 

   
    if (WiFi.status() == WL_CONNECTED){
        Serial.print(WiFi.SSID());
        Serial.print(" ");
        Serial.println(WiFi.RSSI());
        delay(2000);
        }
   
      
    audio.loop();
    if(IrReceiver.decode()){ // put streamURL in serial monitor
        uint16_t command = IrReceiver.decodedIRData.command;
        if (command != last_command){
        if (command == 0xFF16) {
            //Serial.print("łącze kanał 1");
            //audio.stopSong();
            //audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3");
            //last_command=command;
            volume--;
            audio.setVolume(volume);
            Serial.println(audio.getVolume());
            delay(2000);
          }
        else if (command==0xFF19){
            //Serial.print("łącze kanał 2");
            //audio.stopSong();
            //audio.connecttohost("http://somafm.com/wma128/missioncontrol.asx");
            //last_command=command;
            volume++;
            audio.setVolume(volume);
            Serial.println(audio.getVolume());
            delay(2000);
        }
        else if (command==0xFF0D){
            Serial.print("zatrzymije kanał");
            audio.stopSong();
           // audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
           last_command=command;
        }
        
    }
    delay(100);
    IrReceiver.resume();
    log_i("free heap=%i", ESP.getFreeHeap()); 
  } 
}


// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
