/***************************************************************/
/*  app/Window.hpp                                             */
/* *************************************************************/
/*                 This file is a part of:                     */
/*             -- StegPass: Password Manager --                */
/*         https://github.com/Reaganak40/stegpass              */
/***************************************************************/
/*            Copyright(c) 2024 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/***************************************************************
* Headers
***************************************************************/
#include "Config.hpp"

#ifndef SP_DISABLE_LOGGING
// spdlog is a header-only library that relies on templated
// variadic functions, making it TERRIBLY incapatible with
// traditional implementation hiding patterns. So, the library
// must be available by the module or client to use it.
//
// if library not found, logging is disabled.
#include <CodeAnalysis/Warnings.h>
#pragma warning( push )
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

namespace sp {

	class Log {
	public:
		// Initialize the logger internals
		static void Init();

		// Destroys all logger internals
		static void Destroy();
		
		// Get the logger instance
		static spdlog::logger& GetLogger();
	};
}

#endif // !SP_DISABLE_LOGGING

/***************************************************************
* Enable logger if in debug mode
***************************************************************/
#ifdef SP_DEBUG
	#define SP_LOG_TRACE(...)    ::sp::Log::GetLogger().trace(__VA_ARGS__)
	#define SP_LOG_INFO(...)     ::sp::Log::GetLogger().info(__VA_ARGS__)
	#define SP_LOG_WARN(...)     ::sp::Log::GetLogger().warn(__VA_ARGS__)
	#define SP_LOG_ERROR(...)    ::sp::Log::GetLogger().error(__VA_ARGS__)
	#define SP_LOG_CRITICAL(...) ::sp::Log::GetLogger().critical(__VA_ARGS__)
	
	#define SP_INIT_LOG() ::sp::Log::Init()
	#define SP_DESTROY_LOG() ::sp::Log::Destroy()
#else
	#define SP_LOG_TRACE(...)
	#define SP_LOG_INFO(...)
	#define SP_LOG_WARN(...)
	#define SP_LOG_ERROR(...)
	#define SP_LOG_CRITICAL(...)

	#define SP_INIT_LOG()
	#define SP_DESTROY_LOG()
#endif