import os
import can
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
			</interface>
		</node>
	"""

    def __init__(self):
        os.system(f'sudo ifconfig {CAN_ID} down')
        os.system(f'sudo ip link set {CAN_ID} up type can bitrate 125000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on')
        self.can = can.interface.Bus(channel = CAN_ID, bustype = 'socketcan')

    def get_rpm(self):
        msg = self.can.recv()
        if msg is None:
            print('Timeout occurred, no message.')
        rpm = str(msg.data[0])
        return rpm
    
    def get_distance(self):
        msg = self.can.recv()
        if msg is None:
            print('Timeout occurred, no message.')
        distance = str(msg.data[1])
        return distance

    def Quit(self):
        loop.quit()

loop = GLib.MainLoop()
bus = SessionBus()
bus.publish("net.team07.pydbus.ClientServerExample", MyDBUSService())
try:
    loop.run()
except KeyboardInterrupt:
    os.system(f'sudo ifconfig {CAN_ID} down')    
