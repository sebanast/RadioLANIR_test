#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <LittleFS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  #include <LittleFS.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "bbb";
const char* password = "0987654321";

const char* PARAM_STRING1 = "inputString1";
const char* PARAM_STRING2 = "inputString2";
const char* PARAM_STRING3 = "inputString3";
const char* PARAM_STRING4 = "inputString4";
const char* PARAM_STRING5 = "inputString5";
const char* PARAM_STRING6 = "inputString6";

// HTML web page to handle 3 input fields (inputString, inputInt, inputFloat)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP LittleFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>
  <form action="/get" target="hidden-form">
    inputString1 (current value %inputString1%): <input type="text" name="inputString1">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputString2 (current value %inputString2%): <input type="text" name="inputString2">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputString3 (current value %inputString3%): <input type="number " name="inputString3">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
    <form action="/get" target="hidden-form">
    inputString4 (current value %inputString4%): <input type="text" name="inputString4">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
    <form action="/get" target="hidden-form">
    inputString5 (current value %inputString5%): <input type="text" name="inputString5">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
    <form action="/get" target="hidden-form">
    inputString6 (current value %inputString6%): <input type="text" name="inputString6">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form>
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

// Replaces placeholder with stored values
String processor(const String& var){
  //Serial.println(var);
  if(var == "inputString"){
    return readFile(LittleFS, "/inputString.txt");
  }
  else if(var == "inputInt"){
    return readFile(LittleFS, "/inputInt.txt");
  }
  else if(var == "inputFloat"){
    return readFile(LittleFS, "/inputFloat.txt");
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  // Initialize LittleFS
  #ifdef ESP32
    if(!LittleFS.begin(true)){
      Serial.println("An Error has occurred while mounting LittleFS");
      return;
    }
  #else
    if(!LittleFS.begin()){
      Serial.println("An Error has occurred while mounting LittleFS");
      return;
    }
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(PARAM_STRING1)) {
      inputMessage = request->getParam(PARAM_STRING1)->value();
      writeFile(LittleFS, "/inputString1.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_STRING2)) {
      inputMessage = request->getParam(PARAM_STRING2)->value();
      writeFile(LittleFS, "/inputString2.txt", inputMessage.c_str());
    }
    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    else if (request->hasParam(PARAM_STRING3)) {
      inputMessage = request->getParam(PARAM_STRING3)->value();
      writeFile(LittleFS, "/inputString3.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_STRING4)) {
        inputMessage = request->getParam(PARAM_STRING4)->value();
        writeFile(LittleFS, "/inputString4.txt", inputMessage.c_str());
      }
      else if (request->hasParam(PARAM_STRING5)) {
        inputMessage = request->getParam(PARAM_STRING5)->value();
        writeFile(LittleFS, "/inputString5.txt", inputMessage.c_str());
      }
      else if (request->hasParam(PARAM_STRING6)) {
        inputMessage = request->getParam(PARAM_STRING6)->value();
        writeFile(LittleFS, "/inputString6.txt", inputMessage.c_str());
      }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // To access your stored values on inputString, inputInt, inputFloat
  String yourInputString = readFile(LittleFS, "/inputString.txt");
  Serial.print("*** Your inputString: ");
  Serial.println(yourInputString);
  
  int yourInputInt = readFile(LittleFS, "/inputInt.txt").toInt();
  Serial.print("*** Your inputInt: ");
  Serial.println(yourInputInt);
  
  float yourInputFloat = readFile(LittleFS, "/inputFloat.txt").toFloat();
  Serial.print("*** Your inputFloat: ");
  Serial.println(yourInputFloat);
  delay(5000);
}
