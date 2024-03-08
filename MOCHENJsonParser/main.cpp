#define _CRTDBG_MAP_ALLOC   // 使打印的泄漏报告更详细
#include <stdlib.h>         
#include <crtdbg.h>          
#include <windows.h>        // 用于计算运行时间

#include "json/json.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>



/*
测试[]
append &&
insert 操作了 null_json


*/



using namespace mochen::json;



void test01()
{
	std::string str = "k1";
	Json j1{};
	j1[str] = 123;
	j1["k2"] = 456.789;
	j1["k3"] = true;
	j1["k4"] = "qwert";
	printf("%s\n", j1.to_string().c_str());


	std::cout << j1["k1"].get_int() << " "
		<< j1["k2"].get_double() << " "
		<< j1["k3"].get_bool() << " "
		<< j1["k4"].get_string() << " "
		<< std::endl;

	Json j2{};
	j2[0] = 123;
	j2[1] = 123.123;
	j2[2] = true;
	j2[3] = "qwert";
	printf("%s\n", j2.to_string().c_str());

	std::cout << j2[0].get_int() << " "
		<< j2[1].get_double() << " "
		<< j2[2].get_bool() << " "
		<< j2[3].get_string() << " "
		<< std::endl;


	j1["k5"] = j2;

	// j1["k5"][1].clear();
	// j1["k5"].remove(1);
	// j1.remove("k2");
	j2.insert(6, 123456);

	printf("%s\n", j1.to_string().c_str());
	printf("%s\n", j2.to_string().c_str());
}


void test02()
{
	auto str = R"(
		    {
		  "key1": 123,
		  "key2": {  "key3"  : true
		  },
		  "key6": {
		    "key3": [123,23,45,[],
		      [123,213,{"key10"  : 9081.2e-2}]],
		    "key4": 123
		  },
		  "key3": 456
		}   )";

	JsonParser jp(str);
	Json j1 = jp.parse();
	Json j2 = std::move(j1);

	printf("%s\n", j1.to_string().c_str());
	printf("%s\n", j2.to_string().c_str());
	printf("%s\n", j2["key6"]["key3"][4][2]["key10"].to_string().c_str());
}

void test03()
{
	JsonParser jp{};
	jp.loadByFile("test/test.json");
	Json j1 = jp.parse();

	printf("%s\n\n", j1.to_string().c_str());
	printf("%s\n\n", j1["packages"]["node_modules/@babel/helper-module-transforms"]["dependencies"]["@babel/types"].to_string().c_str());
}


void test04()
{
	auto str = R"({
	    "null": null,
	    "bool": false,
	    "integer": 42,
	    "decimal": 3.14,
	    "negative": -10,
	    "scientific_positive": 2.99792458e8,
	    "scientific_negative_decimal": -1.23e-5,
	    "string": "Hello, world!",
	    "array": [1, "two", [3, "four"], {"key": "value"}],
	    "object": {
	      "number": 123,
	      "text": "example",
	      "nested_array": [77, 88, 99],
	      "nested_object": {"inner_key": "inner_value"}
	    }
	    })";

	JsonParser jp(str);
	Json j1 = jp.parse();

	printf("%s\n\n", j1.to_string().c_str());
	printf("%s\n\n", j1["array"][2][1].get_string().c_str());
	printf("%s\n\n", j1["array"][3]["key"].get_string().c_str());
	printf("%d\n\n", j1["object"]["nested_array"][2].get_int());


	j1.remove("array");
	j1["object"]["nested_array"].remove(0);
	printf("%s\n\n", j1.to_string().c_str());



	j1.save("123.json");
	JsonParser jp1{};
	jp1.loadByFile("123.json");
	Json j2 = jp1.parse();

	printf("%s\n\n", j2.to_string().c_str());
	printf("\n\n");
}


void test05()
{
	auto str = R"({
	    "null": null,
	    "bool": false,
	    "integer": 42,
	    "decimal": 3.14,
	    "negative": -10,
	    "scientific_positive": 2.99792458e8,
	    "scientific_negative_decimal": -1.23e-5,
	    "string": "Hello, world!",
	    "array": [1, "two", [3, "four"], {"key": "value"}],
	    "object": {
	      "number": 123,
	      "text": "example",
	      "nested_array": [77, 88, 99],
	      "nested_object": {"inner_key": "inner_value"}
	    }
	    })";

	JsonReader jr(str);

	printf("%s\n\n", jr.startParse().to_string().c_str());
	printf("%s\n\n", jr["array"][2][1].startParse().get_string().c_str());
	printf("%s\n\n", jr["array"][3]["key"].startParse().get_string().c_str());
	printf("%d\n\n", jr["object"]["nested_array"][2].startParse().get_int());
	// printf("%d\n\n", jr[0]["nested_array"][2]["asd"].startParse().getType());  // 没找到返回空值
	printf("\n\n");
}

int main()
{
	{
		test04();

		test05();

		printf("\n\n");
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
