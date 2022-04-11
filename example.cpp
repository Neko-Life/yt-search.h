#include <stdio.h>
#include <string>
#include <regex>
#include <fstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "nlohmann/json.hpp"
#include "encode.h"
#include "yt-search.h"

int main(int argc, const char* argv[]) {
    std::string q;
    for (int a = 1; a < argc; a++)
    {
        q += std::string(argv[a]);
    }
    YSearchResult data = yt_search(q);
    for (auto l : data.trackResults())
    {
        printf("TITLE: %s, %s\n", l.title().c_str(), l.url().c_str());
        printf("DESC: %s\n", l.snippetText().c_str());
        printf("LENGTH: %s\n", l.length().c_str());
        YChannel c = l.channel();
        printf("CHANNEL: %s, %s\n", c.name.c_str(), c.url.c_str());
        for (auto t : l.thumbnails())
        {
            printf("THUMBS: %d, %d, %s\n", t.height, t.width, t.url.c_str());
        }
        auto b = l.bestThumbnail();
        printf("BEST_T: %d, %d, %s\n", b.height, b.width, b.url.c_str());
        printf("T_PARAM: %s\n", l.trackingParams().c_str());
    }
    printf("ESTIMATED_RESULT: %s\n", data.estimatedResults().c_str());
    printf("YET_ONLY_FOUND: %ld\n", data.count());
    printf("WHAT\n");
    return 0;
}
