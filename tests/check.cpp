#include <twitter/client.hpp>
#include "catch.hpp"

SCENARIO("tokens are valid")
{
    GIVEN("valid consumer_key and consumer_secret")
    {
        Twitter::Client client;
        WHEN("checking connection")
        {
            THEN("success")
            {
                REQUIRE_TRUE(client.check_connection(cons_key, cons_secret));
            }
        }
    }
}
SCENARIO("tokens are not valid")
{
    GIVEN("not valid consumer_key and consumer_secret")
    {
        Twitter::Client client;
        WHEN("checking connection")
        {
            THEN("failure")
            {
                REQUIRE_FALSE(client.check_connection("Wm5teEJzN1liSTdvRDJibjVETWlyQligjlfeeqfVSRA", "Wm5hfowefjteEJzN1liSTdvRDJibjVETWlyQlVSRA"));
            }
        }
    }
}
