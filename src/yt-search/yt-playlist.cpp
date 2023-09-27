#include "yt-search/yt-playlist.h"
#include "nlohmann/json.hpp"
#include "yt-search/yt-search.h"
#include <vector>

namespace yt_search {
using json = nlohmann::json;
std::vector<YTrack> YPlaylist::entries(bool force_use_side_track) {
    std::vector<YTrack> res;
    json d = raw;

    if (!force_use_side_track) {
        for (auto &i : {"contents", "twoColumnWatchNextResults", "playlist", "playlist", "contents"}) {
            // printf("%s\n\n", d[i].dump(2).c_str());
            d = d.value<json>(i, {});
            if (d.is_null()) {
                fprintf(stderr, "[YPLAYLIST_ENTRIES] d[%s].is_null\n", i);
                return res;
            }
        }

        if (d.is_array())
            for (auto i = d.begin(); i != d.end(); i++) {
                json data = i->value<json>("playlistPanelVideoRenderer", {});
                if (data.is_null())
                    continue;
                else
                    res.push_back({data});
            }
    }

    if (force_use_side_track || !res.size()) {
        YSearchResult y = { d };

        res = y.sideTrackPlaylist();
        status = 1;
    } else
        status = 0;

    return res;
}

YPlaylist get_playlist(std::string url) {
    YPlaylist ret;
    get_data(url, &ret);
    return ret;
}
} // namespace yt_search
