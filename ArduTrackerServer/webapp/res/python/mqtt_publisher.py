import paho.mqtt.client as mqtt
from time import time
from datetime import datetime
import json
import random

# Mqtt server
mqtt_host = "ardutracker.debug.ovh"
mqtt_id = "123"
mqtt_topic = "math/wnma/ardutrack"
mqtt_port = 21883 # default: 1883

################# MQTT #################
print("[MQTT] Creating new instance")
client = mqtt.Client(mqtt_id)

# Connection
print("[MQTT] Connecting to ", mqtt_host)
client.connect(mqtt_host, port=mqtt_port) 

# Time
print("[MQTT] Publishing message to topic ", mqtt_topic)
millis = int(round(time() * 1000))
timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

# Generate ids
id_list = ["THOR", "IRON MAN", "CAP", "HULK", "SPIDEY"]
my_id = random.choice(id_list)
friend_id = random.choice(id_list)
while(friend_id == my_id):
    friend_id = random.choice(id_list)

# Publish (debug)
msg = {
    "my_id" : my_id,
    "friend_id" : friend_id,
    "seen_millis" : millis,
    "seen_time" : timestamp }
client.publish(mqtt_topic, json.dumps(msg))
print("Done")