#ifndef CANService_H
#define CANService_H

#include <linux/can.h>
#include <thread>
#include <CommonAPI/CommonAPI.hpp>
#include "CANStubImpl.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

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


// Ina219 definition
#define I2C_ADDR 0x42
#define I2C_DEV "/dev/i2c-1"
#define BATTERY_VOLTAGE_100_PERCENT 8260
#define BATTERY_VOLTAGE_0_PERCENT 6000
#define BATTERY_CAPACITY 2400
#define MIN_CHARGING_CURRENT 10
#define SHUNT_MILLIOHMS 100
typedef struct _INA219 INA219;

// CAN definition
# define CAN_FRAME_MAX_LEN 8

// CommonAPI definition
using namespace v1::commonapi;

class CANService
{
public:
  explicit CANService();
  ~CANService();

  // CAN func()
  void initSocket(const char *ifname);
  void readSocket();

  // Battery func()
  void initBattery();
  void readBattery();
  
  // CommonAPI vSomeIP func()
  void initVsomeipService();
  void startCommunicate();

private:
  // CAN var
  int can_fd;
  struct can_frame frame;
  int rd_byte;

  // Battery var
  bool ina_status;
  // INA219 *ina219;
  int mV;
  int percent_charged;
  int battery_current_mA;
  int minutes;

  // RPM
  uint16_t rpm_combine;

  // CommonAPI vSomeIP var
  std::shared_ptr<CommonAPI::Runtime> runtime;
  std::shared_ptr<CANStubImpl> moonService;
};

#endif // CANService_H
