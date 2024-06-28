#include "pch.h"
#include "Logging.hpp"
#include <spdlog/sinks/basic_file_sink.h>

static std::shared_ptr<spdlog::logger> s_Logger = nullptr;

// error handler on critical logs
void CriticalErrorHandler(const std::string& msg)
{
	throw std::runtime_error(msg);
}

void sp::Log::Init()
{
	if (s_Logger != nullptr) {
		return;
	}

	spdlog::set_pattern("%^[%T]: %v%$");
	s_Logger = spdlog::stdout_color_mt("Stegpass");
	s_Logger->set_level(spdlog::level::trace);

	// set error handling
	s_Logger->set_error_handler(CriticalErrorHandler);

}

void sp::Log::Destroy()
{
	if (s_Logger == nullptr) {
		return;
	}

	s_Logger->flush();
	s_Logger = nullptr;
}

spdlog::logger& sp::Log::GetLogger()
{
	if (s_Logger == nullptr) {
		throw std::runtime_error("Logger not initialized.");
	}

	return *s_Logger;
}
