#pragma once

#include <deque>
#include "byte_buffer.h"

class TcpSession
    : public std::enable_shared_from_this<TcpSession>
{
public:
    TcpSession(tcp::socket sock);

    void start();

private:
    void doReadHeader();
    void doReadBody();
    void doWrite();

private:
    tcp::socket sock_;
    ByteBuffer read_msg_;    
    message_queue write_msgs_;
};