#pragma once

#ifndef HTTP_WEBSERVER_SERVER_H
#define HTTP_WEBSERVER_SERVER_H

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.h"
#include "RequestHandler.h"
#include "Parameters.h"

namespace http {

        namespace webserver {

                class server
                        : private boost::noncopyable
                {
                public:
                        explicit server(Parameters & params);
                        ~server(void);
                        void run();

                private:
                        void start_accept();

                        void handle_accept(const boost::system::error_code& e);

                        void handle_stop();

                        Parameters & params_;
        
                        boost::asio::io_service io_service_;

                        boost::asio::signal_set signals_;

                        boost::asio::ip::tcp::acceptor acceptor_;

                        connection_ptr new_connection_;

                        request_handler request_handler_;
                
        

                };
                


        }
}

#endif
