#include <WiFi.h>
#include <time.h>
#include <DHT.h>

//=========================
// CONFIGURACIÓN WIFI
//=========================

const char* ssid = "Xiaomi 12";
const char* password = "miguelangel1";

//=========================
// DHT11
//=========================

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

//=========================
// PINES
//=========================

const int sensorIR = 35;

const int ledTemperatura = 22;
const int ledHumedad = 26;
const int ledObjeto = 13;
const int ledHora = 18;

//=========================
// UMBRALES
//=========================

float temperaturaLimite = 25.0;
float humedadLimite = 70.0;

//=========================
// ALARMA
//=========================

int horaAlarma = 8;
int minutoAlarma = 48;

//=========================

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(sensorIR, INPUT);

  pinMode(ledTemperatura, OUTPUT);
  pinMode(ledHumedad, OUTPUT);
  pinMode(ledObjeto, OUTPUT);
  pinMode(ledHora, OUTPUT);

  digitalWrite(ledTemperatura, LOW);
  digitalWrite(ledHumedad, LOW);
  digitalWrite(ledObjeto, LOW);
  digitalWrite(ledHora, LOW);

  Serial.println("Conectando al WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado.");

  // Hora Colombia (GMT-5)

  configTime(-5 * 3600, 0, "pool.ntp.org");

  Serial.println("Hora sincronizada.");
}

void loop() {

  //-------------------------
  // DHT11
  //-------------------------

  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {

    Serial.println("Error leyendo el DHT11");
    delay(2000);
    return;

  }

  Serial.println("----------------------------");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  //-------------------------
  // LED TEMPERATURA
  //-------------------------

  if (temperatura >= temperaturaLimite)
    digitalWrite(ledTemperatura, HIGH);
  else
    digitalWrite(ledTemperatura, LOW);

  //-------------------------
  // LED HUMEDAD
  //-------------------------

  if (humedad >= humedadLimite)
    digitalWrite(ledHumedad, HIGH);
  else
    digitalWrite(ledHumedad, LOW);

  //-------------------------
  // SENSOR INFRARROJO
  //-------------------------

  int objeto = digitalRead(sensorIR);

  if (objeto == LOW) {

    digitalWrite(ledObjeto, HIGH);
    Serial.println("Objeto detectado.");

  }
  else {

    digitalWrite(ledObjeto, LOW);

  }

  //-------------------------
  // RELOJ
  //-------------------------

  struct tm timeinfo;

  if (getLocalTime(&timeinfo)) {

    Serial.print("Hora actual: ");

    if (timeinfo.tm_hour < 10) Serial.print("0");
    Serial.print(timeinfo.tm_hour);

    Serial.print(":");

    if (timeinfo.tm_min < 10) Serial.print("0");
    Serial.println(timeinfo.tm_min);

    if (timeinfo.tm_hour == horaAlarma &&
        timeinfo.tm_min == minutoAlarma) {

      digitalWrite(ledHora, HIGH);

    } else {

      digitalWrite(ledHora, LOW);

    }

  }

  delay(1000);

}