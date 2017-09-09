#include "stdafx.h"
#include "Parameters.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace boost::property_tree;


Parameters::Parameters()
{
        IpAddress_="127.0.0.1";
        port_="80";
        rootDoc_=".";
        num_threads_ = 1;

        logEnable_ = false;
        logFileName_ = "";
        logType_ = "";
        logLimit_ = "";

        logDbEnable1_ = false;
        logDbName1_ = "";
        logDbUser1_ = "";
        logDbPassword1_ = "";

        logDbEnable2_ = false;
        logDbName2_ = "";
        logDbUser2_ = "";
        logDbPassword2_ = "";

                hostMirrad_ = "localhost";

}

int Parameters::loadIniCfg(std::string iniFile)
{
        try
        {
                ptree pt;
                ini_parser::read_ini(iniFile, pt);
                IpAddress_ = pt.get<std::string>("Server.IpAddress");
                port_ = pt.get<std::string>("Server.Port");
                rootDoc_ = pt.get<std::string>("Server.RootDoc");
                num_threads_ = boost::lexical_cast<std::size_t>(pt.get<std::size_t>("Server.NumThreads"));

                logEnable_ = pt.get<bool>("LoggingText.Enable");
                logFileName_ = pt.get<std::string>("LoggingText.FileName");
                logType_ = pt.get<std::string>("LoggingText.Type");
                logLimit_ = pt.get<std::string>("LoggingText.Limit");
                                
                std::string tmpstr = pt.get<std::string>("LoggingText.Format");
                                

                logDbEnable1_ = pt.get<bool>("LoggingDB1.Enable");
                if (logDbEnable1_)
                {
                        logDbHost1_ = pt.get<std::string>("LoggingDB1.Host");
                        logDbName1_ = pt.get<std::string>("LoggingDB1.Name");
                        logDbType1_ = pt.get<std::string>("LoggingDB1.Type");
                        logDbUser1_ = pt.get<std::string>("LoggingDB1.User");
                        logDbPassword1_ = pt.get<std::string>("LoggingDB1.Password");
                }

                logDbEnable2_ = pt.get<bool>("LoggingDB2.Enable");
                if (logDbEnable2_)
                {
                        logDbHost2_ = pt.get<std::string>("LoggingDB2.Host");
                        logDbName2_ = pt.get<std::string>("LoggingDB2.Name");
                        logDbType2_ = pt.get<std::string>("LoggingDB2.Type");
                        logDbUser2_ = pt.get<std::string>("LoggingDB2.User");
                        logDbPassword2_ = pt.get<std::string>("LoggingDB2.Password");
                }

                                hostMirrad_ = pt.get<std::string>("HTTPSend.HostMirrad");

        }

        catch (std::exception& e)
        {
                std::cout << "Load ini config file " << iniFile << " failed. " << e.what() << ". Load default configuration. " << std::endl;
        }
        return 0;
}

Parameters::~Parameters()
{
}
