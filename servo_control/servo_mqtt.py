import RPi.GPIO as GPIO
import paho.mqtt.client as mqtt
from time import sleep

#---Variable section---
# Variables for MQTT
clientName = 'Accelerometer'
brokerIp = '192.168.178.26'
username = 'martin'
password = 'Jahnel01'
topic = 'accel'
#Variables for servo
pwmPin = 3
pwmFreq = 50

def setAngle(angle):
  duty = angle / 18 + 2
  GPIO.output(pwmPin, True)
  pwm.ChangeDutyCycle(duty)
  sleep(0.5)
  GPIO.output(pwmPin, False)
  pwm.ChangeDutyCycle(0)

def onConnect(client, userdata, flags, rc):
  print('Connected with result code', rc)
  client.subscribe(topic)

def onMessage(client, userdata, msg):
  value = float(msg.payload.decode('utf-8'))
  topic = msg.topic
  print(f'Rotation of {value}° has been published to the topic {topic}')
  setAngle(value)

def main():
  # Connection to broker
  client = mqtt.Client(client_id=clientName, clean_session=True, userdata=None, protocol=mqtt.MQTTv311, transport='tcp')
  client.on_connect = onConnect # Assign onConnect to callback on_connect
  client.on_message = onMessage # Assign onMessage to callback on_message

  client.connect(brokerIp)

  client.loop_forever()

if __name__ == '__main__':
  GPIO.setmode(GPIO.BOARD) # Set names to board mode
  GPIO.setup(pwmPin, GPIO.OUT) # Set GPIO to output mode
  pwm = GPIO.PWM(pwmPin, pwmFreq) # Setup GPIO to pwm mode

  pwm.start(0)

  try:
    main()
  except KeyboardInterrupt:
    print('\nKeyboardInterrupt\nExiting the program')
  finally:
    print('Stopping PWM')
    pwm.stop()
    print('Cleaning up GPIOs')
    GPIO.cleanup()
