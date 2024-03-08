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

		Json j3{};
		j3[0] = 987;
		j3[1] = 654.321;
		printf("%s\n", j3.to_string().c_str());

		//printf("%lf\n", j2[1].get_double());

		//j2[4] = j1;
		//printf("%s\n", j2.to_string().c_str());

		//Json temp = j2[1];
		//j2[4].operator=(temp);

		//j2[4].operator=(j2[1]);
		
		//j2[4] = std::move(j2);

		//j2[4] = j1["k2"];

		j2[4] = j2[1];

		j2[4] = j3[1];

		
		printf("%s\n", j2.to_string().c_str());
		// printf("%lf\n", j2[1].get_double());  /////////////



		j1["k5"] = j1;
		printf("%s\n", j1.to_string().c_str());

		printf("\n\n");
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
