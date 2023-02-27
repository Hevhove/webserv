#include "Connection.hpp"
#include "NotFoundResponse.hpp"

// Constructors
Connection::Connection() {}

Connection::~Connection() {
    // TODO: clear all of the Responses allocated on the heap
}

Connection::Connection(const Connection &src) : Socket(src) { (void)src; }

Connection &Connection::operator=(const Connection &rhs) {
    (void)rhs;
    return (*this);
}

// Public Member Functions
void Connection::handleRequest(char buf[BUFF_SIZE]) {
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
    while (bytes_checked < BUFF_SIZE) {
        // If there is no pair available, or the previous pair is already fully
        // parsed, there might be more information in the current buffer, so we
        // need to allocate a RequestResponse pair
        if (requestResponseList.size() == 0 ||
            requestResponseList[requestResponseList.size() - 1]
                .first->isFullyParsed()) {
            Request *req = new Request;
            std::pair<Request *, Response *> my_pair =
                std::make_pair(req, (Response *)0);
            requestResponseList.push_back(my_pair);
        }
        Request *last_req =
            requestResponseList[requestResponseList.size() - 1].first;
        try {
            bytes_checked = last_req->parseRequest(buf, bytes_checked);
            last_req->parse_status = "OK";
        } catch (Request::BadRequestException &e) {
            bytes_checked = BUFF_SIZE;
            last_req->parse_status = "BadRequest";
        } catch (Request::NotFoundException &e) {
            bytes_checked = BUFF_SIZE;
            // delete the request that was being made and replace with a new
            // request...
            last_req->parse_status = "NotFound";
        } catch (Request::HttpVersionNotSupportedException &e) {
            bytes_checked = BUFF_SIZE;
            last_req->parse_status = "VersionMismatch";
        } catch (Request::ContentTooLargeException &e) {
            bytes_checked = BUFF_SIZE;
            last_req->parse_status = "ContentTooLarge";
        }
        // std::cout << "current bytes checked is: " << bytes_checked <<
        // std::endl;
    }
}

std::string Connection::getRawResponse(void) {
    std::string response;
    for (std::vector<std::pair<Request *, Response *> >::iterator it =
             requestResponseList.begin();
         it != requestResponseList.end(); ++it) {
        Request *req = it->first;
        if (req->parse_status == "NotFound" ||
            req->parse_status == "BadRequest" ||
            req->parse_status == "VersionMismatch" ||
            req->parse_status == "ContentTooLarge") {
            if (req->parse_status == "NotFound")
                it->second = new NotFoundResponse();
            else if (req->parse_status == "BadRequest")
                it->second = new BadRequestResponse();
            else if (req->parse_status == "VersionMismatch")
                it->second = new HttpVersionResponse();
            else if (req->parse_status == "ContentTooLarge")
                it->second = new ContentTooLargeResponse();
            it->second->constructResponse(*req);
            response = it->second->getRawResponse();
            delete it->first;
            delete it->second;
            requestResponseList.erase(it);
            return response;
        } else if (req->isFullyParsed()) {
            try {
                if (req->getRequestMethod() == GET) {
                    it->second = new GetResponse();
                } else if (req->getRequestMethod() == POST) {
                    it->second = new PostResponse();
                }
                if (req->getRequestMethod() == DELETE) {
                    it->second = new DeleteResponse();
                }
            } catch (std::exception &e) {
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
