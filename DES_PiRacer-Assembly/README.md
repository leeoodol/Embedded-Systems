# SEA:ME Pi Racer
-----------------------------------------------------------
## Table of Contents

----------------------------------------

## Assemble Pi Racer
- [Pi-Racer Wiki](https://www.waveshare.com/wiki/JetRacer_Assembly_Manual)
  - Procedure 11 there is some error : M3*10 -> M3*8
> **Important Point** 
>> 1. Steering Casis has to be physicaly neutral  
>> 2. Connect right direction Li-ON battery

## Raspberry Pi Configuration
- About OS
  - raspi os : rasbian os desktop “bulleye” 64-bit
  - hostname : raspberrypi
  - id : team07
  - pwd : 1234
  - connect with ssh : ssh -Y team07@192.168.86.31

- raspi config
  - enable Interfacing Options - I2C
  - enable Interfacing Options - SSH
  - enable Interfacing Options - SPI
  - enable Interfacing Options - Camera Legacy
    ```
    sudo reboot
    ```
  - install dependencies
    ```
    sudo apt update
    sudo apt-get upgrade
    sudo apt install \
        gcc \
        v4l-utils \
        i2c-tools \
        raspi-config \
        python3-dev \
        python3-setuptools \
        python3-venv \
        libopencv-dev
    
    sudo apt-get install build-essential python3 python3-dev python3-pip python3-virtualenv python3-numpy python3-picamera python3-pandas python3-rpi.gpio i2c-tools avahi-utils joystick libopenjp2-7-dev libtiff5-dev gfortran libatlas-base-dev libopenblas-dev libhdf5-serial-dev libgeos-dev git ntp
    sudo apt-get install libilmbase-dev libopenexr-dev libgstreamer1.0-dev libjasper-dev libwebp-dev libatlas-base-dev libavcodec-dev libavformat-dev libswscale-dev libqtgui4 libqt4-test
    ```

## Run Pi Racer
![poster](./image/piracer_overview.drawio.svg)
- Calibrate Pi-Racer
  - to see which path Pi-Racer library's from
  ```
  which python3
  ```
  - set path Pi-Racer library
  ```
  source venv/bin/activate
  ```
  - set path Pi-Racer library
  ```
  deactivate venv/bin/activate
  source env/bin/activate
  ```
  - calibrate (/home/team7/env/lib/python3.9/site-packages/piracer/vehicle.py)
  ```
  class PiRacerBase:
    ...
    def _get_50hz_duty_cycle_from_percent(cls, value: float) -> float:
      return 0.0015 + (value * 0.0005) #ex) 0.0016
  ```
  
- Install Pi-Racer package
  ```
  cd ~
  mkdir piracer_test/
  cd piracer_test/
  python3 -m venv venv
  source venv/bin/ativate

  pip install piracer-py
  ```
- Run control example
  ```
  import time
  from piracer.vehicles import PiRacerStandard

  if __name__ == '__main__':

  
  piracer = PiRacerStandard()

  # Forward
  piracer.set_throttle_percent(0.2)
  time.sleep(2.0)

  # Brake
  piracer.set_throttle_percent(-1.0)
  time.sleep(0.5)
  piracer.set_throttle_percent(0.0)
  time.sleep(0.1)

  # Backward
  piracer.set_throttle_percent(-0.3)
  time.sleep(2.0)

  # Stop
  piracer.set_throttle_percent(0.0)

  # Steering left
  piracer.set_steering_percent(1.0)
  time.sleep(1.0)

  # Steering right
  piracer.set_steering_percent(-1.0)
  time.sleep(1.0)

  # Steering neutral
  piracer.set_steering_percent(0.0)
  ```
- Remote control example
  ```
  from piracer.vehicles import PiRacerStandard
  from piracer.gamepads import ShanWanGamepad

  if __name__ == '__main__':

    shanwan_gamepad = ShanWanGamepad()
    piracer = PiRacerStandard()

    while True:
        gamepad_input = shanwan_gamepad.read_data()

        throttle = gamepad_input.analog_stick_right.y * 0.5
        steering = -gamepad_input.analog_stick_left.x

        print(f'throttle={throttle}, steering={steering}')

        piracer.set_throttle_percent(throttle)
        piracer.set_steering_percent(steering)
  ```
- Grab images example
  - image is grayscale due to MonochromeCamera
  ```
  import cv2
  from piracer.cameras import Camera, MonochromeCamera

  if __name__ == '__main__':
    camera = MonochromeCamera()

    image = camera.read_image()
    cv2.imwrite('image.png', image) #save image as 'image.png'
  ```

- Grab images example using opencv
  ```
  import cv2

  def main():
    camera = cv2.VideoCapture(0)

    if not camera.isOpened():
        print("do not open camera")
        return

    while True:
        ret, frame = camera.read()

        if not ret:
            print("do not read frame")
            break

        cv2.imshow("frame", frame)

        # press 'q' to exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
  
    camera.release()
    cv2.destroyAllWindows()

  if __name__ == "__main__":
    main()
  ```
- OLED display example
  ```
  import os
  import pathlib
  import time
  import signal
  from piracer.vehicles import PiRacerBase, PiRacerStandard, PiRacerPro


  FILE_DIR = pathlib.Path(os.path.abspath(os.path.dirname(__file__)))


  def print_battery_report(vehicle: PiRacerBase):
    battery_voltage = vehicle.get_battery_voltage()
    battery_current = vehicle.get_battery_current()
    power_consumption = vehicle.get_power_consumption()

    display = vehicle.get_display()

    output_text = 'U={0:0>6.3f}V\nI={1:0>8.3f}mA\nP={2:0>6.3f}W'.format(battery_voltage, battery_current,
                                                                                power_consumption)

    display.fill(0)
    display.text(output_text, 0, 0, 'white', font_name=FILE_DIR / 'fonts/font5x8.bin')
    display.show()

  def handle_exit(signal_num, frame):
    display = piracer.get_display()
    display.fill(0)
    display.show()
    sys.exit(0)
  
  if __name__ == '__main__':
  
    piracer = PiRacerStandard()
    signal.signal(signal.SIGINT, handle_exit)
  
    try:
        while True:
          print_battery_report(piracer)
          time.sleep(0.5)
    except KeyboardInterrupt:
      pass
  ```
  
