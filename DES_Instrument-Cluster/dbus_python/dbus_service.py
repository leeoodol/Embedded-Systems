import os
import can
import time
from piracer.vehicles import PiRacerStandard
from pydbus import SessionBus
from gi.repository import GLib

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
			</interface>
		</node>
	"""

    def __init__(self):
        os.system(f'sudo ifconfig {CAN_ID} down')
        os.system(f'sudo ip link set {CAN_ID} up type can bitrate 125000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on')
        self.can = can.interface.Bus(channel = CAN_ID, bustype = 'socketcan')
        self.piracer = PiRacerStandard()

    def get_rpm(self):
        msg = self.can.recv()
        if msg is None:
            print('Timeout occurred, no message.')
        rpm = msg.data[0]
        print(rpm)
        return str(rpm)

    def get_distance(self):
        msg = self.can.recv()
        if msg is None:
            print('Timeout occurred, no message.')
        distance = msg.data[1]
        print(distance)
        return str(distance)

    def energy_report(self):
        battery_voltage = self.piracer.get_battery_voltage()
        battery_current = self.piracer.get_battery_current()
        power_consumption = self.piracer.get_power_consumption()
        battery = str(round((battery_voltage - 9) / 3.2 * 100))
        print(battery)
        return battery

loop = GLib.MainLoop()
bus = SessionBus()
bus.publish("com.example.dbusService", dbusService())
try:
    loop.run()
except KeyboardInterrupt:
    os.system(f'sudo ifconfig {CAN_ID} down')    
    loop.quit()
