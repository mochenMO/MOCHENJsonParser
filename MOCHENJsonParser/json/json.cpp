#include "json.h"


using namespace mochen::json;

// =============================================================================================================
// class Json

Json::Json() : m_type(Type::json_null)
{ }


Json::~Json()
{
	clear();
}


Json::Json(const Json& _json)
{
	auxiliary_deep_copy(_json);
}


Json::Json(Json&& _json) noexcept
{
	m_type = _json.m_type;
	m_value = _json.m_value;
	_json.m_type = Type::json_null;
}


Json::Json(bool _value)
{
	m_type = Type::json_bool;
	m_value.m_bool = _value;
}


Json::Json(int _value)
{
	m_type = Type::json_int;
	m_value.m_int = _value;
}


Json::Json(double _value)
{
	m_type = Type::json_double;
	m_value.m_double = _value;
}


Json::Json(const std::string& _value)
{
	m_type = Type::json_string;
	m_value.m_string = new std::string(_value);
}


Json::Json(const char* _value)
{
	m_type = Type::json_string;
	m_value.m_string = new std::string(_value);
}

Json::Json(Type _type)
{
	m_type = _type;
	switch (m_type)
	{
	case Type::json_null:
		break;
	case Type::json_bool:
		m_value.m_bool = false;
		break;
	case Type::json_int:
		m_value.m_int = 0;
		break;
	case Type::json_double:
		m_value.m_double = 0.0;
		break;
	case Type::json_string:
		m_value.m_string = new std::string();
		break;
	case Type::json_array:
		m_value.m_array = new std::vector<Json>();
		break;
	case Type::json_object:
		m_value.m_object = new std::map<std::string, Json>();
		break;
	default:
		break;
	}
	return;
}


void Json::operator=(const Json& _json)
{
	 clear();  // 赋值运算符重载的实现过程，先清空原有的数据和资源，再进行赋值 
	 auxiliary_deep_copy(_json);
}

void Json::operator=(Json&& _json) noexcept
{
	clear();  // 赋值运算符重载的实现过程，先清空原有的数据和资源，再进行赋值
	m_type = _json.m_type;
	m_value = _json.m_value;
	_json.m_type = Type::json_null;
}

void Json::operator=(bool _value)
{
	if (m_type != Type::json_bool) {
		clear();
		m_type = Type::json_bool;
	}
	m_value.m_bool = _value;
}

void Json::operator=(int _value)
{
	if (m_type != Type::json_int) {
		clear();
		m_type = Type::json_int;
	}
	m_value.m_int = _value;
}

void Json::operator=(double _value)
{
	if (m_type != Type::json_double) {
		clear();
		m_type = Type::json_double;
	}
	m_value.m_double = _value;
}


void Json::operator=(const std::string& _value)
{
	if (m_type != Type::json_string) {
		clear();
		m_type = Type::json_string;
	}
	m_value.m_string = new std::string(_value);
}

void Json::operator=(const char* _value)
{
	if (m_type != Type::json_string) {
		clear();
		m_type = Type::json_string;
	}
	m_value.m_string = new std::string(_value);
}

bool Json::isHaveValue(const std::string& _key)
{
	if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	return (m_value.m_object->find(_key) != m_value.m_object->end());
}


bool Json::isHaveValue(int _index)
{
	if (m_type != Type::json_array) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	return (m_value.m_object->size() > _index);
}

