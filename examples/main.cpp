#include <iostream>
#include <twitter/client.hpp>

int main() {
    Twitter::Client check;
    check.check_connection(cons_key,cons_secret);

    Twitter::Client followers;
    followers.get_followers();
    std::vector<Twitter::Follower> follows = followers.get_followers();

    bool _v;
    std::string s;
    std::cout<<"Enter a command"<<std::endl;
    std::cin>> s;
    if(s=="get_followers_v"){
        _v=true;
        size_t number;
        std::cout <<  "Number of threads:" << std::endl;
        std::cin >> number;

        Twitter::Client thread;
        thread.print_followers_thread(number, follows, _v);
    }

    if(s=="get_followers"){
        _v=false;
        size_t number;
        std::cout <<  "Number of threads:" << std::endl;
        std::cin >> number;

        Twitter::Client thread;
        thread.print_followers_thread(number, follows, _v);
    }

    return 0;
}
