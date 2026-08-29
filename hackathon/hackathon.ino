
#define BLYNK_TEMPLATE_ID "TMPL35CwQFZR_"
#define BLYNK_TEMPLATE_NAME "Hackathon"
#define BLYNK_AUTH_TOKEN "iJbRKU1uAoFXH9fbj4wLkykwQqkYdqLV"




#define CHAT_ID "6281753473"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define PIR_PIN 4
// ---------------- Wi-Fi ----------------

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Noname";
char pass[] = "error404";

float distanceCm;
// ---------------- Pins ----------------

#define DHT_PIN 27
#define MQ2_PIN 34
#define RELAY_PIN 16


// ---------------- DHT ----------------

#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// ---------------- Blynk Timer ----------------

BlynkTimer timer;


// ---------------- Variables ----------------

int gasValue = 0;
int manualRelay = 0;
bool relayState = false;


// Gas threshold
#define GAS_THRESHOLD 200


// ==================================================
// MANUAL BLYNK SWITCH
// V4 = Manual Exhaust Control
// ==================================================

BLYNK_WRITE(V4)
{
  manualRelay = param.asInt();

  Serial.print("Manual Exhaust: ");

  if (manualRelay == 1)
    Serial.println("ON");
  else
    Serial.println("OFF");

  updateRelay();
}


// ==================================================
// UPDATE RELAY
// ==================================================

void updateRelay()
{
  bool highGas = (gasValue > GAS_THRESHOLD);

  // Relay ON if either condition is true
  if (highGas || manualRelay == 1)
  {
    relayState = true;
    digitalWrite(RELAY_PIN, LOW);   // Relay ON
  }
  else
  {
    relayState = false;
    digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
  }
}


// ==================================================
// SEND SENSOR DATA
// ==================================================

void sendSensorData()
{
  // ---------------- DHT11 ----------------

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();


  // ---------------- MQ-2 ----------------

  gasValue = analogRead(MQ2_PIN);


  // ---------------- Temperature ----------------

  if (!isnan(temp))
  {
    Blynk.virtualWrite(V0, temp);
  }


  // ---------------- Humidity ----------------

  if (!isnan(humidity))
  {
    Blynk.virtualWrite(V1, humidity);
  }


  // ---------------- MQ-2 ----------------

  Blynk.virtualWrite(V3, gasValue);


  // ---------------- Relay ----------------

  updateRelay();


  // ==================================================
  // STATUS
  // ==================================================

  if (gasValue > GAS_THRESHOLD)
  {
    Blynk.virtualWrite(V2, "Exhaust ON - High Gas");

    Serial.println("STATUS: HIGH GAS -> EXHAUST ON");
  }

  else if (manualRelay == 1)
  {
    Blynk.virtualWrite(V2, "Exhaust ON - Manual");

    Serial.println("STATUS: MANUAL -> EXHAUST ON");
  }

  else
  {
    Blynk.virtualWrite(V2, "Normal");

    Serial.println("STATUS: NORMAL -> EXHAUST OFF");
  }


  // ---------------- Serial Monitor ----------------

  Serial.print("Temperature: ");
  Serial.print(temp);

  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);

  Serial.print(" % | MQ2: ");
  Serial.print(gasValue);

  Serial.print(" | Manual: ");
  Serial.print(manualRelay);

  Serial.print(" | Relay: ");

  if (relayState)
    Serial.println("ON");
  else
    Serial.println("OFF");

  Serial.println("--------------------------------");
}


// ==================================================
// SETUP
// ==================================================

void setup()
{
  Serial.begin(9600);
 pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);   // Trig
  pinMode(18, INPUT);   // Echo

  // DHT
  dht.begin();

client.setInsecure();
  // MQ-2 ADC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  pinMode(MQ2_PIN, INPUT);
pinMode(PIR_PIN, INPUT);

  // Relay
  pinMode(RELAY_PIN, OUTPUT);

  // Relay OFF initially
  digitalWrite(RELAY_PIN, HIGH);


  // Blynk
  Blynk.begin(auth, ssid, pass);


  // Send data every 2 seconds
  timer.setInterval(2000L, sendSensorData);


  Serial.println("--------------------------------");
  Serial.println("SYSTEM READY");
  Serial.println("MQ-2 + DHT11 + EXHAUST RELAY");
  Serial.println("--------------------------------");
}

bot.sendMessage(CHAT_ID, "✅ Smart Safety System Online", "");
// ==================================================
// LOOP
// ==================================================

void loop()
{
  Blynk.run();
  timer.run();
   digitalWrite(5, LOW);
  delayMicroseconds(2);
  digitalWrite(5, HIGH);
  delayMicroseconds(10);
  digitalWrite(5, LOW);
 
  distanceCm = pulseIn(18, HIGH) * 0.034 / 2;
 
  Serial.print("Distance (cm): ");
   Serial.println(distanceCm);
  
  if(distanceCm < 20){
    digitalWrite(2, HIGH);
   
     Blynk.virtualWrite(V6, "Detected");
  }
  else{
    digitalWrite(2, LOW);
     Blynk.virtualWrite(V6, "Normal");
  }

  // Send Distance value to Blynk Virtual Pin V0
  Blynk.virtualWrite(V0, distanceCm);
   Serial.println(distanceCm);
  

  // Send Distance value to Blynk Virtual Pin V0
  Blynk.virtualWrite(V5, distanceCm);

 delay(1000);

}