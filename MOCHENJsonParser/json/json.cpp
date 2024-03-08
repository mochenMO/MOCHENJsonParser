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
	 clear();  // ��ֵ��������ص�ʵ�ֹ��̣������ԭ�е����ݺ���Դ���ٽ��и�ֵ 
	 auxiliary_deep_copy(_json);
}

void Json::operator=(Json&& _json) noexcept
{
	clear();  // ��ֵ��������ص�ʵ�ֹ��̣������ԭ�е����ݺ���Դ���ٽ��и�ֵ
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

Json& Json::operator[](const std::string& _key) // ͨ��[]��������أ�û�б߽����һᴴ����ֵ�� 
{
	//if (m_type != Type::json_object) {
	//	printf("type error");   // #@$#@%!$%$#!#!#!#!#!#!#!#!%!#%#!%!#%%@#$@#$@$
	//	return getNullJson();
	//}
	//return (*(m_value.m_object))[_key];        // map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�


	// ���� json_null �� json_object �ʹ�����ͬʱ�������NullJson����
	if ((m_type != Type::json_object && m_type != Type::json_null)
		|| this == &getNullJson())  // ��ֹ����NullJson����Ӷ�Ӱ�췵��ֵ (����������᷵��NullJson)
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


Json& Json::operator[](int _index)             // ͨ��[]��������أ�û�б߽����һᴴ����ֵ��
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


	// ���� json_null �� json_array �ʹ�����ͬʱ�������NullJson����
	if ((m_type != Type::json_array && m_type != Type::json_null)
		|| this == &getNullJson())  // ��ֹ����NullJson����Ӷ�Ӱ�췵��ֵ (����������᷵��NullJson)
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


Json& Json::at(const std::string& _key)   // ͨ��at�����б߽��鵫���ᴴ����ֵ  
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


Json& Json::at(int _index)  // ͨ��at�����б߽��鵫���ᴴ����ֵ  
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
	(*(m_value.m_object))[_key] = _json;   // map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�
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
	(*(m_value.m_object))[_key] = std::move(_json);   // map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�
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
	m_value.m_array->erase(m_value.m_array->begin() + _index);   // ɾ���±�Ϊ_index��Ԫ��
	return true;
}


std::string Json::to_string()
{
	std::stringstream ss;   // �ڲ�<<����������غ������������Ӹ������͵�����
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
		ss << m_value.m_double;
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
		for (int i = 0; i < size; ++i) {    // ͨ����������vector�е�����ʱ�����±����
			(*(m_value.m_array))[i].auxiliary_deep_copy((*(_json.m_value.m_array))[i]);
		}
		break;

		//// ����һ��
		//m_value.m_array = new std::vector<Json>(size);
		//for (auto it = _json.m_value.m_array->begin(); it != _json.m_value.m_array->end(); ++it) {    // ͨ����������vector�е�����ʱ�����±����
		//	m_value.m_array->push_back(*it);   // push_back �ڲ����� Josn�Ŀ������캯����Josn�Ŀ������캯������� auxiliary_deep_copy ����
		//}
		//break;
	}
	case Type::json_object:
	{
		m_value.m_object = new std::map<std::string, Json>();
		for (auto it = _json.m_value.m_object->begin(); it != _json.m_value.m_object->end(); ++it) {  // ͨ����������map�е�����ʱ���õ���������
			// (*(m_value.m_object))[it->first].auxiliary_deep_copy(it->second);    // map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�

			(*(m_value.m_object))[it->first].auxiliary_deep_copy(it->second);    // ������at(),��Ϊ���key�����ڻᱨ�쳣
			                                                                     // ��(*(m_value.m_object))[it->first]�����ռ䣬��assisted_deep_copy();
		}
		break;
	}
	default:
		break;
	}
}


Json& Json::getNullJson()
{
	static Json null_json{};    // ��̬�����Ĵ�����Ӧ�ڳ�Ա�����н���
	if (null_json.getType() != Type::json_null) {   // ���û��ƣ���ֹ�û�����Ľ��и�ֵ����
		null_json = Json{};
	}
	return null_json;
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
