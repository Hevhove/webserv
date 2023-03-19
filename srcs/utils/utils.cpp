# include "utils.hpp"

std::vector<std::string> ft_split(const std::string &str, char delim)
{
    std::vector<std::string>    result;
    std::stringstream           ss(str);
    std::string                 item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }
    return result;
}

bool ft_is_non_whitespace(int ch) {
    return !std::isspace(ch);
}

bool ft_is_resource_available(std::string resource) {
    std::ifstream file(resource.c_str());

    return (file.good());
}

bool hasFileExtension(const std::string& s, const std::string& ext) {
  if (s.length() < ext.length()) return false;
  return s.rfind(ext) == (s.length() - ext.length());
}

StatusCode stringToStatusCode(std::string input) {
    int statusCodeInt = atoi(input.c_str());
    return static_cast<StatusCode>(statusCodeInt);
}

std::string		getDefaultErrorPage(StatusCode status_code)
{
    std::map<StatusCode, std::string> DefaultErrorPages;

	DefaultErrorPages[BAD_REQUEST] = "BAD_REQUEST\r\n";
	DefaultErrorPages[UNAUTHORIZED] = "UNAUTHORIZED\r\n";
    DefaultErrorPages[FORBIDDEN] = "FORBIDDEN\r\n";
    DefaultErrorPages[NOT_FOUND] = "NOT_FOUND\r\n\r\n";
    DefaultErrorPages[LENGTH_REQUIRED] = "LENGTH_REQUIRED\r\n";
    DefaultErrorPages[CONTENT_TOO_LARGE] = "<html><body>Error 413: Content Too Large</body></html>\r\n";
    DefaultErrorPages[UNSUPPORTED_MEDIA_TYPE] = "UNSUPPORTED_MEDIA_TYPE\r\n";
    DefaultErrorPages[INTERNAL_SERVER_ERROR] = "INTERNAL_SERVER_ERROR\r\n";
    DefaultErrorPages[HTTP_VERSION_NOT_SUPPORTED] = "HTTP_VERSION_NOT_SUPPORTED\r\n";

	return DefaultErrorPages[status_code];
}