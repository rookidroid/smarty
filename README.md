# <img src="./imgs/smarty.svg" alt="logo" width="64"/> Smarty

A mini robot modified based on [SMARS modular robot](https://www.thingiverse.com/thing:2662828)

![banner](imgs/banner.jpg)

## Objective

The objective of this work is to build a cute little robot for my daughter. It could be a toy for her to play with, but my main intention is to let her have an early access to engineering knowledge and inspire her for future learning.

## Overview

The brain of Smarty is a ESP32 Dev board powered by a 9V battery. It is programmed with Arduino. The Dev board and the power switch are accessible  under the top cover, which is attached via magnets.

<img src="./imgs/smarty_open.jpg" alt="smarty_open" width="400"/>

A remote controller comes together with this design. This remote controller has only one joystick. Another ESP32 Dev board is used in this remote controller and it is also powered by a 9V battery.

<img src="./imgs/smarty_remote.jpg" alt="smarty_remote" width="400"/>

https://user-images.githubusercontent.com/471808/212742878-e41ff177-bd68-45c1-9dd8-94123c825847.mp4

## Software

### ESP32

- Install `esp32` in Arduino IDE's Boards Manager.
- Select `ESP32 Dev Module` for the board.

`smarty.ino` is for the robot, and `joystick.ino` is for the remote controller.

### Android

There is an Android APP that can be used to control the robot too. The source files are under `software/android`.

## Mechanism

### 3D printed parts

#### Body x1

- chassis x1
<img src="./imgs/chassis.jpg" alt="chassis" width="300"/>

- battery_cover x1
<img src="./imgs/battery_cover.jpg" alt="battery_cover" width="300"/>

- board_holder x1
<img src="./imgs/board_holder.jpg" alt="board_holder" width="300"/>

- connector x2
<img src="./imgs/connector.jpg" alt="connector" width="300"/>

- holding_board x2
<img src="./imgs/holding_board.jpg" alt="holding_board" width="300"/>

- top_cover x2
<img src="./imgs/top_cover.jpg" alt="top_cover" width="300"/>

#### Wheel x4

- wheel x4
<img src="./imgs/wheel.jpg" alt="wheel" width="300"/>

- retaining_ring x4
<img src="./imgs/retaining_ring.jpg" alt="retaining_ring" width="300"/>

- mechanical_track x32
<img src="./imgs/mechanical_track.jpg" alt="mechanical_track" width="300"/>

#### Attachmenet

- eyes
<img src="./imgs/eyes.jpg" alt="eyes" width="300"/>

- pusher
<img src="./imgs/pusher.jpg" alt="pusher" width="300"/>

