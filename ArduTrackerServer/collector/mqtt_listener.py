import paho.mqtt.client as mqtt
import mysql.connector
import json

# Database
db_user = "admin_ardutrack"
db_password = "1q9p7lETVi"
db_database = "ardutrackerdb"
db_host = "database"
db_port = "3306" # default: 3306

# Mqtt server
mqtt_host = "ardutracker.debug.ovh"
mqtt_id = "py_client"
mqtt_topic = "math/wnma/ardutrack"
mqtt_port = 21883 # default: 1883

################# Database #################

# Write to database
def mysql_write(data):
    # Connect
    mydb = mysql.connector.connect(
        host = db_host,
        user = db_user,
        password = db_password,
        database = db_database,
        port = db_port
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
    print("[DB] ", cursor.rowcount, " record inserted.")
    mydb.close()

################# MQTT #################
print("[MQTT] Creating new instance")
client = mqtt.Client(mqtt_id)

# Connection
print("[MQTT] Connecting to", mqtt_host)
client.connect(mqtt_host, port=mqtt_port) 

# Callback
def on_message(client, userdata, message):
    print("[Parsing]", message)
    msg = message.payload.decode("utf-8")
    try:
        data = json.loads(msg)
        mysql_write(data)
    except json.JSONDecodeError:
        print("[DecodeError]", data)

client.on_message = on_message

# Subscribe
print("[MQTT] Subscribing to topic", mqtt_topic)
client.subscribe(mqtt_topic)

# Loop
client.loop_forever()