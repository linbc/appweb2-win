
#ifndef _h_CLIENT_MODULE
#define _h_CLIENT_MODULE 1

#include	"http.h"

/////////////////////////////// Forward Definitions ////////////////////////////

class MaClientHandler;
class MaClientHandlerService;
class MaClientModule;

extern "C" {
	extern int mprClientInit(void *handle);
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// MaClientModule //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class MaClientModule : public MaModule {
  private:
	MaClientHandlerService 
					*clientHandlerService;
  public:
					MaClientModule(void *handle);
					~MaClientModule();
};

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// MaClientHandler ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class MaClientHandlerService : public MaHandlerService {
  public:
					MaClientHandlerService();
					~MaClientHandlerService();
	MaHandler		*newHandler(MaServer *server, MaHost *host, char *ex);
};

class MaClientHandler : public MaHandler {
  public:
					MaClientHandler();
					~MaClientHandler();
	MaHandler		*cloneHandler();
	int				matchRequest(MaRequest *rq, char *uri, int uriLen);
	int				run(MaRequest *rq);
};

////////////////////////////////////////////////////////////////////////////////
#endif // _h_Client_MODULE 

//
// Local variables:
// tab-width: 4
// c-basic-offset: 4
// End:
// vim: sw=4 ts=4 
//
