#ifndef YT_PLAYLIST_H
#define YT_PLAYLIST_H

#include "nlohmann/json.hpp"
#include "yt-search/yt-search.h"
#include <string>
#include <vector>

namespace yt_search {
struct YPlaylist {
    nlohmann::json raw;
    std::vector<YTrack> entries() const;
};

YPlaylist get_playlist(std::string url);
} // namespace yt_search

#endif
