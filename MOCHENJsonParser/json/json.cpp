#include "json.h"


using namespace mochen::json;





Json::Json() :m_type(Type::json_null)
{ }


Json::~Json()
{
	clear();
}


Json::Json(const Json& _json)
{
	auxiliary_deep_copy(_json);
}


Json::Json(Json&& _json)
{
	m_type = _json.m_type;
	m_value = _json.m_value;
	_json.m_type = Type::json_null;
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


Json::Json(bool _value)
{
	m_type = Type::json_bool;
	m_value.m_bool = _value;
}


Json::Json(std::string _value)
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
	case Type::json_int:
		m_value.m_int = 0;
		break;
	case Type::json_double:
		m_value.m_double = 0.0;
		break;
	case Type::json_bool:
		m_value.m_bool = false;
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
}


void Json::operator=(const Json& _json)
{
	clear();  // 赋值运算符重载的实现过程，先清空原有的数据和资源，再进行赋值
	auxiliary_deep_copy(_json);
}

void Json::operator=(Json&& _json)
{
	clear();  // 赋值运算符重载的实现过程，先清空原有的数据和资源，再进行赋值
	m_type = _json.m_type;
	m_value = _json.m_value;
	_json.m_type = Type::json_null;
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

void Json::operator=(bool _value)
{
	if (m_type != Type::json_bool) {
		clear();
		m_type = Type::json_bool;
	}
	m_value.m_bool = _value;
}


void Json::operator=(std::string _value)
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
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
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
	if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	return (*(m_value.m_object))[_key];   // map中[]运算符重载，当没有找到key值时，则自动创建一个新的
}


Json& Json::operator[](const char* _key) // 通常[]运算符重载，没有边界检查且会创建新值。 
{
	if (m_type != Type::json_object) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	return (*(m_value.m_object))[_key];   // map中[]运算符重载，当没有找到key值时，则自动创建一个新的
}


Json& Json::operator[](int _index)  // 通常[]运算符重载，没有边界检查且会创建新值。
{
	if (m_type != Type::json_array) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
		return getNullJson();
	}
	int size = m_value.m_array->size();
	if (_index >= size) {
		for (int i = size; i <= _index; ++i) {
			m_value.m_array->push_back(Json{});
		}
	}
	return (*(m_value.m_array))[_index];
}


Json& Json::at(const std::string& _key)   // 通常at函数有边界检查但不会创建新值  
{
	if (m_type != Type::json_object) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
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


int	Json::get_int()
{
	if (m_type != Type::json_int) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_int;
}

double Json::get_double()
{
	if (m_type != Type::json_double) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_double;
}

bool Json::get_bool()
{
	if (m_type != Type::json_bool) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_bool;
}

std::string& Json::get_string()
{
	if (m_type != Type::json_string) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return *m_value.m_string;
}

std::vector<Json>& Json::get_array()
{
	if (m_type != Type::json_array) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return *m_value.m_array;
}

std::map<std::string, Json>& Json::get_object()
{
	if (m_type != Type::json_object) {
		exit(0), printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return *m_value.m_object;
}


Json::operator int&()
{
	if (m_type != Type::json_int) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_int;
}

Json::operator double&()
{
	if (m_type != Type::json_double) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_double;
}

Json::operator bool&()
{
	if (m_type != Type::json_bool) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return m_value.m_bool;
}

Json::operator std::string&()
{
	if (m_type != Type::json_string) {
		printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	}
	return *m_value.m_string;
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
	case Type::json_int:    // 1234
		ss << m_value.m_int;   
		break;
	case Json::Type::json_double:  // 123.456
		ss << m_value.m_double;
		break;
	case Json::Type::json_bool:    // true false
		m_value.m_bool == true ? ss << "true" : ss << "false";
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


Json::Type Json::getValueType()
{
	return m_type;
}


void Json::clear()
{
	switch (m_type)
	{
	case Type::json_null:
		break;
	case Type::json_int:
		m_value.m_int = 0;
		break;
	case Type::json_double:
		m_value.m_double = 0.0;
		break;
	case Type::json_bool:
		m_value.m_bool = false;
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
	case Type::json_int: 
		m_value.m_int = _json.m_value.m_int;
		break;
	case Type::json_double:
		m_value.m_double = _json.m_value.m_int;
		break;
	case Type::json_bool:
		m_value.m_bool = _json.m_value.m_int;
		break;
	case Type::json_string:
		m_value.m_string = new std::string(*_json.m_value.m_string);
		break;
	case Type::json_array:
	{
		int size = _json.m_value.m_array->size();
		m_value.m_array = new std::vector<Json>(size);
		for (int i = 0; i < 0; ++i) {    // 通常拷贝整个vector中的数据时，用下标遍历
			(*(m_value.m_array))[i].auxiliary_deep_copy((*_json.m_value.m_array)[i]);
		}
		break;
	}
	case Type::json_object:
	{
		m_value.m_object = new std::map<std::string, Json>();
		for (auto it = _json.m_value.m_object->begin(); it != _json.m_value.m_object->end(); ++it) {  // 通常拷贝整个map中的数据时，用迭代器遍历
			(*(m_value.m_object))[it->first].auxiliary_deep_copy(it->second);    // map中[]运算符重载，当没有找到key值时，则自动创建一个新的
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
	if (null_json.getValueType() != Type::json_null) {   // 重置机制，防止用户错误的进行赋值操作
		null_json = Json{};
	}
	return null_json;
}