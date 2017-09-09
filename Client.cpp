
#include "stdafx.h"
#include "Client.h"
#include <iostream>

client::client(boost::asio::io_service& io_service,
        const std::string& server, std::string message)
        : resolver_(io_service),
        socket_(io_service)
{
        std::ostream request_stream(&request_); 
        request_stream << "GET " << "/write_cnt.php?"<<message<< " HTTP/1.1\r\n";
        request_stream << "Host: " << server << "\r\n";
        request_stream << "Accept: */*\r\n\r\n";
        request_stream << "Connection: keep-alive\r\n";
        
        tcp::resolver::query query(server, "http");
        resolver_.async_resolve(query,
                boost::bind(&client::handle_resolve, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::iterator));
}

void client::handle_resolve(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator)
{
        if (!err)
        {
                boost::asio::async_connect(socket_, endpoint_iterator,
                        boost::bind(&client::handle_connect, this,
                        boost::asio::placeholders::error));
        }
        else
        {
                std::cout << "Error: " << err.message() << "\n";
        }
}

void client::handle_connect(const boost::system::error_code& err)
{
        if (!err)
        {
                boost::asio::async_write(socket_, request_,
                        boost::bind(&client::handle_write_request, this,
                        boost::asio::placeholders::error));
        }
        else
        {
                std::cout << "Error: " << err.message() << "\n";
        }
}

void client::handle_write_request(const boost::system::error_code& err)
{
        if (!err)
        {
                boost::asio::async_read_until(socket_, response_, "\r\n",
                        boost::bind(&client::handle_read_status_line, this,
                        boost::asio::placeholders::error));
        }
        else
        {
                std::cout << "Error: " << err.message() << "\n";
        }
}

void client::handle_read_status_line(const boost::system::error_code& err)
{
        if (!err)
        {
                std::istream response_stream(&response_);
                std::string http_version;
                response_stream >> http_version;
                unsigned int status_code;
                response_stream >> status_code;
                std::string status_message;
                std::getline(response_stream, status_message);
                if (!response_stream || http_version.substr(0, 5) != "HTTP/")
                {
                        std::cout << "Invalid response\n";
                        return;
                }
                if (status_code != 200)
                {
                        std::cout << "Response returned with status code ";
                        std::cout << status_code << "\n";
                        return;
                }

                std::ostringstream ss;
                boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                        boost::bind(&client::handle_read_headers, this,
                        boost::asio::placeholders::error));
        }
        else
        {
                std::cout << "Error: " << err << "\n";
        }
}

void client::handle_read_headers(const boost::system::error_code& err)
{
        if (!err)
        {
                std::istream response_stream(&response_);
                std::string header;

                // Start reading remaining data until EOF.
                boost::asio::async_read(socket_, response_,
                        boost::asio::transfer_at_least(1),
                        boost::bind(&client::handle_read_content, this,
                        boost::asio::placeholders::error));
        }
        else
        {
                std::cout << "Error: " << err << "\n";
        }
}

void client::handle_read_content(const boost::system::error_code& err)
{
        if (!err)
        {
                boost::asio::async_read(socket_, response_,
                        boost::asio::transfer_at_least(1),
                        boost::bind(&client::handle_read_content, this,
                        boost::asio::placeholders::error));
        }
        else if (err != boost::asio::error::eof)
        {
                std::cout << "Error: " << err << "\n";
        }
}