/*
    Messages can be either GET, POST or DELETE for our HTTP AMessage implementation. Each of these 3
    message types can then be a request or response message which are inherited from this
    abstract class.
*/

#ifndef CLASS_AMESSAGE_HPP
# define CLASS_AMESSAGE_HPP
# include <iostream>

typedef enum HTTPMethod {
    GET = 0,
    POST,
    DELETE
};

class AMessage {
    protected:
        // Parsing
        std::string			_unparsed_request;		// Request text that hasn't been analyzed
	    std::string			_raw_start_line; 		// The complete request line such as: `GET / HTTP/1.1`
	    std::string			_raw_headers;           // Raw headers (general headers, response/request headers, entity headers)
	    std::string			_raw_body;              // HTTP Message Body

        // Parsed Data

    public:
        // Constructors
        AMessage();
		virtual ~AMessage();
		AMessage(const AMessage& src);
		AMessage& operator=(const AMessage& rhs);
};

#endif