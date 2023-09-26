import gi.repository
from pydbus import SessionBus
from gi.repository import GLib
import threading
import time
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad

current_value = 0.5

class MyDBusService:
    """
    <node>
        <interface name='org.example.QtDBusService'>
            <method name='SetValue'>
                <arg type='i' name='value' direction='in'/>
            </method>
        </interface>
    </node>
    """
    
    def SetValue(self, value:int):
        global current_value
        current_value = value/10
        print(f"Received value: {value}")
        
def dbus_loop():
    bus = SessionBus()
    bus.publish("org.example.QtDBusService", MyDBusService())
    loop = GLib.MainLoop()
    loop.run()

def update_loop():
    global current_value
    
    shanwan_gamepad = ShanWanGamepad()
    #piracer = PiRacerPro()
    piracer = PiRacerStandard()
    while True:
        print(f"Current value in loop: {current_value}")
        gamepad_input = shanwan_gamepad.read_data()
        throttle = gamepad_input.analog_stick_right.y * current_value
        steering = -gamepad_input.analog_stick_left.x

        #print(f'throttle={throttle}, steering={steering}')

        piracer.set_throttle_percent(throttle)
        piracer.set_steering_percent(steering)





t = threading.Thread(target=dbus_loop)
t.start()

update_loop()
