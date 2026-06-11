#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SmartCity_ESP32";
const char* password = "12345678";

WebServer server(80);

// PINS
int ldrPin = 35;

int led1 = 2;
int led2 = 4;
int led3 = 5;

String envStatus = "";
String lightStatus = "";
String statusColor = "";

String webpage = "";

void setup() {

  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("SMART STREET LIGHT SYSTEM");

  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {

    int ldrValue = analogRead(ldrPin);

    // DARK CONDITION
    if (ldrValue < 2000) {

      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);

      envStatus = "DARK";
      lightStatus = "ON";
      statusColor = "#22c55e";

    }

    // BRIGHT CONDITION
    else {

      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);

      envStatus = "BRIGHT";
      lightStatus = "OFF";
      statusColor = "#facc15";

    }

    // GRAPH WIDTH
    int graphWidth = map(ldrValue, 0, 4095, 10, 100);

    webpage = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Smart Street Light</title>

<style>

body{
background:#020617;
font-family:Arial;
margin:0;
padding:0;
text-align:center;
color:white;
}

.container{
padding:20px;
}

.card{
background:#111827;
max-width:430px;
margin:auto;
padding:25px;
border-radius:20px;
box-shadow:0px 0px 20px rgba(0,0,0,0.5);
}

h1{
color:#38bdf8;
font-size:28px;
}

.value{
font-size:42px;
font-weight:bold;
margin:15px;
}

.status{
font-size:24px;
font-weight:bold;
margin:15px;
}

.graph-container{
width:100%;
height:35px;
background:#1e293b;
border-radius:20px;
overflow:hidden;
margin-top:20px;
}

.graph-bar{
height:100%;
transition:1s;
border-radius:20px;
}

.label{
margin-top:12px;
font-size:18px;
color:#94a3b8;
}

.footer{
margin-top:25px;
font-size:14px;
color:#64748b;
}

</style>

</head>

<body>

<div class="container">

<div class="card">

<h1>SMART STREET LIGHT</h1>

<div class="value">
LDR Value: )rawliteral";

    webpage += ldrValue;

    webpage += R"rawliteral(
</div>

<div class="status" style="color:)rawliteral";

    webpage += statusColor;

    webpage += R"rawliteral(;">
Environment : )rawliteral";

    webpage += envStatus;

    webpage += R"rawliteral(
</div>

<div class="status">
Street Lights : )rawliteral";

    webpage += lightStatus;

    webpage += R"rawliteral(
</div>

<div class="label">
LIGHT INTENSITY GRAPH
</div>

<div class="graph-container">

<div class="graph-bar" style="
width:)rawliteral";

    webpage += graphWidth;

    webpage += R"rawliteral(%;
background:)rawliteral";

    webpage += statusColor;

    webpage += R"rawliteral(;">
</div>

</div>

<div class="footer">
ESP32 • AUTOMATIC STREET LIGHT SYSTEM
</div>

</div>

</div>

<script>

setTimeout(function(){
location.reload();
},2000);

</script>

</body>
</html>

)rawliteral";

    server.send(200, "text/html", webpage);

  });

  server.begin();

}

void loop() {

  server.handleClient();

}
