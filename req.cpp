#include "req.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string>
#include <typeinfo>
#include "json.hpp"

using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


curl_slist *get_header(CURL *curl) {
    struct curl_slist *chunk = NULL;

    chunk = curl_slist_append(chunk, "app_version: 6.9.4");
    chunk = curl_slist_append(chunk, "platform: ios");
    chunk = curl_slist_append(chunk, "content-type: application/json");
    chunk = curl_slist_append(chunk, "User-agent: Tinder/7.5.3 (iPhone; iOS 10.3.2; Scale/2.00)");
    chunk = curl_slist_append(chunk, "X-Auth-Token: 409fe586-eeb9-43b9-90ba-70fd07b0b8a0");

    return chunk;
}

std::string get_img(std::string url) {
  CURL *curl;
  CURLcode res;
	FILE *fp = nullptr;
  std::string readBuffer;
  std::string saveLocation = "test.jpg";
  curl = curl_easy_init();
  if (curl) {
		fp = fopen(saveLocation.c_str(), "wb");
		if (fp == NULL)
			std::cout << "failed to open file" << std::endl;
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
	fclose(fp);
  return saveLocation;
}

json request(std::string endpoint, std::string data, std::string method) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string url = "https://api.gotinder.com" + endpoint;

	curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, get_header(curl));
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (method == "POST" || method == "post") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        //write res handling
        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        auto j = json::parse(readBuffer);
        // make cust error handling right now just to cerr
        if (j["error"].size() > 0)
            std::cerr << j.dump(4) << std::endl;

        return j;
    }
}

json req() {

    std::string endpoint = "/user/recs";
    std::string data = "{\"last_activity_date\":  \"\"}";
    std::string method = "P";

    json r = request(endpoint, data, method);
//    std::cout << r.dump(4) << std::endl;
    return r;
}

