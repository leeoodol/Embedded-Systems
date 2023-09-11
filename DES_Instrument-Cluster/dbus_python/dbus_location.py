import os
import time
import random
from pydbus import SessionBus
from gi.repository import GLib
from  geopy.geocoders import Nominatim


class dbusService:
    """
        <node>
            <interface name='com.example.locationService'>
                <method name='get_latitude'>
                    <arg type='d' name='latitude' direction='out'/>
                </method>
                <method name='get_longitude'>
                    <arg type='d' name='longitude' direction='out'/>
                </method>


            </interface>
        </node>
    """
    def __init__(self):
        self.battery=0
        self.rpm=0
        self.cnt=1
        self.geolocator = 0


    def get_latitude(self)->float:
        self.geolocator = Nominatim(user_agent="MyApp")
        location = self.geolocator.geocode("Hyderabad")
        return location.latitude

    def get_longitude(self)->float:
        self.geolocator = Nominatim(user_agent="MyApp")
        location = self.geolocator.geocode("Hyderabad")
        return location.longitude
loop = GLib.MainLoop()
bus = SessionBus()

bus.publish("com.example.locationService", dbusService())
loop.run()
