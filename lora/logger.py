# pip install paho-mqtt
import json
import paho.mqtt.client as mqtt
from pprint import pprint

APP_EUI = ''
ACCESS_KEY = ''


def on_connect(client, userdata, flags, rc):
    print("Connected with result code %s" % rc)
    client.subscribe('+/devices/+/activations')
    client.subscribe('+/devices/+/up')


def on_message(client, userdata, msg):
    print('%s' % msg.topic)
    data = json.loads(msg.payload.decode('utf8'))
    pprint(data)
    print()


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.username_pw_set(APP_EUI, ACCESS_KEY)
client.connect('staging.thethingsnetwork.org', 1883, 60)

client.loop_forever()
