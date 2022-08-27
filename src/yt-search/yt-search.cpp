/* Very simple YouTube track searching function. Made with love by Neko Life :heart: */

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "nlohmann/json.hpp"
#include "yt-search/encode.h"
#include "yt-search/yt-search.h"

namespace yt_search {
    bool transverse(nlohmann::json& j, const std::initializer_list<const char*> path) {
        for (auto& i : path)
        {
            j = j.size() ? (j.is_array() ? j.at(strtoul(i, NULL, 10)) : j.value<nlohmann::json>(i, {})) : nlohmann::json();
            if (j.is_null())
            {
                fprintf(stderr, "[YT_SEARCH_TRANSVERSE] j[%s].is_null\n", i);
                return false;
            }
        }
        return true;
    }

    using json = nlohmann::json;

    std::string YTrack::snippetText() const {
        std::string res = "";
        json d = raw;

        if (transverse(d, { "detailedMetadataSnippets", "0", "snippetText", "runs" }))
            if (d.size() && d.is_array()) for (auto i = d.begin(); i != d.end(); i++)
            {
                if (i->is_null()) continue;
                std::string text = i->value<std::string>("text", "");

                if (i->value<bool>("bold", false))
                    res += "**" + text + "**";
                else res += text;
            }

        return res;
    }

    std::string YTrack::id() const {
        return raw.is_null() ? "" : raw.value<std::string>("videoId", "");
    }

    std::string YTrack::url() const {
        return std::string("https://www.youtube.com/watch?v=") + id();
    }

    std::string YTrack::trackingParams() const {
        return raw.is_null() ? "" : raw.value<std::string>("trackingParams", "");
    }

    std::vector<YThumbnail> YTrack::thumbnails() const {
        std::vector<YThumbnail> res;
        json d = raw;

        if (transverse(d, { "thumbnail", "thumbnails" }))
            if (d.size() && d.is_array()) for (auto i = d.begin(); i != d.end(); i++)
            {
                if (i->is_null()) continue;
                res.push_back({
                    i->value<int>("height", 0),
                    i->value<std::string>("url",""),
                    i->value<int>("width",0)
                    });
            }

        return res;
    }

    YThumbnail YTrack::bestThumbnail() const {
        return thumbnails().back();
    }

    // Track length
    // TODO: Parse to ms
    std::string YTrack::length() const {
        json d = raw;

        return transverse(d, { "lengthText" }) ? d.value<std::string>("simpleText", "") : "";
    }

    YChannel YTrack::channel() const {
        json d = raw;

        if (!transverse(d, { "longBylineText","runs","0" })) return {};

        json m = d;

        return {
            d.value<std::string>("text",""),
            transverse(m, { "navigationEndpoint","commandMetadata","webCommandMetadata","url" })
            ? std::string("https://www.youtube.com") + m.get<std::string>()
            : ""
        };
    }

    std::string YTrack::title() const {
        json d = raw;

        if (!transverse(d, { "title","runs","0","text" }))
        {
            d = raw;

            if (!transverse(d, { "title", "simpleText" })) return "";
        }

        return d.get<std::string>();
    }

    // This is interesting
    std::string YSearchResult::estimatedResults() const {
        return raw.is_null() ? "" : raw.value<std::string>("estimatedResults", "");
    }

    std::vector<YTrack> YSearchResult::trackResults() const {
        std::vector<YTrack> res;
        json d = raw;

        transverse(d, { "contents",
        "twoColumnSearchResultsRenderer",
        "primaryContents",
        "sectionListRenderer",
        "contents",
        "0",
        "itemSectionRenderer",
        "contents" });

        if (!d.size() || (d.is_array() && d.at(0).is_null())) return res;

        for (size_t i = 0; i < d.size(); i++)
        {
            json data = d;

            if (!transverse(data, { std::to_string(i).c_str(),"videoRenderer" })) continue;

            res.push_back({
                data
                });
        }

        return res;
    }

    YSearchResult search(std::string search) {
        YSearchResult ret;
        get_data(std::string("https://www.youtube.com/results?search_query=") + encodeURIComponent(search), &ret);
        return ret;
    }
}
