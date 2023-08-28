import os
import can
import time
from piracer.vehicles import PiRacerStandard
from pydbus import SessionBus
from gi.repository import GLib
from threading import Thread
from collections import deque

weights = [0.2, 0.3, 0.5]

data_dis = deque(maxlen=len(weights))
data_rpm = deque(maxlen=len(weights))



def weighted_moving_average(data, weights):
    moving_average = sum(val * weight for val, weight in zip(data, weights))
    return moving_average

CAN_ID = "can1"
        
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
			</interface>
		</node>
	"""

    def can_recv_thread(self, can_bus):
        while True:
            self.msg = can_bus.recv()
            if self.msg is None:
                print('Timeout occurred, no message.')
            else:
                pass
    
    def __init__(self):
        os.system(f'sudo ifconfig {CAN_ID} down')
        os.system(f'sudo ip link set {CAN_ID} up type can bitrate 125000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on')
        self.can = can.interface.Bus(channel = CAN_ID, bustype = 'socketcan')
        self.piracer = PiRacerStandard()
        
        can_thread = Thread(target=self.can_recv_thread, args=(self.can,))
        can_thread.start()

    
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

if __name__ == "__main__":
    loop = GLib.MainLoop()
    bus = SessionBus()
    bus.publish("com.example.dbusService", dbusService())
    
    try:
        loop.run()
    except KeyboardInterrupt:
        os.system(f'sudo ifconfig {CAN_ID} down')
        loop.quit()


