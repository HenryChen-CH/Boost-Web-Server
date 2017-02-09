#include "gtest/gtest.h"
#include "mime_types.hpp"

using namespace std;

TEST(mime_types_test,test1){
	string extension="jpg";
	string mime_type=mime_types::extension_to_type(extension);
	EXPECT_EQ(mime_type,"image/jpeg");
}

TEST(mime_types_test,test2){
	string extension="htm";
	string mime_type=mime_types::extension_to_type(extension);
	EXPECT_EQ(mime_type,"text/html");
}
