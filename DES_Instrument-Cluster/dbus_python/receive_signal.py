#!/usr/bin/env python3

import pydbus 
import dbus
import dbus.service
from dbus.mainloop.glib import DBusGMainLoop
from gi.repository import GLib

class ExampleSignalReceiver(dbus.service.Object):
    def __init__(self):
        bus_name = dbus.service.BusName('com.example.DBusExample', bus=dbus.SessionBus())
        dbus.service.Object.__init__(self, bus_name, '/com/example/DBusExample')

    @dbus.service.signal('com.example.DBusExample')
    def ExampleSignal(self, message):
        print('Signal received:', message)

    @dbus.service.method('com.example.DBusExample')
    def ExampleMethod(self):
        print('Method called')
        return 'Example response'
    
DBusGMainLoop(set_as_default=True)
ExampleSignalReceiver()

bus = dbus.SessionBus()
bus.add_signal_receiver(
    ExampleSignalReceiver.ExampleSignal,
    dbus_interface='com.example.DBusExample',
    signal_name='ExampleSignal'
)

loop = GLib.MainLoop()
loop.run()