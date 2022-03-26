#include "ChiliException.h"
#include <sstream>

ChiliException::ChiliException(int line, const char* file) noexcept
	:m_line(line),
	m_file(file)
{

}

const char* ChiliException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* ChiliException::GetType() const noexcept
{
	return "Chili Exception";
}

int ChiliException::GetLine() const noexcept
{
	return m_line;
}

const std::string& ChiliException::GetFile() const noexcept
{
	return m_file;
}

std::string ChiliException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_file << std::endl
		<< "[Line] " << m_line;
	return oss.str();
}
