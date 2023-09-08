// CANService-main.cpp
#include <iostream>
#include <string>
#include <unistd.h>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/commonapi/CANProxy.hpp>
#include <v1/commonapi/CANProxyBase.hpp>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "defs.h" 
#include "ina219.h" 
#include "CANStubImpl.hpp"
#include <thread>

// Ina219 definition
#define I2C_ADDR 0x42
#define I2C_DEV "/dev/i2c-1"
#define BATTERY_VOLTAGE_100_PERCENT 8260
#define BATTERY_VOLTAGE_0_PERCENT 6000 
#define BATTERY_CAPACITY 2400
#define MIN_CHARGING_CURRENT 10
#define SHUNT_MILLIOHMS 100

// CAN definition
#define CAN_FRAME_MAX_LEN 7

// CommonAPI definition
using namespace v1::commonapi;

int _initSocket(const char *ifname){
	int sock_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (sock_fd < 0)
		std::cout << "Failed to socket create" << std::endl;
  std::cout << "Success to socket create" << std::endl;

	struct ifreq ifr;
	strcpy(ifr.ifr_name, ifname);
	int ret = ioctl(sock_fd, SIOCGIFINDEX, &ifr);
	if (ret < 0)
    std::cout << "Failed to get CAN interface index" << std::endl;
  std::cout << "Success to get CAN interface index : " << ret << std::endl;

	struct sockaddr_can addr;
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	ret = bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
    std::cout << "Failed to socket bind" << std::endl;
	std::cout << "Success to socket bind" << std::endl;
	
	return sock_fd;
}

int main(){

	// CommonAPI vSomeIP
	std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();
  std::shared_ptr<CANStubImpl> moonService = std::make_shared<CANStubImpl>();;
  
  while (!runtime->registerService("local", "can", moonService)) {
    std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

	uint16_t rpm_combine;

	// CAN
    int can_fd = _initSocket("vcan0");
	if (can_fd < 0)
		return can_fd;	
	struct can_frame frame;
	int rd_byte;

	// Battery
	INA219 *ina219 = ina219_create (I2C_DEV, I2C_ADDR, SHUNT_MILLIOHMS,
                     BATTERY_VOLTAGE_0_PERCENT, BATTERY_VOLTAGE_100_PERCENT,
                     BATTERY_CAPACITY, MIN_CHARGING_CURRENT);
	INA219ChargeStatus charge_status;
	int mV;
	int percent_charged;
	int battery_current_mA;
	int minutes;		
	char *error = NULL;
	bool ina_status = false;
	if (ina219_init (ina219, &error))
		ina_status = true;

	while (true)
	{
		// CAN
		rd_byte = read(can_fd, &frame, sizeof(frame));
		if (rd_byte < 0)
      std::cout << "Failed to recieve CAN frame" << std::endl;
		else if (rd_byte < (int)sizeof(struct can_frame))
      std::cout << "Incomplete CAN frame is received" << std::endl;
		else if (frame.can_dlc > CAN_MAX_DLEN)
      std::cout << "Invalid dlc" << std::endl;

    //std::cout << "0x" << frame.can_id << "[" << frame.can_dlc << "]" << std::endl;

		// Battery
		if (ina_status){
			if (ina219_get_status (ina219, &charge_status, &mV, &percent_charged, &battery_current_mA, &minutes, &error)){
				switch (charge_status){
					case INA219_FULLY_CHARGED:
            //std::cout << "Fully charged" << std::endl;
						break;
					case INA219_CHARGING:
            //std::cout << "Charging, "<< minutes << " minutes until fully charged" << std::endl; 
						break;
					case INA219_DISCHARGING:
            //std::cout << "Discharging " << minutes << " minutes left" << std::endl;
						break;
				}
				//printf ("Battery voltage: %.2f V\n", mV / 1000.0); // Convert to V
    	  		//printf ("Battery current: %d mA\n", battery_current_mA); 
    	  		//printf ("Battery charge: %.d %%\n", percent_charged);
				frame.data[5] = percent_charged;
			}
		}
		
		// print frame data
		for (int i = 0; i <= frame.can_dlc; i++)
			printf("%02X ",frame.data[i]);
		printf("\n");

		// unsigned char -> uint16_t
		rpm_combine = frame.data[2] | uint16_t(frame.data[3]) << 8;

		// send data to client
    moonService->humPublisher(frame.data[0]);
    moonService->tmpPublisher(frame.data[1]);
    moonService->rpmPublisher(rpm_combine);
    moonService->spdPublisher(frame.data[4]);
    moonService->batPublisher(frame.data[5]);
    //std::cout << "Waiting for calls... (Abort with CTRL+C)" << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    //	usleep(1000);
	}

    return 0;
}