Json& Json::operator[](const std::string& _key) // 通常[]运算符重载，没有边界检查且会创建新值。 
{
	//if (m_type != Type::json_object) {
	//	printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	//	return getNullJson();
	//}
	//return (*(m_value.m_object))[_key];        // map中[]运算符重载，当没有找到key值时，则自动创建一个新的


	// 遇到 json_null 和 json_object 就创建，同时避免操作NullJson对象
	if ((m_type != Type::json_object && m_type != Type::json_null)
		|| this == &getNullJson())  // 防止操作NullJson对象从而影响返回值 (错误的连续会返回NullJson)
	{
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	if (m_type == Type::json_null) {
		// m_type = Type::json_object;
		(*this) = Json(Type::json_object);
	}
	return (*(m_value.m_object))[_key];

}


Json& Json::operator[](int _index)             // 通常[]运算符重载，没有边界检查且会创建新值。
{
	//if (m_type != Type::json_array) {
	//	printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	//	return getNullJson();
	//}
	//int size = m_value.m_array->size();
	//if (_index >= size) {
	//	for (int i = size; i <= _index; ++i) {
	//		m_value.m_array->push_back(Json{});
	//	}
	//}
	//return (*(m_value.m_array))[_index];


	// 遇到 json_null 和 json_array 就创建，同时避免操作NullJson对象
	if ((m_type != Type::json_array && m_type != Type::json_null)
		|| this == &getNullJson())  // 防止操作NullJson对象从而影响返回值 (错误的连续会返回NullJson)
	{
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	if (m_type == Type::json_null) {
		// m_type = Type::json_array;
		(*this) = Json(Type::json_array);
	}
	int size = m_value.m_array->size();
	if (_index >= size) {
		for (int i = size; i <= _index; ++i) {
			m_value.m_array->push_back(Json{});
		}
	}

	// printf("%d json_array\n", (int)(*(m_value.m_array))[_index].getType());
	return (*(m_value.m_array))[_index];
}


Json& Json::at(const std::string& _key)   // 通常at函数有边界检查但不会创建新值  
{
	if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	auto it = m_value.m_object->find(_key);
	if (it == m_value.m_object->end()) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	return it->second;
}


Json& Json::at(int _index)  // 通常at函数有边界检查但不会创建新值  
{
	if (m_type != Type::json_array) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	int size = m_value.m_array->size();
	if (_index < 0 || _index >= size) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	return (*(m_value.m_array))[_index];
}

bool Json::get_bool()
{
	if (m_type != Type::json_bool) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_bool;
}


int	Json::get_int()
{
	if (m_type != Type::json_int) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_int;
}

double Json::get_double()
{
	if (m_type != Type::json_double) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_double;
}


std::string Json::get_string()
{
	if (m_type != Type::json_string) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return *m_value.m_string;
}

std::vector<Json> Json::get_array()
{
	if (m_type != Type::json_array) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return *m_value.m_array;
}

std::map<std::string, Json> Json::get_object()
{
	if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return *m_value.m_object;
}


bool Json::is_null() { return m_type == Type::json_null; }
bool Json::is_bool() { return m_type == Type::json_bool; }
bool Json::is_int() { return m_type == Type::json_int; }
bool Json::is_double() { return m_type == Type::json_double; }
bool Json::is_array() { return m_type == Type::json_array; }
bool Json::is_object() { return m_type == Type::json_object; }


bool Json::append(const Json& _json)
{
	if (m_type != Type::json_array) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	m_value.m_array->push_back(_json);
	return true;
}

bool Json::append(Json&& _json) noexcept
{
	if (m_type != Type::json_array) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	m_value.m_array->push_back(std::move(_json));
	return true;
}


bool Json::insert(int _index, const Json& _json)
{
	if (m_type == Type::json_null) {
		// m_type = Type::json_array;
		// m_value.m_array = new std::vector<Json>();
		(*this) = Json(Type::json_array);
	}
	else if (m_type != Type::json_array) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	int size = m_value.m_array->size();
	if (_index >= size) {
		for (int i = size; i <= _index; ++i) {
			m_value.m_array->push_back(Json{});
		}
	}
	(*(m_value.m_array))[_index] = _json;
	return true;
}


bool Json::insert(int _index, Json&& _json) noexcept
{
	if (m_type == Type::json_null) {
		// m_type = Type::json_array;
		// m_value.m_array = new std::vector<Json>();
		(*this) = Json(Type::json_array);
	}
	else if (m_type != Type::json_array) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	int size = m_value.m_array->size();
	if (_index >= size) {
		for (int i = size; i <= _index; ++i) {
			m_value.m_array->push_back(Json{});
		}
	}
	(*(m_value.m_array))[_index] = std::move(_json);
	return true;


}


bool Json::insert(const std::string& _key, const Json& _json)
{
	if (m_type == Type::json_null) {
		// m_type = Type::json_object;
		// m_value.m_object = new std::map<std::string, Json>();
		(*this) = Json(Type::json_object);
	}
	else if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	(*(m_value.m_object))[_key] = _json;   // map中[]运算符重载，当没有找到key值时，则自动创建一个新的
	return true;
}

bool Json::insert(const std::string& _key, Json&& _json) noexcept
{
	if (m_type == Type::json_null) {
		// m_type = Type::json_object;
		// m_value.m_object = new std::map<std::string, Json>();
		(*this) = Json(Type::json_object);
	}
	else if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	(*(m_value.m_object))[_key] = std::move(_json);   // map中[]运算符重载，当没有找到key值时，则自动创建一个新的
	return true;
}


bool Json::remove(const std::string& _key) 
{
	if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	auto it = m_value.m_object->find(_key);
	if (it == m_value.m_object->end()) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	it->second.clear();
	m_value.m_object->erase(_key);
	return true;
}


bool Json::remove(int _index)
{
	if (m_type != Type::json_array) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	int size = m_value.m_array->size();
	if (_index < 0 || _index >= size) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	(*(m_value.m_array))[_index].clear();
	m_value.m_array->erase(m_value.m_array->begin() + _index);   // 删除下标为_index的元素
	return true;
}


std::string Json::to_string()
{
	std::stringstream ss;   // 内部<<运算符的重载函数，可以连接各种类型的数据
	switch (m_type)
	{
	case Type::json_null:   // null
		ss << "null";
		break;
	case Json::Type::json_bool:    // true false
		m_value.m_bool == true ? ss << "true" : ss << "false";
		break;
	case Type::json_int:    // 1234
		ss << m_value.m_int;   
		break;
	case Json::Type::json_double:  // 123.456
		// ss << m_value.m_double;   // ss默认只存两位小数
		ss << std::fixed << std::setprecision(8) << m_value.m_double;;  // #include <iomanip>  
		break;
	case Json::Type::json_string:  // "abcdef"
		ss << "\"" << *m_value.m_string << "\"";
		break;
	case Json::Type::json_array:   // [json, json, json, json]
	{
		ss << "[";
		for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it) {
			if (it != m_value.m_array->begin()) {
				ss << ", ";
			}
			ss << it->to_string();
		}
		ss << "]";
		break;
	}
	case Json::Type::json_object:  // {"key":json, "key":json}
	{
		ss << "{";
		for(auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it) {
			if (it != m_value.m_object->begin()) {
				ss << ", ";
			}
			ss << "\"" << it->first << "\"" << ":" << it->second.to_string();
		}
		ss << "}";
		break;
	}
	default:
		break;
	}
	return ss.str();
}


Json::Type Json::getType()
{
	return m_type;
}


void Json::clear()
{
	switch (m_type)
	{
	case Type::json_null:
		break;
	case Type::json_bool:
		m_value.m_bool = false;
		break;
	case Type::json_int:
		m_value.m_int = 0;
		break;
	case Type::json_double:
		m_value.m_double = 0.0;
		break;
	case Type::json_string:
		delete m_value.m_string;
		break;
	case Type::json_array:
	{
		for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it) {
			it->clear();
		}
		delete m_value.m_array;
		break;
	}
	case Type::json_object:
	{
		for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it) {
			it->second.clear();
		}
		delete m_value.m_object;
		break;
	}
	default:
		break;
	}
	m_type = Type::json_null;
}


