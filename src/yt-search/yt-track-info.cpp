#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <ostream>
#include "nlohmann/json.hpp"
#include "yt-search/yt-track-info.h"

using json = nlohmann::json;

// "audio/webm; codecs=\"opus\""
// "video/mp4; codecs=\"avc1.4d401f\""

Mime_Type::Mime_Type() : str("") {}

Mime_Type::Mime_Type(std::string str) {
    if (!str.length())
    {
        this->str = "";
        return;
    }
    this->str = str;
    size_t l_type_index = str.find('/');
    if (l_type_index)
    {
        this->format = str.substr(0, l_type_index);
        size_t l_format_index = str.find(';');
        if (l_format_index) this->format = str.substr(l_type_index + 1, l_format_index - (l_type_index + 1));
    }
    size_t l_codecs_index = str.find('"');
    if (l_codecs_index) this->codecs = str.substr(l_codecs_index, str.length() - l_codecs_index);
}

Mime_Type::~Mime_Type() = default;

YTInfo get_track_info(std::string url) {
    std::ostringstream os;

    curlpp::Cleanup curl_cleanup;

    curlpp::Easy req;

    req.setOpt(curlpp::options::Url(url));
    req.setOpt(curlpp::options::Header("User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:98.0) Gecko/20100101 Firefox/98.0"));
    req.setOpt(curlpp::options::WriteStream(&os));
    req.perform();

    // MAGIC INIT
    const std::string rawhttp = os.str();

    static const std::string var = "var ytInitialPlayerResponse = ";
    static const std::string end = "};";

    const size_t sI = rawhttp.find(var);

    YTInfo data;
    if (sI == std::string::npos)
    {
        // If this getting printed to the console, the magic may be expired
        fprintf(stderr, "Not a valid youtube page (or youtube update, yk they like to change stuffs)\nvar_start: %ld\n", sI);
        return data;
    }

    const size_t eI = rawhttp.find(end, sI);
    if (eI == std::string::npos)
    {
        // If this getting printed to the console, the magic may be expired
        fprintf(stderr, "Not a valid youtube page (or youtube update, yk they like to change stuffs)\nvar_start: %ld\nvar_end: %ld\n", sI, eI);
        return data;
    }

    // MAGIC FINALIZE
    const size_t am = sI + var.length();
    const std::string tJson = rawhttp.substr(am, eI - am + 1);
    data.raw = json::parse(tJson);
    return data;
}
