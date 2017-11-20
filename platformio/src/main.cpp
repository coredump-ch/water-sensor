#include "mbed.h"
#include <functional>
#include <algorithm>
#include "DS18B20.h"
#include "RN2483.h"
#include "secrets.h"

// SHT configuration
const uint8_t SHT3X_I2C_ADDR = 0x45<<1;

const bool USE_ADR = false;

// Measurement interval
const float INTERVAL = 30.0;

float calculate_temp(char msb, char lsb) {
    return -45.0f + 175.0f * (msb<<8 | lsb) / 65535.0f;
}

float calculate_humi(char msb, char lsb) {
    return 100.0f * (msb<<8 | lsb) / 65535.0f;
}

int send_command(I2C& i2c, uint8_t address, uint16_t command) {
    char cmd[2] = {char(command>>8), char(command & 0xFF)};
    return i2c.write(address, cmd, sizeof(cmd));
}

static const PinName LED_RED = P1_22;
static const PinName LED_YELLOW = P0_17;
static const PinName LED_GREEN = P1_16;

static const PinName DS18B20_IO = P1_31;

static const PinName RN2483_TX = P0_19;
static const PinName RN2483_RX = P0_18;

int main() {
    //printf("Start the super awesome water temperature sensor reader\n");

    // Initialize LEDs
    DigitalOut led_red(LED_RED);
    DigitalOut led_yellow(LED_YELLOW);
    DigitalOut led_green(LED_GREEN);

    //printf("LEDs initialized\n");

    // Initialize DS18B20 sensor
    OneWire one_wire(DS18B20_IO);
    DS18B20 ds18b20(one_wire);
    
    //printf("DS18B20 initialized\n");

    // Initialize SHT sensor
    //I2C i2c_0(p28, p27);
    //I2C i2c_1(p9, p10);

    // Initialize the RN2483 module
    RN2483 lora(RN2483_TX, RN2483_RX);

    // Set up I²C sensor
    //i2c_1.frequency(20000);

    //printf("RN2483 initialized\n");

    led_red = 0;
    led_yellow = 0;
    led_green = 0;

    //if (std::all_of(std::begin(DEV_EUI), std::end(DEV_EUI), [](uint8_t v) -> bool { return v == 0; })) {
    
    //printf("getHWEUI\n");
    
    if(false)
    {
        uint8_t buffer[17] = {};
        uint8_t bytes = lora.getHWEUI(buffer, 17);
        if (bytes) {
            led_green = 1;
            //printf("\nHWEUI: %s\n", buffer);
        } else {
            led_red = 1;
        }
        wait(5.0);
        return 0;
    }

    //lora.init();

    // Join the network
    bool joined = false;
    while (!joined) {
        led_yellow = 1;
        //printf("Joining TTN via OTAA...\n");
        joined = lora.initOTAA(DEV_EUI, APP_EUI, APP_KEY, USE_ADR);
        if (joined) {
            led_green = 1;
            led_red = 0;
            //printf("Joined TTN successfully!\n");
        } else {
            led_red = 1;
            //printf("Joining TTN failed\n");
        }
        led_yellow = 0;

        wait(5.0);
    }

    // Main loop
    while(1) {
        //printf("------\nStart measurement...\n");

        led_green = 1;
        wait(0.2);

        int error;

        // Start measurement with clock stretching and high repeatability
        //error = send_command(i2c_1, SHT3X_I2C_ADDR, 0x2C06);
        //if (error) {
        //    printf("i2c.write failed: %i\n", error);
        //}

        // Start conversion
        ds18b20.start_measurement();
        wait(0.5);

        //char data[6] = {};
        //error = i2c_1.read(SHT3X_I2C_ADDR, data, 6);
        //if (error) {
        //    printf("i2c_1.read failed: %i\n", error);
        //}

        //for(int i=0; i<6; ++i) {
        //    printf("%02x", data[i]);
        //}
        //float sht_temp = calculate_temp(data[0], data[1]);
        //printf(" -> Temp = %.2f", sht_temp);

        //float sht_humi = calculate_humi(data[3], data[4]);
        //printf(" Humi = %.2f\n", sht_humi);

        bool timeout = ds18b20.wait_for_completion();
        if (timeout) {
            //printf("Conversion timed out");
        }

        float ds_temp = ds18b20.read_temperature();
        //printf("1-Wire Temp %.2f\n", ds_temp);

        led_green = 0;
        wait(0.2);

        // Measurement done, send it to TTN
        led_yellow = 1;
        uint8_t payload[12] = {};
        memcpy(payload, &ds_temp, 4);
        //memcpy(payload + 4, &sht_temp, 4);
        //memcpy(payload + 8, &sht_humi, 4);
        lora.send(1, payload, 12);
        led_yellow = 0;

        wait(INTERVAL);
    }
}
