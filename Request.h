#pragma once

#ifndef HTTP_WEBSERVER_REQUEST_H
#define HTTP_WEBSERVER_REQUEST_H

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "header.h"

namespace http {
        namespace webserver {
                                
                struct request
                {
                        std::string     method;
                        std::string     uri;
                        std::string msg;
                        int     http_version_major;
                        int     http_version_minor;
                        std::vector<header>     headers;
                        std::string             content;
                        boost::posix_time::ptime tstamp;
                        
                        void clear() {
                                method.clear();
                                uri.clear();
                                http_version_major = 0;
                                http_version_minor = 0;
                                headers.clear();
                                content.clear();
                        }

                        std::string get_header(std::string name) {
                                std::vector<header>::iterator hi;
                                for (hi = headers.begin(); hi != headers.end(); hi++) {
                                        if ((*hi).name == name)
                                                return (*hi).value;
                                }

                                return "";
                        }

                        bool is_http11() const {
                                if ((http_version_major > 1) ||
                                        ((http_version_major == 1) && (http_version_minor > 0)))
                                        return true;

                                return false;
                        }

                        bool want_keepalive() {
                                bool rc = is_http11();

                                std::string cxn_hdr =
                                        boost::to_lower_copy(get_header("connection"));

                                if (cxn_hdr == "close")
                                        rc = false;
                                else if (cxn_hdr == "keep-alive")
                                        rc = true;

                                return rc;
                        }
                };

        }
}

#endif