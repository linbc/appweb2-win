///
///	@file 	copyHandler.cpp
/// @brief 	Copy static content handler
/// @overview This handler manages static content such as HTML, GIF 
///		or JPEG pages. Data is returned to the user in the background 
///		using the DataStreams buffering layer.
//
/////////////////////////////////// Copyright //////////////////////////////////
//
//	@copy	default
//	
//	Copyright (c) Mbedthis Software LLC, 2003-2007. All Rights Reserved.
//	
//	This software is distributed under commercial and open source licenses.
//	You may use the GPL open source license described below or you may acquire 
//	a commercial license from Mbedthis Software. You agree to be fully bound 
//	by the terms of either license. Consult the LICENSE.TXT distributed with 
//	this software for full details.
//	
//	This software is open source; you can redistribute it and/or modify it 
//	under the terms of the GNU General Public License as published by the 
//	Free Software Foundation; either version 2 of the License, or (at your 
//	option) any later version. See the GNU General Public License for more 
//	details at: http://www.mbedthis.com/downloads/gplLicense.html
//	
//	This program is distributed WITHOUT ANY WARRANTY; without even the 
//	implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//	
//	This GPL license does NOT permit incorporating this software into 
//	proprietary programs. If you are unable to comply with the GPL, you must
//	acquire a commercial license to use this software. Commercial licenses 
//	for this software and support services are available from Mbedthis 
//	Software at http://www.mbedthis.com 
//	
//	@end
//
////////////////////////////////// Includes ////////////////////////////////////

#include	"clientHandler.h"
#include "client.h"
//#if BLD_FEATURE_CLIENT_MODULE
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// ClientModule ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int mprClientInit(void *handle)
{
	if (maGetHttp() == 0) {
		return MPR_ERR_NOT_INITIALIZED;
	}
	(void) new MaClientModule(handle);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

MaClientModule::MaClientModule(void *handle) : MaModule("Client", handle)
{
	clientHandlerService = new MaClientHandlerService();
}

////////////////////////////////////////////////////////////////////////////////

MaClientModule::~MaClientModule()
{
	delete clientHandlerService;
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// MaClientHandlerService //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MaClientHandlerService::MaClientHandlerService() : 
	MaHandlerService("clientHandler")
{
}

////////////////////////////////////////////////////////////////////////////////

MaClientHandlerService::~MaClientHandlerService()
{
}

////////////////////////////////////////////////////////////////////////////////

MaHandler *MaClientHandlerService::newHandler(MaServer *server, MaHost *host, 
	char *extensions)
{
	return new MaClientHandler();
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// MaClientHandler //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MaClientHandler::MaClientHandler() : MaHandler("clientHandler", 0, 
	MPR_HANDLER_GET | MPR_HANDLER_HEAD | MPR_HANDLER_HEAD | 
	MPR_HANDLER_TERMINAL)
{
}

////////////////////////////////////////////////////////////////////////////////

MaClientHandler::~MaClientHandler()
{
}

////////////////////////////////////////////////////////////////////////////////

MaHandler *MaClientHandler::cloneHandler()
{
	return new MaClientHandler();
}

////////////////////////////////////////////////////////////////////////////////
//
//	Override default matchRequest and handle all file types
//

int MaClientHandler::matchRequest(MaRequest *rq, char *uri, int uriLen)
{
	return 1;
}

////////////////////////////////////////////////////////////////////////////////

int MaClientHandler::run(MaRequest *rq)
{
	MprFileInfo		*info;
	char			*fileName;
	bool			clientData=false;
	int				flags=0, ret = 0;
	const int		offset=4;//offset length "url=" 4 

	fileName = rq->getFileName();
	mprAssert(fileName);

	flags = rq->getFlags();
	info = rq->getFileInfo();
	hitCount++;

	char* paramURL = rq->getUrl()->query;//get url param
	int len = strlen(paramURL)-offset;
	if (_access(fileName, 0) != 0 && len > 0)
	{
		char pDownURL[MPR_HTTP_MAX_URL], pEncodeURL[MPR_HTTP_MAX_URL];
		memset(pDownURL,0,sizeof(pDownURL));
		memset(pEncodeURL,0,sizeof(pEncodeURL));
		memcpy(pDownURL, paramURL + offset, len);
		maDecode64(pEncodeURL,len,pDownURL);		//base64Decode

		char filepath[260];
		memset(filepath,0,sizeof(filepath));
		mprGetDirName(filepath,sizeof(filepath),fileName);
		if (!mprMakeDir(filepath))
		{
			int contentLen = 0;
			char *szContent=NULL,*strContent=NULL;
			MaClient* http_client = new MaClient();
			if(!http_client->getRequest(pEncodeURL))
			{
				ret = http_client->getResponseCode();
				if (ret >=200 && ret < 400)
				{
					szContent = http_client->getResponseContent(&contentLen);
					if (contentLen > 0)
					{
						FILE *fp;
						if(fp=fopen(fileName,"wb"))
						{
							fwrite(szContent, contentLen, 1, fp);
							fclose(fp);
						}
					}
				}
				else
				{
					mprLog(4, "clientHandler: get url param error.url query:%s, error code:%d\n", paramURL, ret);
				}
			}
			delete http_client;
		}
	}
	rq->openDoc(fileName);
	mprLog(4, "clientHandler: file already exist.path=%s %s\n", fileName);

	clientData = 1;

#if BLD_FEATURE_IF_MODIFIED
	if (flags & MPR_HTTP_IF_MODIFIED) {
		int		same;

		//
		//	If both checks, the last modification time and etag, claim that the 
		//	request doesn't need to be performed, skip the transfer.
		//
		same = rq->matchModified((int) info->mtime) && 
			rq->matchEtag(rq->getEtag());

#if BLD_FEATURE_RANGES
		if (rq->isRangedOutput()) {
			if (!same) {
				/*
				 *	Need to transfer the entire resource
				 */
				rq->deRangeOutput();
			}
		} else 
#endif
		if (same) {
			rq->setResponseCode(MPR_HTTP_NOT_MODIFIED);
			clientData = 0;
		}
 	}
#endif

	if (clientData) {
		if (flags & (MPR_HTTP_GET_REQUEST | MPR_HTTP_HEAD_REQUEST | 
				MPR_HTTP_POST_REQUEST)) {
			rq->insertDataStream(rq->getDocBuf());
			rq->getDocBuf()->setSize(info->size);
		}
		rq->setResponseCode(200);
	}

	rq->flushOutput(MPR_HTTP_BACKGROUND_FLUSH, MPR_HTTP_FINISH_REQUEST);
	return MPR_HTTP_HANDLER_FINISHED_PROCESSING;
}

////////////////////////////////////////////////////////////////////////////////
//#else
void mprClientHandlerDummy() {}

//#endif // BLD_FEATURE_CLIENT_MODULE
//
// Local variables:
// tab-width: 4
// c-basic-offset: 4
// End:
// vim: sw=4 ts=4 
//
