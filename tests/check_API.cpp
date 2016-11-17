#include <twitter/client.hpp>
#include "catch.hpp"

SCENARIO("get followers correctly")
{
    GIVEN("authorized client and json object with his followers")
    {
        Twitter::Client client;
  //      Twitter::Client:: json followers= R"([{"followers_count":66,"id":2202089073,"name":"timagol1k","screen_name":"74Toddler"}])"_json;
        WHEN("get followers")
        {
            Twitter::Client::json followers_from_get = client.get_followers();
           bool are_same;
                for(Twitter::Client::json::iterator it =Twitter::Client::json jsn_users.begin(); it!=jsn_users.end(); ++it) {

                   Twitter::Client::json jsn_follow_c=it.value()["followers_count"];
                   Twitter::Client::json jsn_id = it.value()["id"];
                   Twitter::Client::json jsn_name = it.value()["name"];
                   Twitter::Client::json jsn_screen_name = it.value()["screen_name"];
                    if(jsn_follow_c==66 && jsn_id==2202089073 && jsn_name=="timagol1k" && jsn_screen_name=="74Toddler") are_same=true;
            
            THEN("json objects are the same")
            {
                REQUIRE(are_same);
            }
        }
    }
}
