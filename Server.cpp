#include "stdafx.h"
#include "Server.h"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>


namespace http {
        namespace webserver {

                server::server(Parameters & params) : params_(params),
                        signals_(io_service_),
                        acceptor_(io_service_),
                        new_connection_(),
                        request_handler_(params.rootDoc_)
                {
                        signals_.add(SIGINT);
                        signals_.add(SIGTERM);
#if defined(SIGQUIT)
                        signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
                        signals_.async_wait(boost::bind(&server::handle_stop, this));

                        boost::asio::ip::tcp::resolver resolver(io_service_);
                        boost::asio::ip::tcp::resolver::query query(params.IpAddress_, params.port_);
                        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
                        acceptor_.open(endpoint.protocol());
                        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
                        acceptor_.bind(endpoint);
                        acceptor_.listen();

                        start_accept();
                }

                server::~server()
                {
                }

                void server::run()
                {
                        std::vector<boost::shared_ptr<boost::thread> > threads;
                        for (std::size_t i = 0; i < params_.num_threads_; ++i)
                        {
                                boost::shared_ptr<boost::thread> thread(new boost::thread(
                                        boost::bind(&boost::asio::io_service::run, &io_service_)));
                                threads.push_back(thread);                              
                        }

                        for (std::size_t i = 0; i < threads.size(); ++i)
                                threads[i]->join();
                }

                void server::start_accept()
                {
                        new_connection_.reset(new connection(io_service_, request_handler_, params_));
                        acceptor_.async_accept(new_connection_->socket(), new_connection_->peer,
                                boost::bind(&server::handle_accept, this,
                                boost::asio::placeholders::error));                             
                           
                }

                void server::handle_accept(const boost::system::error_code& e)
                {
                        if (!e)
                        {
                                new_connection_->start();
                        }

                        start_accept();
                }

                void server::handle_stop()
                {
                        io_service_.stop();
                }

        
        }
}
