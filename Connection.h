#pragma once

#ifndef HTTP_WEBSERVER_CONNECTION_H
#define HTTP_WEBSERVER_CONNECTION_H

#include <boost/scoped_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.h"
#include "request.h"
#include "RequestHandler.h"
#include "RequestParser.h"
#include "Parameters.h"

#include <soci.h>
#include <soci-postgresql.h>

namespace Logbook
{
        class CLog;
}
namespace http {
        namespace webserver {

                class base_connection
                {
                public:
                        explicit base_connection(boost::asio::io_service& io_service,
                                request_handler& handler, Parameters & params) :
                                strand_(io_service),
                                request_handler_(handler),
                                params_(params)
                        {
                        }                       
                        
                        ~base_connection(void);
                        
                        boost::asio::ip::tcp::endpoint peer;
                        
                protected:
                        void logRequest();
                        void logMessage();

                        void AnalizeMessage(std::string message);

                        boost::asio::io_service::strand strand_;

                        request_handler& request_handler_;

                        boost::array<char, 8192> buffer_;

                        request request_;

                        reply reply_;

                        request_parser request_parser_;

                        bool keepalive_;

                        void reset()
                        {
                                request_.clear();
                                reply_.clear();
                                request_parser_.reset();
                        }
                protected:
                        int AddLogWithSynT(std::string strVal);
                        int AddLogWithSynDT(std::string strVal);
                        int AddLogIramPrec(std::string strVal);
                        int EnableLogging();
                        void DisableLoggging();
                        void logDBMessage(std::string logDbHost, std::string logDbName, std::string logDbUser, std::string logDbPassword, std::string table,
                                std::string column_datetime, std::string column_message, std::string datetime, std::string message);

                        void logDBMessage1(soci::session sql, std::string table, std::string column_datetime, std::string column_message, std::string datetime, std::string message);
                        void logMirradMessage(std::string message);


                        Parameters & params_;
                
                        int EnableLog(std::string filename, std::string filetype, const char * format, std::string limit = "");
                        int EnableIramPrecLog(std::string filename, std::string filetype, const char * format, std::string limit = "");
                        void DisableLog();
                        void DisableIramPrecLog();

                        boost::shared_ptr<Logbook::CLog> log_;
                        boost::shared_ptr<Logbook::CLog> logIramPrec_;
                        
                };
                                
                class connection
                        : public boost::enable_shared_from_this<connection>,
                          public base_connection,
                          private boost::noncopyable
                {
                public:
                        explicit connection(boost::asio::io_service& io_service,
                                request_handler& handler, Parameters & params) :
                                base_connection(io_service, handler, params),
                                socket_(io_service)
                        {
                        }
                        
                        ~connection(void);
                        
                        
                        boost::asio::ip::tcp::socket& socket();

                        void start();

                private:
                        void read_more();

                        void handle_read(const boost::system::error_code& e,
                                std::size_t bytes_transferred);

                        void handle_write(const boost::system::error_code& e);

                        boost::asio::ip::tcp::socket socket_;

                
                };

                typedef boost::shared_ptr<connection> connection_ptr;

        }
} 

#endif // HTTP_WEBSERVER_CONNECTION_H
