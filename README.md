# ArduTracker

WHAT IT IS
WHY IT IS

THE ESP32 BOARD

<p align="center">
  <img src="./essay/img/esp32-pinout.png" height="" alt="nRF24L01 Module" align="center">
</p>

## Software

Arduino IDE

## Circuit

<p align="center">
  <img src="./essay/img/ardutracker.png" height="" align="center">
</p>

### SPI devices

<p align="center">
  <img src="./essay/img/spi.png" height="250px" alt="" align="center">
</p>

### Connecting the nRF24L01 with ESP32

<p align="center">
  <img src="./img/nRF24L01.png" height="250px" align="center">
</p>

nRF24L01 | ESP32
--- | ---
GND | GND
VCC | 3.3V
CE | D4
CSN | D5
SCK | D18
MOSI | D23
MISO | D19

### Connecting the MicroSD Module with ESP32

To connect the SD Module I have adapted a simple Arduino example

<p align="center">
  <img src="./img/microsd.jpg" height="250px" alt="nRF24L01 Module" align="center">
</p>

MicroSD Module | ESP32
--- | ---
GND | GND
VCC | 5V or 3.3V
CS | D22
SCK | D18
MOSI | D23
MISO | D19

<p align="center">
  <img src="./img/microsd_hack.jpg" height="250px" alt="" align="center">
</p>

Explain why the sd card adapter does not play well with others

## Useful links:

### Tutorials
- [Communication nRF24L01 avec cartes ESP32 et ESP8266 ](http://electroniqueamateur.blogspot.com/2019/12/communication-nrf24l01-avec-cartes.html?m=0)
- [https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/](https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/)

### Circuit
- [ESP32 Fritzing file](https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357?u=steelgoose)
