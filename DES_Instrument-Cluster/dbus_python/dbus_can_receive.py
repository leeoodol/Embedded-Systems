from pydbus import SessionBus

bus = SessionBus()

the_object = bus.get("net.team07.pydbus.ClientServerExample")

reply = the_object.get_rpm()
print(reply)
reply = the_object.get_distane()
print(reply)
