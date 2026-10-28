#include <OneWire.h>
#include <DallasTemperature.h>

// Pin connections
const int TEMP_PIN = 2;
const int BUZZER_PIN = 8;
const int LED_PIN = 7;

// Temperature thresholds in Celsius
const float FIRE_TEMP = 50.0;
const float RESET_TEMP = 48.0;

// DS18B20 setup
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

bool alarmActive = false;

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  sensors.begin();

  Serial.println("Temperature-Based Fire Alarm");
  Serial.println("System Started");
}

void loop() {
  sensors.requestTemperatures();

  float temperature = sensors.getTempCByIndex(0);

  // Sensor error: activate alarm as a fault warning
  if (temperature == DEVICE_DISCONNECTED_C ||
      temperature < -55.0 ||
      temperature > 125.0) {

    alarmActive = true;
    Serial.println("ERROR: Sensor disconnected or faulty!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    // Activate alarm at 50 C
    if (temperature >= FIRE_TEMP) {
      alarmActive = true;
    }

    // Reset alarm only when temperature falls below 48 C
    else if (temperature < RESET_TEMP) {
      alarmActive = false;
    }
  }

  // Alarm outputs
  if (alarmActive) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("ALARM ON!");
  }
  else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Serial.println("Normal temperature");
  }

  Serial.println("--------------------");
  delay(1000);
}

