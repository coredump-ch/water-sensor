# Water Temperature Sensors

## Firmware

The firmware is located in the `electron` directory.

To build:

    $ cd electron
    $ particle login
    $ rm electron_firmware_*.bin
    $ make
    $ particle flash --usb electron_firmware_*.bin

(Support for local toolchain is planned.)

- If `DEBUG_MODE` is set to 1, the data will be logged to serial every 5 seconds.
- If `DEBUG_MODE` is set to 0, the data will be logged to the particle cloud as an event every hour.

## Event data format

The events are logged as events to the particle cloud with the event name `measurement`.

The data contains the measurements as comma separated `key=value` pairs. The
temperature values are returned as decimal milli-degrees celsius.

Keys:

- `t1`: Main temperature sensor
- `v`: Battery voltage
- `c`: Battery capacity (percent)

Here's an example event:

    {
        "data":"t1=24997,v=3.945,c=85.3",
        "ttl":"300",
        "published_at":"2016-04-26T20:55:26.915Z",
        "coreid":"4a005b001451343334363036",
        "name":"measurement"
    }

## Message format (LoRaWAN)

LoRaWAN uses a binary message format. Payload bytes correspond to the following
C struct (bytes in network byte order):

```c
struct {
    uint32_t millicelsius; // Integer like 23420
    float batteryVoltage; // Float like 3.945454
    float batteryCharge; // Charge percentage like 85.38253
} payload;
```

In Python, messages can be parsed using the `!iff` format specifier:

```python
import struct
struct.unpack(̈́'!iff', payload)
```

TO DISCUSS:

- Do we want a binary format, or stick with ascii strings?
- If we want a binary format, how do we keep it upgradeable?
