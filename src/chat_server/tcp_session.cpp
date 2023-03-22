#include "precompiled.h"
#include "byte_buffer.h"
#include "tcp_session.h"

TcpSession::TcpSession(tcp::socket sock)
	: sock_(std::move(sock))
{
}

void TcpSession::start()
{
	doReadHeader();
}

void TcpSession::doReadHeader()
{
    auto self(shared_from_this());
    asio::async_read(sock_,
        asio::buffer(read_msg_.data(), ByteBuffer::header_length),
        [this, self](error_code ec, std::size_t length)
        {
            if (!ec && read_msg_.decodeHeader())
            {
                std::cout << "(" << sock_.native_handle() << "): " << "header parsed" << std::endl;

                doReadBody();
            }
            else
            {
                //room_.leave(shared_from_this());
            }
        });
}

void TcpSession::doReadBody()
{
    auto self(shared_from_this());
    asio::async_read(sock_,
        asio::buffer(read_msg_.body(), read_msg_.bodyLength()),
        [this, self](error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "(" << sock_.native_handle() << "): " << "body parsed" << std::endl;
                std::cout << "(" << sock_.native_handle() << "): ";
                std::cout.write(read_msg_.body(), length);
                std::cout << std::endl;
                write_msgs_.push_back(read_msg_);
                doWrite();
                //room_.deliver(read_msg_);
                doReadHeader();
            }
            else
            {
                //room_.leave(shared_from_this());
            }
        });
}

void TcpSession::doWrite()
{
    auto self(shared_from_this());
    asio::async_write(sock_,
        asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
        [this, self](error_code ec, std::size_t length)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    doWrite();
                }
            }
            else
            {
                std::cout << "(" << sock_.native_handle() << "): " << "closed" << std::endl;
                //room_.leave(shared_from_this());
            }
        });
}