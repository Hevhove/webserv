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

	DefaultErrorPages[BAD_REQUEST] = "<html><body>BAD_REQUEST</body></html>\r\n";
	DefaultErrorPages[UNAUTHORIZED] = "<html><body>UNAUTHORIZED</body></html>\r\n";
    DefaultErrorPages[FORBIDDEN] = "<html><body>FORBIDDEN</body></html>\r\n";
    DefaultErrorPages[NOT_FOUND] = "<html><body>NOT_FOUND\r\n</body></html>\r\n";
    DefaultErrorPages[LENGTH_REQUIRED] = "<html><body>LENGTH_REQUIRED</body></html>\r\n";
    DefaultErrorPages[CONTENT_TOO_LARGE] = "<html><body>Error 413: Content Too Large</body></html>\r\n";
    DefaultErrorPages[UNSUPPORTED_MEDIA_TYPE] = "<html><body>UNSUPPORTED_MEDIA_TYPE</body></html>\r\n";
    DefaultErrorPages[INTERNAL_SERVER_ERROR] = "<html><body>INTERNAL_SERVER_ERROR</body></html>\r\n";
    DefaultErrorPages[HTTP_VERSION_NOT_SUPPORTED] = "<html><body>HTTP_VERSION_NOT_SUPPORTED</body></html>\r\n";

	return DefaultErrorPages[status_code];
}

bool is_subdirectory(std::string root_folder, std::string folder_name) {
    DIR* root = opendir(root_folder.c_str()); // open the root folder
    if (root == NULL) {
        return false; // root folder does not exist
    }

    bool is_subdir = false;
    struct dirent* entry;
    while ((entry = readdir(root)) != NULL) {
        if (entry->d_type == DT_DIR) {
            std::string sub_folder_name = std::string(entry->d_name);
            if (sub_folder_name == "." || sub_folder_name == "..") {
                continue; // skip the current and parent directory entries
            }
            std::string sub_folder_path = root_folder + "/" + sub_folder_name;
            if (sub_folder_name == folder_name) {
                is_subdir = true; // folder name matches the subdirectory name
                break;
            } else if (is_subdirectory(sub_folder_path, folder_name)) {
                is_subdir = true; // folder name is a subdirectory of the sub folder
                break;
            }
        }
    }

    closedir(root); // close the root folder
    return is_subdir;
}
