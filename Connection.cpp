#include "stdafx.h"
#include "connection.h"
#include <vector>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "RequestHandler.h"
#include "connection.h"
#include "Common.h"
#include "../Logging/LogFactory.h"
#include "../Logging/Log.h"
#include "Message.h"
#include "Client.h"

//#pragma warning(disable : 4996);

namespace http {
        namespace webserver {     
                
                base_connection::~base_connection()
                {
                }

                std::string formatTime(std::string fmt, boost::posix_time::ptime now)
                {                       
                        std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet(fmt.c_str()));
                        std::stringstream ss;
                        ss.imbue(loc);
                        ss << now;                      
                        return ss.str();                        
                }
                
                void base_connection::logRequest()
                {       
                        using namespace boost::posix_time;
                        using namespace boost::gregorian;
                        std::string addrstr = peer.address().to_string();
                        std::string timestr = formatTime("%Y-%m-%d %H:%M:%S.%f", request_.tstamp);                      

                        std::string fstring = (boost::format("%s  - \"%s %s HTTP/%d.%d\" %d %lu")
                                % addrstr
                                % request_.method.c_str()
                                % request_.uri.c_str()
                                % request_.http_version_major
                                % request_.http_version_minor
                                % reply_.status
                                % reply_.content.size()).str();

                        AddLogWithSynDT(fstring);                       
                        
                        if (request_.msg != "") 
                        {
                                std::string msg = request_.msg;                 
                                
                                boost::replace_all(request_.msg, "%20", " ");                                   
                                
                                AddLogWithSynDT(request_.msg);  
                                                        
                                Message mssg(request_.msg);
                                std::string str = mssg.index_stattion + " " + mssg.idvoa + " " + mssg.time_begin + " " + mssg.strrainsum;
                                AddLogIramPrec(str);                            
                                logDBMessage(params_.logDbHost1_, params_.logDbName1_, params_.logDbUser1_, params_.logDbPassword1_, "messages", "datetime", "message", timestr, request_.msg);                         
                                logDBMessage(params_.logDbHost2_, params_.logDbName2_, params_.logDbUser2_, params_.logDbPassword2_, "messages", "datetime", "message", timestr, request_.msg);                         
                                logMirradMessage(msg);
                        }

                }

                void base_connection::logMessage()
                {
                        using namespace boost::posix_time;
                        using namespace boost::gregorian;               
                        std::string timestr = formatTime("%Y-%m-%d %H:%M:%S.%f", request_.tstamp);
                        printf("[%s] [client %s]", timestr.c_str(), request_.msg.c_str());
                }

                void base_connection::logDBMessage(std::string logDbHost, std::string logDbName, std::string logDbUser, std::string logDbPassword, 
                        std::string table, std::string column_datetime, std::string column_message,     std::string datetime, std::string message)
                {
                        try
                        {
                                soci::session sql(*soci::factory_postgresql(), "dbname=" + logDbName + " user=" + logDbUser + " password=" + logDbPassword + " host=" + logDbHost);
                                sql << "insert into " << table << "( " << column_datetime<<","<<column_message << ") values(:datetime, :message)", soci::use(datetime), soci::use(message);
                        }
                        catch (soci::postgresql_soci_error const & e)
                        {
                                std::cout << "PostgreSQL error: " << e.sqlstate()
                                        << " " << e.what() << std::endl;
                        }
                        catch (std::exception const & e)
                        {
                                std::cout << "Some other error: " << e.what() << std::endl;
                        }
                }

                void base_connection::logDBMessage1(soci::session sql, std::string table, std::string column_datetime, std::string column_message, std::string datetime, std::string message)
                {
                        
                }

                void base_connection::logMirradMessage(std::string message)
                {
                        try
                        {                               
                                boost::asio::io_service io_service;
                                client c(io_service, "www.mirrad.com.ua", message);
                                io_service.run();       
                                //std::cout << message <<"\n";
                                std::string tmpStr = "Message:" + message + " send to " + params_.hostMirrad_;
                                AddLogWithSynDT(tmpStr);                                
                        }
                        catch (std::exception& e)
                        {
                                std::string tmpStr = "Exception send message to " + params_.hostMirrad_ + ". " + e.what();                              
                                AddLogWithSynDT(tmpStr);
                        }
                }

