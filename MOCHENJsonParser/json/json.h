/*// 还未解决的问题
* 1.怎么解决没找到的问题，
*
*
*
*/


/*// 所学到的知识
* 1.用union代替class，大幅节约存储空间
* 2.用class enum 代替 enum，避免其隐式转换层int
* 3.容器只能存非const数据，同时begin和end返回的是迭代器的实体，不是引用
* 4.map中[]运算符重载，当没有找到key值时，则自动创建一个新的
* 5.通常拷贝整个vector中的数据时，用下标遍历。拷贝整个map中的数据时，用迭代器遍历
* 6.赋值运算符重载的实现过程，先清空原有的数据和资源，再进行赋值
* 7.类型转换运算符重载，自动对返回值进行类型转换
* 8.通常[]运算符重载，没有边界检查且会创建新值。通常at函数有边界检查但不会创建新值
* 9.少用全局的静态变量，且静态变量的创建，应在成员函数中进行
* 10.当你写了std::string后，请再写一个const char* 的重载函数，以此解决其与bool重载和string类型转换重载函数所引发的冲突问题。
*/




#pragma once
#ifndef _MOCHEN_JSON_H_
#define _MOCHEN_JSON_H_


#include <string>
#include <vector>
#include <map>
#include <sstream>    // std::stringstream


namespace mochen 
{

namespace json
{
	
	


class Json
{
public:
	enum class Type
	{
		json_null = 0,
		json_int,
		json_double,
		json_bool,
		json_string,
		json_array,
		json_object
	};
private:
	union Value
	{
		int							 m_int;
		double					     m_double;
		bool						 m_bool;
		std::string					*m_string;
		std::vector<Json>			*m_array;
		std::map<std::string, Json>	*m_object;
	};
private:
	Type m_type;
	Value m_value;
public:
	Json();
	~Json();

	Json(const Json& _json);  
	Json(Json&& _json);
	Json(int _value);
	Json(double _value);
	Json(bool _value);
	Json(std::string _value);
	Json(const char* _value);
	Json(Type _type);

	void operator=(const Json& _json);    
	void operator=(Json&& _json);
	void operator=(int _value);
	void operator=(double _value);
	void operator=(bool _value);
	void operator=(std::string _value);
	void operator=(const char* _value);

	bool isHaveValue(const std::string& _key);
	// bool isHaveValue(const char* _key);
	bool isHaveValue(int _index);

	Json& operator[](const std::string& _key);   // 通常[]运算符重载，没有边界检查且会创建新值。
	Json& operator[](const char* _key);
	Json& operator[](int _index);               

	Json& at(const std::string& _key);           // 通常at函数有边界检查但不会创建新值
	// bool at(const char* _key);
	Json& at(int _index);

	int	get_int();
	double get_double();
	bool get_bool();
	std::string& get_string();
	std::vector<Json>& get_array();
	std::map<std::string, Json>& get_object();

	bool is_null();
	bool is_int();
	bool is_double();
	bool is_bool();
	bool is_array();
	bool is_object();

	operator int&();   // 类型转换运算符重载, operator T&(); 包括引用和值，两种类型的转换
	operator double&();
	operator bool&();
	operator std::string&();

	bool insert(const std::string& _key, const Json& _json);
	// bool insert(const std::string& _key, Json&& _json);
	// bool insert(const char* _key, const Json& _json);
	// // bool insert(const char* _key, Json&& _json);
	bool insert(int _index, const Json& _json);
	// bool insert(int _index, Json&& _json);

	// bool append(const Json& _json);
	// bool append(Json&& _json);

	bool remove(const std::string& _key);
	// bool remove(const char* _key);
	bool remove(int _index);

	std::string to_string();
	Type getValueType();
	void clear();
	void auxiliary_deep_copy(const Json& _json);
	Json& getNullJson();
};



class JsonParser
{
private:
	std::string m_str;
	int m_index;

public:
	JsonParser();
	~JsonParser();

	//Parser(const Parser& _parser);
	//Parser(Parser&& _parser);
	//Parser(std::string _str);

	//Parser& operator=(const Parser& _parser);
	//Parser& operator=(Parser&& _parser);




};



//class JsonParser_Only_Read
//{
//private:
//	std::string m_str;
//	int m_index;
//
//public:
//	RParser();
//	~RParser();
//
//	RParser(const RParser& _rParser);
//	RParser(RParser&& _rParser);
//	RParser(std::string _str);
//
//	RParser& operator=(const RParser& _rParser);
//	RParser& operator=(RParser&& _rParser);
//
//	int	get_int();
//	float get_float();
//	bool get_bool();
//	std::string get_string();
//	std::vector<Json> get_array();
//	std::map<std::string, Json> get_object();
//
//	Json& find(const std::string& _key);         // 会创建新的值，并用新建的值，处理空值的情况
//	Json& find(int _index);
//
//	Json& operator[](const std::string& _key);   // 会创建新的值，并用新建的值，处理空值的情况
//	Json& operator[](int _index);
//
//};













};

};






#endif // !_MOCHEN_JSON_H_










