// =====================================================================================
// 
//       Filename:  TException.h
// 
//    Description:  exception classes and "macros"
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



#ifndef _TEXCEPTION_
#define _TEXCEPTION_

#include <string>
#include <stdexcept>
#include <iosfwd>

//	a class to collect call-stack information and to output it

class ExceptionTracer
{
public:

	enum
	{
		e_stackSize	=	25
	};

			ExceptionTracer(void);
			~ExceptionTracer(void);

};


//	A class to do a formatted error message.

class Error_Msg
{
public:

			Error_Msg(void)								{ }
			Error_Msg(	const std::string&	a,
						const std::string&	b		= {},
						const std::string&	c		= {},
						size_t		len		= 50,
						char		fill	= ' ');
						
	friend std::ostream& operator << (std::ostream& output, const Error_Msg& msg);
	
						operator const std::string&(void) const			{ return msg_; }
						operator const char*(void) const				{ return msg_.c_str(); }
private:

	std::string	msg_;
	size_t		len_;
	char		fill_;
	
};

// ------------------------------------------------------------------------------------------------------------------------


class Exception_Msg
{
public:

			Exception_Msg(	const std::string&	a		= {},
							const std::string&	b		= {},
							const std::string&	c		= {});
						
						operator const std::string&(void) const			{ return msg_; }
						//operator const char*(void)					{ return msg_.c_str(); }
	const char*		c_str(void)	const									{ return msg_.c_str(); }
private:

	std::string msg_;
	
};

// ------------------------------------------------------------------------------------------------------------------------

 class fatal_error : public std::runtime_error
{
	public:	explicit fatal_error ( const std::string& what_arg ) : runtime_error{what_arg} { }
};



//	some inline 'macros'

inline void dthrow_if_nil_(bool ptr, const std::string& a, const std::string& b = {}, const std::string& c = {})
{
	if (! ptr)
	{
		//ExceptionTracer();
		throw std::runtime_error{Exception_Msg{a, b, c}};
	}
}

inline void dthrow_if_(bool test, const std::string& a, const std::string& b = {}, const std::string& c = {})
{
	if (test)
	{
		//ExceptionTracer();
		throw std::runtime_error{Exception_Msg{a, b, c}};
	}
}

inline void dthrow_if_range(bool test, const std::string& a, const std::string& b = {}, const std::string& c = {})
{
	if (test)
	{
		//ExceptionTracer();
		throw std::range_error{Exception_Msg{a, b, c}};
	}
}

inline void dthrow_msg_(const std::string& a, const std::string& b = {}, const std::string& c = {})
{
	//ExceptionTracer();
	throw std::runtime_error{Exception_Msg{a, b, c}};
}

// ------------------------------------------------------------------------------------------------------------------------

	//	The following functions throw FATAL errors

inline void dfail_if_nil_(bool ptr, const std::string& a, const std::string& b = {}, const std::string& c = {})
{
	if (! ptr)
	{
		ExceptionTracer xx;
		throw fatal_error{Exception_Msg{a, b, c}};
	}
}

inline void dfail_if_(bool test, const std::string& a, const std::string& b = {}, const std::string& c = {})
{
	if (test)
	{
		ExceptionTracer xx;
		throw fatal_error{Exception_Msg{a, b, c}};
	}
}

inline void dfail_msg_(const std::string& a, const std::string& b = {}, const std::string& c = {})
{
	ExceptionTracer xx;
	throw fatal_error{Exception_Msg{a, b, c}};
}

// ------------------------------------------------------------------------------------------------------------------------

#endif

