#define _CRTDBG_MAP_ALLOC   // ʹ��ӡ��й©�������ϸ
#include <stdlib.h>         
#include <crtdbg.h>          
#include <windows.h>        // ���ڼ�������ʱ��

#include "json/json.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <string_view>


//bool func(const std::string& _key) {
//	// ģ������߼�������򵥵�������ҵļ�
//	std::cout << _key << std::endl;
//	return true; // �������Ƿ��� true
//}
//
//
//
//
//int main()
//{
//    std::map<int, std::string> myMap = { {1, "One"}, {2, "Two"}, {3, "Three"}, {4, "Three1"}, {5, "Three2"} };
//
//    // ɾ����Ϊ2��Ԫ��
//    myMap.erase(2);
//    myMap.erase(4);
//
//    // ���ɾ����Ľ��
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
