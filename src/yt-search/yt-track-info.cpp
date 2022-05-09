#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <ostream>
#include "nlohmann/json.hpp"
#include "yt-search/yt-track-info.h"

using json = nlohmann::json;

bool YTInfo::empty() {
    return raw.is_null();
};

YTInfo get_track_info(std::string url) {
    curlpp::Cleanup curl_cleanup;

    std::ostringstream os;
    curlpp::Easy req;

    req.setOpt(curlpp::options::Url(url));
    req.setOpt(curlpp::options::Header("User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:98.0) Gecko/20100101 Firefox/98.0"));
    req.setOpt(curlpp::options::WriteStream(&os));
    req.perform();

    std::string rawhttp = os.str();

    static const char var[31] = "var ytInitialPlayerResponse = ";
    static const char end[3] = "};";

    bool sw = false;
    size_t sI = -1;
    size_t eI = -1;

    for (size_t i = 0; i < rawhttp.length(); i++)
    {
        if (sw == false)
            for (size_t v = 0; v < sizeof(var) - 1; v++)
            {
                if (rawhttp[i + v] != var[v]) break;
                if (v == sizeof(var) - 2)
                {
                    sI = i + sizeof(var) - 1;
                    sw = true;
                }
            }
        else for (size_t v = 0; v < sizeof(end) - 1; v++)
        {
            if (rawhttp[i + v] != end[v]) break;
            if (v)
            {
                eI = i + sizeof(end) - 2;
                break;
            }
        }
        if ((int)eI > -1) break;
    }

    YTInfo data;
    if ((int)sI < 0 || (int)eI < 0)
    {
        fprintf(stderr, "Not a valid youtube watch url (or youtube update, yk they like to change stuffs)\nvar_start: %ld\nvar_end: %ld\n", sI, eI);
        return data;
    }

    std::string tJson = rawhttp.replace(rawhttp.begin() + eI, rawhttp.end(), "").replace(rawhttp.begin(), rawhttp.begin() + sI, "");
    data.raw = json::parse(tJson);
    return data;
}
