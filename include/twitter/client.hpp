#ifndef TWITTERCLIENT_CLIENT_HPP
#define TWITTERCLIENT_CLIENT_HPP

#include <string>

namespace Twitter {
	class Client {
	public:

	auto check_connection() -> bool;
        auto get_followers() -> void;

        auto check_connection_signature() -> bool;

        static auto write_to_string(void* data, size_t size, size_t nmemb, void* stream) -> size_t;
    };

}


#endif //TWITTERCLIENT_CLIENT_HPP