void Json::auxiliary_deep_copy(const Json& _json)
{
	m_type = _json.m_type;
	switch (m_type)
	{
	case Type::json_null:
		break;
	case Type::json_bool:
		m_value.m_bool = _json.m_value.m_int;
		break;
	case Type::json_int: 
		m_value.m_int = _json.m_value.m_int;
		break;
	case Type::json_double:
		m_value.m_double = _json.m_value.m_double;
		break;
	case Type::json_string:
		m_value.m_string = new std::string(*_json.m_value.m_string);
		break;
	case Type::json_array:
	{
		int size = _json.m_value.m_array->size();
		m_value.m_array = new std::vector<Json>(size);
		for (int i = 0; i < size; ++i) {    // 通常拷贝整个vector中的数据时，用下标遍历
			(*(m_value.m_array))[i].auxiliary_deep_copy((*(_json.m_value.m_array))[i]);
		}
		break;

		//// 测试一下
		//m_value.m_array = new std::vector<Json>(size);
		//for (auto it = _json.m_value.m_array->begin(); it != _json.m_value.m_array->end(); ++it) {    // 通常拷贝整个vector中的数据时，用下标遍历
		//	m_value.m_array->push_back(*it);   // push_back 内部调用 Josn的拷贝构造函数，Josn的拷贝构造函数会掉用 auxiliary_deep_copy 函数
		//}
		//break;
	}
	case Type::json_object:
	{
		m_value.m_object = new std::map<std::string, Json>();
		for (auto it = _json.m_value.m_object->begin(); it != _json.m_value.m_object->end(); ++it) {  // 通常拷贝整个map中的数据时，用迭代器遍历
			// (*(m_value.m_object))[it->first].auxiliary_deep_copy(it->second);    // map中[]运算符重载，当没有找到key值时，则自动创建一个新的

			(*(m_value.m_object))[it->first].auxiliary_deep_copy(it->second);    // 不能用at(),因为如果key不存在会报异常
			                                                                     // 先(*(m_value.m_object))[it->first]创建空间，再assisted_deep_copy();
		}
		break;
	}
	default:
		break;
	}
}


