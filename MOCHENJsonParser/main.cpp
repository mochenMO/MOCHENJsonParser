#define _CRTDBG_MAP_ALLOC   // 使打印的泄漏报告更详细
#include <stdlib.h>         
#include <crtdbg.h>          
#include <windows.h>        // 用于计算运行时间

#include "json/json.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <string_view>


//bool func(const std::string& _key) {
//	// 模拟查找逻辑，这里简单地输出查找的键
//	std::cout << _key << std::endl;
//	return true; // 假设总是返回 true
//}
//
//
//
//
//int main()
//{
//    std::map<int, std::string> myMap = { {1, "One"}, {2, "Two"}, {3, "Three"}, {4, "Three1"}, {5, "Three2"} };
//
//    // 删除键为2的元素
//    myMap.erase(2);
//    myMap.erase(4);
//
//    // 输出删除后的结果
//    for (auto it = myMap.begin(); it != myMap.end(); ++it) {
//        std::cout << it->first << " : " << it->second << std::endl;
//    }
//
//
//
//    //std::vector<int> arr = { 0,1,2,3,4,5,6,7 };
//    //
//    //arr.erase(arr.begin() + 4);
//
//    //for (auto it = arr.begin(); it != arr.end(); ++it) {
//    //    std::cout << *it << std::endl;
//    //}
//
//
//
//    std::string str = "asdasd";
//    str += 1234;
//
//    std::cout << str << std::endl;
//
//	return 0;
//}



using namespace mochen::json;







int main()
{
	{
		std::string str = "K1";

		Json j1(Json::Type::json_object);
		j1[str] = 123;
		j1["k2"] = 123.123;
		j1["k3"] = true;
		j1["k4"] = "qwert";






		printf("%s\n", j1.to_string().c_str());


	}

	_CrtDumpMemoryLeaks();
	return 0;
}
