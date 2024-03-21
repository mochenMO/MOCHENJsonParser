/*// 介绍与说明
* 1.注意用getBool, getInt这类函数获取值时，如果类型不匹配则输出的值是未知的。
* 2.面对损坏数据的情况，解析器尽京可能的把正确的数据解析出来，请留意输出的日志
* 
* 
*/





/*// 还未解决的问题
* 1.怎么解决没找到的问题，
*
*
*
*/


/*// 所学到的知识
* 1.用union代替class，大幅节约存储空间
* 2.用class enum 代替 enum，避免其隐式转换层int
* 3.容器只能存非const数据，通常存值类型，同时begin和end返回的是迭代器的实体，不是引用。
* 4.map中[]运算符重载，当没有找到key值时，则自动创建一个新的
* 5.通常拷贝整个vector中的数据时，用下标遍历。拷贝整个map中的数据时，用迭代器遍历。效率更高，可读性更好。
* 
* vector 的 push_back 会根据参数调用指定的构造函数，map 的 [] 会根据参数调用指定的构造函数，
* 
* 6.赋值运算符重载的实现过程，先清空原有的数据和资源，再进行赋值
*	// 7.类型转换运算符重载，自动对返回值进行类型转换
* 8.通常[]运算符重载函数，没有边界检查且会创建新值。通常at函数有边界检查且不会创建新值
* 9.少用全局的静态变量，且静态变量的创建，应在成员函数中进行
* 10.参数const char*优先匹配bool而不是const std::string&，因此当同时有const std::string&和bool的重载版本时，请再写一个const char*的重载版本。
* 11.如果一个类他有移动操作的需求，则当有一个const class& 版本的重载函数时，请再写一个 class&& 的重载版本。
*    如何判断一个类它有移动操作的需求？如链表，树等，构建该类时需要频繁的插入和删除操作的。还有就是资源管理器类，如内存池，资源池等，用于统筹规划管理资源的类
*    如何判断一个类需要手写拷贝操作？该类有指针成员。
*    如何判断一个类没有拷贝操作的需求？该类是资源管理器类，如内存池，资源池等，用于统筹规划管理资源的类，它没有拷贝操作。
*    ### 总结：在设计一个类时，要先确定，是否有移动操作的需求？是否有拷贝操作的需求？是否需要手写拷贝操作？
* 12.如果明确一个函数不会抛出异常，则建议将其设为noexcept。如移动类函数。注意：建议函数的声明和定义中都标明noexcept提高可读性
*    如何明确一个函数会不会抛出异常？
*                     a.是移动类函数，则不会抛出异常。
*                     b.函数中用到了其他C++三方库和STL库，则会抛出异常
*                     c.函数中只用到了C语言库，如操作系统库window.h和C语言库标准库，则会抛出异常
*                     b.根据开发环境和使用环境，明确的会不会抛出异常的
*                     e.通常只把移动类函数设为noexcept即可。
*/




#pragma once
#ifndef _MOCHEN_JSON_H_
#define _MOCHEN_JSON_H_

#define _CRT_SECURE_NO_WARNINGS

#include "../log/log.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>    // std::stringstream
#include <iomanip>    // std::setprecision 扩展 std::stringstream 存储的小数精度
#include <stack>

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
		json_bool,
		json_int,
		json_double,
		json_string,
		json_array,
		json_object
	};
private:
	union Value
	{
		bool						 m_bool;
		int							 m_int;
		double					     m_double;
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
	Json(Json&& _json) noexcept;                // 把移动类函数设为noexcept,提高代码运行效率
	Json(bool _value);
	Json(int _value);
	Json(double _value);
	Json(const std::string& _value);
	Json(const char* _value);                    // 写一个const char*的重载版本，解决const char*优先匹配bool的问题
	Json(Type _type);

	void operator=(const Json& _json);    
	void operator=(Json&& _json) noexcept;
	void operator=(bool _value);
	void operator=(int _value);
	void operator=(double _value);
	void operator=(const std::string& _value);
	void operator=(const char* _value);          // 写一个const char*的重载版本，解决const char*优先匹配bool的问题

	bool isHaveValue(const std::string& _key);  
	bool isHaveValue(int _index);               

	Json& operator[](const std::string& _key);   // 通常[]运算符重载函数，没有边界检查且会创建新值。
	Json& operator[](int _index);               

	Json& at(const std::string& _key);           // 通常at函数有边界检查且不会创建新值
	Json& at(int _index);

	bool getBool();   // 注意，当类型不匹配时输出的值是未知的
	int	getInt();                              
	double getDouble();                        
	std::string getString();    // 注意返回值类型
	std::vector<Json> getArray();              
	std::map<std::string, Json> getObject();   

	bool isNull();
	bool isBool();   
	bool isInt();    
	bool isDouble(); 
	bool isString(); 
	bool isArray();  
	bool isObject();

	bool append(const Json& _json);      // 操作对象是 m_value.m_array
	bool append(Json&& _json) noexcept;  // 操作对象是 m_value.m_array

	bool insert(int _index, const Json& _json);
	bool insert(int _index, Json&& _json) noexcept;
	bool insert(const std::string& _key, const Json& _json);   // const char*不会匹配int因此不用再写且重载函数
	bool insert(const std::string& _key, Json&& _json) noexcept;

	bool remove(const std::string& _key);
	bool remove(int _index);

	std::string toString();
	Type getType();     
	void clear();
	void deepCopy(const Json& _json);  
	bool save(const std::string& _filename);
};



class JsonParser
{
protected:
	char* m_string;
	int m_index;
public:
	JsonParser();
	~JsonParser();

	JsonParser(const JsonParser&) = delete;
	JsonParser(JsonParser&&) = delete;

	JsonParser(const std::string& _string);
	JsonParser(const char* _string);
	JsonParser(std::string&& _string);

	JsonParser& operator=(const JsonParser&) noexcept = delete;
	JsonParser& operator=(JsonParser&&)  noexcept  = delete;
	
	void loadByString(const std::string& _string);   
	void loadByString(const char* _string);            
	void loadByString(std::string&& _string);       

	bool open(const std::string& _filename);
	
	char getNextValidChar();

	Json parse();
	Json parseNull();            
	Json parseBool();            
	Json parseNumber();          
	std::string parseString();  // 不返回Json是因为josn_object的key也是std::string，如果返回Json还要申请内存并释放
	Json parseArray();           
	Json parseObject();          

	void clear();
	bool isReadEndOfFile();    
};


// 使用的前提条件：保证Josn文件中的数据是正确的可读的
class JsonReader : public JsonParser
{
public:
	JsonReader();
	~JsonReader();

	JsonReader(const JsonReader&) = delete;
	JsonReader(JsonReader&&) = delete;

	JsonReader(const std::string& _string);
	JsonReader(const char* _string);
	JsonReader(std::string&& _string);

	JsonReader& operator=(const JsonReader&) noexcept = delete;
	JsonReader& operator=(JsonReader&&)  noexcept = delete;

	Json startRead(); 

	JsonReader& operator[](const std::string _key);
	JsonReader& operator[](int _index);
};


extern inline log::Logger* getJsonLogger();   // 可供其他文件使用









};

};






#endif // !_MOCHEN_JSON_H_










