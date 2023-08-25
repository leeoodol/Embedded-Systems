import os
import can
import time
from piracer.vehicles import PiRacerStandard
import matplotlib.pyplot as plt

CAN_ID = "can1"

data_dis = []
data_rpm = []
weights = [0.2, 0.3, 0.5]
window_size = 3
alpha = 0.3

def exponential_moving_average(data, alpha):
    ema = [data[0]]  # Initialize EMA with the first data point
    for i in range(1, len(data)):
        ema_value = alpha * data[i] + (1 - alpha) * ema[-1]
        ema.append(ema_value)
    return ema

def weighted_moving_average(data, weights):
    moving_averages = []
    window_size = len(weights)
    for i in range(len(data) - window_size + 1):
        window = data[i : i + window_size]
        average = sum(window[j] * weights[j] for j in range(window_size)) / sum(weights)
        moving_averages.append(average)
    return moving_averages


def moving_average(data, window_size):
    moving_averages = []
    for i in range(len(data) - window_size + 1):
        window = data[i : i + window_size]
        average = sum(window) / window_size
        moving_averages.append(average)
    return moving_averages


if __name__ == '__main__':
    os.system(f'sudo ifconfig {CAN_ID} down')
    os.system(f'sudo ip link set {CAN_ID} up type can bitrate 125000 dbitrate 8000000 restart-ms 1000 berr-reporting on fd on')
    can_bus = can.interface.Bus(channel = CAN_ID, bustype = 'socketcan')
    piracer = PiRacerStandard()
    
    # piracer = PiRacerPro()
    piracer = PiRacerStandard()

    # Forward
    piracer.set_throttle_percent(0.5)
    start_time = time.time()
    
    while True:
        elapsed_time = time.time() - start_time
        print(elapsed_time)
        if elapsed_time >= 60.0:
            break
       
        msg = can_bus.recv()
        rpm = msg.data[0]
        data_rpm.append(int(rpm))
        print(rpm)
        
        distance = msg.data[1]
        data_dis.append(int(distance))
        print(distance)
    
    # Brake
    piracer.set_throttle_percent(-1.0)
    time.sleep(0.5)
    piracer.set_throttle_percent(0.0)
    time.sleep(0.1)
    
    os.system(f'sudo ifconfig {CAN_ID} down')
    result_ma_rpm = moving_average(data_rpm, window_size)
    result_wma_rpm = weighted_moving_average(data_rpm, weights)
    result_ema_rpm = exponential_moving_average(data_rpm, alpha)
        
    result_ma_dis = moving_average(data_dis, window_size)
    result_wma_dis = weighted_moving_average(data_dis, weights)
    result_ema_dis = exponential_moving_average(data_dis, alpha)
        
    # Plotting
    plt.figure(figsize=(12, 6))
    plt.subplot(2, 1, 1)
    plt.plot(data_rpm, label='Raw Data')
    plt.plot(result_ma_rpm, label='Moving Average')
    plt.plot(result_wma_rpm, label='Weighted Moving Average')
    plt.plot(result_ema_rpm, label='Exponential Moving Average')
    plt.legend()
    plt.title('RPM Data and Moving Averages')
        
    plt.subplot(2, 1, 2)
    plt.plot(data_dis, label='Raw Data')
    plt.plot(result_ma_dis, label='Moving Average')
    plt.plot(result_wma_dis, label='Weighted Moving Average')
    plt.plot(result_ema_dis, label='Exponential Moving Average')
    plt.legend()
    plt.title('Distance Data and Moving Averages')
        
    plt.tight_layout()
    plt.show()
        
