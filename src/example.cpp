#include "nlohmann/json.hpp"
#include "yt-search/encode.h"
#include "yt-search/yt-playlist.h"
#include "yt-search/yt-search.h"
#include "yt-search/yt-track-info.h"
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <fstream>
#include <regex>
#include <stdio.h>
#include <string>
#include <vector>

using namespace yt_search;

int main(int argc, const char *argv[]) {
    if (argc < 2)
    {
        fprintf(stderr, "Provide a search query\n");
        return 1;
    }

    std::string q = "";
    for (int a = 1; a < argc; a++) {
        q += q.length() ? (" " + std::string(argv[a])) : std::string(argv[a]);
    }
    const YSearchResult data = search(q);
    std::vector<YTrack> res = data.trackResults();
    // YPlaylist data = get_playlist(q);
    // std::vector<YTrack> res = data.entries();
    for (auto l : res) {
        printf("TITLE: %s, %s\n", l.title().c_str(), l.url().c_str());
        printf("DESC: %s\n", l.snippetText().c_str());
        printf("LENGTH: %s\n", l.length().c_str());
        YChannel c = l.channel();
        printf("CHANNEL: %s, %s\n", c.name.c_str(), c.url.c_str());
        for (auto t : l.thumbnails()) {
            printf("THUMBS: %d, %d, %s\n", t.height, t.width, t.url.c_str());
        }
        auto b = l.bestThumbnail();
        l.bestThumbnail();
        l.bestThumbnail();
        printf("BEST_T: %d, %d, %s\n", b.height, b.width, b.url.c_str());
        printf("T_PARAM: %s\n", l.trackingParams().c_str());
    }
    // printf("ESTIMATED_RESULT: %s\n", data.estimatedResults().c_str());
    printf("YET_ONLY_FOUND: %ld\n", res.size());
    printf("WHAT\n");

    if (res.size()) {
        auto r = res[0];
        auto d = get_track_info(r.url());
        auto p = d.audio_info(251);
        printf("\nitag: %d\n", p.itag());
        printf("audio_quality: %s\n", p.audio_quality().c_str());
        printf("average_bitrate: %ld\n", p.average_bitrate());
        printf("bitrate: %ld\n", p.bitrate());
        printf("channels: %d\n", p.channel());
        printf("duration: %ld\n", p.duration());
        printf("index_range: %ld, %ld\n", p.index_range().first, p.index_range().second);
        printf("init_range: %ld, %ld\n", p.init_range().first, p.init_range().second);
        printf("last_modified: %ld\n", p.last_modified());
        printf("length: %ld\n", p.length());
        printf("loudness: %lf\n", p.loudness());
        auto mt = p.mime_type();
        printf("mime_str: %s\n", mt.str.c_str());
        printf("mime_fmt: %s\n", mt.format.c_str());
        printf("mime_codecs: %s\n", mt.codecs.c_str());
        printf("mime_type: %s\n", mt.type.c_str());
        printf("projection: %s\n", p.projection_type().c_str());
        printf("quality: %s\n", p.quality().c_str());
        printf("sample_rate: %ld\n", p.sample_rate());
        printf("url: %s\n", p.url().c_str());
    }
    return 0;
}
