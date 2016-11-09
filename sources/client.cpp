#include <twitter/client.hpp>
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <../include/json.hpp>

namespace Twitter{
    using json=nlohmann::json ;

    auto Twitter::Client::write_to_string(void *ptr, size_t size, size_t count, void *stream) -> size_t
    {
        ((std::string*)stream)->append((char*)ptr, 0, size*count);
        return size*count;
    }

    auto Twitter::Client::check_connection()-> bool {

        std::string separator="&";

        CURL *Handle = curl_easy_init();
        CURLcode res;

        if(Handle){

            curl_easy_setopt(Handle, CURLOPT_URL, "https://api.twitter.com/oauth2/token");
            curl_easy_setopt(Handle, CURLOPT_POST, 1);

            curl_slist* slist=nullptr;

            slist=curl_slist_append(slist, "Authorization: Basic Wm5teEJzN1liSTdvRDJibjVETWlyQlVSRDpsbWVMRDl4bmNiUGY2M1NMeUtQSUk5OVB5cGxPdkh1OXRhbkZhVDh2T055ek1haG5PYw==");
            slist=curl_slist_append(slist, separator.c_str());
            slist=curl_slist_append(slist, "Content-Type: application/x-www-form-urlencoded");
            slist=curl_slist_append(slist, "charsets: utf-8");
            curl_easy_setopt(Handle, CURLOPT_HTTPHEADER, slist);

            std::string data="grant_type=client_credentials";
            // POST- запрос c авторизацией
            curl_easy_setopt(Handle, CURLOPT_POSTFIELDS, data.c_str() );
            curl_easy_setopt(Handle, CURLOPT_POSTFIELDSIZE, data.length() );

            curl_easy_setopt(Handle, CURLOPT_SSL_VERIFYPEER, 1);

            std::string content; std::string header;

            //  сохраняем html код cтраницы в строку content
            curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEDATA,     &content);

            // Загловок ответа сервера выводим в консоль
            curl_easy_setopt(Handle, CURLOPT_HEADERFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEHEADER, &header);

            curl_easy_setopt(Handle, CURLOPT_VERBOSE, 1L);

//            curl_easy_setopt(Handle, CURLOPT_HEADER, 1); //заголовки ответа сервера будут отображаться вместе с html-кодом страницы

            res = curl_easy_perform(Handle);
            curl_slist_free_all(slist);

            std::cout<< curl_easy_strerror(res) << std::endl;
            std::cout << "content: " << std::endl << content << std::endl;
            std::cout << "header: " << std::endl << header << std::endl;

            curl_easy_cleanup(Handle);
        }
        return true;
    }

    auto Twitter::Client::get_followers()-> void {
        CURL *Handle=curl_easy_init();

        if(Handle){
            std::string content; std::string header;

            //  сохраняем html код cтраницы в строку content
            curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEDATA,     &content);

            // Загловок ответа сервера выводим в консоль
            curl_easy_setopt(Handle, CURLOPT_HEADERFUNCTION, write_to_string);
            curl_easy_setopt(Handle, CURLOPT_WRITEHEADER, &header);

            curl_slist* slist=nullptr;
            slist=curl_slist_append(slist, "Authorization: Bearer AAAAAAAAAAAAAAAAAAAAAPCkxgAAAAAANQPSyH7K0q6Hocmj1%2FscKemMKiM%3DzhdMAhdr30t9MP9CkaSPD7Jz6WbJ3gdWgxpX7n6I0ZwFQVDPdS");
            curl_easy_setopt(Handle, CURLOPT_HTTPHEADER, slist);

            std::string URL_REQUEST;

            URL_REQUEST="https://api.twitter.com/1.1/followers/list.json?cursor=-1&screen_name=niyaz160297";

            curl_easy_setopt(Handle, CURLOPT_URL, URL_REQUEST.c_str());
            curl_easy_setopt(Handle, CURLOPT_SSL_VERIFYPEER, 1);
            curl_easy_setopt(Handle, CURLOPT_VERBOSE, 1L);

            if(curl_easy_perform(Handle)==CURLE_OK){

                json jsn_obj=json::parse(content);
                json jsn_users=jsn_obj["users"];
                int count = 0;
                for(json::iterator it = jsn_users.begin(); it!=jsn_users.end(); ++it) {

                    std::cout << ++count << ")";

                    json jsn_id = it.value()["id"];
                    if (!jsn_id.is_null())
                        std::cout << "id: " << jsn_id.begin().value() << std::endl;

                    json jsn_id_str = it.value()["id_str"];
                    if (!jsn_id_str.is_null())
                        std::cout << "id_string: " << jsn_id_str.begin().value() << std::endl;

                    json jsn_name = it.value()["name"];
                    if (!jsn_name.is_null())
                        std::cout << "name: " << jsn_name.begin().value()<<std::endl;

                    json jsn_screen_name = it.value()["screen_name"];
                    if (!jsn_screen_name.is_null())
                        std::cout << "screen_name: " << jsn_screen_name.begin().value() << std::endl;

                    json jsn_location = it.value()["location"];
                    if (!jsn_location.is_null())
                        std::cout << "location:  " << jsn_location.begin().value() << std::endl;

                    json jsn_follow_c=it.value()["followers_count"];
                    if(!jsn_follow_c.is_null())
                        std::cout<<"followers count: " << jsn_follow_c.begin().value() << std::endl;

                    std::cout << std::endl;
                }
            }


            std::cout<<"header:"<<header<<std::endl;
            std::cout<<content<< std::endl;
            curl_slist_free_all(slist);

        }


    }

    auto Twitter::Client::check_connection_signature() -> bool {
        CURL *Handle=curl_easy_init();

        std::string separator="&";

        if(Handle){
            curl_slist* authlist=nullptr;
            std::string ouath_consumer_key="ZnmxBs7YbI7oD2bn5DMirBURD&";
            std::string oauth_nonce="&";
            std::string oauth_signature="&";
            std::string oauth_signature_method="HMAC-SHA1&";
            //std::string oauth_timestamp="";
            std::string oauth_token="AAAAAAAAAAAAAAAAAAAAAPCkxgAAAAAANQPSyH7K0q6Hocmj1%2FscKemMKiM%3DzhdMAhdr30t9MP9CkaSPD7Jz6WbJ3gdWgxpX7n6I0ZwFQVDPdS&";
            std::string oauth_version="1.0";

            authlist=curl_slist_append(authlist, ouath_consumer_key.c_str());
            authlist=curl_slist_append(authlist, oauth_nonce.c_str());
//            authlist=curl_slist_append(authlist, oauth_signature.c_str());
            authlist=curl_slist_append(authlist, oauth_signature_method.c_str());
            //authlist=curl_slist_append(authlist, timestamp);
            authlist=curl_slist_append(authlist, oauth_token.c_str());
            authlist=curl_slist_append(authlist, oauth_version.c_str());
            authlist=curl_slist_append(authlist, "Content-Type: application/x-www-form-urlencoded");


        }
    }
}
