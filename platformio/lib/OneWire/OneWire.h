#include <cstdint>
#include "mbed.h"

class OneWire {
public:
    OneWire(PinName pin):
        _pin(pin)
    {}

    enum Command: uint8_t {
        SEARCH_ROM = 0xF0,
    };

    void reset();

    void write_byte(uint8_t byte);
    void write_bit(bool bit);
    uint8_t read_byte();
    bool read_bit();

    void search_rom(uint8_t addr[8]);
private:
    DigitalInOut _pin;
};
