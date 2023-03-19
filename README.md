# webserv

## Resource Dump

## Web/HTTP server overview

- [HTTP server from scratch guide](https://github.com/Dungyichao/http_server/blob/master/doc/HTTP%20Server_%20Everything%20you%20need%20to%20know%20to%20Build%20a%20simple%20HTTP%20server%20from%20scratch_pdf2.pdf)
- [What is a web server](https://developer.mozilla.org/en-US/docs/Learn/Common_questions/What_is_a_web_server)
- [NGINIX explained in 100 sec](https://www.youtube.com/watch?v=JKxlsvZXG7c)
- [Beej Guide to Network Programming](https://beej.us/guide/bgnet/html/split-wide/index.html)
- [Poll Server Overview](https://www.softprayog.in/programming/io-multiplexing-select-poll-epoll-in-linux)

### HTTP protocol

- [RFC](https://www.rfc-editor.org/rfc/rfc9112.html)
- [Overview](https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview)
- [HTTP status code](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status)
- [HTTP header](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers)
- [HTTP Basics](https://www3.ntu.edu.sg/home/ehchua/programming/webprogramming/http_basics.html)
- [HTTP Message Parsing](https://www.ietf.org/archive/id/draft-ietf-httpbis-p1-messaging-11.html#introduction)
- [HTTP RFC Summary](http://www.cs.cmu.edu/~srini/15-441/F11/responses)

### TCP

- [TCP/IP socket](https://www.ibm.com/docs/en/zvse/6.2?topic=SSB27H_6.2.0/fa2ti_what_is_socket_connection.htm)

## CGI

- [Youtube Tutorial](https://www.youtube.com/watch?v=IU88XroupIQ&list=PLDhd2asKgB6Wo2uxQS3C9PBG_wjNGQLnP)

### URI

- [Difference URI vs URL](https://www.hostinger.com/tutorials/uri-vs-url)

### Testing
#### Telnet
Make a file (request.txt) with contents:
```
GET / HTTP/1.1
Host: localhost


```
Make sure there are 2 linebreaks after the headers. Then launch this command: `( cat request.txt; sleep 3 ) | telnet localhost 3490`
It will launch the GET request via telnet and keep the connection open for 3 seconds to receive the response from the server.

#### CURL
##### GET request
Launch a GET request with the following command: `curl localhost:3490 -X GET`

##### POST request
Launch a POST request to the PHP script like so:
```
---REQUEST PARSED---$
POST /cgi-bin/add-entry.php HTTP/1.1$
Host: localhost:3490^M$
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/111.0^M$
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8^M$
Accept-Language: en-US,en;q=0.5^M$
Accept-Encoding: gzip, deflate, br^M$
Content-Type: multipart/form-data; boundary=---------------------------23156686413367236551284241151^M$
Content-Length: 10046^M$
Origin: http://localhost:3490^M$
Connection: keep-alive^M$
Referer: http://localhost:3490/^M$
Upgrade-Insecure-Requests: 1^M$
Sec-Fetch-Dest: document^M$
Sec-Fetch-Mode: navigate^M$
Sec-Fetch-Site: same-origin^M$
Sec-Fetch-User: ?1$
---END OF REQUEST---$
```

`curl -X POST -H "Content-Type: multipart/form-data;" -F "description=Coolboard" -F "price=500" -F "phone=1256" http://localhost:3490/cgi-bin/add-entry.php`

Launch a POST request via curl with an attached file like this:
```
curl -X POST -H "Content-Type: multipart/form-data;" -F "description=Coolboard" -F "price=500" -F "phone=1256" -F "picture=@/Users/Hendrik/Downloads/surfboard7.jpeg" http://localhost:3491/cgi-bin/add-entry.php
```

##### DELETE Requests
Note on DELETE requests: in our front-end, we are using a form which launches a PHP request. In this manner we can only launch POST requests, where the body of the POST request will contain "Method=DELETE". In order to implement a true DELETE request, we have added some logic so entries can be removed immediately by the below format via curl.

`curl -X DELETE http://localhost:3490/images/3.jpeg`

#### Config
We style our config after the nginx configuration files, but limit the scope of them significantly.

- Server blocks
    - Listening Port
    - Root folder
    - Index page
    - Server name (call with `curl -H "Host: 42surf.com" http://localhost/public/www/42surf.html`)
    - Error pages
    - Client Max Body Size
    - Location (Routes)
        - Index
        - Limit_except
        - Client Max Body Size

Example Config:
```
http {
    server {
        listen 80;

        server_name first;
        index multi_form_upload.html
        root /var/www/;
        location /about/ {
            root /public/www/
            index about.html
            limit_except GET POST;
            client_max_body_size 10;
        }
    }

    server {
        listen 81;

        server_name second;
        error_pages 404 /www/error_pages/404.html;
        error_pages 401 /www/error_pages/401.html;
        error_pages 500 /www/error_pages/500.html;
        root /var/www/;
    }
}
```

Config rules:
- TODO: No empty lines
- No ; at end of line
- CHECK? must be enclosed in http block
- TODO: after limit_except we expect GET, POST or DELETE


#### Other notes

The Content-Length is optional in an HTTP request. For a GET or DELETE the length must be zero. For POST, if Content-Length is specified and it does not match the length of the message-line, the message is either truncated, or padded with nulls to the specified length.
The Content-Length is always returned in the HTTP response even when there is no content, in which case the value is zero.
