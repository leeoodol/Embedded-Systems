// CANClient.cpp
#include "CANClient.hpp"

CANClient::CANClient() {
	_humValue = 0;
	_tmpValue = 0;
	_rpmValue = 0;
	_spdValue = 0;
	_batValue = 0;
	std::cout << "success init can client" << std::endl;
}

CANClient::~CANClient() {
	std::cout << "close can client" << std::endl;
}

void CANClient::initVsomeipClient() {
	runtime = CommonAPI::Runtime::get();
  moonProxy = runtime->buildProxy<CANProxy>("local", "can");
  std::cout << "Checking availability!" << std::endl;
  while (!moonProxy->isAvailable())
    std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void CANClient::startSubscribeHumidity() {
	moonProxy->getHumAttribute().getValue(callStatus, _humValue);
	if (callStatus != CommonAPI::CallStatus::SUCCESS) {
  } else {
		std::cout << "Got attribute humidity value: " << int(_humValue) << std::endl;

		moonProxy->getHumAttribute().getChangedEvent().subscribe([&](const uint8_t& humVal) {
			std::cout << "Received change humidity message: " << int(humVal) << std::endl;
		});
	}
}

void CANClient::startSubscribeTemperature() {
	moonProxy->getTmpAttribute().getValue(callStatus, _tmpValue);
	if (callStatus != CommonAPI::CallStatus::SUCCESS) {
  } else {
		std::cout << "Got attribute temperature value: " << int(_tmpValue) << std::endl;

		moonProxy->getTmpAttribute().getChangedEvent().subscribe([&](const uint8_t& tmpVal) {
			std::cout << "Received change temperature message: " << int(tmpVal) << std::endl;
		});
	}
}

void CANClient::startSubscribeRPM() {
	moonProxy->getRpmAttribute().getValue(callStatus, _rpmValue);
	if (callStatus != CommonAPI::CallStatus::SUCCESS) {
  } else {
		std::cout << "Got attribute RPM value: " << int(_rpmValue) << std::endl;

		moonProxy->getRpmAttribute().getChangedEvent().subscribe([&](const uint8_t& rpmVal) {
			std::cout << "Received change RPM message: " << int(rpmVal) << std::endl;
		});
	}
}

void CANClient::startSubscribeSpeed() {
	moonProxy->getSpdAttribute().getValue(callStatus, _spdValue);
	if (callStatus != CommonAPI::CallStatus::SUCCESS) {
  } else {
		std::cout << "Got attribute speed value: " << _spdValue << std::endl;

		moonProxy->getHumAttribute().getChangedEvent().subscribe([&](const uint8_t& spdVal) {
			std::cout << "Received change speed message: " << spdVal << std::endl;
		});
	}
}

void CANClient::startSubscribeBattery() {
	moonProxy->getBatAttribute().getValue(callStatus, _batValue);
	if (callStatus != CommonAPI::CallStatus::SUCCESS) {
    std::cout << "Remote call A failed!" << std::endl;
  } else {
		std::cout << "Got attribute battery value: " << int(_batValue) << std::endl;

		moonProxy->getBatAttribute().getChangedEvent().subscribe([&](const uint8_t& batVal) {
			std::cout << "Received change battery message: " << int(batVal) << std::endl;
		});
	}
}

int main() {
	CANClient canClient;
	canClient.initVsomeipClient();

	canClient.startSubscribeHumidity();
	canClient.startSubscribeTemperature();
	canClient.startSubscribeRPM();
	canClient.startSubscribeSpeed();
	canClient.startSubscribeBattery();

	while (true) {
		std::cout << "Waiting data ... " << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
	
	return 0;
}
