#pragma once

#ifndef HTTP_WEBSERVER_MIME_TYPES_H
#define HTTP_WEBSERVER_MIME_TYPES_H

#include <string>

namespace http {
        namespace webserver {
                namespace mime_types {
                                    
                        std::string extension_to_type(const std::string& extension);

                } 
        }
} 
#endif
