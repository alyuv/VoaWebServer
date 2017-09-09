#pragma once

#include <iostream>
#include <vector>

class Message
{
public:
	Message(std::string msg);
	~Message();
	//void Analise();

	std::string dt_receive;     //date time recive ten-packtime
	std::string idvoa;             // id voa
	std::string numvoa;
	std::string index_stattion;			   //index statation
	std::string name_station;          // name statation
	
	std::string t;        // temerature voa
	std::string power;          //power	
	std::string status;         //status voa
	std::string date_begin;       //date beginning of the formation of a ten-packtime
	std::string time_begin;       //time beginning of the formation of a ten-packtime
	std::vector<std::string> rain[10];        //rain sum
	double rainsum;        //rain sum
	std::string strrainsum;

protected:

};

