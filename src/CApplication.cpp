// =====================================================================================
// 
//       Filename:  CApplication.cpp
// 
//    Description:  application framework
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


#include <sys/stat.h>

#include <cstdlib>	
#include <fstream>
#include <memory>

#include "CApplication.h"

//#include "cp_string.h"
#include "TException.h"
//#include "memory_utils.h"

#include <boost/algorithm/string.hpp>


// === Static Globals ===


CApplication*	CApplication::sTheApplication	=	nullptr;		
CErrorHandler*	CApplication::sCErrorHandler	=	nullptr;		

// ---------------------------------------------------------------------------
//		CApplication
// ---------------------------------------------------------------------------


CApplication::CApplication (int argc, char* argv[])
	: mArgc{argc}, mArgv{argv}, tokens_{nullptr}
	
{
	this->Init();
}

CApplication::CApplication (int argc, char* argv[], const std::vector<std::string>& tokens)
	: mArgc{argc}, mArgv{argv}, tokens_{&tokens}
	
{
	this->Init();
}

void CApplication::Init (void)
{
	mState=EProgramState::programState_StartingUp;

			//	let's make public the error handler class

	CApplication::sCErrorHandler = &mMyError;
	mMyError.SetErrorStream(&std::cerr);

	//	copy pointers to args so we have a local copy
	//	
	//for (int i = 0; i < argc; ++i)
	//	mArgv[i] = argv[i];
		
	//	now let's find our where we are located.

	fs::path home_dir{mArgv[0]};
	if (home_dir.is_absolute())
	{
		//	truncate leaf name

		mAppFolder = home_dir.parent_path();
	}
	else
	{
		//	ok, we have a partial path name so let's save our curr. working dir
		//	and move on to where our program is and get the whole path name
		//	then we'll move back to our saved original cwd.

		fs::path save_where_i_am_now = fs::current_path();

		fs::path where_program_is = home_dir.parent_path();

		fs::current_path(where_program_is);

		mAppFolder = fs::current_path();

		fs::current_path(save_where_i_am_now);
	}
	dfail_if_(mAppFolder.c_str()[0] != '/', "Hey, I don't know where I am!");

}		// -----  end of method CApplication::Init  -----

// ---------------------------------------------------------------------------
//		~CApplication
// ---------------------------------------------------------------------------
//	Destructor


CApplication::~CApplication ()

{
}

// ===========================================================================
// Accessors													   Accessors 
// ===========================================================================

//----------------------------------------------------------------------------


CApplication::EProgramState CApplication::GetState ()
{
	return mState;
}

//----------------------------------------------------------------------------


void CApplication::StartUp (void)
{
	try
	{	
		this->SetupProgramOptions();
		this->CheckArgs ();
	}
	catch(...)
	{
		//	we're outta here!

		this->Done();
		throw;
	}

	if (mState == EProgramState::programState_StartingUp)
	{
		mState = EProgramState::programState_Started;
	}
}

//----------------------------------------------------------------------------


void CApplication::Run ()
{

	this->Do_Run();
}

//----------------------------------------------------------------------------


void CApplication::Quit (void)
{
			//	Null out our address so we won't think we're still alive

	mState=EProgramState::programState_Quitting;

	this->Do_Quit();

	mState=EProgramState::programState_Done;
}

//----------------------------------------------------------------------------


void	CApplication::CheckArgs (void)

{
	//	the second alternative is available to help support TDD by providing an
	//	alternate way to pass command line options in to the program.
	
	if (! this->tokens_)
		this->ParseProgramOptions();
	else
		this->ParseProgramOptions(tokens_);
	this->Do_CheckArgs();
}

//----------------------------------------------------------------------------

fs::path	CApplication::FindSupportFolder(const std::string& folderName)

{
	fs::path temp;

	fs::path appDir{mAppFolder};
	appDir /= folderName;
	if (fs::exists(appDir))
		temp = appDir;

	return temp;
}

//----------------------------------------------------------------------------

fs::path	CApplication::FindResource(const fs::path& resourceFolderPath, const std::string& resourceName)

{
	fs::path temp;
	
	fs::path resourceDir{resourceFolderPath};
	resourceDir	/=	resourceName;
	if (fs::exists(resourceDir))
		temp = resourceDir;

	return temp;
}

//----------------------------------------------------------------------------

void	CApplication::LoadResource(const std::string& resourceFolderPath,
									const std::string& resourceName,
									std::string& theResource)

{
	//	first let's make sure our resource file really exists.
	
	fs::path resourceFilePath = this->FindResource(resourceFolderPath, resourceName);
	dthrow_if_(resourceFilePath.empty(), "Missing 'resource' file ==> ", resourceName);

	std::ifstream resourceFile(resourceFilePath.string(), std::ios_base::in | std::ios_base::binary);
	dthrow_if_(! resourceFile.is_open(), "Unable to open 'resource' file ==> ", resourceFilePath.c_str());

	//	now let's read the whole file into our string

	theResource.assign(std::istreambuf_iterator<char>(resourceFile), std::istreambuf_iterator<char>());
	//std::getline(resourceFile, theResource, '\0');
	resourceFile.close();
}

//----------------------------------------------------------------------------

fs::path	CApplication::LookForResource(const std::string& resourceName)

{	
	fs::path temp;
	
	if (! mResources.empty())
		temp = this->FindResource(mResources, resourceName);
	
	return temp;
}

//----------------------------------------------------------------------------------------


void	CApplication::SetupProgramOptions(void)
{
	this->Do_SetupProgramOptions();
	
}

//----------------------------------------------------------------------------------------

void 	CApplication::ParseProgramOptions(void)
{
	decltype(auto) options = po::parse_command_line(mArgc, mArgv, mNewOptions);
	this->Do_ParseProgramOptions(options);
	po::store(options, mVariableMap);
	if (this->GetArgCount() == 0	||	mVariableMap.count("help"))
	{
		std::cout << mNewOptions << "\n";
		dfail_msg_("\nExit after 'help'.");
	}
	po::notify(mVariableMap);    

}

void 	CApplication::ParseProgramOptions(const std::vector<std::string>* tokens)
{
	decltype(auto) options = po::command_line_parser(*tokens).options(mNewOptions).run();
	this->Do_ParseProgramOptions(options);
	po::store(options, mVariableMap);
	if (mVariableMap.count("help"))
	{
		std::cout << mNewOptions << "\n";
		dfail_msg_("\nExit after 'help'.");
	}
	po::notify(mVariableMap);    

}

//----------------------------------------------------------------------------------------

void	CApplication::Do_StartUp (void)
{
	//	nothing to do
	
}

//----------------------------------------------------------------------------------------

void	CApplication::Do_CheckArgs (void)
{
	//	nothing to do
	
}

//----------------------------------------------------------------------------------------

void	CApplication::Do_Run (void)
{
	//	nothing to do
	
}

//----------------------------------------------------------------------------------------

void	CApplication::Do_Quit (void)
{
	//	nothing to do
	
}

//----------------------------------------------------------------------------------------

void	CApplication::Do_SetupProgramOptions(void)
{
	//	nothing to do
	
}

//----------------------------------------------------------------------------------------

void	CApplication::Do_ParseProgramOptions(po::parsed_options&)
{
	//	nothing to do.
}

//----------------------------------------------------------------------------------------


