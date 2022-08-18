/*
    Description: Use GoPLUS Module for four-channel servo test and three-channel ADC test.
*/
#include <M5Stack.h>
#include <Wire.h>
#include "GoPlus2.h"
#include <math.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "MONSUPERARDUINO";
const char* password = "PASSWORD1234";

GoPlus2 goPlus;

#define X_LOCAL 40
#define Y_LOCAL 30

#define X_OFFSET 160
#define Y_OFFSET 23
#define MAXSPEED 164

int _hub1, hub1 = 0;

size_t received = 0;

int speed1 = 0;
int speed2 = 0;

int flag = 0;
int num = 0;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Mario Kart EP</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .controls{
    display:flex;
    justify-content: center;
    flex-direction:column;
    align-items: center;
  }
  .central_line{
    display:flex;
    justify-content: space-between;
    width: 270px;
  }

  .button {
    width: 80px;
    min-height: 80px;
    text-align: center;
    background-color: #0f8b8d;
    border:none;
    border-radius: 15px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }

   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>Mario Kart EP</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>Mario Kart EP</h1>
  </div>
  <div class="content">
    <div class="card">
      <p class="state">state: <span id="speed">%SPEED%</span></p>
      <div class="controls">
        <p><button id="buttonForward" class="button"><img src ="images/fleche-vers-le-haut.png" alt="icone de fleche haut"></button></p>
        <div class="central_line">
          <span id="btg"><button id="buttonLeft" class="button"><img src ="images/fleche-gauche.png" alt="icone de fleche gauche"></button></span>
          <span id="btc"><button id="buttonNothing" class="button"></button></span>
          <span id="btd"><button id="buttonRight" class="button"><img src ="images/fleche-droite.png" alt="icone de fleche droite"></button></span>
        </div>
        <p><button id="buttonBack" class="button"><img src ="images/fleche-vers-le-bas.png" alt="icone de fleche bas"></button></p>
      </div>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    document.getElementById('speed').innerHTML = event.data;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('buttonForward').addEventListener('click', forward);
    document.getElementById('buttonLeft').addEventListener('click', left);
    document.getElementById('buttonRight').addEventListener('click', right);
    document.getElementById('buttonBack').addEventListener('click', back);
    document.getElementById('buttonNothing').addEventListener('click',nothing);
  }
  function nothing(){
    websocket.send("nothing");
  }
  function forward(){
    websocket.send("forward");
  }
  function left(){
    websocket.send("left");
  }
  function right(){
    websocket.send("right");
  }
  function back(){
    websocket.send("back");
  }
</script>
</body>
</html>
)rawliteral";

void notifyClients() {
  ws.textAll(String(speed1)+"-"+String(speed2));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "forward") == 0) {
      speed1=MAXSPEED;
      speed2=MAXSPEED;
      notifyClients();
    } if (strcmp((char*)data, "back") == 0) {
      speed1=-MAXSPEED;
      speed2=-MAXSPEED;
      notifyClients();
    } if (strcmp((char*)data, "left") == 0) {
      speed1=0;
      speed2=MAXSPEED;
      notifyClients();
    } if (strcmp((char*)data, "right") == 0) {
      speed1=MAXSPEED;
      speed2=0;
      notifyClients();
    } if (strcmp((char*)data, "nothing") == 0) {
      speed1=0;
      speed2=0;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "SPEED"){
    return (String(speed1)+"-"+String(speed2));
  }
}





void doTask()
{
  if(num == 4){
    num = 0;
  }else{
    num++;
  }
  M5.Lcd.println(num);
}

void forward(int speedOne,int speedTwo){
  goPlus.Motor_write_speed(MOTOR_NUM0, speedOne); // valeur inversé moteur num0 : moteur droit
  goPlus.Motor_write_speed(MOTOR_NUM1, -speedTwo*0.95); // moteur gauche
}


void setup() {

   M5.begin();
   Serial.begin(115200);
   goPlus.begin();
   delay(100);

   // Connect to Wi-Fi
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
     delay(1000);
     Serial.println("Connecting to WiFi..");
   }
      // Print ESP Local IP Address
    Serial.println(WiFi.localIP());
  
    initWebSocket();
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();


//   goPlus.hub1_set_io(HUB1_R_O_ADDR, 1); //set digital_output to digital_input
//   goPlus.hub2_set_io(HUB2_R_O_ADDR, 1); //set digital_output to digital_input
//   goPlus.hub3_set_io(HUB3_R_O_ADDR, 1); //set digital_output to digital_input
}

void loop() {
   ws.cleanupClients();
   forward(speed1,speed2);

  
//    int val1 = goPlus.hub1_d_o_read_value(HUB1_R_O_ADDR);  //read digtial_input
//    int val2 = goPlus.hub2_d_o_read_value(HUB2_R_O_ADDR);  //read digtial_input
//    int val3 = goPlus.hub3_d_o_read_value(HUB3_R_O_ADDR);  //read digtial_input
//    Serial.println(val1);
//    Serial.println(val2);
//    Serial.println(val3);

}
