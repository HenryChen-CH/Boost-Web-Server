#include "gtest/gtest.h"
#include "mime_types.h"
#include <string>

TEST(mime_types_test, test1){
	std::string extension = "jpg";
	std::string mime_type =  mime_types::extension_to_type(extension);
	EXPECT_EQ(mime_type, "image/jpeg");
}

TEST(mime_types_test, test2){
	std::string extension = "htm";
	std::string mime_type = mime_types::extension_to_type(extension);
	EXPECT_EQ(mime_type, "text/html");
}
