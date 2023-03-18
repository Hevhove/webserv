#include "Connection.hpp"
#include <typeinfo>

// Constructors
Connection::Connection() {

}

Connection::Connection(Config config) {
	_config = config;
}

Connection::~Connection() {
    // TODO: clear all of the Responses allocated on the heap
}

// Public Member Functions
void    Connection::handleRequest(char buf[BUFF_SIZE]) {
    /*
    we have something in the buffer now, we don't know what...
    how do we know which connection we are on right now?

    Make a pair of a request pointer, and NULL (for now...)
    ONLY IF
         1. The current requestResponseList is empty
         2. The previous request exists was fully parsed
             - A no-body request ends with \r\n\r\n
             - A bodied request has reached content-length
    */
    int bytes_checked = 0;
    while (bytes_checked < BUFF_SIZE)
    {
        // If there is no pair available, or the previous pair is already fully parsed,
        // there might be more information in the current buffer, so we need to allocate a RequestResponse pair
        if (requestResponseList.size() == 0
            || requestResponseList[requestResponseList.size() - 1].first->isFullyParsed())
        {
            Request* req = new Request;
            std::pair<Request*, Response*> my_pair = std::make_pair(req, (Response*)0);
            requestResponseList.push_back(my_pair);
        }
        Request* last_req = requestResponseList[requestResponseList.size() - 1].first;
        try {
                bytes_checked = last_req->parseRequest(buf, bytes_checked, _config); // Add _config to function argument
                last_req->parse_status = "OK";
				last_req->setStatusCode(OK);
        }
		catch (std::exception& e) {
                bytes_checked = BUFF_SIZE;
				try {
					e = dynamic_cast<Request::BadRequestException &>(e);
					last_req->setStatusCode(BAD_REQUEST);
				} catch (std::bad_cast const&) {}
				try {
					e = dynamic_cast<Request::NotFoundException &>(e);
					last_req->setStatusCode(NOT_FOUND);
				} catch (std::bad_cast const&) {}
				try {
					e = dynamic_cast<Request::HttpVersionNotSupportedException &>(e);
	                last_req->parse_status = "VersionMismatch";
					last_req->setStatusCode(HTTP_VERSION_NOT_SUPPORTED);
				} catch (std::bad_cast const&) {}
				try {
					e = dynamic_cast<Request::ContentTooLargeException &>(e);
	                last_req->parse_status = "ContentTooLarge";
					last_req->setStatusCode(CONTENT_TOO_LARGE);
				} catch (std::bad_cast const&) {}
		}
    }
}

std::string Connection::getRawResponse(void) {
    std::string response;
    for (std::vector<std::pair<Request*, Response*> >::iterator it = requestResponseList.begin(); it != requestResponseList.end(); ++it) {
        Request* req = it->first;
		if (req->getStatusCode() != OK)
        {
            it->second = new NotOkResponse();
            it->second->constructResponseWithBody(
				*req, _config.getDefaultErrorPage(req->getStatusCode())
			);
            response = it->second->getRawResponse();
            delete it->first;
            delete it->second;
            requestResponseList.erase(it);
            return response;
        }
        else if (req->isFullyParsed()) {
            try {
                if (req->getRequestMethod() == GET) {
                    it->second = new GetResponse();
                }
                else if (req->getRequestMethod() == POST) {
                    it->second = new PostResponse();
                }
                if (req->getRequestMethod() == DELETE) {
                it->second = new DeleteResponse();
                }
            } catch (std::exception& e) {
                // TODO
            }
            it->second->constructResponse(*req);
            response = it->second->getRawResponse();
            delete it->first;
            delete it->second;
            requestResponseList.erase(it);
            return response;
        }
    }
    return response;
}

