// VoaWebServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "Server.h"
#include "Parameters.h"

#include "../Logging/LogFactory.h"
#include "../Logging/Log.h"
#include "Message.h"

#if defined(_WIN32)

#define strDevType "WIN32"

std::string ConfigFile = "VoaWebServer.ini";
//const std::string ver = "1.0.0-2015.04.21";
//const std::string ver = "1.0.0 - 01.10.2015";
const std::string ver = "1.0.0 - 31.10.2016";
//22.10.2015 add functional for IRAM(ARX_PREC)

using namespace std;

int main(int argc, char* argv[])
{       
        Parameters parameters;

        try
        {
                if (!parameters.loadIniCfg(ConfigFile))
                {
                        std::cout << "Program Type: " << strDevType << std::endl;
                        std::cout << "Version: " << ver << std::endl;

                        http::webserver::server s(parameters);                    
                        s.run();
                }

                else
                {
                        std::cout <<"Load default configuration." <<std::endl;
                }
        }
        catch (std::exception& e)
        {
                std::cerr << "Main catch exception: " << e.what() << std::endl;
        }

        return 0;
}

#endif // defined(_WIN32)