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

Here's an example event:

    {
        "data":"t1=24997",
        "ttl":"300",
        "published_at":"2016-04-26T20:55:26.915Z",
        "coreid":"4a005b001451343334363036",
        "name":"measurement"
    }
