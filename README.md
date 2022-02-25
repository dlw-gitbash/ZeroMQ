ZeroMQ
Router/Dealer模型

    ROUTER其实指的是ZeroMQ里面的一种套接字类型（ZMQ_ROUTER类型）。这个套接字把接收到的请求（ZMQ_REQ）公平的排队分发，而且ROUTER还会标志客户的身份，从而确保能够将应答数据给到客户端。这样就刚好可以解决了我们想要的“监听和接受客户端请求，并丢入任务管理器”。[这里的任务管理器指的是DEALER]
    DEALER其实指的是ZeroMQ里面的一种套接字类型（ZMQ_DEALER）。这个套接字会把“任务”负载均衡的分发到后端的工作线程（或进程）Worker的。并且，当DEALER接受到Worker的处理结果的时候，DEALER还会把处理结果数据传递给ROUTER，由ROUTER将应答回给客户端。这样一来，DEALER也解决了我们想要的“任务管理调度和Worker分配工作的问题”。
