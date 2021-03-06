#include <twitter/client.hpp>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>


namespace Twitter{

    Twitter::Client::Client() : bearer_token(""){
        Handle = curl_easy_init();
    }
    Twitter::Client::~Client(){
        curl_easy_cleanup(Handle);
    }

    using json=nlohmann::json ;

    auto Twitter::Client::write_to_string(void *ptr, size_t size, size_t count, void *stream) -> size_t {
        ((std::string*)stream)->append((char*)ptr, 0, size*count);
        return size*count;
    }

    const std::string base64_padding[] = {"", "==","="};
    auto Twitter::Client::encode64(const std::string s)->std::string{
        namespace bai = boost::archive::iterators;

        std::stringstream os;
        // convert binary values to base64 characters
        typedef bai::base64_from_binary
        // retrieve 6 bit integers from a sequence of 8 bit bytes
                <bai::transform_width<const char *, 6, 8> > base64_enc; // compose all the above operations in to a new iterator

        std::copy(base64_enc(s.c_str()), base64_enc(s.c_str() + s.size()),
                  std::ostream_iterator<char>(os));

        os << base64_padding[s.size() % 3];
        return os.str();
    }



    auto Twitter::Client::check_connection(const std::string consumer_key, const std::string consumer_secret)-> bool {

        std::string encoded_token=encode64(consumer_key+":"+consumer_secret);
        std::string separator="&";
        CURLcode res;
        if(Handle){

            curl_easy_setopt(Handle, CURLOPT_URL, "https://api.twitter.com/oauth2/token");
            curl_easy_setopt(Handle, CURLOPT_POST, 1);

            curl_slist* slist=nullptr;
            std::string auth="Authorization: Basic "+encoded_token;
            slist=curl_slist_append(slist, auth.c_str());
            slist=curl_slist_append(slist, separator.c_str());
            slist=curl_slist_append(slist, "Content-Type: application/x-www-form-urlencoded");
            slist=curl_slist_append(slist, "charsets: utf-8");
            curl_easy_setopt(Handle, CURLOPT_HTTPHEADER, slist);

            std::string data="grant_type=client_credentials";
            // POST- запрос c авторизацией
            curl_easy_setopt(Handle, CURLOPT_POSTFIELDS, data.c_str() );
            curl_easy_setopt(Handle, CURLOPT_POSTFIELDSIZE, data.length() );
            curl_easy_setopt(Handle, CURLOPT_SSL_VERIFYPEER, 1);

            std::string content, header;
            //  сохраняем html код cтраницы в строку content
            curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEDATA,     &content);
            curl_easy_setopt(Handle, CURLOPT_HEADERFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEHEADER, &header);
            curl_easy_setopt(Handle, CURLOPT_VERBOSE, 1L);

            if(curl_easy_perform(Handle)==CURLE_OK) {
              try {
                  curl_slist_free_all(slist);
                  curl_easy_reset(Handle);

                  json jsn_obj = json::parse(content);
                  json jsn_token = jsn_obj.begin().value();
                  bearer_token = jsn_token.dump();
                  bearer_token.erase(bearer_token.begin());
                  bearer_token.erase(bearer_token.end()-1);
                  std::cout << "bearer_token= " << bearer_token << std::endl;
                  return true;
              }

                catch(const std::exception &except){
                    std::cerr<<except.what()<<std::endl;
                }
            }
            curl_easy_reset(Handle);

        }
        return false;
    }

    auto Twitter::Client::print_followers(json &followers) -> void{
        if( !followers.is_null()){
            for(json::iterator it = followers.begin(); it!=followers.end(); ++it) {

                json jsn_follow_c=it.value()["followers_count"];
                if(!jsn_follow_c.is_null())
                    std::cout<<"followers count: " << jsn_follow_c.begin().value() << std::endl;

                json jsn_id = it.value()["id"];
                if (!jsn_id.is_null())
                    std::cout << "id: " << jsn_id.begin().value() << std::endl;
                
                json jsn_name = it.value()["name"];
                if (!jsn_name.is_null())
                    std::cout << "name: " << jsn_name.begin().value()<<std::endl;

                json jsn_screen_name = it.value()["screen_name"];
                if (!jsn_screen_name.is_null())
                    std::cout << "screen_name: " << jsn_screen_name.begin().value() << std::endl;
                std::cout << std::endl;
            }
        }
    }

    auto Twitter::Client::get_followers()-> Client::json {

            if(Handle){
            std::string content, header;
            //  сохраняем html код cтраницы в строку content
            curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEDATA,     &content);
            curl_easy_setopt(Handle, CURLOPT_HEADERFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEHEADER, &header);

            curl_slist* slist=nullptr;
            std::string str = "Authorization: Bearer AAAAAAAAAAAAAAAAAAAAAPCkxgAAAAAANQPSyH7K0q6Hocmj1%2FscKemMKiM%3DzhdMAhdr30t9MP9CkaSPD7Jz6WbJ3gdWgxpX7n6I0ZwFQVDPdS";
            slist=curl_slist_append(slist, str.c_str());
            curl_easy_setopt(Handle, CURLOPT_HTTPHEADER, slist);

            std::string URL_REQUEST;
            URL_REQUEST="https://api.twitter.com/1.1/followers/list.json?cursor=-1&screen_name=niyaz160297&count=1";
            curl_easy_setopt(Handle, CURLOPT_URL, URL_REQUEST.c_str());
            curl_easy_setopt(Handle, CURLOPT_SSL_VERIFYPEER, 1);
            curl_easy_setopt(Handle, CURLOPT_VERBOSE, 1L);

            if(curl_easy_perform(Handle)==CURLE_OK){
                try {
                    json jsn_obj = json::parse(content);
                    json jsn_users = jsn_obj["users"];

                    print_followers(jsn_users);
                    curl_slist_free_all(slist);
                    curl_easy_reset(Handle);
                    return jsn_users;
                }
                catch(const std::exception &except){
                    std::cerr<<except.what()<<std::endl;
                }
            }
            curl_slist_free_all(slist);
            curl_easy_reset(Handle);
        }
    return nullptr;
    }

    auto Twitter::Client::check_connection_signature() -> bool {

        time_t t;
        time(&t);

        std::string ts = boost::lexical_cast<std::string>(t);

        if(Handle){
            curl_easy_setopt(Handle, CURLOPT_URL, "https://api.twitter.com/1.1/statuses/update.json");
            curl_easy_setopt(Handle, CURLOPT_POST, 1);
            curl_slist* authlist=nullptr;
            std::string ouath_consumer_key="ZnmxBs7YbI7oD2bn5DMirBURD&";
            std::string oauth_nonce=encode64(ts)+"&";
            std::string oauth_signature="&";
            std::string oauth_signature_method="HMAC-SHA1&";
            std::string oauth_timestamp=ts;
            std::string oauth_token="AAAAAAAAAAAAAAAAAAAAAPCkxgAAAAAANQPSyH7K0q6Hocmj1%2FscKemMKiM%3DzhdMAhdr30t9MP9CkaSPD7Jz6WbJ3gdWgxpX7n6I0ZwFQVDPdS&";
            std::string oauth_version="1.0";

            authlist=curl_slist_append(authlist, ouath_consumer_key.c_str());
            authlist=curl_slist_append(authlist, oauth_nonce.c_str());
           // authlist=curl_slist_append(authlist, oauth_signature.c_str());
            authlist=curl_slist_append(authlist, oauth_signature_method.c_str());
            authlist=curl_slist_append(authlist, oauth_timestamp.c_str());
            authlist=curl_slist_append(authlist, oauth_token.c_str());
            authlist=curl_slist_append(authlist, oauth_version.c_str());
            authlist=curl_slist_append(authlist, "Content-Type: application/x-www-form-urlencoded");

            std::string data="grant_type=client_credentials";
            // POST- запрос c авторизацией

            std::string content, header;
            //  сохраняем html код cтраницы в строку content
            curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEDATA,     &content);
            curl_easy_setopt(Handle, CURLOPT_HEADERFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEHEADER, &header);
            curl_easy_setopt(Handle, CURLOPT_VERBOSE, 1L);

//            curl_easy_setopt(Handle, CURLOPT_HEADER, 1); //заголовки ответа сервера будут отображаться вместе с html-кодом страницы

            if(curl_easy_perform(Handle)==CURLE_OK) {
                curl_slist_free_all(authlist);
                std::cout<<content<<std::endl;
                std::cout<<header<<std::endl;
                return true;
            }
            curl_easy_reset(Handle);
        }
        return false;
    }
}
