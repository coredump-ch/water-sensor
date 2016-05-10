// Firmware for water temperature sensor on the Particle Electron.
//
// (c) 2016 Coredump Rapperswil. Licensed under MIT license.

// Set this to 1 for debug output via serial (9600 baud)
#define DEBUG_MODE 1

// I2C address of the SHT sensor
int shtAddress = 0x40;

// Constants
const int SHT21_TRIG_RH_MEASUREMENT_HM = 0xe5;
const int SHT21_TRIG_T_MEASUREMENT_NHM = 0xf3;

// This will be run once
void setup() {

    // Set I2C frequency to 50 KHz
    Wire.setSpeed(50000);

    // Enable clock stretching (TODO rnestler: maybe remove to save energy?)
    Wire.stretchClock(true);

    // Enable I2C
    Wire.begin();

#if DEBUG_MODE
    Particle.disconnect();
    Serial.begin(9600);
#endif

}

// This will be run in a loop
void loop() {

#if DEBUG_MODE
    Serial.println("mi wile toki!");
#endif

    // Request a temperature measurement from the sensor
    Wire.beginTransmission(shtAddress);
    Wire.write(SHT21_TRIG_T_MEASUREMENT_NHM);
    Wire.endTransmission();

#if DEBUG_MODE
    Serial.println("mi wile sona e pilin seli anu lete!");
#endif

    // Request to read 3 bytes (hi, lo, crc).
    delay(100); // TODO
    if (Wire.requestFrom(shtAddress, 3) == 0) {
#if DEBUG_MODE
        Serial.println("I2C read request timeout");
#else
        Particle.publish("i2c-error", "I2C read request timeout", 60, PRIVATE);
#endif
    }

    if (Wire.available()) {
        // Get battery stats
        FuelGauge fuel;
        float batteryVoltage = fuel.getVCell(); // Actual voltage
        float batteryChargeLevel = fuel.getSoC(); // Percentage, 0-100

#if DEBUG_MODE
        Serial.println("Battery voltage: " + String(batteryVoltage) + ", SoC: " + String(batteryChargeLevel));
#endif

        // Read temperature (16 bit, high then low bits)
        uint8_t temp[2];
        temp[0] = Wire.read(); // hi
        temp[1] = Wire.read(); // lo

        // Read CRC
        uint8_t crc = Wire.read();

        // Validate checksum
        if (!sht2x_crc_valid(temp, 2, crc)) {
            // CRC validation failed
            String data = String::format("data=%0x%0x,crc=%0x", temp[0], temp[1], crc);
#if DEBUG_MODE
            Serial.println("CRC checksum failed.");
            Serial.println(data);
#else
            Particle.publish("crc-error", data, 60, PRIVATE);
#endif
        } else {
            // CRC validation successful. Calculate temperature.
            int32_t val = temp[0] << 8 | temp[1];
            int32_t mcelsius = ((val * 21965) >> 13) - 46850;

            // Notify user
#if DEBUG_MODE
            Serial.print("hi=");
            Serial.print(temp[0], HEX);
            Serial.print(" lo=");
            Serial.print(temp[1], HEX);
            Serial.print("\nval=");
            Serial.print(val, HEX);
            Serial.print("\ntemp=");
            Serial.print(mcelsius);
            Serial.print('\n');
#else
            String data = String::format("t1=%d,v=%.3f,c=%.1f",
                    mcelsius, batteryVoltage, batteryChargeLevel);
            Particle.publish("measurement", data, 300, PRIVATE);
#endif
        }

    }

#if DEBUG_MODE
    delay(5000);
#else
    delay(1000); // Wait for data to be sent. TODO: Make this nicer than a delay.
    System.sleep(SLEEP_MODE_DEEP, 60); // Sleep for 1h
#endif
}

/**
 * Taken directly from Sensirion SHT21 sample code.
 */
const uint16_t POLYNOMIAL = 0x131; // P(x)=x^8+x^5+x^4+1 = 100110001
bool sht2x_crc_valid(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum) {
    uint8_t crc = 0;
    uint8_t byteCtr;
    // Calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr) {
        crc ^= (data[byteCtr]);
        for (uint8_t bit = 8; bit > 0; --bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc == checksum;
}
