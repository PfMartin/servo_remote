import RPi.GPIO as GPIO
from time import sleep

pwmPin = 3
pwmFreq = 50


# Motor has a range between 2% and 12%
# Range is 10%, which equals 180°
# 180° / (18 %/°) + 2% = 12%
# 0° / 18 (18 %/°) + 2% = 2

def setAngle(angle):
  duty = angle / 18 + 2
  GPIO.output(pwmPin, True)
  pwm.ChangeDutyCycle(duty)
  sleep(1)
  GPIO.output(pwmPin, False)
  pwm.ChangeDutyCycle(0)

def main():
  while(True):
    setAngle(25)
    setAngle(90)
    setAngle(160)
    setAngle(90)

if __name__ == "__main__":
  GPIO.setmode(GPIO.BOARD) # Set names to board mode
  GPIO.setup(pwmPin, GPIO.OUT) # Set GPIO to output mode
  pwm = GPIO.PWM(pwmPin, pwmFreq) # Setup GPIO to pwm mode

  pwm.start(0)

  try:
    main()

  except:
    print('\nAn exception has occurred\nExiting the program')

  finally:
    print('Stopping PWM')
    pwm.stop()
    print('Cleaning up GPIOs')
    GPIO.cleanup()
