#ifndef YT_TRACK_INFO_H
#define YT_TRACK_INFO_H

#include "nlohmann/json.hpp"

struct YTInfo {
    nlohmann::json raw;
    bool empty();
};

YTInfo get_track_info(std::string url);

#endif
