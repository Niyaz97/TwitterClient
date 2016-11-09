#include <iostream>
#include <twitter/client.hpp>

int main() {
    Twitter::Client check;
    check.check_connection();

    Twitter::Client followers;
    followers.get_followers();
}
