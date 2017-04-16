# pip install paho-mqtt
import ssl
import json
import base64
import struct
import paho.mqtt.client as mqtt
from pprint import pprint

APP_ID = ''
ACCESS_KEY = ''
PAYLOAD_FORMAT = '<fff'
DEBUG = False


CONNECT_RETURN_CODES = {
    0: 'Connection Accepted',
    1: 'Connection Refused, unacceptable protocol version',
    2: 'Connection Refused, identifier rejected',
    3: 'Connection Refused, Server unavailable',
    4: 'Connection Refused, bad user name or password',
    5: 'Connection Refused, not authorized',
}


def on_connect(client, userdata, flags, rc):
    print("Connected with result code %s: %s" % (rc, CONNECT_RETURN_CODES.get(rc, 'Unknown')))
    if rc == 0:
        client.subscribe('+/devices/+/activations')
        client.subscribe('+/devices/+/up')


def on_message(client, userdata, msg):
    print('\n%s' % msg.topic)
    data = json.loads(msg.payload.decode('utf8'))
    if DEBUG:
        pprint(data)
    print('Message details:')
    print('  Dev ID: %s' % data['dev_id'])
    print('  Dev EUI: %s' % data['hardware_serial'])
    print('  Data rate: %s' % data['metadata']['data_rate'])
    print('  Receiving gateways:')
    for gw in data['metadata']['gateways']:
        print('    - ID: %s' % gw['gtw_id'])
        print('      Coords: %s,%s' % (gw['latitude'], gw['longitude']))
        print('      Alt: %sm' % gw['altitude'])
        print('      RSSI: %s' % gw['rssi'])
        print('      SNR: %s' % gw['snr'])

    if msg.topic.endswith('/up'):
        # Upstream message
        payload = data.get('payload_raw')
        if payload is None:
            print('Upstream msg is missing payload')
            return
        bytestring = base64.b64decode(payload)
        try:
            unpacked = struct.unpack(PAYLOAD_FORMAT, bytestring)
        except struct.error as e:
            print('Invalid payload format: %s' % e)
            return
        msg = 'DS Temp: %.2f°C | SHT Temp: %.2f°C | SHT Humi: %.2f%%RH'
        print(msg % (unpacked[0], unpacked[1], unpacked[2]))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(APP_ID, ACCESS_KEY)
client.tls_set('mqtt-ca.pem', tls_version=ssl.PROTOCOL_TLSv1_2)
client.connect('eu.thethings.network', 8883, 60)

client.loop_forever()
