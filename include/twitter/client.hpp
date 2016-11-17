#ifndef TWITTERCLIENT_CLIENT_HPP
#define TWITTERCLIENT_CLIENT_HPP

#include <string>
#include <map>
#include <curl/curl.h>
#include <json.hpp>

namespace Twitter {
	class Client {
	public:
        using json=nlohmann::json;
        Client();
        ~Client();

        auto get_key(const std::string ) -> std::string;
        auto get_secret(const std::string ) -> std::string;

        auto encode64(const std::string )->std::string;
   		auto check_connection(const std::string , const std::string ) -> bool;
        auto get_followers() -> json;

        auto check_connection_signature() -> bool;

        static auto write_to_string(void* data, size_t size, size_t nmemb, void* stream) -> size_t;

    private:
        std::string bearer_token;
        CURL* Handle;
    };

}

const std::string cons_key="ZnmxBs7YbI7oD2bn5DMirBURD";
const std::string cons_secret="lmeLD9xncbPf63SLyKPII99PyplOvHu9tanFaT8vONyzMahnOc";




#endif //TWITTERCLIENT_CLIENT_HPP
