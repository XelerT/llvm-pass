#include <fstream>
#include <filesystem>

inline auto read_file (std::string &file_path)
{
        if (!std::filesystem::exists(file_path))
                throw std::runtime_error("No such file.");
        
        std::ifstream input(file_path);
        std::string str {};
        input.seekg(0, std::ios::end);
        str.reserve(input.tellg());
        input.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(input)),
                    std::istreambuf_iterator<char>());
        
        return str;
}
