import os
import can
import time
from piracer.vehicles import PiRacerStandard, PiRacerPro
from pydbus import SessionBus
from gi.repository import GLib

CAN_ID = "can0"

class MyDBUSService(object):
    """
		<node>
			<interface name='net.team07.pydbus.ClientServerExample'>
				<method name='get_rpm'>
					<arg type='s' name='rpm' direction='out'/>
				</method>
				<method name='get_distance'>
					<arg type='s' name='distance' direction='out'/>
				</method>
                <method name='get_battery'>
					<arg type='s' name='battery' direction='out'/>
				</method>
			</interface>
		</node>
	"""

    def __init__(self):
        os.system(f'sudo ifconfig {CAN_ID} down')
        os.system(f'sudo ip link set {CAN_ID} up type can bitrate 125000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on')
        self.can = can.interface.Bus(channel = CAN_ID, bustype = 'socketcan')

    def receive_can_message(self):
        msg = self.can.recv(timeout=1)  # Wait for 1 second for a message
        if msg is None:
            print('Timeout occurred, no message.')
            return None
        return msg

    def get_rpm(self):
        msg = self.receive_can_message()
        if msg is None:
            print('Timeout occurred, no message.')
        rpm = msg.data[0]
        return str(rpm)
    
    def get_distance(self):
        msg = self.receive_can_message()
        if msg is None:
            print('Timeout occurred, no message.')
        distance = msg.data[1]
        return str(distance)

    def get_battery():
        battery_voltage = piracer.get_battery_voltage()
        #battery_current = piracer.get_battery_current()
        #power_consumption = piracer.get_power_consumption()
        battery = (battery_voltage - 6) / 2.4 * 100
        return str(battery)


    def Quit(self):
        loop.quit()

piracer = PiRacerStandard()
loop = GLib.MainLoop()
bus = SessionBus()
bus.publish("net.team07.pydbus.ClientServerExample", MyDBUSService())
try:
    loop.run()
except KeyboardInterrupt:
    os.system(f'sudo ifconfig {CAN_ID} down')    

