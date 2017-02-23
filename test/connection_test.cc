#include "connection.h"
#include <gtest/gtest.h>

using namespace std;

TEST(connection_test,prefix_test){
     vector<string> uris={"/foo/bar","/foo"};
     string uri="/foo/bar/a.txt";

     string result=connection::longest_prefix_matching(uris,uri);
     EXPECT_EQ(result,"/foo/bar");
}