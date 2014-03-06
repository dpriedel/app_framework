// =====================================================================================
// 
//       Filename:  ErrorHandler.h
// 
//    Description:  Header for class which knows how to display error messages from caught exceptions
// 
//        Version:  1.0
//        Created:  01/30/2014 11:14:10 AM
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


#ifndef _CErrorHandler_
#define _CErrorHandler_

#include <iostream>
//#include "stream_utils.h"

class TException;
class exception;

	//	for output

//typedef	counted_ostream_iterator<const char*>	error_otor;

class CErrorHandler
{
public:

		CErrorHandler (std::ostream* errorStream = &std::clog) ;
		virtual ~CErrorHandler (void) ;

	void	HandleException(TException& theException);
	void	HandleException(std::exception& theException);
	void	HandleException(void);
	void	DisplayErrorMsg(const char* errorMsg);
	void	DisplayErrorMsg(int errorCode);			//	for global errno
	
		//	a handle to provide access
		
	static	CErrorHandler*	GetErrorHandler(void)		{ return sCErrorHandler; }

		//	to change where our error messages get written to.
		//	returns previous error stream
		
	std::ostream*	SetErrorStream(std::ostream* errorStream);
		
private:

	static	CErrorHandler*	sCErrorHandler;

	std::ostream*	mErrorStream;					//	where we write messages to

	//	let's preallocate our message storage areas
	
	std::string	mFatalMessage;
	std::string mMessageStr;

};

#endif
