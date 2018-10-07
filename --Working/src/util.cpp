#include "main.h"
#include <cmath>
#include <utility>
#include "definitions.hpp"
#include "util.hpp"

namespace util {
	double min(double a, double b) {
		return ((a < b) ? a : b);
	}

	double max(double a, double b) {
		return ((a > b) ? a : b);
	}

	double degrees_to_radians(double degrees) {
		return degrees * PI / 180.0;
	}

	double tankScaleJoystick(double amt) {
		return amt / std::cos(degrees_to_radians(45));
	}

	bool within127(int a) {
		return limit127(a) == a;
	}

	int limit127(int a) {
		return util::max(util::min(a, 127), -127);
	}

	int step0(int a) {
		if (a > 0)
			a--;
		else if (a < 0)
			a++;
		return a;
	}

	std::string timestamp() {
	  time_t now;
	  struct tm* timeinfo;

	  time (&now);
	  timeinfo = localtime (&now);

	  char ts [20];
	  strftime (ts, 20, "%F %T",timeinfo);
	  new std::string(ts);
	}

	std::pair<std::string, std::string> separateFirst(std::string s, std::string regex) {
		if (s.find(regex) == std::string::npos)
			return std::pair(s, std::string());
		std::string first = s.substr(0, s.find(regex));
		s.erase(0, s.find(regex) + 1);
		return std::pair(first, s);
	}

	std::string getLoggingLevelName(logging_levels level) {
		std::string levelmsg = "";
	  switch (level) {
	    case LOG_SEVERE:
	      levelmsg = "SEVERE";
	      break;
	    case LOG_ERROR:
	      levelmsg = "ERROR";
	      break;
	    case LOG_WARNING:
	      levelmsg = "WARNING";
	      break;
	    case LOG_INFO:
	      levelmsg = "INFO";
	      break;
	    default:
	      levelmsg = "DEBUG";
	      break;
	  }
		return levelmsg;
	}
}
