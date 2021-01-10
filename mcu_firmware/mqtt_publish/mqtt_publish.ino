#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

const int mpuAddress = 0x68;

//Variables for the WiFi connection
const char* ssid = "AsGuadeInternet";
const char* wifiPassword = "Katzenlaube";

//Variables for the connection to the MQTT Broker
const char* mqttServer = "192.168.178.78";
const char* mqttTopic = "movement";
const char* mqttUser = "pi";
const char* mqttPassword = "Jahnel01";
const char* clientId = "ESP32|MPU6050";
const int listenerPort = 1883;

const int ledPin = 12;

int16_t gyroZRaw, gyroZ;
int16_t gyroZOffset = 135;

//Initialize the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqttServer, listenerPort, wifiClient);

// Function to setup any pin as output pin
void pinSetupOutput(int pin) {
  Serial.print("Declaring pin ");
  Serial.print(pin);
  Serial.println(" as output pin");
  Serial.println("==============================");
  pinMode(pin, OUTPUT);
}

bool publisher(int16_t value) {
  if (client.publish(mqttTopic, String(value).c_str())) {
    Serial.println(String(value).c_str());
  }
  
  return true;
}

void setup() {
  Serial.begin(9600);

  pinSetupOutput(ledPin);

  //Initialize I2C
  Serial.println("Initializing I2C bus");
  Wire.begin();
  Wire.beginTransmission(mpuAddress);
  Wire.write(0x6B);                   // PWR_MGMT_1 register
  Wire.write(0);                      // set to zero for a wake up
  Wire.endTransmission(true);
  Serial.println("==============================");

  //Initialize WiFi connection
  Serial.println("Initializing WiFi connection");
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.begin() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Debugging Output for Wifi Connection
  Serial.println("WiFi connection established");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("==============================");

  //Initialize connection to the MQTT Broker
  Serial.println("Initializing connection to MQTT Broker");
  if (client.connect(clientId, mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT Broker");
  }
  else {
    Serial.println("Connection to MQTT Broker failed");
  }
  Serial.println("==============================");
}

void loop() { 
  Wire.beginTransmission(mpuAddress);
  Wire.write(0x47);                 // Starting at register 0x47 (GYRO_ZOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(mpuAddress, 2, true);

  gyroZRaw = Wire.read() << 8 | Wire.read();   //Reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  gyroZ = gyroZRaw - gyroZOffset;

  //Publish the value and indicate a success by lighting up the led
  if (publisher(gyroZ)) {
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);

  
  
//  publisher("LED on");
//  digitalWrite(ledPin, HIGH);
//  delay(1000);
//  publisher("LED off");
//  digitalWrite(ledPin, LOW);
//  delay(1000);
}
