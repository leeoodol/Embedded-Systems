import time
from piracer.vehicles import PiRacerStandard

if __name__ == '__main__':

    # piracer = PiRacerPro()
    piracer = PiRacerStandard()

    # Forward
    piracer.set_throttle_percent(0.5)
    time.sleep(60.0)

    # Brake
    piracer.set_throttle_percent(0.0)
    time.sleep(0.1)
