///
///	@file 	simpleServer.cpp
/// @brief 	Embed the AppWeb server in a simple multi-threaded 
//			application.
///
////////////////////////////////////////////////////////////////////////////
//
//	Copyright (c) Mbedthis Software LLC, 2003-2007. All Rights Reserved.
//	The latest version of this code is available at http://www.mbedthis.com
//
//	This software is open source; you can redistribute it and/or modify it 
//	under the terms of the GNU General Public License as published by the 
//	Free Software Foundation; either version 2 of the License, or (at your 
//	option) any later version.
//
//	This program is distributed WITHOUT ANY WARRANTY; without even the 
//	implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//	See the GNU General Public License for more details at:
//	http://www.mbedthis.com/downloads/gplLicense.html
//	
//	This General Public License does NOT permit incorporating this software 
//	into proprietary programs. If you are unable to comply with the GPL, a 
//	commercial license for this software and support services are available
//	from Mbedthis Software at http://www.mbedthis.com
//
/////////////////////////////// Includes ///////////////////////////////

#include	"appweb.h"
#include	<crtdbg.h>
#include	<signal.h>
#include "clientHandler.h"
/////////////////////////////////// Code ///////////////////////////////
bool running = true;

/// Handle termination signals
/** Put the global variable stopEvent to 'true' if a termination signal is caught **/
void OnSignal(int s)
{
	switch (s)
	{
	case SIGINT:
	case SIGTERM:
		printf("OnSignal %u",s);
		running = false;
		break;
#ifdef WIN32
	case SIGBREAK:
		running = false;
		break;
#else
	case SIGPIPE:
		perror("send SIGPIPE\n");
		break;
#endif
	}

	signal(s, OnSignal);
}

/// Define hook 'OnSignal' for all termination signals
void HookSignals()
{
	signal(SIGINT, OnSignal);
	signal(SIGTERM, OnSignal);
#ifdef WIN32
	signal(SIGBREAK, OnSignal);
#else
	/*{
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		if(sigaction( SIGPIPE, &sa, 0 ) == -1){
			perror("failed to ignore SIGPIPE; sigaction");
			exit(EXIT_FAILURE);
		} 
	}*/
	signal(SIGPIPE, OnSignal);
#endif
}

/// Unhook the signals before leaving
void UnhookSignals()
{
	signal(SIGINT, 0);
	signal(SIGTERM, 0);
#ifdef _WIN32
	signal(SIGBREAK, 0);
#endif
}

int main(int argc, char** argv)
{
#if WIN32
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF  );
	//_CrtSetBreakAlloc(67);
#endif
	MaHttp		*http;					// Http service inside our app
	MaServer	*server;				// For the HTTP server
	Mpr			mpr("simpleServer");	// Initialize the run time

#if BLD_FEATURE_LOG
	//
	//	Do the following two statements only if you want debug trace
	//
	mpr.addListener(new MprLogToFile());
	mpr.setLogSpec("stdout:4");
#endif

	//
	//	Start the Mbedthis Portable Runtime with 10 pool threads
	//
	mpr.start(MPR_SERVICE_THREAD);

#if BLD_FEATURE_MULTITHREAD
	mpr.poolService->setMaxPoolThreads(10);
#endif

	//
	//	Create Http and Server objects for this application. We set the
	//	ServerName to be "default" and the initial serverRoot to be ".".
	//	This will be overridden in simpleServer.conf.
	//
	http = new MaHttp();
	server = new MaServer(http, "default", ".");

	//
	//	Activate the copy module and handler
	//
	new MaClientModule(0);
	new MaDirModule(0);
	new MaCopyModule(0);	
#if BLD_FEATURE_CONFIG_PARSE
	//
	//	Configure the server with the configuration directive file
	//
	if (server->configure("simpleServer.conf") < 0) {
		mprFprintf(MPR_STDERR, 
			"Can't configure the server. Error on line %d\n", 
			server->getLine());
		exit(2);
	}
#endif
	
	//
	//	Start the server
	//
	if (http->start() < 0) {
		mprFprintf(MPR_STDERR, "Can't start the server\n");
		exit(2);
	}

	//
	//	Tell the MPR to loop servicing incoming requests. We can 
	//	replace this call with a variety of event servicing 
	//	mechanisms offered by AppWeb.
	//
	//mpr.serviceEvents(0, -1);
	HookSignals();

	while (running)
		mpr.serviceEvents(1, 50);	
	UnhookSignals();


	//
	//	Orderly shutdown
	//
	http->stop();
	delete server;
	delete http;

	//
	//	MPR run-time will automatically stop and be cleaned up
	//
	return 0;
}

//
// Local variables:
// tab-width: 4
// c-basic-offset: 4
// End:
// vim: sw=4 ts=4 
//
