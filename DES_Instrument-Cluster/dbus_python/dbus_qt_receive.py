from pydbus import SessionBus
import time

bus = SessionBus()
the_object = bus.get("net.team07.pydbus.ClientServerExample")

try:
    while True:
        reply_rpm = the_object.get_rpm()
        print("RPM:", reply_rpm)
        
        time.sleep(1)  # Adjust the delay as needed
except KeyboardInterrupt:
    print("Keyboard interrupt detected. Exiting...")

