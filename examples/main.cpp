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
    try {
        std::string s;
        std::cout << "Enter a command" << std::endl;
        std::cin >> s;
        if(!std::cin) throw std::invalid_argument("Wrong command");

        if (s == "get_followers_v") {
            _v = true;
            if (print_follows.print_followers_thread(number, follows, _v))
                std::cout << "Printed successfully" << std::endl;
        }

        if (s == "get_followers") {
            _v = false;
            if (print_follows.print_followers_thread(number, follows, _v))
                std::cout << "Printed successfully" << std::endl;
        }        
    } catch(const std::invalid_argument& e) {
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
}
