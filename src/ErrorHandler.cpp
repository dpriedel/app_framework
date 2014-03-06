// =====================================================================================
// 
//       Filename:  ErrorHandler.cpp
// 
//    Description:  Implementation for class which knows how to display error messages from caught exceptions
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


#include <cstdlib>
//#include <typeinfo>
#include <stdexcept>
#include <cstring>
//#include <iostream>

#include "TException.h"
#include "ErrorHandler.h"



CErrorHandler*	CErrorHandler::sCErrorHandler		= nullptr;

//	******************************
//
//	CErrorHandler
//
//	******************************


CErrorHandler::CErrorHandler (std::ostream* errorStream)
	:	mErrorStream(errorStream)
	
{
	sCErrorHandler = this;
}

//----------------------------------------------------------------------------

CErrorHandler::~CErrorHandler (void)

{
}

//----------------------------------------------------------------------------

void	CErrorHandler::HandleException(std::exception& theException)
{
		//	Standard exceptions have a 'what' method which will return
		//	a string.
		
		//	For now, we treat all standard exceptions as fatal errors.
				
	if (dynamic_cast<fatal_error*>(&theException))
		mFatalMessage = "Fatal error.  Processing stopped.";
	else
		mFatalMessage = "Warning.  Processing continues.";

	*mErrorStream << "\n\n>>> ";	
	*mErrorStream << theException.what() << ' ' << mFatalMessage << " <<<" << std::endl;
	mErrorStream->flush();
	
	//if (dynamic_cast<fatal_error*>(&theException))
	//	exit(9);
		
}

//----------------------------------------------------------------------------

void	CErrorHandler::HandleException(void)

{
	//	We display a generic error message.
	
	mMessageStr = "An Error of type --> 'Unknown' <-- occured.";
	mFatalMessage = "Fatal error.  Processing stopped.";

	*mErrorStream << "\n\n>>> ";	
	*mErrorStream << mMessageStr << ' ' << mFatalMessage << " <<<" << std::endl;
	mErrorStream->flush();

	//exit(10);
}

//----------------------------------------------------------------------------

void	CErrorHandler::DisplayErrorMsg(const char* errorMsg)

{
	*mErrorStream << "\n\n>>> ";	
	*mErrorStream << errorMsg << " <<<" << std::endl;

}

//----------------------------------------------------------------------------


void	CErrorHandler::DisplayErrorMsg(int errorCode)

{
	*mErrorStream << "\n\n>>> ";	
	*mErrorStream << strerror(errorCode) << " <<<" << std::endl;
}

//----------------------------------------------------------------------------

std::ostream*	CErrorHandler::SetErrorStream(std::ostream* errorStream)

{
	std::ostream* temp = mErrorStream;
	mErrorStream = errorStream;
	return temp;
}

//----------------------------------------------------------------------------




