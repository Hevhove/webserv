# Webserv

![Score](extras/score.png)
![Demo](extras/webserv.gif)

#### Performance Test
Below is the benchmarking test of our server for 200 concurrent users for 15 seconds. Please feel free to run your own test and submit any differing results to us. Project goal is to hit at least 99% uptime, which we succeed at with 100%.

```
hvan-hov@inception:~$ siege -c 200 -t 15S http://localhost:3488
** SIEGE 4.0.4
** Preparing 200 concurrent users for battle.
The server is now under siege...
Lifting the server siege...
Transactions:		         193 hits
Availability:		      100.00 %
Elapsed time:		       14.36 secs
Data transferred:	        7.76 MB
Response time:		        5.37 secs
Transaction rate:	       13.44 trans/sec
Throughput:		        0.54 MB/sec
Concurrency:		       72.12
Successful transactions:         193
Failed transactions:	           0
Longest transaction:	       11.28
Shortest transaction:	        0.00
```

## Description
For this project we are writing a webserver from scratch using the C functions select/poll/epoll (choice is up to us - we chose poll!). The rest of the server will be written in object-oriented style in C++. I found this one a very interesting project with immediately some cool use cases for features we have learnt in the past in C++ such as polymorphism, virtual functions, etc!

The requirements are quite numerous for this one, and it was the biggest project at the school so far:Your program has to take a configuration file as argument, or use a default path.

## Requirements
- You can’t execve another web server.
- Your server must never block and the client can be bounced properly if necessary.
- It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O operations between the client and the server (listen included).
- poll() (or equivalent) must check read and write at the same time.
- You must never do a read or a write operation without going through poll() (or equivalent).
- Checking the value of errno is strictly forbidden after a read or a write operation.
- You don’t need to use poll() (or equivalent) before reading your configuration file. Because you have to use non-blocking file descriptors, it is possible to use read/recv or write/send functions with no poll() (or equivalent), and your server wouldn’t be blocking. But it would consume more system resources. Thus, if you try to read/recv or write/send in any file descriptor without using poll() (or equivalent), your grade will be 0.
- You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO (un- derstanding what and how they do it is very useful).
- A request to your server should never hang forever.
- Your server must be compatible with the web browser of your choice.
- We will consider that NGINX is HTTP 1.1 compliant and may be used to compare headers and answer behaviors.
- Your HTTP response status codes must be accurate.
- You server must have default error pages if none are provided.
- You can’t use fork for something else than CGI (like PHP, or Python, and so forth).
- You must be able to serve a fully static website.
- Clients must be able to upload files.
- You need at least GET, POST, and DELETE methods.
- Stress tests your server. It must stay available at all cost.
- Your server must be able to listen to multiple ports (see Configuration file).

## Config Requirements
Choose the port and host of each ’server’.
- Setup the server_names or not.
- The first server for a host:port will be the default for this host:port (that means
it will answer to all the requests that don’t belong to an other server).
- Setup default error pages.
- Limit client body size.
- Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):
    - Define a list of accepted HTTP methods for the route.
    - Define a HTTP redirection.
    - Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
    - Turn on or off directory listing.
Set a default file to answer if the request is a directory.
    - Execute CGI based on certain file extension (for example .php).
    - Make it work with POST and GET methods.
    - Make the route able to accept uploaded files and configure where they should
be saved.
    - Your server should work with one CGI (php-CGI, Python, and so forth).

## Our approach
Our approach is calling the run method of our single server object, onto which we load a parsed config which is provided by the user. Some example configs are provided in the conf/ folder.

Initially we set up the server's listening socket or multiple listening sockets if we are running the server for multiple ports. Once the server is set-up, we run the main Server::run loop which will use poll(2) to either:
- check the current list of listening sockets in pollfd[] for a POLLIN event (data available to be read on the fd)
- check an already open connection for either POLLIN or POLLOUT
    - Incoming data: we need to parse the HTTP request from the client (StartLine, Headers, Body)
    - Outgoing data: Once an entire valid HTTP request is received on a particular connection, process the particular request asked (GET/POST/DELETE), which are implemented as subclasses of the Response Class.

We keep the TCP connection open for the client to send more information through after we complete their initial request. When reading from a particular file descriptor, we keep adding requests on this connection to a RequestResponse std::vector which is filled with std::pairs of Request and Response pointers. Once the request has been processed and handled by the server, we delete the pair from the vector and clean up memory.

When the client requests a GET of the root page (/), we send back the index page. We preprocess this index page by calling the CGI (PHP) to load the table of entries to the 42SURF page. Anytime we send out any HTML containing a php script from our cgi-bin folder, we do this. Similarly, POST and DELETE requests will call their respective PHP scripts. Because it's not possible to send a direct DELETE request via an HTML button (this sends a post request with a body indicating a resource should be deleted), we have implemented a separate DELETE class which works with cURL. The front-end however will call the table-deletion as a POST request.

We've kept the front-end and database aspect of this server extremely simple as the main focus was on getting the server fast and response with a good uptime when running a benchmarking service such as `siege`. The database is simply a folder public/www/images and the metadata of the pictures is saved in a json format under the data/ folder. The front-end is a simple table in HTML embellished with some CSS.

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
- error_pages url must be relative to webserv root
- CHECK? must be enclosed in http block
- TODO: after limit_except we expect GET, POST or DELETE
- ROOT must be in format public/www (only one slash in middle)
- index must be in format index.html (no slashes)

#### Other notes

The Content-Length is optional in an HTTP request. For a GET or DELETE the length must be zero. For POST, if Content-Length is specified and it does not match the length of the message-line, the message is either truncated, or padded with nulls to the specified length.
The Content-Length is always returned in the HTTP response even when there is no content, in which case the value is zero.
