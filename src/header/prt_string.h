#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

template <class... T>
std::string format(const char *fmt, const T &...t)
{
    const auto len = snprintf(nullptr, 0, fmt, t...);
    std::string r;
    r.resize(static_cast<size_t>(len) + 1);
    snprintf(&r.front(), len + 1, fmt, t...);
    r.resize(static_cast<size_t>(len));
    return r;
}

const std::vector<std::string> split(const std::string &str, const std::string &pattern)
{
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos)
    {
        if (end - begin != 0)
        {
            result.push_back(str.substr(begin, end - begin));
        }
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length())
    {
        result.push_back(str.substr(begin));
    }
    return result;
}
