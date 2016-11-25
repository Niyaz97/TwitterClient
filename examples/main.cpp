#include <iostream>
#include <twitter/client.hpp>

int main() {
    Twitter::Client check;
    if(check.check_connection(cons_key,cons_secret))
        std::cout<<"Connected successfully"<<std::endl;

    Twitter::Client followers;
    followers.get_followers();
    std::vector<Twitter::Follower> follows = followers.get_followers();

    Twitter::Client print_follows;
    size_t number;
    bool _v;
    std::string s;
    std::cout<<"Enter a command"<<std::endl;
    std::cin>> s;
    if(s=="get_followers_v"){
        _v=true;
        if(print_follows.print_followers_thread(number, follows, _v))
            std::cout<<"Printed successfully"<<std::endl;
    }

    if(s=="get_followers"){
        _v=false;
        if(print_follows.print_followers_thread(number, follows, _v))
            std::cout<<"Printed successfully"<<std::endl;
    }

    else{
        std::cout<<"Wrong command"<<std::endl;
    }

    return 0;
}
