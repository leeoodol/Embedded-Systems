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
            </interface>
        </node>
    """

    def get_rpm(self):
        rpm = random.randint(50, 60)
        return str(rpm)

loop = GLib.MainLoop()
bus = SessionBus()

bus.publish("com.example.dbusService", dbusService())
loop.run()
