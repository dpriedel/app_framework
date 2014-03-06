// =====================================================================================
// 
//       Filename:  CApplication.h
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


#ifndef _C_LApplication
#define _C_LApplication

#include <string>

#include <boost/filesystem.hpp> // includes boost/filesystem/path.hpp
#include <boost/program_options.hpp>

#include "ErrorHandler.h"

	//	This application provides basic features that any application might need.
	//	the member functions will grow as experience dictates.
	
namespace fs = boost::filesystem;		//	alias to save some typing
namespace po = boost::program_options;


class CApplication
{
private:
	
	void Init(void);

public:

	enum class EProgramState
	{
		programState_StartingUp,
		programState_Started,				//	have done initial start up
		programState_ProcessingEvents,
		programState_Quitting,
		programState_Done
	};

	static CApplication*	sTheApplication;		
	static CErrorHandler*	sCErrorHandler;
	
	//	our ctor takes a pointer to our name.  this should be our path name
	
					CApplication(void)=delete;
					CApplication (int argc, char* argv[]);
					CApplication(int argc, char* argv[], const std::vector<std::string>& tokens);
	virtual			~CApplication (void);

	EProgramState	GetState (void) ;

	void	StartUp (void) ;
	void	CheckArgs (void) ;
	void	Run (void) ;
	void	Quit (void) ;

			//	The following functions are used to
			//	retrieve arguments passed to the program

	const char*		GetArg (short argNo)
						{ return(argNo > 0	?	mArgv[argNo] : NULL); }
	
	short			GetArgCount (void)		{ return mArgc - 1; }	//	ignore program name
	
	bool			IsDone (void)			{ return mState==EProgramState::programState_Done; }
	void			Done (void)				{ mState=EProgramState::programState_Done; }

	const fs::path&	GetAppFolder(void)	{ return mAppFolder; }

	//	this routine will look for folders related to the application.
	//	folders must be peers of the application.

	fs::path		FindSupportFolder(const std::string& folderName);
	fs::path		FindResource(const fs::path& resourceFolderPath, const std::string& resourceName);
	fs::path		LookForResource(const std::string& resourceName);

		//	use this for text based resources such as simple tables or lists.
		//	
	void			LoadResource(const std::string& resourceFolderPath,
									const std::string& resourceName,
									std::string& theResource);

	//	Setup for parsing program options.

	void	SetupProgramOptions(void);
	void 	ParseProgramOptions(void);
	void 	ParseProgramOptions(const std::vector<std::string>* tokens);

protected:

	virtual void	Do_StartUp (void);
	virtual void	Do_CheckArgs (void);

		//	main processing loop here
		
	virtual void	Do_Run (void);

	virtual void	Do_Quit (void);

	//	Give applications a chance to modify parameters

	virtual	void	Do_SetupProgramOptions(void);

	//	Applications can override this if they need to do anything special 
	//	with parsed program options.
	
	virtual	void	Do_ParseProgramOptions(po::parsed_options& options);

	CErrorHandler	mMyError;				//	code to display error messages

	fs::path		mAppFolder;				//	path to where we are.
	fs::path		mResources;				//	where we find things we need
	
			//	The following member variables are class 'property' variables

	EProgramState	mState;

	int				mArgc;
	char**			mArgv;

	po::positional_options_description	mPositional;			//	old style options
	po::options_description				mNewOptions;			//	new style options (with identifiers)
	po::variables_map					mVariableMap;

	const std::vector<std::string>*		tokens_;
	
};


#endif
