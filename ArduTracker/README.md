# Project Notes

- SD card adapter requires 5v instaed of 3.3volt, otherwise it does not work properly ([Issue reference](https://rntlab.com/question/card-mount-failed/))

### Debugging MQTT

- Install this software on your PC and connect to mqtt://ardutracker.debug.ovh:21883 ([Mqtt Explorer](http://mqtt-explorer.com/))



#### Upload ports in MacOS

Ignore this, just for developers:

- usb 2: `pio run -t upload --upload-port /dev/cu.usbserial-0001`