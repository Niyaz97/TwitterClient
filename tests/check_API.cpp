#include <twitter/client.hpp>
#include "catch.hpp"

SCENARIO("get followers correctly")
{
    GIVEN("authorized client and json object with his followers")
    {
        Twitter::Client client;
        Twitter::Client:: json followers= R"([{"id":2202089073,"name":"timagol1k","screen_name":"74Toddler","followers_count":66}])"_json;
        WHEN("get followers")
        {
            Twitter::Client::json followers_from_get = client.get_followers();
            bool are_same = (followers == followers_from_get);

            THEN("json objects are the same")
            {
                REQUIRE(are_same);
            }
        }
    }
}
