/* Very simple YouTube track searching function. Made with love by Neko Life :heart: */

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "nlohmann/json.hpp"
#include "yt-search/encode.h"
#include "yt-search/yt-search.h"

namespace yt_search {
    using json = nlohmann::json;

    std::string YTrack::snippetText() {
        std::string res = "";
        json d = raw["detailedMetadataSnippets"][0]["snippetText"]["runs"];
        if (d[0].is_null()) return res;

        for (size_t i = 0; i < d.size(); i++)
        {
            json data = d[i];
            bool bold = data["bold"].is_null() ? false : data["bold"].get<bool>();
            std::string text = data["text"].is_null() ? "" : data["text"].get<std::string>();

            if (bold) res += "**" + text + "**";
            else res += text;
        }
        return res;
    }

    std::string YTrack::id() {
        return raw["videoId"].is_null() ? "" : raw["videoId"].get<std::string>();
    }

    std::string YTrack::url() {
        return std::string("https://www.youtube.com/watch?v=") + id();
    }

    std::string YTrack::trackingParams() {
        return raw["trackingParams"].is_null() ? "" : raw["trackingParams"].get<std::string>();
    }

    std::vector<YThumbnail> YTrack::thumbnails() {
        std::vector<YThumbnail> res;
        json d = raw["thumbnail"]["thumbnails"];
        if (d[0].is_null()) return res;
        for (size_t i = 0; i < d.size(); i++)
        {
            json data = d[i];
            res.push_back({
                data["height"].get<int>(),
                data["url"].get<std::string>(),
                data["width"].get<int>()
            });
        }
        return res;
    }

    YThumbnail YTrack::bestThumbnail() {
        return thumbnails().back();
    }

    // Track length
    // TODO: Parse to ms
    std::string YTrack::length() {
        json res = raw["lengthText"]["simpleText"];
        if (res.is_null()) return "";
        return res.get<std::string>();
    }

    YChannel YTrack::channel() {
        json d = raw["ownerText"]["runs"][0];
        if (d.is_null()) return {};
        json m = d["navigationEndpoint"]["commandMetadata"]["webCommandMetadata"]["url"];
        return {
            d["text"].is_null() ? "" : d["text"].get<std::string>(),
            m.is_null() ? "" : std::string("https://www.youtube.com") + m.get<std::string>()
        };
    }

    std::string YTrack::title() {
        json d = raw["title"]["runs"][0]["text"];
        if (d.is_null()) return "";
        return d.get<std::string>();
    }

    // This is interesting
    std::string YSearchResult::estimatedResults() {
        return raw["estimatedResults"];
    }

    std::vector<YTrack> YSearchResult::trackResults() {
        std::vector<YTrack> res;
        json d = raw["contents"]["twoColumnSearchResultsRenderer"]["primaryContents"]["sectionListRenderer"]["contents"][0]["itemSectionRenderer"]["contents"];
        if (d[0].is_null()) return res;

        for (size_t i = 0; i < d.size(); i++)
        {
            json data = d[i]["videoRenderer"];
            if (data.is_null()) continue;
            res.push_back({
                data
            });
        }
        return res;
    }

    size_t YSearchResult::count() {
        return trackResults().size();
    }

    YSearchResult search(std::string search) {
        const std::string ec = encodeURIComponent(std::string(search));
        std::ostringstream os;

        curlpp::Cleanup curl_cleanup;

        curlpp::Easy req;

        req.setOpt(curlpp::options::Url(std::string("https://www.youtube.com/results?search_query=") + ec));
        req.setOpt(curlpp::options::Header("User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:98.0) Gecko/20100101 Firefox/98.0"));
        req.setOpt(curlpp::options::WriteStream(&os));
        req.perform();

        // MAGIC INIT
        const std::string rawhttp = os.str();

        static const std::string var = "var ytInitialData = ";
        static const std::string end = "};";

        const size_t sI = rawhttp.find(var);

        YSearchResult data;
        if (sI == std::string::npos)
        {
            // If this getting printed to the console, the magic may be expired
            fprintf(stderr, "Not a valid youtube search query (or youtube update, yk they like to change stuffs)\nvar_start: %ld\n", sI);
            return data;
        }

        const size_t eI = rawhttp.find(end, sI);
        if (eI == std::string::npos)
        {
            // If this getting printed to the console, the magic may be expired
            fprintf(stderr, "Not a valid youtube search query (or youtube update, yk they like to change stuffs)\nvar_start: %ld\nvar_end: %ld\n", sI, eI);
            return data;
        }

        // MAGIC FINALIZE
        const size_t am = sI + var.length();
        const std::string tJson = rawhttp.substr(am, eI - am + 1);
        data.raw = json::parse(tJson);
        return data;
    }
}
