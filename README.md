# webserv

## Resource Dump

## Web/HTTP server overview

- [HTTP server from scratch guide](https://github.com/Dungyichao/http_server/blob/master/doc/HTTP%20Server_%20Everything%20you%20need%20to%20know%20to%20Build%20a%20simple%20HTTP%20server%20from%20scratch_pdf2.pdf)
- [What is a web server](https://developer.mozilla.org/en-US/docs/Learn/Common_questions/What_is_a_web_server)
- [NGINIX explained in 100 sec](https://www.youtube.com/watch?v=JKxlsvZXG7c)
- [Beej Guide to Network Programming](https://beej.us/guide/bgnet/html/split-wide/index.html)

###

- [TCP/IP socket](https://www.ibm.com/docs/en/zvse/6.2?topic=SSB27H_6.2.0/fa2ti_what_is_socket_connection.htm)
  [Poll Server Overview](https://www.softprayog.in/programming/io-multiplexing-select-poll-epoll-in-linux)

### HTTP protocol

- [RFC](https://www.rfc-editor.org/rfc/rfc9112.html)
- [Overview](https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview)
- [HTTP status code](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status)
- [HTTP header](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers)
- [HTTP Basics](https://www3.ntu.edu.sg/home/ehchua/programming/webprogramming/http_basics.html)
- [HTTP Message Parsing](https://www.ietf.org/archive/id/draft-ietf-httpbis-p1-messaging-11.html#introduction)
- [HTTP RFC Summary](http://www.cs.cmu.edu/~srini/15-441/F11/responses)

## CGI

[Youtube Tutorial](https://www.youtube.com/watch?v=IU88XroupIQ&list=PLDhd2asKgB6Wo2uxQS3C9PBG_wjNGQLnP)

## Config file

The config file have the following structure:

```
context_name_1 {
    key_name_1 value_1_1 value2_1;
    key_name_2 value_2_1;
    context_name_1_1 {
        ...
    }
}

context_name_2 {
    ...
}
```

The following alternatives for `context_name` are expected:

- `server`
- `location` - only as an inner context of `server` context - that must be followed by the path before the `{`;

For server, the following `key_names` are expected:

- `listen` - defines the HTTP port that will listen for new connections;
- `server_name` - defines the host name that can be used to access the server;
- `root` - set root directory for requests ;
- `client_max_body_size` - set the maximum size of the request body receivable;
- `error_page` - defines which files to display when specific error occurs;
- ...

[nginx official doc](https://nginx.org/en/docs/http/ngx_http_core_module.html)
