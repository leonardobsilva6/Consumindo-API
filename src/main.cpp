
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "POCO X6 Pro 5G";
const char* password = "leonardo";

const int trigPin = 5;
const int echoPin = 18;
const int inPin = 4;

#define SOUND_SPEED 0.034

long duration;
float distanceCm;

bool lampadaLigada = false;

WebServer server(80);

void pagina() {
  String html = "<html><head><meta http-equiv='refresh' content='1'>";
  html += "<meta charset='UTF-8'></head><body>";
  html += "<h1>Monitoramento ESP32</h1>";
  

  if (distanceCm < 0) {
    html += "<h2>Distancia atual: Sem leitura</h2>";
  } else {
    html += "<h2>Distancia atual: " + String(distanceCm) + " cm</h2>";
  }

  html += "<h2>Lampada: ";
  html += lampadaLigada ? "Ligada" : "Desligada";
  html += "</h2></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(inPin, OUTPUT);

  digitalWrite(inPin, HIGH);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.println(WiFi.localIP());

  server.on("/", pagina);
  server.begin();
}

void loop() {
  server.handleClient();

  static unsigned long tempo = 0;

  if (millis() - tempo >= 500) {
    tempo = millis();

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 30000);

    if (duration == 0) {
      distanceCm = -1;
    } else {
      distanceCm = duration * SOUND_SPEED / 2;
    }

    if (distanceCm >= 0 && distanceCm <= 20) {
      digitalWrite(inPin, LOW);
      lampadaLigada = true;
    } else {
      digitalWrite(inPin, HIGH);
      lampadaLigada = false;
    }
  }
}
