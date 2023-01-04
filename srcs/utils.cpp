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
    std::ifstream file(resource);

    return (file.good());
}

bool hasFileExtension(const std::string& s, const std::string& ext) {
  if (s.length() < ext.length()) return false;
  return s.rfind(ext) == (s.length() - ext.length());
}
