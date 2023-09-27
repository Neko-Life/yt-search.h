#!/bin/bash

cd build && cmake .. -DNLOHMANN_JSON_INCLUDE_DIR=$(pwd)/../../../include/ -DCURLPP_INCLUDE_DIR=$(pwd)/../../curlpp/include/ -DCURLPP_LIB_DIR=$(pwd)/../../curlpp/build/
