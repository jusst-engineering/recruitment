// g++ example.cpp -o example -lcurl -ljsoncpp

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  curl = curl_easy_init();
      if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://time.jsontest.com/");
            //curl_easy_setopt(curl, CURLOPT_URL, "http://0.0.0.0:8808/ws");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
      }
    Json::Reader reader;
    Json::Value obj;
    reader.parse(readBuffer, obj); 

    std::cout << obj["time"].asString() << std::endl;
    //std::cout << obj.asString() << std::endl;
  return 0;
}
