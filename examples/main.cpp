#include <iostream>
#include <twitter/client.hpp>

int main() {
    Twitter::Client check;
    check.check_connection(cons_key,cons_secret);

    Twitter::Client followers;
    followers.get_followers();
}
