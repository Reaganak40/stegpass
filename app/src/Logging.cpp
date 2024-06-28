#include "pch.h"
#include "Logging.hpp"

static std::shared_ptr<spdlog::logger> s_Logger = nullptr;

void sp::Log::Init()
{
	if (s_Logger != nullptr) {
		return;
	}

	spdlog::set_pattern("%^[%T]: %v%$");
	s_Logger = spdlog::stdout_color_mt("StegPass");
	s_Logger->set_level(spdlog::level::trace);
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
