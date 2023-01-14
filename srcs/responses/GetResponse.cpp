#include "GetResponse.hpp"
#include "utils.hpp"
#include <cstddef>
#include <fstream>
#include <string>

// Constructors
GetResponse::GetResponse() {

}

GetResponse::~GetResponse() {

}

GetResponse::GetResponse(const GetResponse& src) {
    (void)src;
}

GetResponse& GetResponse::operator=(const GetResponse& rhs) {
    (void)rhs;
	return (*this);
}

std::pair<size_t, size_t> findPhpBlock(const std::string& html, size_t pos = 0) {
    // Find the position of the next PHP block
    size_t startPos = html.find("<?php", pos);
    size_t endPos = html.find("?>", startPos);
    std::cout << "start end pos : " << startPos << " AND " << endPos << std::endl;
   if (startPos != std::string::npos && endPos != std::string::npos) {
        return std::make_pair(startPos, endPos);
    } else {
        return std::make_pair(std::string::npos, std::string::npos);
    }
}

std::string processPhpInHtml(const std::string& resource) {
    std::ifstream               htmlFile(resource);
    std::string                 html((std::istreambuf_iterator<char>(htmlFile)), std::istreambuf_iterator<char>());
    std::pair<size_t, size_t>   phpBlock;

    // Read the entire HTML file into a string and locate the PHP code blocks
    if (htmlFile.is_open())
    {
        phpBlock = findPhpBlock(html);
        htmlFile.close();
    }
    else
    {
        // Handle error opening the file
    }

    // Iterate over the PHP blocks and execute the PHP code
    std::string result;
    result += html.substr(0, phpBlock.first);

    // Execute the PHP code using the system function and capture the output
    std::string command = "php -r \"echo " + html.substr(phpBlock.first, phpBlock.second - phpBlock.first + 2) + "\"";
    std::cout << "command is " << command << std::endl;
    result += std::system(command.c_str());
    result += html.substr(phpBlock.second);
    return result;
}

std::string executePhpBlock(std::string html) {
    std::string php_start_tag = "<?php";
    std::string php_end_tag = "?>";

    size_t start_pos = html.find(php_start_tag);
    size_t end_pos = html.find(php_end_tag);

    std::string php_code = html.substr(start_pos + php_start_tag.length(), end_pos - start_pos - php_start_tag.length());
    std::cout << "php code is: " << php_code << std::endl;
    exit(-1);
    return (php_code);
}

std::string processPhp(std::string res) {
    std::string command;
    std::string result;

    command = "php " + res;
    FILE *in = popen(command.c_str(), "r");

    if (in) {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), in)) {
            result += buffer;
        }
        pclose(in);
    }
    return (result);
}

// Public methods
void    GetResponse::setRawBody() {
    std::ifstream   file(_resource);
    std::string     content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    //
    // _raw_body = content;
    // std::cout << "resource is " << _resource << std::endl;
    // _raw_body = processPhpInHtml(_resource);
    //_raw_body = executePhpBlock(content);
    if (hasFileExtension(_resource, ".html") || hasFileExtension(_resource, ".php"))
    {
        _raw_body = processPhp(_resource);
    }
    else
    {
        _raw_body = content;
    }
    setContentLengthHeader();
    // std::cout << "raw body after processing is : " << _raw_body << std::endl;
}

void    GetResponse::setHeaders() {
    setDateHeader();
    //setContentLengthHeader();
    setConnectionHeader();
    setContentTypeHeader();
    // add more headers if desired below...
    // add here...
}

void    GetResponse::constructResponse(Request& req) {
    std::string path = (req.getURI()).getPath();   
    
    setResource(req.getURI().getPath());
    _raw_status_line = _http_version + " 200 OK" + "\r\n"; 
       
    // set the headers
    setHeaders();
    setRawHeaders();

    // include the body
    setRawBody();
    setRawResponse();
    //printResponse();
}
