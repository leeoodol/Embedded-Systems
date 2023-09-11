from pydbus import SessionBus
import time

bus = SessionBus()
the_object = bus.get("net.team07.pydbus.ClientServerExample")

try:
    while True:
        reply_rpm = the_object.get_rpm()
        print("RPM:", reply_rpm)
        
        reply_distance = the_object.get_distance()
        print("Distance:", reply_distance)
        
        reply_battery = the_object.energy_report()
        print("Battery:", reply_battery)
        
        # Delay for a short period before the next iteration
        time.sleep(1)  # Adjust the delay as needed
except KeyboardInterrupt:
    print("Keyboard interrupt detected. Exiting...")

