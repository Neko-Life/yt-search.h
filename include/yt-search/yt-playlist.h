#ifndef YT_PLAYLIST_H
#define YT_PLAYLIST_H

#include "nlohmann/json.hpp"
#include "yt-search/yt-search.h"
#include <string>
#include <vector>

namespace yt_search {
struct YPlaylist {
    nlohmann::json raw;
    int status;

    std::vector<YTrack> entries(bool force_use_side_track = false);
};

YPlaylist get_playlist(std::string url);
} // namespace yt_search

#endif
