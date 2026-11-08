#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>
#define WIFI_SSID     ""
#define WIFI_PASS     ""

#define AIO_USERNAME  ""
#define AIO_KEY       ""

#define DHTPIN 5

DHT dht(DHTPIN, DHT11);
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, AIO_USERNAME, AIO_KEY); // Adafruit broker

// Thats where the measurements will be stored (published)
Adafruit_MQTT_Publish temperature_publisher = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidity_publisher = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");

void connect_mqtt();

void setup() {
  Serial.begin(115200);
  Serial.println("dht test");
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected successfully!");
}

void loop() {
  connect_mqtt();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error reading measuremnts from sensor");
    delay(2000);
    return;
  }

  Serial.println(humidity);
  Serial.println(temperature);

  //publish humidity
  if (!humidity_publisher.publish(humidity)) {
    Serial.println("Error with temperature publisher");
  }
  // publish temperature
  if (!temperature_publisher.publish(temperature)) {
    Serial.println("Error with temperature publisher");
  }

  delay(10000);
}


void connect_mqtt() {
  int conn;
  if (mqtt.connected()) // check if broker is already connected
    return;

  conn = mqtt.connect();
  while(conn != 0) {
    Serial.println(mqtt.connectErrorString(conn));
    mqtt.disconnect();
    conn = mqtt.connect();
    delay(5000);
  }
}