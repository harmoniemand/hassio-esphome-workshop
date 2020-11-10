#include "esphome.h"
#include "bsec.h"

class CE_BSEC : public PollingComponent, public Sensor
{
public:
  // constructor
  Bsec iaqSensor;

  Sensor *RawTemperatureSensor = new Sensor();
  Sensor *PressureSensor = new Sensor();
  Sensor *RawHumiditySensor = new Sensor();
  Sensor *GasResistanceSensor = new Sensor();
  Sensor *IaqSensor = new Sensor();
  Sensor *IaqAccuracySensor = new Sensor();
  Sensor *TemperatureSensor = new Sensor();
  Sensor *HumiditySensor = new Sensor();
  Sensor *StaticIaqSensor = new Sensor();
  Sensor *Co2EquivalentSensor = new Sensor();
  Sensor *BreathVocEquivalentSensor = new Sensor();


  // Sensor *TSensor = new Sensor();
  // Sensor *PSensor = new Sensor();
  // Sensor *HSensor = new Sensor();
  // Sensor *AQSensor = new Sensor();
  Sensor *ErrorSensor = new Sensor();

  CE_BSEC() : PollingComponent(10000) {}

  void setup() override
  {
    // This will be called by App.setup()
    // iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);

    iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
    // output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
    // Serial.println(output);
    checkIaqSensorStatus();

    bsec_virtual_sensor_t sensorList[10] = {
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    };

    iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
    checkIaqSensorStatus();
  }

  void update() override
  {

    if (iaqSensor.run())
    { // If new data is available

       RawTemperatureSensor->publish_state(iaqSensor.rawTemperature);
       PressureSensor->publish_state(iaqSensor.pressure);
       RawHumiditySensor->publish_state(iaqSensor.rawHumidity);
       GasResistanceSensor->publish_state(iaqSensor.gasResistance);
       IaqSensor->publish_state(iaqSensor.iaq);
       IaqAccuracySensor->publish_state(iaqSensor.iaqAccuracy);
       TemperatureSensor->publish_state(iaqSensor.temperature);
       HumiditySensor->publish_state(iaqSensor.humidity);
       StaticIaqSensor->publish_state(iaqSensor.staticIaq);
       Co2EquivalentSensor->publish_state(iaqSensor.co2Equivalent);
       BreathVocEquivalentSensor->publish_state(iaqSensor.breathVocEquivalent);

    }
    else
    {
      checkIaqSensorStatus();
    }

    if (iaqSensor.status != BSEC_OK)
    {
      ErrorSensor->publish_state(1);
    }
    else
    {
      ErrorSensor->publish_state(0);
    }
  }

  void checkIaqSensorStatus(void)
  {
    if (iaqSensor.status != BSEC_OK)
    {
      if (iaqSensor.status < BSEC_OK)
      {
        // output = "BSEC error code : " + String(iaqSensor.status);
        // Serial.println(output);
        ErrorSensor->publish_state(1);
      }
      else
      {
        // output = "BSEC warning code : " + String(iaqSensor.status);
        // Serial.println(output);
        ErrorSensor->publish_state(2);
      }
    }
    else if (iaqSensor.bme680Status != BME680_OK)
    {
      if (iaqSensor.bme680Status < BME680_OK)
      {
        // output = "BME680 error code : " + String(iaqSensor.bme680Status);
        // Serial.println(output);
        ErrorSensor->publish_state(1);
      }
      else
      {
        // output = "BME680 warning code : " + String(iaqSensor.bme680Status);
        // Serial.println(output);
        ErrorSensor->publish_state(2);
      }
    }
    else
    {
      ErrorSensor->publish_state(0);
    }
  }
};