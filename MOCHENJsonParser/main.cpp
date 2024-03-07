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




int main()
{
	{
		std::string str = "K1";
		Json j1{};
		j1[str] = 123;
		j1["k2"] = 456.789;
		j1["k3"] = true;
		j1["k4"] = "qwert";
		printf("%s\n", j1.to_string().c_str());


		Json j2{};
		j2[0] = 123;
		j2[1] = 123.123;
		j2[2] = true;
		j2[3] = "qwert";
		printf("%s\n", j2.to_string().c_str());


		//j2[4] = j1;
		//printf("%s\n", j2.to_string().c_str());


		//j2[4] = j2;  
		j2[4] = std::move(j2);

		printf("%s\n", j2.to_string().c_str());
		printf("%s\n", j1.to_string().c_str());

		printf("\n\n");
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
