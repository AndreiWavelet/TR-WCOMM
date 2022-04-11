# TR-WCOMM
This is a electronics and firmware project for a file transmission device based on STM32L051 MCU and NRF24L01+ Transceiver. It's goal is to be small, low power, easy to use, easy to manufacture and to be able to send files over a outdoors distance of at least 100m. 


## What is looks like and how it works

The device is a small PCB with three LEDs in green, yellow and blue, a push button, a SD Card socket and a U.FL antenna connector. There is space for mounting a battery connector ou  directly soldering battery leads to the PCB.

![](./Doc/board-render.png)

Once there is power, the user must connect the antenna and the SD Card press once the push button. All three leds will flash thrice, indicating that the device is on. Then, the user can press the button briefly (<1s) to activate the file transmission. Another identical device can be used to receive the file. If the user presses the button for more than 3s, it will power off, flashing all leds 4 times.

By default, it will send the file "data.dat" at the root of the SD Card, but if user implements the file "wcomm.cfg" at the root of the SD Card, the name of the transmitted file, along with some other configuration, can be changed.

### wcomm.cfg file example

```
Example wcomm.cfg
file = myfile.bin
dest = 12345
addr = 23568
ch = 10
```

### wcomm.cfg attributes

|Attribute  |Meaning                                            |
|:---------:|:-------------------------------------------------:|
|file       |Name of the file to be sent.                       |
|dest       |Destination address, up to 5 characters.           |
|addr       |Device address for reception, up to 5 characters.  |
|ch         |Channel to transmit and receive, 0-127             |

