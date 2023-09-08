// CANStubImpl.hpp
#ifndef CANSTUBIMPL_H_
#define CANSTUBIMPL_H_
#include <CommonAPI/CommonAPI.hpp>
#include <v1/commonapi/CANStubDefault.hpp>

class CANStubImpl: public v1::commonapi::CANStubDefault {
public:
  CANStubImpl();
  virtual ~CANStubImpl();
  virtual void humPublisher(const uint8_t  canHum);
  virtual void tmpPublisher(const int8_t   canTmp);
  virtual void rpmPublisher(const uint16_t canRpm);
  virtual void spdPublisher(const uint8_t  canSpd);
  virtual void batPublisher(const uint8_t  canBat);

private:
  uint8_t  _hum;
  int8_t   _tmp;
  uint16_t _rpm;
  uint8_t  _spd;
  uint8_t  _bat;
};
#endif /* CANSTUBIMPL_H_ */
