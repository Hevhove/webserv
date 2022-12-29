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

#### Other notes

The Content-Length is optional in an HTTP request. For a GET or DELETE the length must be zero. For POST, if Content-Length is specified and it does not match the length of the message-line, the message is either truncated, or padded with nulls to the specified length.
The Content-Length is always returned in the HTTP response even when there is no content, in which case the value is zero.
