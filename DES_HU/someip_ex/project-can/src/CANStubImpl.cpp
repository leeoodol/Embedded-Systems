// CANStubImpl.cpp
#include "CANStubImpl.hpp"

CANStubImpl::CANStubImpl() { 
  _hum = 0;
  _tmp = 0;
  _rpm = 0;
  _spd = 0;
  _bat = 0;
}

CANStubImpl::~CANStubImpl() {}

void CANStubImpl::humPublisher(const uint8_t  canHum) {
  if (_hum != canHum) {
    _hum = canHum;
    setHumAttribute(_hum);
    std::cout <<  "New CAN Humadity Data = " << int(_hum) << "!" << std::endl;
  }
}

void CANStubImpl::tmpPublisher(const int8_t   canTmp) {
  if (_tmp != canTmp) {
    _tmp = canTmp;
    setTmpAttribute(_tmp);
    std::cout <<  "New CAN Temperature Data = " << int(_tmp) << "!" << std::endl;
  }
}

void CANStubImpl::rpmPublisher(const uint16_t canRpm) {
  if (_rpm != canRpm) {
    _rpm = canRpm;
    setRpmAttribute(_rpm);
    std::cout <<  "New CAN RPM Data = " << int(_rpm) << "!" << std::endl;
  }
}

void CANStubImpl::spdPublisher(const uint8_t  canSpd) {
  if (_spd != canSpd) {
    _spd = canSpd;
    setSpdAttribute(_spd);
    std::cout <<  "New CAN Speed Data = " << int(_spd) << "!" << std::endl;
  }
}

void CANStubImpl::batPublisher(const uint8_t  canBat) {
  if (_bat != canBat) {
    _bat = canBat;
    setBatAttribute(_bat);
    std::cout <<  "New CAN Battery Data  = " << int(_bat) << "!" << std::endl;
  }
}
