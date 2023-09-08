#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <thread>
#include <CommonAPI/CommonAPI.hpp>
#include "CANStubImpl.hpp"


int main() {

    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    while (true) {
        int nbytes = read(s, &frame, sizeof(struct can_frame));
        if (nbytes > 0) {
            std::cout << "Received CAN ID: " << frame.can_id << " Data: ";
            for (int i = 0; i < frame.can_dlc; i++) {
                std::cout << static_cast<int>(frame.data[i]) << " ";
            }
            std::cout << std::endl;
        }
    }

    CommonAPI::Runtime::setProperty("LogContext", "CAN");
    CommonAPI::Runtime::setProperty("LogApplication", "CAN");
    CommonAPI::Runtime::setProperty("LibraryBase", "CAN");

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();

    std::string domain = "local";
    std::string instance = "can"; //instance id
    std::string connection = "team07";

    std::shared_ptr<CANStubImpl> junhoService = std::make_shared<CANStubImpl>();
    while (!runtime->registerService(domain, instance, junhoService, connection)) {
        std::cout << "Register Service failed, trying again in 100 milliseconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Successfully Registered Service!" << std::endl;

    while (true) {
        myService->incCounter(); // Change value of attribute, see stub implementation
        std::cout << "Waiting for calls... (Abort with CTRL+C)" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}