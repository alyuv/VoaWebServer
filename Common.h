#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>


/*std::string FormatTime(std::string fmt, boost::posix_time::ptime now, std::string function)
{
	static std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet(fmt.c_str()));
	std::stringstream ss;
	ss.imbue(loc);
	ss << now;
	std::cout << ss.str() << function << " \n";
	return ss.str();
}*/

