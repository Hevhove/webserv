#include <regex>
#include <iostream>

bool is_valid_uri(const std::string& uri) {
    
    std::regex uri_regex("^(/[a-z0-9-._~!$&'()*+,;=:@]+)*)(?:\\?((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?(?:\\#((?:[a-z0-9-._~!$&'()*+,;=:/?@]|%[0-9A-F]{2})*))?$");

    return std::regex_match(uri, uri_regex);
}

int main(void)
{
    std::string str1 = "/";
    std::string str2 = "/page1/lol/var1=lol";
    std::string str3 = "/page1/lol/var1=lol&var2=lel";

    if (is_valid_uri(str1))
        std::cout << "true" << std::endl;
    if (is_valid_uri(str2))
        std::cout << "true" << std::endl;
    if (is_valid_uri(str3))
        std::cout << "true" << std::endl;
}
