import socket
import network
from machine import Pin, PWM
import utime
import time

js_up = 1
js_down = 1
js_left = 1
js_right = 1

bt_up = 1
bt_down = 1
bt_left = 1
bt_right = 1

yellow_led = Pin(1, Pin.OUT)
green_led = Pin(0, Pin.OUT)

yellow_led.value(1)
green_led.value(0)


ssid = 'smartyrobot'
password = 'smartyrobot'

port = 1234
ip = '192.168.4.1'

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)

# Wait for connect or fail
wait = 10
while wait > 0:
    if wlan.status() < 0 or wlan.status() >= 3:
        break
    wait -= 1
    print('waiting for connection...')
    time.sleep(1)
 
# Handle connection error
if wlan.status() != 3:
    raise RuntimeError('wifi connection failed')
else:
    print('connected')
    print('IP: ', wlan.ifconfig()[0])
    yellow_led.value(0)
    green_led.value(1)
    
    js_up_pin = Pin(16, Pin.IN, pull=Pin.PULL_UP)
    js_down_pin = Pin(17, Pin.IN, pull=Pin.PULL_UP)
    js_left_pin = Pin(19, Pin.IN, pull=Pin.PULL_UP)
    js_right_pin = Pin(18, Pin.IN, pull=Pin.PULL_UP)

    bt_up_pin = Pin(13, Pin.IN, pull=Pin.PULL_UP)
    bt_down_pin = Pin(12, Pin.IN, pull=Pin.PULL_UP)
    bt_left_pin = Pin(14, Pin.IN, pull=Pin.PULL_UP)
    bt_right_pin = Pin(15, Pin.IN, pull=Pin.PULL_UP)
    


#     ip=wlan.ifconfig()[0]
    
    sock = socket.socket(socket.AF_INET, # Internet
             socket.SOCK_DGRAM) # UDP
#     sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
    
    while True:        
        js_up_new = js_up_pin.value()
        js_down_new = js_down_pin.value()
        js_left_new = js_left_pin.value()
        js_right_new = js_right_pin.value()
        
        bt_up_new = bt_up_pin.value()
        bt_down_new = bt_down_pin.value()
        bt_left_new = bt_left_pin.value()
        bt_right_new = bt_right_pin.value()
        
        if js_up != js_up_new or \
           js_down != js_down_new or \
           js_left != js_left_new or \
           js_right != js_right_new:

            js_up = js_up_new
            js_down = js_down_new
            js_left = js_left_new
            js_right = js_right_new
            if (js_up+js_down+js_left+js_right) == 4:
                sock.sendto('L0:R0:', (ip, port))
            elif (js_up+js_down+js_left+js_right) == 2:
                if js_up == 0 and js_left == 0:
                    sock.sendto('L-128:R-255:', (ip, port))
                elif js_up == 0 and js_right == 0:
                    sock.sendto('L-255:R-128:', (ip, port))
                elif js_down == 0 and js_left == 0:
                    sock.sendto('L128:R255:', (ip, port))
                elif js_down == 0 and js_right == 0:
                    sock.sendto('L255:R128:', (ip, port))
                else:
                    sock.sendto('L0:R0:', (ip, port))
            else:
                if js_up == 0:
                    sock.sendto('L-255:R-255:', (ip, port))
                elif js_down == 0:
                    sock.sendto('L255:R255:', (ip, port))
                elif js_left == 0:
                    sock.sendto('L255:R-255:', (ip, port))
                elif js_right == 0:
                    sock.sendto('L-255:R255:', (ip, port))
        elif bt_up != bt_up_new or \
           bt_down != bt_down_new or \
           bt_left != bt_left_new or \
           bt_right != bt_right_new:

            bt_up = bt_up_new
            bt_down = bt_down_new
            bt_left = bt_left_new
            bt_right = bt_right_new
            if (bt_up+bt_down+bt_left+bt_right) == 4:
                sock.sendto('L0:R0:', (ip, port))
            elif (bt_up+bt_down+bt_left+bt_right) == 2:
                if bt_up == 0 and bt_left == 0:
                    sock.sendto('L-128:R-255:', (ip, port))
                elif bt_up == 0 and bt_right == 0:
                    sock.sendto('L-255:R-128:', (ip, port))
                elif bt_down == 0 and bt_left == 0:
                    sock.sendto('L128:R255:', (ip, port))
                elif bt_down == 0 and bt_right == 0:
                    sock.sendto('L255:R128:', (ip, port))
                else:
                    sock.sendto('L0:R0:', (ip, port))
            else:
                if bt_up == 0:
                    sock.sendto('L-255:R-255:', (ip, port))
                elif bt_down == 0:
                    sock.sendto('L255:R255:', (ip, port))
                elif bt_left == 0:
                    sock.sendto('L255:R-255:', (ip, port))
                elif bt_right == 0:
                    sock.sendto('L-255:R255:', (ip, port))
        utime.sleep_ms(100)
            

            
