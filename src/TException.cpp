// =====================================================================================
// 
//       Filename:  TException.cpp
// 
//    Description:  Exception class and call stack unwinder
// 
//        Version:  1.0
//        Created:  01/17/2014 11:18:10 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  David P. Riedel (dpr), driedel@cox.net
//        License:  GNU General Public License v3
//        Company:  
// 
// =====================================================================================

	/* This file is part of CollectEDGARData. */

	/* CollectEDGARData is free software: you can redistribute it and/or modify */
	/* it under the terms of the GNU General Public License as published by */
	/* the Free Software Foundation, either version 3 of the License, or */
	/* (at your option) any later version. */

	/* CollectEDGARData is distributed in the hope that it will be useful, */
	/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
	/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
	/* GNU General Public License for more details. */

	/* You should have received a copy of the GNU General Public License */
	/* along with CollectEDGARData.  If not, see <http://www.gnu.org/licenses/>. */



#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>


#include <execinfo.h>

//	use the following define to avoid a conflict between glibc and libiberty for extern basename() declaration
#define HAVE_DECL_BASENAME 1
#include <demangle.h>

#include "TException.h"



//	******************************
//
//	Error_Msg
//
//	******************************




Error_Msg::Error_Msg(	const std::string& a,
						const std::string& b,
						const std::string& c,
						size_t		len,
						char		fill)
						
	:	msg_(a), len_(len), fill_(fill)
	
{
	if (!b.empty())	msg_ += b;
	if (!c.empty())	msg_ += c;
}

// ------------------------------------------------------------------------------------------------------------------------

std::ostream& operator << (std::ostream& output, const Error_Msg& msg)

{
	std::ios_base::iostate save_state = output.rdstate();
	output.fill(msg.fill_);
	output << std::setw(msg.len_) << std::left << msg.msg_;
	output.setstate(save_state);
	return output;
}

// ------------------------------------------------------------------------------------------------------------------------

//	******************************
//
//	Exception_Msg
//
//	******************************




Exception_Msg::Exception_Msg(	const std::string& a,
								const std::string& b,
								const std::string& c)
						
	
{
	if (!a.empty())	msg_ += a;
	if (!b.empty())	msg_ += b;
	if (!c.empty())	msg_ += c;
}

// ------------------------------------------------------------------------------------------------------------------------


//	******************************
//
//	ExceptionTracer
//
//	******************************

ExceptionTracer::ExceptionTracer(void)
{
}

// ------------------------------------------------------------------------------------------------------------------------


ExceptionTracer::~ExceptionTracer(void)
{
	void *trace[e_stackSize];
	char **messages	=	(char **)NULL;
	std::string strFunctionName;
	std::string strLibraryName;
	std::string strEntireLine;

	int trace_size	=	backtrace(trace, e_stackSize);
	messages		=	backtrace_symbols(trace, trace_size);

	std::cerr << "[bt] Execution path:" << std::endl;

	for (int i = 0;	i < trace_size;	++i)
	{
		// test the demangler
		strEntireLine	=	messages[i];

		if (strEntireLine.find('(') == std::string::npos || strEntireLine.find(')') ==  std::string::npos) 
		{
		  strFunctionName	=	strEntireLine;
		  strLibraryName	=	"";
		}
		else
		{
			strLibraryName	=	strEntireLine.substr(0, strEntireLine.find('('));
			strFunctionName	=	strEntireLine.substr(strEntireLine.find('(') + 1, strEntireLine.find(')'));

			if (strFunctionName.find('+') != std::string::npos)
			{
				strFunctionName	=	strFunctionName.substr(0, strFunctionName.find('+'));
			}
		}

		//std::cout << cplus_demangle (strFunctionName.c_str() , DMGL_VERBOSE) << std::endl; // the test
		std::cerr << "[bt] " << cplus_demangle (strFunctionName.c_str() , DMGL_VERBOSE) << std::endl;
		
		//std::cerr << "[bt] " << messages[i] << std::endl;
	}

	free(messages);
}

// ------------------------------------------------------------------------------------------------------------------------


