#pragma once

#include <vector>

class Parameters
{
public:
	Parameters();
	~Parameters();
	int loadIniCfg(std::string iniFile);

	//add Geters&Setters
	std::string IpAddress_;
	std::string port_;
	std::string rootDoc_;
	/// The number of threads that will call io_service::run().
	std::size_t num_threads_;

	bool logIramPrecEnable_ = true;
	std::string logIramPrecType_ = "TextLog";
	std::string logIramPrecFileName_ = ".IRAM\\ARX_PREC\\H99999.TXT";
	std::string logIramPrecLimit_ = "1000";
	const char * logIramPrecDirFormat_ = "G%Y/M%m/D%d/";
		
	bool logEnable_;
	std::string logFileName_;
	std::string logType_;
	std::string logLimit_;
	//const char * logDirFormat_;
	const char * logDirFormat_="G%Y/M%m/";

	bool logDbEnable1_;
	std::string logDbHost1_;
	std::string logDbName1_;
	std::string logDbType1_;
	std::string logDbUser1_;
	std::string logDbPassword1_;	

	bool logDbEnable2_;
	std::string logDbHost2_;
	std::string logDbName2_;
	std::string logDbType2_;
	std::string logDbUser2_;
	std::string logDbPassword2_;
	std::string hostMirrad_;

private:
};

