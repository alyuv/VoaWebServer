#pragma once

#include <iostream>
#include <vector>

class Message
{
public:
        Message(std::string msg);
        ~Message();


        std::string dt_receive;     
        std::string idvoa;          
        std::string numvoa;
        std::string index_stattion; 
        std::string name_station;   
        
        std::string t;     
        std::string power;       
        std::string status; 
        std::string date_begin;
        std::string time_begin;
        std::vector<std::string> rain[10];  
        double rainsum;
        std::string strrainsum;

protected:

};

