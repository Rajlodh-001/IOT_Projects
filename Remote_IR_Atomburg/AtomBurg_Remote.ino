// ---------------------------------------------------------------------------------------

// NOTE: in principle this code is universal and can be used on Arduino AVR as well. However, AVR is only supported with version 1.3 of the webSocketsServer. Also, the Websocket
// library will require quite a bit of memory, so wont load on Arduino UNO for instance. The ESP32 and ESP8266 are cheap and powerful, so use of this platform is recommended.
// Modified code version 0.0.98
 ---------------------------------------------------------------------------------------

#include <ESP8266WiFi.h>      // needed to connect to WiFi
#include <ESP8266WebServer.h> // needed to create a simple webserver (make sure tools -> board is set to ESP32, otherwise you will get a "WebServer.h: No such file or directory" error)
#include <WebSocketsServer.h> // needed for instant communication between client and server through Websockets
#include <ArduinoJson.h>      // needed for JSON encapsulation (send multiple variables with one string)
// #include <LittleFS.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLedPin = 12; // GPIO pin that IR LED is connected to

IRsend IrSender(kIrLedPin);
// SSID and password of Wifi connection:
const char *ssid = ""; // SSid of WiFi 
const char *password = ""; // Password of WiFi

// The String below "webpage" contains the complete HTML code that is sent to the client whenever someone connects to the webserver
String webpage = "<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>Remote</title> <!-- <link rel='stylesheet' href='style.css'> --> <style> .body { /* border-width: 20px; border-color: black; */ border-color: black; stroke-width: 20px; background-color: aquamarine; border-radius: 10px; } #container { font-family: Arial, sans-serif; text-align: center; font-size: 1.5rem; font-weight: bold; border-color: black; stroke-width: 20px; } #wifi { text-align: center; padding: 10px 15px; } .rect_btn { font-size: 3rem; margin: 18px; padding: 15px 30px; border-radius: 10px; border: none; background-color: hsl(0, 78%, 56%); color: white; font-weight: bold; cursor: pointer; } .rect_btn:hover { background-color: hsl(0, 78%, 66%); } .rect_btn:active { background-color: hsl(0, 78%, 76%); } .rect_btn_sm { font-size: 1.2rem; width: 110px; margin: 18px 18px; padding: 15px 30px; border-radius: 12px; border: none; background-color: hsl(210, 100%, 50%); color: white; font-weight: bold; cursor: pointer; } .rect_btn_sm:hover { background-color: hsl(210, 100%, 60%); } .rect_btn_sm:active { background-color: hsl(210, 100%, 70%); } #tmargin { margin-top: 15px; } .squre_btn { font-size: 3rem; margin: 18px; padding: 15px 30px; border-radius: 10px; border: none; background-color: hsl(210, 100%, 50%); color: white; font-weight: bold; cursor: pointer; } .squre_btn:hover { background-color: hsl(210, 100%, 60%); } .squre_btn:active { background-color: hsl(210, 100%, 70%); } input { font-size: 2rem; width: 150px; text-align: center; } #diceResult { margin: 20px; } #diceImages img { width: 150px; margin: 5px; } </style></head><body> <div id='container'> <h1>Remote</h1> <!-- <label id='wifi'>WiFi : <span id='ssid'></span></label> --> <!-- <button onclick='rollDice()'>Roll Dice</button> --> <div> <button id='btn' class='rect_btn' onclick='button(0);'>Power</button> </div> <div id='button_space'> <button id='btn' onclick='button(1);' class='squre_btn'>1</button> <button onclick='button(2);' class='squre_btn'>2</button> </div> <div> <button id='btn' onclick='button(3);' class='squre_btn'>3</button> <button id='btn' onclick='button(4);' class='squre_btn'>4</button> </div> <div> <button id='btn' onclick='button(5);' class='squre_btn'>5</button> </div> <div id='tmargin'> <button id='btn' onclick='button(10);' class='rect_btn_sm'>Boost</button> <button id='btn' onclick='button(20);' class='rect_btn_sm'>LED</button> </div> <div><button id='btn' onclick='button(30);' class='rect_btn_sm'>Timer</button> <button id='btn' onclick='button(40);' class='rect_btn_sm'>sleep</button> </div> </div> <script> function init() { Socket = new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage = function (event) { processCommand(event); }; } let buttonclk = ' '; function button(event) { switch (event) { case 0: buttonclk = 'CF8976'; break; case 1: buttonclk = 'CFD12E'; break; case 2: buttonclk = 'CF09F6'; break; case 3: buttonclk = 'CF51AE'; break; case 4: buttonclk = 'CFC936'; break; case 5: buttonclk = 'CF11EE'; break; case 10: buttonclk = 'CFF10E'; break; case 20: buttonclk = 'CFE916'; break; case 30: buttonclk = 'CF6996'; break; case 40: buttonclk = 'CF718E'; break; default: console.log('end'); break; } console.log(buttonclk); tempd(); } function tempd() { var msg = { brand: 'AtomBurg', type: 'Remote', iRcode: buttonclk }; Socket.send(JSON.stringify(msg)); } /*function processCommand(event) { var obj = JSON.parse(event.data); document.getElementById('ssid').innerHTML = obj.ssid; console.log(obj.rand1); console.log(obj.rand2); } */ function processCommand(event) { var obj = JSON.parse(event.data); document.getElementById('rand1').innerHTML = obj.rand1; document.getElementById('rand2').innerHTML = obj.rand2; console.log(obj.rand1); console.log(obj.rand2); } window.onload = function (event) { init(); } /* function initaldata() { const ssid = document.getElementById('ssid'); ssid.textContent = buttonclk; } initaldata(); */ </script></body></html>";

