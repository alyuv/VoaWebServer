#pragma once

#ifndef HTTP_WEBSERVER_REQUEST_HANDLER_H
#define HTTP_WEBSERVER_REQUEST_HANDLER_H

#include <string>
#include <boost/noncopyable.hpp>

namespace http {
        namespace webserver {

                struct reply;
                struct request;

                class request_handler
                        : private boost::noncopyable
                {
                public:
                        explicit request_handler(const std::string& doc_root);

                        void handle_request(const request& req, reply& rep, bool keepalive);

                private:
                        std::string doc_root_;

                        static bool url_decode(const std::string& in, std::string& out);
                };

        }
}

#endif
