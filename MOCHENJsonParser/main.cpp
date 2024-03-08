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





int main()
{
	{

	}

	_CrtDumpMemoryLeaks();
	return 0;
}
