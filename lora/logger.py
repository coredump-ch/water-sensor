# pip install paho-mqtt
import json
import base64
import struct
import paho.mqtt.client as mqtt
from pprint import pprint

APP_EUI = ''
ACCESS_KEY = ''
PAYLOAD_FORMAT = '!iff'


def on_connect(client, userdata, flags, rc):
    print("Connected with result code %s" % rc)
    client.subscribe('+/devices/+/activations')
    client.subscribe('+/devices/+/up')


def on_message(client, userdata, msg):
    print('\n%s' % msg.topic)
    data = json.loads(msg.payload.decode('utf8'))
    pprint(data)

    if msg.topic.endswith('/up'):
        # Upstream message
        payload = data.get('payload')
        if payload is None:
            print('Upstream msg is missing payload')
            return
        bytestring = base64.b64decode(payload)
        try:
            unpacked = struct.unpack(PAYLOAD_FORMAT, bytestring)
        except struct.error as e:
            print('Invalid payload format: %s' % e)
            return
        msg = 'Temp: %.3f°C | Voltage: %.2f | Charge: %.2f%%'
        print(msg % (unpacked[0] / 1000, unpacked[1], unpacked[2]))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(APP_EUI, ACCESS_KEY)
client.connect('staging.thethingsnetwork.org', 1883, 60)

client.loop_forever()
