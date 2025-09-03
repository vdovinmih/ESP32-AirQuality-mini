#include "Adafruit_AGS02MA.h"
#include "Adafruit_AHTX0.h"

constexpr byte TVOC_ADDRESS = 0x1A;
constexpr byte TEMP_ADDRESS = 0x38;

constexpr byte I2C_SDA = 13, I2C_SCL = 14;

Adafruit_AGS02MA ags;
Adafruit_AHTX0 aht;

#include "Zigbee.h"

/* Zigbee endpints configuration */
#define TEMP_SENSOR_ENDPOINT_NUMBER 10
#define TVOC_SENSOR_ENDPOINT_NUMBER 11
uint8_t button = BOOT_PIN;

// Optional Time cluster variables

float tvoc;
float temperature;
float relative_humidity;


ZigbeeTempSensor zbTempSensor = ZigbeeTempSensor(TEMP_SENSOR_ENDPOINT_NUMBER);
ZigbeeAnalog zbTVOC = ZigbeeAnalog(TVOC_SENSOR_ENDPOINT_NUMBER);

/************************ Temp sensor *****************************/
static void temp_sensor_value_update(void *arg) {
  sensors_event_t humidity, temp;
  for (;;) {
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    
    temperature = temp.temperature;
    relative_humidity = humidity.relative_humidity;
    tvoc = float(ags.getTVOC());

    Serial.printf("Updated temperature: %.2f°C, Humidity: %.2f%%, TVOC: %.0f\r\n", temperature, relative_humidity, tvoc);

    zbTempSensor.setTemperature(temperature);
    zbTempSensor.setHumidity(relative_humidity);
    zbTVOC.setAnalogInput(tvoc);

    delay(2000);
  }
}

/********************* Arduino functions **************************/
void setup() {
  Serial.begin(115200);

  delay(2000);

  Serial.print("Starting I2C interface...");
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println(F("done."));

  ags.begin(&Wire, TVOC_ADDRESS);

    ags.printSensorDetails();
  delay(100);

  aht.begin(&Wire, TEMP_ADDRESS);
  aht.getTemperatureSensor()->printSensorDetails();
  aht.getHumiditySensor()->printSensorDetails();

  // Init button switch
  pinMode(button, INPUT_PULLUP);

  zbTempSensor.setManufacturerAndModel("Mike_DIY", "AirSensor_v1");
  zbTempSensor.setMinMaxValue(-40, 85);
  zbTempSensor.setTolerance(0.3);

  zbTVOC.addAnalogInput();
  zbTVOC.setAnalogInputMinMax(0, 99999);
  zbTVOC.setAnalogInputResolution(1);
  zbTVOC.setAnalogInputDescription("TVOC (ppb)");

  // Add humidity cluster to the temperature sensor device with min, max and tolerance values
  zbTempSensor.addHumiditySensor(0, 100, 2);

  //Zigbee.setDebugMode(true);

  // Add endpoint to Zigbee Core
  Zigbee.addEndpoint(&zbTempSensor);
  Zigbee.addEndpoint(&zbTVOC);

  Serial.println("Starting Zigbee...");
  // When all EPs are registered, start Zigbee in End Device mode

#ifdef ZIGBEE_MODE_ZCZR
#undef ZIGBEE_MODE_ED
  if (!Zigbee.begin(ZIGBEE_ROUTER)) {
#endif
#ifdef ZIGBEE_MODE_ED
  if (!Zigbee.begin()) {
#endif

    Serial.println("Zigbee failed to start!");
    Serial.println("Rebooting...");
    ESP.restart();
  } else {
    Serial.println("Zigbee started successfully!");
  }
  Serial.println("Connecting to network");
  while (!Zigbee.connected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Start Temperature sensor reading task
  xTaskCreate(temp_sensor_value_update, "temp_sensor_update", 4096, NULL, 10, NULL);
}

void loop() {
  // Checking button for factory reset
  if (digitalRead(button) == LOW) {  // Push button pressed
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(button) == LOW) {
      delay(50);
      if ((millis() - startTime) > 3000) {
        // If key pressed for more than 3secs, factory reset Zigbee and reboot
        Serial.println("Resetting Zigbee to factory and rebooting in 1s.");
        delay(1000);
        Zigbee.factoryReset();
      }
    }
    zbTempSensor.report();
  }
  delay(100);
}
