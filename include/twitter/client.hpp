#ifndef TWITTERCLIENT_CLIENT_HPP
#define TWITTERCLIENT_CLIENT_HPP

#include <string>
#include <map>
#include <curl/curl.h>
#include "json.hpp"
#include <mutex>

namespace Twitter {

    struct Follower{
        size_t id=0;
        size_t follower_count=0;
        std::string name="";
        std::string screen_name="";
        auto out() ->void{
            std::cout<< "followers count: " << follower_count << std::endl;
            std::cout << "id: " << id << std::endl;
            std::cout << "name: " << name <<std::endl;
            std::cout << "screen_name: " << screen_name << std::endl;
        }
    };

    class Client {
    public:
        using json=nlohmann::json;

        Client();
        Client(std::vector<Twitter::Follower>);
        ~Client();

        auto encode64(const std::string )->std::string;
        auto check_connection(const std::string , const std::string ) -> bool;


        auto print_followers_thread(size_t, const std::vector<Twitter::Follower>&, bool) -> bool;
        auto print_followers(const size_t, const size_t, const std::vector<Twitter::Follower>&, bool) -> void ;
        auto print() -> void;

        auto get_followers() -> std::vector<Follower>;

        auto check_connection_signature() -> bool;

        static auto write_to_string(void* data, size_t size, size_t nmemb, void* stream) -> size_t;
        bool flag;
    private:
        std::string bearer_token;
        CURL* Handle;
    };

}

const std::string cons_key="ZnmxBs7YbI7oD2bn5DMirBURD";
const std::string cons_secret="lmeLD9xncbPf63SLyKPII99PyplOvHu9tanFaT8vONyzMahnOc";



#endif //TWITTERCLIENT_CLIENT_HPP