Json& Json::getNullJson()
{
	static Json null_json{};    // 静态变量的创建，应在成员函数中进行
	if (null_json.getType() != Type::json_null) {   // 重置机制，防止用户错误的进行赋值操作
		null_json = Json{};
	}
	return null_json;
}


bool Json::save(const std::string& _path)
{
	FILE* fp;
	if ((fp = fopen(_path.c_str(), "w")) == nullptr) {
		printf("open file fild");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	if (this->getType() == Type::json_null) {
		printf("json is nulljosn");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}
	std::string str = std::move(this->to_string());
	fwrite(str.c_str(), 1, str.size(), fp);
	fclose(fp);
	return true;
}

//Json::operator int&()
//{
//	if (m_type != Type::json_int) {
//		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
//	}
//	return m_value.m_int;
//}
//
//Json::operator double&()
//{
//	if (m_type != Type::json_double) {
//		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
//	}
//	return m_value.m_double;
//}
//
//Json::operator bool&()
//{
//	if (m_type != Type::json_bool) {
//		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
//	}
//	return m_value.m_bool;
//}
//
//Json::operator std::string&()
//{
//	if (m_type != Type::json_string) {
//		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
//	}
//	return *m_value.m_string;
//}


// =============================================================================================================
// class JsonParser




JsonParser::JsonParser() : m_string(nullptr), m_index(0)
{}

JsonParser::~JsonParser()
{
	if (m_string != nullptr) {
		free(m_string);  // 这里不能用delete
	}
	m_string = nullptr;
}

JsonParser::JsonParser(const std::string& _string)
{
	JsonParser(_string.c_str());
}


JsonParser::JsonParser(const char* _string)
{
	m_string = (char*)malloc(sizeof(char) * (strlen(_string) + 1));   // +1保存结尾的\0
	strcpy(m_string, _string);
	m_index = 0;
}

JsonParser::JsonParser(std::string&& _string)
{
	m_string = (char*)(_string.c_str());
	m_index = 0;
}



void JsonParser::loadByString(const std::string& _string)
{
	loadByString(_string.c_str());
}

void JsonParser::loadByString(const char* _string)
{
	clear();
	m_string = (char*)malloc(sizeof(char) * (strlen(_string) + 1));   // +1保存结尾的\0
	strcpy(m_string, _string);
	m_index = 0;
}

void JsonParser::loadByString(std::string&& _string)
{
	clear();
	m_string = (char*)(_string.c_str());
	m_index = 0;
}


bool JsonParser::loadByFile(const std::string& _path)
{
	clear();

	FILE* fp;
	if ((fp = fopen(_path.c_str(), "r")) == nullptr) {
		printf("open file fild");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return false;
	}

	fseek(fp, 0, SEEK_END);   
	int size = ftell(fp);     // 计算文件的大小
	fseek(fp, 0, SEEK_SET);

	m_string = (char*)malloc(sizeof(char) * (size + 1));// +1保存结尾的\0
	fread(m_string, 1, size, fp);
	m_string[size] = '\0';

	fclose(fp);
	return true;
}


char JsonParser::get_next_token()
{
	while (m_string[m_index] == ' ' 
		|| m_string[m_index] == '\n' 
		|| m_string[m_index] == '\n' 
		|| m_string[m_index] == '\t') 
	{
		++m_index;
	}
	return m_string[m_index];
}

Json JsonParser::parse()
{
	char ch = get_next_token();
	switch (ch)
	{
	case 'n':
		return parse_null();
	case 't':
	case 'f':
		return parse_bool();
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return parse_number();
	case '"':
		return Json(parse_string());
	case '[':
		return parse_array();
	case '{':
		return parse_object();
	default:  // 取到'\0'了，或者格式错了
		printf("parse failed"); // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return Json(Json::Type::json_null);
	}
}

Json JsonParser::parse_null()
{
	if (strncmp(&m_string[m_index], "null", 4) == 0) {
		m_index += 4;  // 跳过null     
		return Json(Json::Type::json_null);
	}
	else {
		// m_index = strlen(m_string);         // 将m_index设为最后
		printf("parse json_null failed");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return Json(Json::Type::json_null);
	}
}


Json JsonParser::parse_bool()
{
	if (strncmp(&m_string[m_index], "true", 4) == 0) {
		m_index += 4;  // 跳过true    
		return Json(true);
	}
	else if (strncmp(&m_string[m_index], "false", 5) == 0) {
		m_index += 5;  // 跳过false 
		return Json(false);
	}
	else {
		// m_index = strlen(m_string);    // 将m_index设为最后
		printf("parse json_bool failed");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return Json(Json::Type::json_null);
	}
}



Json JsonParser::parse_number()
{
	// 科学计数法说明
	// -123.445e-2    -123.445e+2    123.445E2  123.445E+
	// -123   -123.123


	//int tempPos = m_index;   // 保存number的起始位置
	//if (m_string[m_index] == '-') {
	//	++m_index;
	//}
	//while (m_string[m_index] >= '0' && m_string[m_index] <= '9') {
	//	++m_index;
	//}
	//if (m_string[m_index] != '.' && m_string[m_index] != 'e') {
	//	return Json(atoi(&m_string[tempPos]));   // atoi遇到非数字字符会自动停止
	//}
	//++m_index;   // 跳过'.'
	//while (m_string[m_index] >= '0' && m_string[m_index] <= '9') {
	//	++m_index;
	//}

	char* startPtr = &m_string[m_index];   // 保存number的起始位置
	char* endPtr = nullptr;                // 保存number的结束位置
	if (m_string[m_index] == '-') {
		++m_index;
	}
	while (m_string[m_index] >= '0' && m_string[m_index] <= '9') {
		++m_index;
	}
	// 判断是不是科学技数法和小数
	if(m_string[m_index] != '.'
		&& m_string[m_index] != 'e'
		&& m_string[m_index] != 'E')
	{
		int res = strtol(startPtr, &endPtr, 0);
		while (m_string[m_index] != *endPtr) {
			++m_index;
		}
		return Json(res);
	}
	// 判断是不是科学技数法和小数
	//if (m_string[m_index] == '.'
	//	|| m_string[m_index] == 'e'
	//	|| m_string[m_index] == 'E')
	//{
	//	double res = strtod(startPtr, &endPtr);
	//	while (*startPtr != *endPtr) {
	//		++m_index;
	//	}
	//	return Json(res);
	//}
	//m_index = strlen(m_string);    // 将m_index设为最后
	//printf("parse json_number failed");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	//return Json(Json::Type::json_null);

	// 处理科学技数法和小数
	double res = strtod(startPtr, &endPtr);
	while (m_string[m_index] != *endPtr) {
		++m_index;
	}
	return Json(res);
}


std::string JsonParser::parse_string()
{
	// "string"    "123\"string\"2\"233"   
	
	++m_index;  // 跳过开头的"号
	int startPos = m_index;

	while (m_string[m_index] != '\0') {
		if (m_string[m_index] == '"' && m_string[m_index - 1] != '\\') {
			break;
		}
		++m_index;
	}
	if (m_string[m_index] == '\0') {
		printf("parse json_string failed");
		return "";
	}
	else {
		++m_index;  // 跳过结尾的"号
		return std::string(m_string, startPos, m_index - startPos - 1);
	}

}



Json JsonParser::parse_array()
{
	Json arr(Json::Type::json_array);
	++m_index;  // 跳过开头的'['
	char ch = get_next_token();

	// 空数组
	if (ch == ']') {
		++m_index; // 跳过最后的']'
		return arr;
	}
	// 非空数组
	while (m_string[m_index] != '\0') {
		arr.append(std::move(parse()));
		ch = get_next_token();
		if (ch == ']') {
			++m_index;  // 跳过最后的']'
			return arr;
		}
		else if (ch == ',' ) {
			++m_index;  // 跳过','
			ch = get_next_token();
		}
		else {
			break;
		}
	}
	// m_index = strlen(m_string);    // 将m_index设为最后
	printf("parse json_array failed");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	return Json(Json::Type::json_null);
}



Json JsonParser::parse_object()
{
	Json obj(Json::Type::json_object);
	std::string key;
	++m_index;  // 跳过开头的'{'
	char ch = get_next_token();

	// 空对象
	if (ch == '}') {
		++m_index; // 跳过最后的'}'
		return obj;
	}
	// 非空对象
	while (m_string[m_index] != '\0') {
		if (ch != '"') {
			break;
		}
		key = std::move(parse_string());

		ch = get_next_token();
		if (ch != ':') {  
			break;
		}
		++m_index; // 跳过':'

		ch = get_next_token();
		obj[key] = std::move(parse());

		ch = get_next_token();
		if (ch == '}') {
			++m_index;  // 跳过最后的'}'
			return obj;
		}

		if (ch != ',') {
			break;
		}
		++m_index;  // 跳过','
		ch = get_next_token();
	}
	// m_index = strlen(m_string);    // 将m_index设为最后
	printf("parse json_object failed");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	return Json(Json::Type::json_null);
}


void JsonParser::clear()
{
	if (m_string != nullptr) {
		free(m_string);  // 这里不能用delete
	}
	m_string = nullptr;
	m_index = 0;
}

bool JsonParser::isReadEntireFile()
{
	get_next_token();  // 移动到'\0'
	return (m_string[m_index] == '\0');
}

// =============================================================================================================
// class JsonReader


JsonReader::JsonReader()
{
	m_string = nullptr;
	m_index = 0;
}

JsonReader::~JsonReader()
{
	if (m_string != nullptr) {
		free(m_string);  // 这里不能用delete
	}
	m_string = nullptr;
}



JsonReader::JsonReader(const std::string& _string) : JsonParser(_string)
{ }

JsonReader::JsonReader(const char* _string) : JsonParser(_string)
{ }

JsonReader::JsonReader(std::string&& _string) : JsonParser(_string)
{ }

JsonReader& JsonReader::operator[](const std::string _key)
{
	// auto s = R"(
	// {
	// "k1":{"k2" : 754},
	// "k4":{"k2" : {"k2":456, "k5":[1,2,3]}},
	// "k2":123,
	// }
	// )";

	char ch = get_next_token();
	if (ch != '{') {
		printf("read json_object failed");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		m_index = strlen(m_string);
		return *this;
	}
	++m_index;    // 跳过开头的'{'
	std::stack<char> stack;

	while (true) {
		ch = get_next_token();
		switch(ch)
		{
		case '"': {
			if (_key.compare(parse_string()) == 0) {   // 结束后m_index 指向 '"'的后面，但不知道指没指向':' 
				ch = get_next_token(); // 取到':'
				++m_index;             // 跳过':'
				return *this;
			}
			ch = get_next_token(); // 取到':'
			break;   // 退出时 m_string[m_index] = ':' 之后还要跳过':'
		}
		case '{': {
			stack.push('{');
			while (stack.empty() == false) {
				++m_index;
				ch = get_next_token();
				if (ch == '}') {
					stack.pop();
				}
				else if (ch == '{') {
					stack.push('{');
				}
			}
			break; // 退出时 m_string[m_index] = '}' 之后还要跳过'}'
		}
		case '[': {
			stack.push('[');
			while (stack.empty() == false) {
				++m_index;
				ch = get_next_token();
				if (ch == ']') {
					stack.pop();
				}
				else if (ch == '[') {
					stack.push('[');
				}
			}
			break; // 退出时 m_string[m_index] = ']' 之后还要跳过']'
		}
		default:
			break;
		}
		++m_index;  // 跳过指定的字符
	}
	return *this;   // 此时 m_string[m_index] = '}'
}


JsonReader& JsonReader::operator[](int _index)
{
	 auto s = R"([1,2,3,[4,5,6],7])";

	char ch = get_next_token();
	if (ch != '[') {
		printf("read json_array failed");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		m_index = strlen(m_string);
		return *this;
	}
	++m_index;    // 跳过开头的'['
	std::stack<char> stack;

	while (true) {
		ch = get_next_token();

		if (_index == 0) {
			return *this;
		}

		switch (ch)
		{
		case ',': {
			--_index;
			break;  // 退出时 m_string[m_index] = ',' 之后还要跳过','
		}
		case '{': {
			stack.push('{');
			while (stack.empty() == false) {
				++m_index;
				ch = get_next_token();
				if (ch == '}') {
					stack.pop();
				}
				else if (ch == '{') {
					stack.push('{');
				}
			}
			break; // 退出时 m_string[m_index] = '}' 之后还要跳过'}'
		}
		case '[': {
			stack.push('[');
			while (stack.empty() == false) {
				++m_index;
				ch = get_next_token();
				if (ch == ']') {
					stack.pop();
				}
				else if (ch == '[') {
					stack.push('[');
				}
			}
			break; // 退出时 m_string[m_index] = ']' 之后还要跳过']'
		}
		default:
			break;
		}
		++m_index;  // 跳过指定的字符
	}
	return *this;   // 此时 m_string[m_index] = ']'
}


Json JsonReader::startParse()
{
	if (m_string == nullptr) {
		printf("m_string is nullptr");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return Json(Json::Type::json_null);
	}
	else {
		Json json = std::move(parse());
		m_index = 0;
		return json;
	}
}