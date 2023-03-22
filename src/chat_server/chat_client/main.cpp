#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "byte_buffer.h"

using boost::asio::ip::tcp;
using boost::system::error_code;
namespace asio = boost::asio;

typedef std::deque<ByteBuffer> chat_message_queue;

class chat_client
{
public:
    chat_client(asio::io_context& ioc, const tcp::resolver::results_type& endpoints)
        : ioc_(ioc)
        , sock_(ioc)
    {
        do_connect(endpoints);
    }

    void write(const ByteBuffer& msg)
    {
        asio::post(ioc_,
            [this, msg]()
            {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(msg);
                if (!write_in_progress)
                {
                    do_write();
                }
            });
    }

    void close()
    {
        asio::post(ioc_, [this]() { sock_.close(); });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints)
    {
        asio::async_connect(sock_, endpoints,
            [this](error_code ec, tcp::endpoint ep)
            {
                if (!ec)
                {
                    do_read_header();
                }
            });
    }

    void do_read_header()
    {
        asio::async_read(sock_, asio::buffer(read_msg_.data(), ByteBuffer::header_length),
            [this](error_code ec, std::size_t length)
            {
                if (!ec && read_msg_.decodeHeader())
                {
                    do_read_body();
                }
                else
                {
                    sock_.close();
                }
            });
    }

    void do_read_body()
    {
        asio::async_read(sock_, asio::buffer(read_msg_.body(), read_msg_.bodyLength()),
            [this](error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    std::cout.write(read_msg_.body(), read_msg_.bodyLength());
                    std::cout << "\n";
                    do_read_header();
                }
                else
                {
                    sock_.close();
                }
            });
    }

    void do_write()
    {
        asio::async_write(sock_, asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
            [this](error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty())
                    {
                        do_write();
                    }
                }
                else
                {
                    sock_.close();
                }
            });
    }

    asio::io_context& ioc_;
    tcp::socket sock_;
    ByteBuffer read_msg_;
    message_queue write_msgs_;
};

int main(int argc, char* argv[])
{
    try
    {
        asio::io_context ioc;

        tcp::resolver resolver(ioc);
        auto endpoints = resolver.resolve("127.0.0.1", "3037");

        chat_client c(ioc, endpoints);

        std::thread t([&ioc]() { ioc.run(); });

        char line[ByteBuffer::max_body_length + 1] = { 0, };
        while (std::cin.getline(line, ByteBuffer::max_body_length + 1))
        {
            ByteBuffer msg;
            msg.bodyLength(std::strlen(line));
            std::memcpy(msg.body(), line, msg.bodyLength());
            msg.encodeHeader();
            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}