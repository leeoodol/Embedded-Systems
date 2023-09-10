import os
import can
import time
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad
from pydbus import SessionBus
from gi.repository import GLib
from threading import Thread
from collections import deque

weights = [0.2, 0.3, 0.5]

data_dis = deque(maxlen=len(weights))
data_rpm = deque(maxlen=len(weights))
mode = 0.5


def weighted_moving_average(data, weights):
    moving_average = sum(val * weight for val, weight in zip(data, weights))
    return moving_average

CAN_ID = "can0"
        
class dbusService(object):
    """
		<node>
			<interface name='com.example.dbusService'>
				<method name='get_rpm'>
					<arg type='s' name='rpm' direction='out'/>
				</method>
				<method name='get_distance'>
					<arg type='s' name='distance' direction='out'/>
				</method>
                <method name='energy_report'>
                    <arg type='s' name='battery' direction='out'/>
                </method>
                <method name='echo_mode'>
                </method>
                <method name='normal_mode'>
                </method>
                <method name='sport_mode'>
                </method>
			</interface>
		</node>
	"""
    
    def __init__(self):
        os.system(f'sudo ifconfig {CAN_ID} down')
        os.system(f'sudo ip link set {CAN_ID} up type can bitrate 125000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on')
        self.can = can.interface.Bus(channel = CAN_ID, bustype = 'socketcan')
        self.piracer = PiRacerStandard()
        self.shanwan_gamepad = ShanWanGamepad()
        
        can_thread_rec = Thread(target=self.can_recv_thread, args=(self.can,))
        can_thread_rec.start()
        
        can_thread_pad = Thread(target=self.game_pad)
        can_thread_pad.start()
        
    def game_pad(self):
        gamepad_input = self.shanwan_gamepad.read_data()
        throttle = gamepad_input.analog_stick_right.y * mode
        steering = -gamepad_input.analog_stick_left.x

        self.piracer.set_throttle_percent(throttle)
        self.piracer.set_steering_percent(steering)
    
    def can_recv_thread(self, can_bus):
        while True:
            self.msg = can_bus.recv()
            if self.msg is None:
                print('Timeout occurred, no message.')
            else:
                pass
        

    
    def get_rpm(self):
        data_rpm.append(self.msg.data[0])
        rpm = str(round(weighted_moving_average(data_rpm, weights)))
        print(rpm)
        return rpm
    

    def get_distance(self):
        data_dis.append(self.msg.data[1])
        distance = str(round(weighted_moving_average(data_dis, weights)))
        print(distance)
        return distance
        
        

    def energy_report(self):
        battery_voltage = self.piracer.get_battery_voltage()
        battery_current = self.piracer.get_battery_current()
        power_consumption = self.piracer.get_power_consumption()
        battery = str(round((battery_voltage - 9) / 3.2 * 100))
        print(battery)
        return battery
    
    def echo_mode(self):
        mode = 0.3
        
    def normal_mode(self):
        mode = 0.5
        
    def sport_mode(self):
        mode = 0.7
        

if __name__ == "__main__":
    loop = GLib.MainLoop()
    bus = SessionBus()
    bus.publish("com.example.dbusService", dbusService())
    
    try:
        loop.run()
    except KeyboardInterrupt:
        os.system(f'sudo ifconfig {CAN_ID} down')
        loop.quit()


