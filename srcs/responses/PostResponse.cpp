#include "PostResponse.hpp"
#include <fstream>
#include <sstream>
#include <string>

// Constructors
PostResponse::PostResponse() {

}

PostResponse::~PostResponse() {

}

// Public methods
std::string trimNonNumerical(std::string input) {
    std::string output = "";
    for (unsigned int i = 0; i < input.length(); i++) {
        if (isdigit(input[i])) {
            output += input[i];
        }
    }
    return output;
}

void parseMultipartFormData(std::ifstream& tmpFile) {
    std::string line;
    std::string separator;
    std::string name;
    std::string value;
    std::string picture;
    std::string description;
    std::string price;
    std::string phone;

    std::getline(tmpFile, separator);
    std::cout << "separator is" << separator << std::endl;
    while (tmpFile.good())
    {
        std::getline(tmpFile, line);
        if (line.find("Content-Disposition: form-data; name=") != std::string::npos)
        {
            name = line.substr(line.find("name=") + 6, line.length() - 1);
            name = name.substr(0, name.find("\""));
            std::getline(tmpFile, line);
            if (name != "picture")
                std::getline(tmpFile, line);
            value = line.substr(0, line.length() - 1);
        }
        if (name == "description")
        {
            description = value;
        }
        else if (name == "price")
        {
            price = value;
        }
        else if (name == "phone")
        {
            phone = value;
        }
        else if (name == "picture")
        {
            if (line.find("Content-Type: image/jpeg") != std::string::npos)
            {
                std::getline(tmpFile, line);
                while (std::getline(tmpFile, line))
                {
                    if (line.find(trimNonNumerical(separator)) == std::string::npos)
                    {
                        picture += line;
                        picture += '\n';
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
    std::ofstream dataFile("tmp/data-entry.txt");
    dataFile << "description=" << description << "&price=" << price << "&phone=" << phone;
    dataFile.close();

    std::ofstream pictureFile("tmp/pic-entry.jpeg", std::ios::binary);
    pictureFile.write(picture.c_str(), picture.size());
    pictureFile.close();
}

void moveAndRenameFile(const std::string& currentPath, const std::string& newPath) {
    if (rename(currentPath.c_str(), newPath.c_str()) != 0) {
        std::perror("Error renaming file");
    }
}

void    PostResponse::executePostResponse(Request& req) {
    std::ofstream   tmpFile;
    std::ifstream   inputFile;
    const char*     file_path = "tmp/data-entry.txt";
    std::string     command;
    
    // write the body of the POST request to a tmp file
    tmpFile.open(file_path);
    tmpFile << req.getRawBody();
    tmpFile.close();

    // take the contents of the tmpFile, and split them
    inputFile.open(file_path);
    parseMultipartFormData(inputFile);
    inputFile.close();

    // execute the php script with the contents of the file to add to the json
    const char*     filePath2 = "tmp/id_file";
    command = "php " + _resource + " " + file_path + " > " + filePath2; 
    std::system(command.c_str());
    
    // take the id number and move the pic-entry.jpeg into the correct name in the images folder
    std::ifstream   tmpFile2(filePath2);
    std::string     id_string((std::istreambuf_iterator<char>(tmpFile2)), std::istreambuf_iterator<char>());
    moveAndRenameFile("tmp/pic-entry.jpeg", "public/www/images/" + id_string + ".jpeg");
    // delete the tmp file
    remove(file_path);
    remove("tmp/post-entry.txt");
    remove(filePath2);
}

void    PostResponse::executePostDeleteResponse(Request& req) {
    std::ofstream   tmpFile;
    const char*     file_path = "tmp/delete-entry.txt";
    std::string     command;
    
    // write the body of the POST request to a tmp file
    tmpFile.open(file_path);
    // std::cout << "DELETE BODY" << req.getRawBody() << std::endl;
    tmpFile << req.getRawBody();
    tmpFile.close();
    
    // execute the php script with the contents of the file
    command = "php " + _resource + " " + file_path; 
    std::system(command.c_str());

    // grab the id from the tmpFile
    std::string id = req.getRawBody().substr(req.getRawBody().find("&") + 4);
    std::cout << "ID FOR DELETE: " << id << std::endl;
    // delete the relevant entry from the folder public/www/images
    std::string remove_path = "public/www/images/" + id + ".jpeg";
    remove(remove_path.c_str());
    // TODO: check return of above path! does file not exist, what then?

    // delete the tmp file
    remove(file_path);
    // TODO: check return value ret and throw error if fail...
}

void    PostResponse::setLocationHeader(void) {
    _headers.insert(std::make_pair("Location", "/"));
}

void    PostResponse::setRefreshHeader(double sec) {
    std::stringstream   ss;
    std::string         str;

    ss << sec;
    str = ss.str();
    _headers.insert(std::make_pair("Refresh", str + ";url=/"));
}

void    PostResponse::setHeaders(void) {
    setDateHeader();
    // setContentLengthHeader();
    setConnectionHeader("close");
    // setContentTypeHeader();
    setLocationHeader();
    // setCacheControl("no-cache");
    // setRefreshHeader(0.3);
    // add more headers if desired below...
    // add here...
}

void    PostResponse::constructResponse(Request& req) {
    std::string path = (req.getURI()).getPath();   
    
    setResource(req.getURI().getPath());
    _raw_status_line = _http_version + " 302 Found" + "\r\n"; 

    // execute the php...
    if (path.find("add-entry.php") != std::string::npos)
        executePostResponse(req);
    else
        executePostDeleteResponse(req);
    // TODO: what if curl launches a dumb post request? -->Prevent!

    // set the headers
    setHeaders();
    setRawHeaders();
    
    // set response
    setRawResponse();
    // printResponse();
}
