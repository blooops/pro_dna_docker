/*	log.cpp
 * 	Author : Rohit Roy
 *
 */

#include "log.h"

Logger::Logger() {
	m_filename = "";
	m_level = LogLevel::MAX;
	m_fileLogFlag = false;
}

Logger::~Logger() {
	;
}

std::string Logger::getLogFileName() const {
	return m_filename;
}

void Logger::setLogFileName(std::string filename) {
	m_filename = filename;
}

void Logger::openStream(std::string filepath) {
	m_filename = filepath;
	if (m_stream.is_open())
		m_stream.close();
	m_stream.open(m_filename.c_str());
	m_filename = filepath;
}

void Logger::closeStream() {
	if (m_stream.is_open())
		m_stream.close();
}

void Logger::write(std::string str) {
	if (m_level == LogLevel::MAX) {
		std::cout << std::endl << "Logging from: " << __FILE__ << std::endl;
		std::cout << "Line Number: " << __LINE__ << std::endl;
		if (m_fileLogFlag) {
			m_stream << std::endl << "Logging from: " << __FILE__ << std::endl;
			m_stream << "Line Number: " << __LINE__ << std::endl;

		}
	}
	std::cout << str << std::endl;

	if (m_fileLogFlag) {
		m_stream << str << std::endl;
	}
}

LogLevel Logger::getLogLevel() const {
	return m_level;
}
void Logger::setLogLevel(LogLevel level) {
	m_level = level;
}

void Logger::setFileLogFlag() {
	m_fileLogFlag = true;
}
