#ifndef CANClient_H
#define CANClient_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/commonapi/CANProxy.hpp>
#include <v1/commonapi/CANProxyBase.hpp>

// CommonAPI definition
using namespace v1::commonapi;

class CANClient
{
public:
  CANClient();
  ~CANClient();

  // CommonAPI vSomeIP func()
  void initVsomeipClient();
  void startSubscribeHumidity();
  void startSubscribeTemperature();
  void startSubscribeRPM();
  void startSubscribeSpeed();
  void startSubscribeBattery();

private:
  // CommonAPI vSomeIP var
  std::shared_ptr<CommonAPI::Runtime> runtime;
  std::shared_ptr<CANProxy<>> moonProxy;
  CommonAPI::CallStatus callStatus;
  
  uint8_t  _humValue;
  int8_t   _tmpValue;
  uint16_t _rpmValue;
  uint8_t  _spdValue;
  uint8_t  _batValue;
};

#endif // CANClient_H
