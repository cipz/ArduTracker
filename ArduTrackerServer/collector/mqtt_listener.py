import paho.mqtt.client as mqtt
import mysql.connector
import json

# Database
db_user = "admin_ardutrack"
db_password = "1q9p7lETVi"
db_database = "admin_ardutrackerdb"
db_host = "ip1.debug.ovh"

# Mqtt server
mqtt_host = "debug.ovh"
mqtt_id = "py_client"
mqtt_topic = "math/wnma/ardutrack"

################# Database #################

# Write to database
def mysql_write(data):
    # Connect
    mydb = mysql.connector.connect(
        host = db_host,
        user = db_user,
        password = db_password,
        database = db_database
    )
    # Insert
    print("[DB] Inserting into", db_database)
    cursor = mydb.cursor()
    sql = "INSERT INTO tracking_log (my_id, friend_id, seen_millis, seen_time) VALUES (%s,%s,%s,%s);"
    val = (
        data["my_id"], 
        data["friend_id"], 
        data["seen_millis"], 
        data["seen_time"])
    cursor.execute(sql, val)
    mydb.commit()
    print("[DB]", cursor.rowcount, "record inserted.")
    mydb.close()

################# MQTT #################
print("[MQTT] Creating new instance")
client = mqtt.Client(mqtt_id)

# Connection
print("[MQTT] Connecting to", mqtt_host)
client.connect(mqtt_host) 

# Callback
def on_message(client, userdata, message):
    msg = message.payload.decode("utf-8")
    data = json.loads(msg)
    mysql_write(data)

client.on_message = on_message

# Subscribe
print("[MQTT] Subscribing to topic", mqtt_topic)
client.subscribe(mqtt_topic)

# Loop
client.loop_forever()