/*	log.h
 * 	Author : Rohit Roy
 *
 */

#pragma once

#include <string>
#include "structure.h"
#include <fstream>
#include <iostream>


enum class LogLevel {
	MAX = 0, MIN = 1
};

class Logger {
private:
	std::string m_filename;
	LogLevel m_level;
	std::ofstream m_stream;
	bool m_fileLogFlag;

public:
	Logger();
	~Logger();
	std::string getLogFileName() const;
	void setLogFileName(std::string);
	void openStream(std::string);
	void closeStream();
	void write(std::string);
	LogLevel getLogLevel() const;
	void setLogLevel(LogLevel level);
	void setFileLogFlag();
};
