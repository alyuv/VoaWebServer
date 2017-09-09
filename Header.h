#pragma once

#ifndef HTTP_WEBSERVER_HEADER_H
#define HTTP_WEBSERVER_HEADER_H

#include <string>

namespace http {
        namespace webserver {

                struct header
                {
                        std::string name;
                        std::string value;
                        header() { }

                        header(std::string name_, std::string value_) {
                                name = name_;
                                value = value_;
                        }

                };
        }
}

#endif