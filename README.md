# <img src="./imgs/smarty.svg" alt="logo" width="64"/> Smarty

<img src="./imgs/smarty_1.webp" alt="smarty_1" width="300"/><img src="./imgs/smarty_2.webp" alt="smarty_2" width="300"/>

## Overview

This compact, remote-controllable robot car features a lightweight 3D-printed chassis, making it both durable and customizable. Powered by a 9V battery and driven by an ESP32 development board, the car supports wireless control via Wi-Fi. Perfect for hobbyists and educational projects, it combines mobility, smart control, and DIY engineering in one sleek package.

## Mechanism

### 3D-Printed Parts

| Filename          | Thumbnail                                                                     | Required # | Note                                                     |
| ----------------- | ----------------------------------------------------------------------------- | ---------- | -------------------------------------------------------- |
| chassis           | <img src="./imgs/chassis.jpg" alt="chassis" width="400"/>                     | 1          | 8 x [6mm (diameter) x 2mm (thickness)] magnets           |
| motor_cover       | <img src="./imgs/motor_cover.jpg" alt="motor_cover" width="400"/>             | 2          |                                                          |
| pcb_holder        | <img src="./imgs/pcb_holder.jpg" alt="pcb_holder" width="400"/>               | 1          |                                                          |
| top_cover         | <img src="./imgs/top_cover.jpg" alt="top_cover" width="400"/>                 | 1          | 4 x [6mm (diameter) x 2mm (thickness)] magnets           |
| battery_cover     | <img src="./imgs/battery_cover.jpg" alt="battery_cover" width="400"/>         | 1          | 4 x [6mm (diameter) x 2mm (thickness)] magnets           |
| fill_block        | <img src="./imgs/fill_block.jpg" alt="fill_block" width="400"/>               | 0 or 2     | Same numbers as slave wheels                             |
| wheel_master      | <img src="./imgs/wheel_master.jpg" alt="wheel_master" width="360"/>           | 4 or 2     | 4-wheel drive or 2-wheel drive                           |
| wheel_slave       | <img src="./imgs/wheel_slave.jpg" alt="wheel_slave" width="400"/>             | 0 or 2     | 4-wheel drive or 2-wheel drive                           |
| tire              | <img src="./imgs/tire.jpg" alt="tire" width="400"/>                           | 4          | Print with TPU, compatible with Lego 56891 Tire 37 x 18R |
| wheel_master_belt | <img src="./imgs/wheel_master_belt.jpg" alt="wheel_master_belt" width="360"/> | 2          |                                                          |
| wheel_slave_belt  | <img src="./imgs/wheel_slave_belt.jpg" alt="wheel_slave_belt" width="400"/>   | 2          |                                                          |
| belt              | <img src="./imgs/belt.jpg" alt="belt" width="400"/>                           | 2          | Print with TPU                                           |
| connector_front   | <img src="./imgs/connector_front.jpg" alt="connector_front" width="400"/>     | 1          |                                                          |
| connector_trailer | <img src="./imgs/connector_trailer.jpg" alt="connector_trailer" width="400"/> | 1          |                                                          |

## Software

### ESP32

- Install `esp32` in Arduino IDE's Boards Manager.
- Select `ESP32 Dev Module` for the board.
