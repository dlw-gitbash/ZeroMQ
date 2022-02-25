#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include "getTime.h"

int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cout << "usage:" << argv[0] << "ip port index" << std::endl;
        return -1;
    }

    int major, minor, patch;
    zmq_version(&major, &minor, &patch);
    std::cout << "Current 0MQ version is " << major << "," << minor << "," << patch << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;

    char addr[128] = { 0 };
    snprintf(addr, sizeof(addr), "tcp://%s:%s", argv[1], argv[2]);

    void* context = zmq_ctx_new();
    void* worker = zmq_socket(context, ZMQ_REP);
    int rec = zmq_connect(worker, addr);
    if (rec) {
        std::cout << "zmq_connect " << addr << " error:" << rec << std::endl;
        zmq_close(worker);
        zmq_ctx_term(context);
        return -2;
    }

    std::cout<<"worker zmq_connect " << addr <<" done!" << std::endl;

    char buf[128] = { 0 };

    while (1) {
        rec = zmq_recv(worker, buf, sizeof(buf), 0);
        std::cout << getTime() << " recv request(" << buf << ") from client, rec=" << rec << ",request.len =" << strlen(buf) << std::endl;
        snprintf(buf, (int)sizeof(buf), "worker=%s &result=world &time=%ld", argv[3], (int)time(NULL));
        rec = zmq_send(worker, buf, strlen(buf), 0);
        std::cout << getTime() << " send reply(" << buf << ") to client, rec =" << rec << ",reply.len = " << strlen(buf) << std::endl;
    }

    zmq_close(worker);
    zmq_ctx_term(context);

    return 0;
}