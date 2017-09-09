#include "stdafx.h"
#include "Message.h"
#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>


Message::Message(std::string msg)
{
        std::size_t index = 0;
        std::string errmsg = "ALARM!Amount of precipitation in the last 60 minutes exceeded 300 mm";            
        index = msg.find(errmsg, index);
        if (index != std::string::npos) msg.replace(index, 69 , "-01 -01 -01 -01 -01 -01 -01 -01 -01 -01");                                                                                                                     

        std::vector<std::string> splitVec;
        boost::array<char, 1>separator = {' '};
        boost::algorithm::split(splitVec, msg, boost::algorithm::is_any_of(separator)); 
        std::size_t count = 0;

        idvoa = splitVec[0].substr(5, 3);

        switch (atoi(idvoa.c_str()))
        {
        case 1:
                numvoa = "016";
                index_stattion = "33350";
                name_station = "Gostomel";
                break;

        case 2:
                numvoa = "019";
                index_stattion = "33354";
                name_station = "Baryshivka";
                break;
        case 3:
                numvoa = "025";
                index_stattion = "33464";
                name_station = "Bila Tserkva";
                break;
        case 4:
                numvoa = "027";
                index_stattion = "33236";
                name_station = "Oster";
                break;
        }
        
        t = splitVec[1];
        power = splitVec[2];
        status = splitVec[3];
        date_begin = splitVec[4];
        time_begin = splitVec[5];

        for (std::size_t i = 6; i < splitVec.size(); i++)
        {
                if (splitVec[i] != "-01")
                {
                        rainsum = rainsum + atoi(splitVec[i].c_str());                  
                }               
                else count++;   

        }
        
       
        if (count == 10)
        {
                strrainsum = "99999.0";
        }
        else
        {
                rainsum = rainsum * 0.1;
                strrainsum = std::to_string(rainsum);
                char buf[64];
                sprintf(buf, "%1.1f", rainsum);
                strrainsum.assign(buf);
        }

}


Message::~Message()
{
}
