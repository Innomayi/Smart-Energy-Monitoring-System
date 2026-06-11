#include <WiFi.h>
#include <WebServer.h>

// WiFi Hotspot
const char* ssid = "ESP32_EnergyMonitor";
const char* password = "12345678";

// Pins
#define LDR_PIN 34
#define LED_PIN 26

WebServer server(80);

int ldrValue = 0;
String statusText = "";
String statusColor = "";

// Stylish Web Page
String webpage() {

  String page = R"rawliteral(

<!DOCTYPE html>
<html>
<head>

<title>IoT Energy Monitor</title>

<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv='refresh' content='2'>

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Arial;
}

body{
height:100vh;
display:flex;
justify-content:center;
align-items:center;
background:linear-gradient(135deg,#0f2027,#203a43,#2c5364);
overflow:hidden;
}

.container{
width:360px;
padding:30px;
border-radius:25px;
background:rgba(255,255,255,0.08);
backdrop-filter:blur(12px);
box-shadow:0 8px 32px rgba(0,0,0,0.4);
text-align:center;
color:white;
animation:fadeIn 1s ease;
}

h1{
font-size:30px;
margin-bottom:25px;
color:#00ffff;
}

.sensor-box{
padding:25px;
border-radius:20px;
background:rgba(255,255,255,0.07);
margin-bottom:20px;
}

.label{
font-size:18px;
color:#dddddd;
margin-bottom:10px;
}

.value{
font-size:50px;
font-weight:bold;
color:#ffd54f;
}

.status{
padding:15px;
border-radius:15px;
font-size:20px;
font-weight:bold;
background:)rawliteral";

  page += statusColor;

  page += R"rawliteral(;
color:white;
box-shadow:0 0 15px rgba(255,255,255,0.2);
}

.footer{
margin-top:20px;
font-size:14px;
color:#cccccc;
}

.glow1{
position:absolute;
width:300px;
height:300px;
background:#00ffff;
border-radius:50%;
filter:blur(120px);
top:-120px;
left:-120px;
opacity:0.2;
}

.glow2{
position:absolute;
width:250px;
height:250px;
background:#ff00ff;
border-radius:50%;
filter:blur(120px);
bottom:-100px;
right:-100px;
opacity:0.2;
}

@keyframes fadeIn{
from{
opacity:0;
transform:translateY(20px);
}
to{
opacity:1;
transform:translateY(0);
}
}

</style>

</head>

<body>

<div class="glow1"></div>
<div class="glow2"></div>

<div class="container">

<h1>IoT Energy Monitor</h1>

<div class="sensor-box">

<div class="label">
Light Intensity
</div>

<div class="value">
)rawliteral";

  page += String(ldrValue);

  page += R"rawliteral(
</div>

</div>

<div class="status">
)rawliteral";

  page += statusText;

  page += R"rawliteral(
</div>

</div>

</body>
</html>

)rawliteral";

  return page;
}

// Handle Homepage
void handleRoot() {

  ldrValue = analogRead(LDR_PIN);

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // DARK CONDITION
  if (ldrValue < 1000) {
    digitalWrite(LED_PIN, HIGH);
    statusText = "Lights ON - Low Light Detected";
    statusColor = "#00c853";
  }

  // BRIGHT CONDITION
  else {
    digitalWrite(LED_PIN, LOW);
    statusText = "Lights OFF - Sufficient Light";
    statusColor = "#ff9800";
  }


  server.send(200, "text/html", webpage());
}

void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  // Start WiFi Hotspot
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("WiFi Hotspot Started");

  Serial.print("Open Browser: ");
  Serial.println(WiFi.softAPIP());

  // Web Server
  server.on("/", handleRoot);

  server.begin();

  Serial.println("Web Server Started");
}

void loop() {

  server.handleClient();
}