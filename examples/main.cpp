#include <iostream>
#include <twitter/client.hpp>

int main(int argc, char** argv) {
    Twitter::Client check;
    if(check.check_connection(cons_key,cons_secret))
        std::cout<<"Connected successfully"<<std::endl;

    Twitter::Client followers;
    followers.get_followers();
    std::vector<Twitter::Follower> follows = followers.get_followers();

    Twitter::Client print_follows;
    size_t number=3;
        
    print_follows.print_followers_thread(number, follows, argc == 2 && strcmp(argv[1], "-v") == 0);
    return 0;
}
