#ifndef NOT_OK_RESPONSE_HPP
# define NOT_OK_RESPONSE_HPP

# include "Response.hpp"

class NotOkResponse : public Response {
    private:

    public:
        // Constructors
        NotOkResponse();
		virtual ~NotOkResponse();
        
        // Public methods
        void        constructResponse(Request& req);
        void        constructResponseWithBody(Request& req, std::string raw_body);
        void        setHeaders();
        void        printResponse(void);
};

#endif