int interval = 1000;              // send data to the client every 1000ms -> 1s
unsigned long previousMillis = 0; // we use the "millis()" command for time reference and this will output an unsigned long

int resendtry = 5;
const int TxPin = 5;
char *ircode = "";
bool sendssid = true;
// Initialization of webserver and websocket
ESP8266WebServer server(80);                       // the server uses port 80 (standard port for websites
WebSocketsServer webSocket = WebSocketsServer(81); // the websocket uses port 81 (standard port for websockets

void setup()
{
  Serial.begin(9600); // init serial port for debugging
  IrSender.begin();

  WiFi.begin(ssid, password);                                                   // start WiFi interface
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid)); // print SSID to the serial interface for debugging

  while (WiFi.status() != WL_CONNECTED)
  { // wait until WiFi is connected
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP()); // show IP address that the ESP32 has received from router

  server.on("/", []() {                     // define here wat the webserver needs to do
    server.send(200, "text/html", webpage); //    -> it needs to send out the HTML string "webpage" to the client
  });
  server.begin(); // start server

  webSocket.begin();                 // start websocket
  webSocket.onEvent(webSocketEvent); // define a callback function -> what does the ESP32 need to do when an event from the websocket is received? -> run function "webSocketEvent()"
}

void loop()
{
  server.handleClient(); // Needed for the webserver to handle all clients
  webSocket.loop();      // Update function for the webSockets

  unsigned long now = millis(); // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
  if (resendtry > 0)
  { // check if "interval" ms has passed since last time the clients were updated

    String jsonString = "";                   // create a JSON string for sending data to the client
    StaticJsonDocument<200> doc;              // create a JSON container
    JsonObject object = doc.to<JsonObject>(); // create a JSON Object
    object["rand1"] = random(100);            // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
    object["rand2"] = random(100);
    serializeJson(doc, jsonString);     // convert JSON object to string
    Serial.println(jsonString);         // print JSON string to console for debug purposes (you can comment this out)
    webSocket.broadcastTXT(jsonString); // send JSON string to clients

    resendtry--;
    delay(500);

    // ---------------
    sendssid = false;
  }
}

void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.println("Client " + String(num) + " disconnected");
    break;
  case WStype_CONNECTED:
    Serial.println("Client " + String(num) + " connected");
    // optionally you can add code here what to do when connected
    break;
  case WStype_TEXT: // if a client has sent data, then type == WStype_TEXT
    // try to decipher the JSON string received
    StaticJsonDocument<200> doc; // create a JSON container
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    else
    {
      // JSON string was received correctly, so information can be retrieved:
      const char *g_brand = doc["brand"];
      const char *g_type = doc["type"];

      const char *g_iRcode = doc["iRcode"];
      Serial.println("Received Remote info from User : " + String(num));
      Serial.println("Brand: " + String(g_brand));
      Serial.println("Type: " + String(g_type));
      Serial.println("Color: " + String(g_iRcode));

      String hexString = String(g_iRcode);

      // Convert hex string to integer
      uint32_t hexValue = strtoul(hexString.c_str(), NULL, 16);

      // Print the hex value
      Serial.print("Hex Value: ");
      Serial.println(hexValue, HEX);

      // Send the IR code
      IrSender.sendNEC(hexValue, 32);
    }
    Serial.println("");
    break;
  }
}
