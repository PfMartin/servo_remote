import paho.mqtt.client as mqtt

# Variable section
clientName = 'Accelerometer'
brokerIp = '192.168.178.26'
username = 'martin'
password = 'Jahnel01'
topic = 'accel'

def on_connect(client, userdata, flags, rc):
  print('Connected with result code', rc)
  client.subscribe(topic)

def on_message(client, userdata, msg):
  print(msg.topic, msg.payload)

# Connection to broker
client = mqtt.Client(client_id=clientName, clean_session=True, userdata=None, protocol=mqtt.MQTTv311, transport='tcp')
client.on_connect = on_connect
client.on_message = on_message

client.connect(brokerIp)

client.loop_forever()
# Mqtt settings for receiving messages
