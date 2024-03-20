#define _CRTDBG_MAP_ALLOC   // 使打印的泄漏报告更详细
#include <stdlib.h>         
#include <crtdbg.h>          
#include <windows.h>        // 用于计算运行时间

#include "json/json.h"


#include <string>
#include <vector>
#include <map>
#include <iostream>


using namespace mochen::json;


// 测试创建 json_object 和 json_array 并测试删除里面的元素
void test01()
{
	std::string str = "k1";
	Json j1{};
	j1[str] = 123;
	j1["k2"] = 456.789;
	j1["k3"] = true;
	j1["k4"] = "qwert";
	printf("%s\n", j1.toString().c_str());


	std::cout << j1["k1"].getInt() << " "
		<< j1["k2"].getDouble() << " "
		<< j1["k3"].getBool() << " "
		<< j1["k4"].getString() << " "
		<< std::endl;

	Json j2{};
	j2[0] = 123;
	j2[1] = 123.123;
	j2[2] = true;
	j2[3] = "qwert";
	printf("%s\n", j2.toString().c_str());

	std::cout << j2[0].getInt() << " "
		<< j2[1].getDouble() << " "
		<< j2[2].getBool() << " "
		<< j2[3].getString() << " "
		<< std::endl;


	j1["k5"] = j2;
	printf("%s\n", j1.toString().c_str());

	j1["k5"].remove(1);
	printf("%s\n", j1.toString().c_str());
	j1.remove("k2");
	printf("%s\n", j1.toString().c_str());

	j2.insert(6, 123456);
	printf("%s\n", j2.toString().c_str());
}


// 测试 JsonParser 并用[]读取复杂路径的中的数据
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

	printf("%s\n", j1.toString().c_str());   // j1移动过了应该输出 null
	printf("%s\n", j2.toString().c_str());
	printf("%s\n", j2["key6"]["key3"][4][2]["key10"].toString().c_str());
}


// 测试读取并解析json文件
void test03()
{
	JsonParser jp{};
	jp.open("test/test.json");
	Json j1 = jp.parse();

	printf("%s\n\n", j1.toString().c_str());
	printf("%s\n\n", j1["packages"]["node_modules/@babel/helper-module-transforms"]["dependencies"]["@babel/types"].toString().c_str());
	printf("%s\n\n", j1["packages"][""]["dependencies"].toString().c_str());
}

// 测试先解析字符型的数据，再将解析的数据保存
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

	printf("%s\n\n", j1.toString().c_str());
	printf("%s\n\n", j1["array"][2][1].getString().c_str());
	printf("%s\n\n", j1["array"][3]["key"].getString().c_str());
	printf("%d\n\n", j1["object"]["nested_array"][2].getInt());


	j1.remove("array");
	j1["object"]["nested_array"].remove(0);
	printf("%s\n\n", j1.toString().c_str());



	j1.save("test/save.json");
	JsonParser jp1{};
	jp1.open("test/save.json");
	Json j2 = jp1.parse();

	printf("%s\n\n", j2.toString().c_str());

	printf("\n\n");
}


// 测试 JsonReader 和当输入的索引有误时，是否返回空的json
void test05()
{
	auto str = R"({
	    "null": null,
	    "bool": false,
	    "integer": 42,
	    "decimal": 3.143123,
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

	printf("%s\n\n", jr.startRead().toString().c_str());
	printf("%s\n\n", jr["array"][2][1].startRead().getString().c_str());
	printf("%s\n\n", jr["array"][3]["key"].startRead().getString().c_str());
	printf("%d\n\n", jr["object"]["nested_array"][2].startRead().getInt());

	printf("%d\n\n", jr[0]["nested_array"]["test"][2].startRead().getType());  // 索引"test"有问题，没找到，故返回空值
	printf("%d\n\n", jr["array"][10].startRead().getType());                   // 索引10有问题，没找到，故返回空值

	printf("\n\n");
}


// 测试对各种损坏数据的解析情况（解析器尽京可能的把正确的数据解析出来)
void test06()
{
	auto str1 = R"({"key1": })";
	JsonParser j1{};
	j1.loadByString(str1);
	printf("%s\n",j1.parse().toString().c_str());    // 报错 {"key1":null}

	auto str2 = R"({"key1": null})";
	JsonParser j2{};
	j2.loadByString(str2);
	printf("%s\n", j2.parse().toString().c_str());   // {"key1":null}

	auto str3 = R"({"key1":123}{"key2":456})";
	JsonParser j3{};
	j3.loadByString(str3);
	printf("%s\n", j3.parse().toString().c_str());   // {"key1":123}  不会报错因为{"key1":123}是一个完整的数据
	printf("%s\n", j3.isReadEndOfFile() ? "true\0":"false\0"); // 用 isReadEndOfFile 判断是否读到文件结尾，结果为false

	auto str4 = R"([123,1134,{"key1": }])";
	JsonParser j4{};
	j4.loadByString(str4);
	printf("%s\n", j4.parse().toString().c_str());  // 报错 [123, 1134, {"key1":null}]

	auto str5 = R"([123,1134,{"key1": 123})";
	JsonParser j5{};
	j5.loadByString(str5);
	printf("%s\n", j5.parse().toString().c_str());  // 报错 null

	auto str6 = R"({"": 123})";
	JsonParser j6{};
	j6.loadByString(str6);
	printf("%s\n", j6.parse().toString().c_str()); //  {"":null}
}



int main()
{
	{
		// test01(); // 测试创建 json_object 和 json_array 并测试删除里面的元素
		// test02(); // 测试 JsonParser 并用[]读取复杂路径的中的数据
		// test03(); // 测试读取并解析json文件
		// test04(); // 测试先解析字符型的数据，再将解析的数据保存
		// test05(); // 测试 JsonReader 和当输入的索引有误时，是否返回空的json 
		test06();    // 测试对各种损坏数据的解析情况（解析器尽京可能的把正确的数据解析出来)



		auto str5 = R"([123,1134,{"key1": 123}])";
		JsonParser j5{};
		j5.loadByString(str5);
		printf("%s\n", j5.parse().isHaveValue("key") ? "true": "false");  // 报错 false
		printf("\n\n");
	}

	return 0;
}
