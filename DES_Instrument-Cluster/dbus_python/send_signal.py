#!/usr/bin/env python3

import dbus
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop

class ExampleDBusService(dbus.service.Object):
    def __init__(self):
        bus_name = dbus.service.BusName('com.example.DBusExample', bus=dbus.SessionBus())
        dbus.service.Object.__init__(self, bus_name, '/com/example/DBusExample')

    @dbus.service.signal('com.example.DBusExample')
    def ExampleSignal(self, message):
        print('Signal sent:', message)
        
DBusGMainLoop(set_as_default=True)
ExampleDBusService()

bus = dbus.SessionBus()
dbus_service = bus.get_object('com.example.DBusExample', '/com/example/DBusExample')

dbus_iface = dbus.Interface(dbus_service, 'com.example.DBusExample')
dbus_iface.ExampleSignal('hello world')