/*

To make a POST-request with curl:
curl -X POST https://reqbin.com/echo/post/form -H "Content-Type: application/x-www-form-urlencoded" -d "key1=value1&key2=value2" 

A Response message from a server to client on a POST request can be :
    - 201: Created
    - 400: Bad Request
    - 411: Length Required
    - 413: Content too large
    - 415: Unsupported media type
    - 500: Internal Server Error
    - 505: HTTP Version Not Supported

Example:

HTTP/1.1 201 Created
Content-Type: image/jpeg
Location: http://example.com/images/123.jpeg
Content-Length: 12345

{
  "id": 123,
  "filename": "image.jpeg",
  "url": "http://example.com/images/123.jpeg"
}
*/

#ifndef CLASS_POST_RESPONSE_HPP
# define CLASS_POST_RESPONSE_HPP

# include "Response.hpp"

class PostResponse : public Response {
    private:
        // Private methods
        void    setHeaders();
        void    setRawBody();
        void    setLocationHeader(void);
        void    setRefreshHeader(double sec);

    public:
        // Constructors
        PostResponse();
		virtual ~PostResponse();

        // Public methods
        void        constructResponse(Request& req);
        void        executePostResponse(Request& req);
        void        printResponse(void);
        void        setStatusCode(StatusCode sc);
        void        executePostDeleteResponse(Request& req);
};

#endif
