#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <iostream>

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
    void* request = zmq_socket(context, ZMQ_REQ);
    int rec = zmq_connect(request, addr);
    if (rec) {
        std::cout << "zmq_connect " << addr << "error:" << rec << std::endl;
        zmq_close(request);
        zmq_ctx_term(context);
        return -2;
    }

    std::cout << "client zmq_connect" << addr << " done!" << std::endl;

    char buf[128] = { 0 };

    while (1) {
        snprintf(buf, (int)sizeof(buf), "index=%s&cmd=hello&time=%ld", argv[3], (int)time(NULL)); 
        rec = zmq_send(request, buf, strlen(buf), 0);
        std::cout << time(NULL) << "recv request(" << buf << ") from client, rec=" << rec << ",request.len =" << strlen(buf) << std::endl;
        rec = zmq_recv(request, buf, sizeof(buf), 0);
        std::cout << time(NULL) << " recv reply(" << buf << ") from server, rec =" << rec << ",reply.len = " << strlen(buf) << std::endl;
        Sleep(3);
    }

    zmq_close(request);
    zmq_ctx_term(context);

    return 0;
}