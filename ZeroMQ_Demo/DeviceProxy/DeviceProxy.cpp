#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include "getTime.h"

int main(int argc, char** argv)
{
    std::cout << argc << " " << argv << std::endl;
    if (argc < 5) {
        std::cout << "usage : " << argv[0] << "router_ip router_port dealer_ip dealer_port" << std::endl;
        return -1;
    }

    int major, minor, patch;
    zmq_version(&major, &minor, &patch);
    std::cout << "Current 0MQ version is " << major << "," << minor << "," << patch << std::endl;
    std::cout<<"--------------------------------------------------------------------"<<std::endl;
    char addr[128] = { 0 };
    void* context = zmq_ctx_new();

    snprintf(addr, sizeof(addr), "tcp://%s:%s", argv[1], argv[2]);
    void* router = zmq_socket(context, ZMQ_ROUTER);
    int rc = zmq_bind(router, addr);
    std::cout << getTime() << " router bind:" << addr << (rc ? " error" : " ok") << std::endl;
    if (rc) {
        std::cout << getTime() << "router bind error:" << zmq_strerror(zmq_errno()) << std::endl;
        zmq_close(router);
        zmq_ctx_term(context);
        return -2;
    }

    snprintf(addr, sizeof(addr), "tcp://%s:%s", argv[3], argv[4]);
    void* dealer = zmq_socket(context, ZMQ_DEALER);
    rc = zmq_bind(dealer, addr);
    std::cout << getTime() << " router bind:" << addr<< (rc ? " error" : " ok") << std::endl;
    if (rc) {
        std::cout << getTime() << "router bind error:" << zmq_strerror(zmq_errno()) << std::endl;
        zmq_close(router);
        zmq_close(dealer);
        zmq_ctx_term(context);
        return -3;
    }

    zmq_proxy(router, dealer, NULL);

    zmq_close(router);
    zmq_close(dealer);
    zmq_ctx_term(context);

    return 0;
}