                int base_connection::AddLogWithSynT(std::string strVal)
                {
                        if (log_)
                        {
                                return log_->AddRecordWithSynT(strVal);
                        }

                        return -1;
                }

                int base_connection::AddLogWithSynDT(std::string strVal)
                {
                        
                        if (log_)
                        {                               
                                return log_->AddRecordWithSynDT(strVal);
                        }

                        return -1;
                }

                int base_connection::AddLogIramPrec(std::string strVal)
                {

                        if (logIramPrec_)
                        {
                                return logIramPrec_->AddRecordIramPrec(strVal);
                        }

                        return -1;
                }
                                
                int base_connection::EnableLogging()
                {
                        if (params_.logEnable_)
                        {
                                EnableLog(params_.logFileName_, params_.logType_, params_.logDirFormat_, params_.logLimit_);
                        }
                        if (params_.logIramPrecEnable_)
                        {
                                EnableIramPrecLog(params_.logIramPrecFileName_, params_.logIramPrecType_, params_.logIramPrecDirFormat_, params_.logIramPrecLimit_);
                        }
                        return 0;
                }
                                
                void base_connection::DisableLoggging()
                {
                        DisableLog();
                        DisableIramPrecLog();
                }

                int base_connection::EnableLog(std::string filename, std::string filetype, const char * format, std::string limit)
                {
                        log_.reset(Logbook::CLogFactory::CreateLog(filename, filetype, format, limit));
                        return 0;
                }

                int base_connection::EnableIramPrecLog(std::string filename, std::string filetype, const char * format, std::string limit)
                {
                        logIramPrec_.reset(Logbook::CLogFactory::CreateLog(filename, filetype, format, limit));
                        return 0;
                }

                void base_connection::DisableLog()
                {
                        log_.reset();
                }

                void base_connection::DisableIramPrecLog()
                {
                        logIramPrec_.reset();
                }

                connection::~connection()
                {

                }

                boost::asio::ip::tcp::socket& connection::socket()
                {
                        return socket_;
                }

                void connection::start()
                {
                        EnableLogging();
                        read_more();
                }

                void connection::read_more()
                {
                        socket_.async_read_some(boost::asio::buffer(buffer_),
                                strand_.wrap(
                                boost::bind(&connection::handle_read, shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)));
                }

                void connection::handle_read(const boost::system::error_code& e,
                        std::size_t bytes_transferred)
                {
                        if (e) {
                                return;
                        }

                        boost::tribool result;
                        boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
                                request_, buffer_.data(), buffer_.data() + bytes_transferred);
                        if (result) {
                                keepalive_ = request_.want_keepalive();
                                request_.tstamp = boost::posix_time::microsec_clock::universal_time();
                                request_handler_.handle_request(request_, reply_, keepalive_);
                                boost::asio::async_write(socket_, reply_.to_buffers(),
                                        strand_.wrap(
                                        boost::bind(&connection::handle_write, shared_from_this(),
                                        boost::asio::placeholders::error)));

                                logRequest();                   
                        }
                        else if (!result) {                             
                                reply_ = reply::stock_reply(reply::bad_request);
                                boost::asio::async_write(socket_, reply_.to_buffers(),
                                        strand_.wrap(
                                        boost::bind(&connection::handle_write, shared_from_this(),
                                        boost::asio::placeholders::error)));
                        }
                        else {
                                read_more();
                        }
                }

                void connection::handle_write(const boost::system::error_code& e)
                {
                        if (e) {
                                return;
                        }

                        if (keepalive_) {
                                reset();
                                read_more();
                        }
                        else {
                                boost::system::error_code ignored_ec;
                                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                        }
                }
                
                
        }
}
