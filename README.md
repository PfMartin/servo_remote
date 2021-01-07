# Servo Remote

## Setup mosquitto on your Raspberry Pi
1. Download mosquitto

```plaintext
sudo apt update && sudo apt upgrade -y

sudo apt install mosquitto -y
sudo apt install mosquitto-clients -y
```

1. Setup config file
```plaintext
sudo nano /etc/mosquitto/mosquitto.conf
```

1. Delete the line
```plaintext
include_dir /etc/mosquitto/conf.d
```

1. Add the following lines to the bottom of the file
```plaintext
allow_anonymous false
password_file /etc/mosquitto/pwfile
listener 1883
```

1. Create password file
```plaintext
sudo mosquitto_passwd -c /etc/mosquitto/pwfile username
```

1. Reboot the system
```plaintext
sudo reboot
```

1. Test the broker
    1. Subscribe to a topic
    ```plaintext
    mosquitto_sub -d -u username -P password -t test
    ```

    1. Publish to the same topic
    ```plaintext
    mosquitto_pub -d -u username -P password -t test -m "Did you get that?"
    ```
