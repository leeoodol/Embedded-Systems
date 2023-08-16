import os
import time
import random
from pydbus import SessionBus
from gi.repository import GLib

class dbusService:
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

    def get_rpm(self):
        rpm = random.randint(100, 200)
        return str(rpm)

    def get_distance(self):
        distance = random.randint(10, 20)
        return str(distance)

    def energy_report(self):
        battery = random.randint(50,60)
        return str(battery)



loop = GLib.MainLoop()
bus = SessionBus()

bus.publish("com.example.dbusService", dbusService())
loop.run()

