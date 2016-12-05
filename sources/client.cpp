#include <twitter/client.hpp>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>

namespace Twitter{
    std::mutex mutex;

    int count=0;
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

            if(curl_easy_perform(Handle)==CURLE_OK) {
                try {
                    curl_slist_free_all(slist);
                    curl_easy_reset(Handle);

                    json jsn_obj = json::parse(content);
                    json jsn_token = jsn_obj.begin().value();
                    bearer_token = jsn_token.dump();
                    bearer_token.erase(bearer_token.begin());
                    bearer_token.erase(bearer_token.end()-1);
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

    auto Twitter::Client::print_followers(const size_t thread_num, const size_t n, const std::vector<Twitter::Follower>& followers, bool v) -> void {


        for(size_t i = thread_num; i < followers.size(); i += n)
        {
            std::lock_guard<std::mutex> lk(mutex);
            std::cout << "Thread " << thread_num + 1 << std::endl;
           if(v == true) {
               std::time_t start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
               std::cout << "Start time: " << ctime(&start);
               std::cout << followers[i].screen_name << std::endl;
               std::time_t end = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
               std::cout << "End time: " << ctime(&end);
           }
           else {
               std::cout << followers[i].screen_name << std::endl;
               std::cout << followers[i].id << std::endl<<std::endl;
           }
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
        }
    }

    auto Twitter::Client::print_followers_thread(size_t n, const std::vector<Twitter::Follower>& followers, bool v) -> bool {
        count =0;
        if (n < 1 || n > std::thread::hardware_concurrency()) {
            std::cerr << "error" << std::endl;
            return false;
        }

        std::vector<std::thread> follow_thread;

        for (size_t i = 0; i < n; ++i) {
            follow_thread.push_back(std::thread(&Twitter::Client::print_followers, this, i, n, std::ref(followers), v));
        }

        for (size_t i = 0; i < n; ++i) {
            if (follow_thread[i].joinable())
                follow_thread[i].join();
        }

        return true;
    }

    auto Twitter::Client::get_followers()-> std::vector<Follower> {
        std::vector<Twitter::Follower> result;
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
            URL_REQUEST="https://api.twitter.com/1.1/followers/list.json?cursor=-1&screen_name=niyaz160297";
            curl_easy_setopt(Handle, CURLOPT_URL, URL_REQUEST.c_str());
            curl_easy_setopt(Handle, CURLOPT_SSL_VERIFYPEER, 1);

            if(curl_easy_perform(Handle)==CURLE_OK){
                try {
                    json jsn_obj = json::parse(content);
                    json jsn_users = jsn_obj["users"];
                    for(json::iterator it = jsn_users.begin(); it!=jsn_users.end(); ++it) {
                        Follower follow;

                        json jsn_follow_c=it.value()["followers_count"];
                        if(!jsn_follow_c.is_null())
                            follow.follower_count=jsn_follow_c.begin().value();

                        json jsn_id = it.value()["id"];
                        if (!jsn_id.is_null())
                            follow.id=jsn_id.begin().value();

                        json jsn_name = it.value()["name"];
                        if (!jsn_name.is_null())
                            follow.name=jsn_name.begin().value();

                        json jsn_screen_name = it.value()["screen_name"];
                        if (!jsn_screen_name.is_null())
                            follow.screen_name=jsn_screen_name.begin().value();

                        result.push_back(follow);
                    }
                    curl_slist_free_all(slist);
                    curl_easy_reset(Handle);
                    return result;
                }
                catch(const std::exception &except){
                    std::cerr<<except.what()<<std::endl;
                }
            }
            curl_slist_free_all(slist);
            curl_easy_reset(Handle);
        }
        return result;
    }
}
