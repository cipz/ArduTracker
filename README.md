# ArduTracker

#### Developers

- Ciprian Voinea
- Federico Carboni
- Mariano Sciacco
- Prof. Palazzi


# Board

### Esp32 Circuit board

![nRF24L01 Module](./essay/img/esp32-pinout.png)

### Firmware

Arduino IDE + Platformio

#### Additional libraries
- [ArduinoJSON v6](https://arduinojson.org/v6/)


### Circuit

![circuit](./essay/img/ardutracker.png)


### SPI devices

![spi](./essay/img/spi.png)


### Connecting nRF24L01 to the ESP32

<p align="center">
  <table align="center">
    <tr>
      <td>
        <table style="text-align:center">
          <tr style="font-weight: bold;">
            <td>nRF24L01</td>
            <td>ESP32</td>
          </tr>
          <tr>
            <td>GND</td>
            <td>GND</td>
          </tr>
          <tr>
            <td>VCC</td>
            <td>3.3V</td>
          </tr>
          <tr>
            <td>CE</td>
            <td>D4</td>
          </tr>
          <tr>
            <td>CSN</td>
            <td>D5</td>
          </tr>
          <tr>
            <td>SCK</td>
            <td>D18</td>
          </tr>
          <tr>
            <td>MOSI</td>
            <td>D23</td>
          </tr>
          <tr>
            <td>MISO</td>
            <td>D19</td>
          </tr>
        </table>
      </td>
      <td>
        <p align="center">
          <img src="./img/nRF24L01.png" height="" align="center">
        </p>
      </td>
    </tr>
  </table>
</p>


### Connecting MicroSD Module to the ESP32

To connect the SD Module I have adapted a simple Arduino example:

<p align="center">
  <table align="center">
    <tr>
      <td>
        <table style="text-align:center">
          <tr style="font-weight: bold;">
            <td>MicroSD Module</td>
            <td>ESP32</td>
          </tr>
          <tr>
            <td>GND</td>
            <td>GND</td>
          </tr>
          <tr>
            <td>VCC</td>
            <td>5V or 3.3V</td>
          </tr>
          <tr>
            <td>CS</td>
            <td>D22</td>
          </tr>
          <tr>
            <td>SCK</td>
            <td>D18</td>
          </tr>
          <tr>
            <td>MOSI</td>
            <td>D23</td>
          </tr>
          <tr>
            <td>MISO</td>
            <td>D19</td>
          </tr>
        </table>
      </td>
      <td align="center">
        <p align="center">
          <img src="./img/microsd-pinout.jpg" height="" alt="nRF24L01 Module" align="center">
        </p>
      </td>
    </tr>
  </table>
</p>

![Microsd-hack](./img/microsd_hack.jpg)


# Server

## Raspberry Pi and MQTT server
The Mosquitto MQTT server can be set on a Raspberry Pi in the network and it uses IBM's Node-Red.
To configure it I have followed [this](https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/) tutorial.

## Docker

To setup the entire server part of the ardutracker project, you need to have installed Docker with docker-compose in a x86_64 capable machine with at least 1GB of ram a single core CPU. 

Move to the `/ArduTrackerServer/` directory and type in a terminal `docker-compose up -d`.
After that, you can stop all the services using `docker-compose down`, always in the same directory.

### Useful links:

#### Tutorials
- [Communication nRF24L01 avec cartes ESP32 et ESP8266 ](http://electroniqueamateur.blogspot.com/2019/12/communication-nrf24l01-avec-cartes.html?m=0)
- [Interfacing Micro SD Card Module with Arduino](https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/)
- [ESP32 MQTT – Publish and Subscribe with Arduino IDE](https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/)

#### Circuit
- [ESP32 Fritzing file](https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357?u=steelgoose)
