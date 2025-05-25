#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

// Define the GPIO pin and their connections
#define DHTPIN 23
#define DHTTYPE DHT22
#define relayModule 12
#define I2C_SDA 21
#define I2C_SCL 22

// Create an object of the DHTesp class
DHT_Unified dht(DHTPIN, DHTTYPE);

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(115200);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHT22 Sensor Test"));
  pinMode(relayModule, OUTPUT);
  Serial.println("LED Control Example");
  // initialize the lcd to use user defined I2C pins
  lcd.init(I2C_SDA, I2C_SCL);
  lcd.backlight();
  lcd.print("Initializing...");
  // Print temperature sensor details.
  sensor_t sensor;
}

void relayOutput(bool condition)
{
  if (condition)
  {
    digitalWrite(relayModule, HIGH);
    Serial.println("Condition met, LED is ON");
  }
  else
  {
    digitalWrite(relayModule, LOW);
    Serial.println("Condition not met, LED is OFF");
  }
}

void loop()
{
  //Measurement
  delay(2000);
  // Get temperature and humidity event and print its value.
  sensors_event_t event;

  dht.temperature().getEvent(&event);
  float temperature = event.temperature;

  dht.humidity().getEvent(&event);
  float relative_humidity = event.relative_humidity;

  if (isnan(temperature) || isnan(relative_humidity))
  {
    Serial.println(F("Error reading data!"));
  }
  else
  {
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));
    Serial.print(F("Humidity: "));
    Serial.print(relative_humidity);
    Serial.println(F("%"));
  };

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("°C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(relative_humidity);
  lcd.print("%");

  bool tempCondition = temperature >= 24;
  bool humidityCondition = relative_humidity <= 50;

  bool condition = tempCondition || humidityCondition;

  relayOutput(condition);
};