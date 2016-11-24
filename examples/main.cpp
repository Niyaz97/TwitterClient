#include <iostream>
#include <twitter/client.hpp>

int main() {
    Twitter::Client check;
    check.check_connection(cons_key,cons_secret);

    Twitter::Client followers;
    followers.get_followers();

    size_t number;
    std::cout <<  "Number of threads" << std::endl;
    std::cin >> number;

    Twitter::Client thread;
    thread.print_followers_thread(number);
    return 0;
}
