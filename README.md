# <img src="./imgs/smarty.svg" alt="logo" width="64"/> Smarty

<img src="./imgs/smarty_1.jpg" alt="smarty_photo1" width="300"/><img src="./imgs/smarty_4.jpg" alt="smarty_photo2" width="300"/>

## Objective

The objective of this work is to build a cute little robot for my daughter. It could be a toy for her to play with, but my main intention is to let her have an early access to engineering knowledge and inspire her for future learning.

## Overview

This compact, remote-controllable robot car features a lightweight 3D-printed chassis, making it both durable and customizable. Powered by a 9V battery and driven by an ESP32 development board, the car supports wireless control via Wi-Fi. Perfect for hobbyists and educational projects, it combines mobility, smart control, and DIY engineering in one sleek package.

## Mechanism

### 3D-Printed Parts

| Filename           | Thumbnail                                                                     | Required # |
| ------------------ | ----------------------------------------------------------------------------- | ---------- |
| chassis            | <img src="./imgs/chassis.jpg" alt="chassis" width="400"/>                     | 1          |
| motor_cover        | <img src="./imgs/motor_cover.jpg" alt="motor_cover" width="400"/>             | 2          |
| pcb_holder         | <img src="./imgs/pcb_holder.jpg" alt="pcb_holder" width="400"/>               | 1          |
| top_cover          | <img src="./imgs/top_cover.jpg" alt="top_cover" width="400"/>                 | 1          |
| battery_cover      | <img src="./imgs/battery_cover.jpg" alt="battery_cover" width="400"/>         | 1          |
| fill_block         | <img src="./imgs/fill_block.jpg" alt="fill_block" width="400"/>               |            |
| master_wheel_oring | <img src="./imgs/master_wheel.jpg" alt="master_wheel_oring" width="400"/>     |            |
| slave_wheel_oring  | <img src="./imgs/slave_wheel.jpg" alt="slave_wheel_oring" width="400"/>       |            |
| connector_front    | <img src="./imgs/connector_front.jpg" alt="connector_front" width="400"/>     | 1          |
| connector_trailer  | <img src="./imgs/connector_trailer.jpg" alt="connector_trailer" width="400"/> | 1          |

## Software

### ESP32

- Install `esp32` in Arduino IDE's Boards Manager.
- Select `ESP32 Dev Module` for the board.

`smarty.ino` is for the robot, and `joystick.ino` is for the remote controller.
