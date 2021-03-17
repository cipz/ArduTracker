import paho.mqtt.client as mqtt
import mysql.connector
import json
import sys

# Database
db_user = "admin_ardutrack"
db_password = "1q9p7lETVi"
db_database = "ardutrackerdb"
db_host = "database"
db_port = "3306" # default: 3306

# Mqtt server
mqtt_host = "mosquitto"
mqtt_id = "py_client"
mqtt_topic = "math/wnma/ardutrack/config/"
mqtt_port = 1883 # default: 1883

################# Database #################

if(len(sys.argv) != 2):
    # print("Not enough argouments")
    print("error")
    exit()

mac = sys.argv[1]

# Connect
mydb = mysql.connector.connect(
    host = db_host,
    user = db_user,
    password = db_password,
    database = db_database,
    port = db_port
)

# Fetch params
# print("[DB] Reading configurations", db_database)
cursor = mydb.cursor()
sql = "SELECT configuration FROM tracking_board WHERE id_mac='"+ mac +"' LIMIT 1;"
# print('[SQL]',sql)

cursor.execute(sql)
result = cursor.fetchall()

for row in result:
    ################# MQTT #################
    # print("[MQTT] Creating new instance")
    client = mqtt.Client(mqtt_id)

    # Connection
    # print("[MQTT] Connecting to ", mqtt_host)
    client.connect(mqtt_host, port=mqtt_port) 

    # Publish
    client.publish(mqtt_topic + mac, row[0])
    # print("Config sent.")
    print("done")

mydb.close()
exit()