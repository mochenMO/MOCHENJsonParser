#include "json.h"

using namespace mochen::json;
using namespace mochen;

// =============================================================================================================
// 全局区

// 创建 Logger 类
log::Logger* createJsonLogger()   // 初始化 JsonLogger 的函数 (注意该函数只能被运行一次)
{
	static log::Logger JsonLogger("JsonLogger");
	// std::shared_ptr<log::FileLogAppender> fileLogAppender = std::make_shared<log::FileLogAppender>("json");
	// JsonLogger.addAppender(fileLogAppender);
	return &JsonLogger;
}

inline log::Logger* mochen::json::getJsonLogger()  // 获得JsonLogger的函数，写该函数是为了为多文件共享该JsonLogger做准备（注意加命名空间）
{
	static log::Logger* temp = createJsonLogger();
	return temp;
}

log::Logger* JsonLogger = getJsonLogger();


// 创建一个空的Json用于处理搜寻结果为空的情况
inline Json* getNullJson()
{
	static Json nullJson{};
	return &nullJson;
}


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
	deepCopy(_json);
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
	 deepCopy(_json);
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
		logger_error(JsonLogger, "Json::isHaveValue(): type error, Jsonis not a json_object type");
		return false;
	}
	return (m_value.m_object->find(_key) != m_value.m_object->end());
}


bool Json::isHaveValue(int _index)
{
	if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::isHaveValue(): type error, Json is not a json_array type");
		return false;
	}
	return (m_value.m_object->size() > _index);
}

Json& Json::operator[](const std::string& _key) // 通常[]运算符重载，没有边界检查且会创建新值。 
{
	// 遇到 json_null 和 json_object 就创建，同时避免操作NullJson对象
	if ((m_type != Type::json_object && m_type != Type::json_null)
		|| this == getNullJson())     // 防止操作NullJson对象从而影响返回值 (错误的连续调用会返回NullJson)
	{
		logger_error(JsonLogger, "Json::operator[](): type error, Json is not a json_object type");
		return *getNullJson();        // 注意：这里不能直接返回Json(Type::json_null)，因为连续的连续调用修改它的类型，最终返回的就不是真正NullJson了
	}
	if (m_type == Type::json_null) {
		(*this) = Json(Type::json_object);
	}
	return (*(m_value.m_object))[_key];

}


Json& Json::operator[](int _index) // 通常[]运算符重载，没有边界检查且会创建新值。
{
	// 遇到 json_null 和 json_array 就创建，同时避免操作NullJson对象
	if ((m_type != Type::json_array && m_type != Type::json_null)
		|| this == getNullJson())  // 防止操作NullJson对象从而影响返回值 (错误的连续调用会返回NullJson)
	{
		logger_error(JsonLogger, "Json::operator[](): type error, Json is not a json_array type");
		return *getNullJson();   // 注意：这里不能直接返回Json(Type::json_null)，因为连续的连续调用修改它的类型，最终返回的就不是真正NullJson了
	}
	if (m_type == Type::json_null) {
		(*this) = Json(Type::json_array);
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
		logger_error(JsonLogger, "Json::at(): type error, Json is not a json_object type");
		return *getNullJson();
	}
	auto it = m_value.m_object->find(_key);
	if (it == m_value.m_object->end()) {
		logger_error(JsonLogger, "Json::at(): valid key, not found this key <%s>", _key.c_str());
		return *getNullJson();
	}
	return it->second;
}


Json& Json::at(int _index)  // 通常at函数有边界检查但不会创建新值  
{
	if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::at(): type error, Json is not a json_array type");
		return *getNullJson();
	}
	int size = m_value.m_array->size();
	if (_index < 0 || _index >= size) {
		logger_error(JsonLogger, "Json::at(): value out of range <%d>", _index);
		return *getNullJson();
	}
	return (*(m_value.m_array))[_index];
}

bool Json::getBool()
{
	if (m_type != Type::json_bool) {
		logger_error(JsonLogger, "Json::getBool(): type error, Json is not a json_bool type");
		return false;
	}
	else {
		return m_value.m_bool;
	}
}


int	Json::getInt()
{
	if (m_type != Type::json_int) {
		logger_error(JsonLogger, "Json::getInt(): type error, Json is not a json_int type");
		return 0;
	}
	else {
		return m_value.m_int;
	}
}

double Json::getDouble()
{
	if (m_type != Type::json_double) {
		logger_error(JsonLogger, "Json::getDouble(): type error, Json is not a json_double type");
		return 0.0;
	}
	else {
		return m_value.m_double;
	}
	
}


std::string Json::getString()
{
	if (m_type != Type::json_string) {
		logger_error(JsonLogger, "Json::getString(): type error, Json is not a json_string type");
		return "";
	}
	else {
		return *m_value.m_string;
	}
}

std::vector<Json> Json::getArray()
{
	if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::getArray(): type error, Json is not a json_array type");
		return std::vector<Json>{};
	}	
	else {
		return *m_value.m_array;
	}
}

std::map<std::string, Json> Json::getObject()
{
	if (m_type != Type::json_object) {
		logger_error(JsonLogger, "Json::getObject(): type error, Json is not a json_object type");
		return std::map<std::string, Json>{};
	}
	else {
		return *m_value.m_object;
	}
}

bool Json::isNull() { return m_type == Type::json_null; }
bool Json::isBool() { return m_type == Type::json_bool; }
bool Json::isInt() { return m_type == Type::json_int; }
bool Json::isDouble() { return m_type == Type::json_double; }
bool Json::isString() { return m_type == Type::json_string; }
bool Json::isArray() { return m_type == Type::json_array; }
bool Json::isObject() { return m_type == Type::json_object; }


bool Json::append(const Json& _json)
{
	if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::append(): type error, Json is not a json_array type");
		return false;
	}
	m_value.m_array->push_back(_json);
	return true;
}

bool Json::append(Json&& _json) noexcept
{
	if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::append(): type error, Json is not a json_array type");
		return false;
	}
	m_value.m_array->push_back(std::move(_json));
	return true;
}


bool Json::insert(int _index, const Json& _json)
{
	if (m_type == Type::json_null) {
		(*this) = Json(Type::json_array);
	}
	else if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::insert(): type error, Json is not a json_array type");
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
		(*this) = Json(Type::json_array);
	}
	else if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::insert(): type error, Json is not a json_array type");
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
		(*this) = Json(Type::json_object);
	}
	else if (m_type != Type::json_object) {
		logger_error(JsonLogger, "Json::insert(): type error, Json is not a json_object type");
		return false;
	}
	(*(m_value.m_object))[_key] = _json;   // map中[]运算符重载，当没有找到key值时，则自动创建一个新的
	return true;
}

bool Json::insert(const std::string& _key, Json&& _json) noexcept
{
	if (m_type == Type::json_null) {
		(*this) = Json(Type::json_object);
	}
	else if (m_type != Type::json_object) {
		logger_error(JsonLogger, "Json::insert(): type error, Json is not a json_object type");
		return false;
	}
	(*(m_value.m_object))[_key] = std::move(_json);   // map中[]运算符重载，当没有找到key值时，则自动创建一个新的
	return true;
}


bool Json::remove(const std::string& _key) 
{
	if (m_type != Type::json_object) {
		logger_error(JsonLogger, "Json::remove(): type error, Json is not a json_object type");	
		return false;
	}
	auto it = m_value.m_object->find(_key);
	if (it == m_value.m_object->end()) {
		logger_error(JsonLogger, "Json::remove(): valid key, not found this key <%s>", _key.c_str());
		return false;
	}
	it->second.clear();
	m_value.m_object->erase(_key);
	return true;
}


bool Json::remove(int _index)
{
	if (m_type != Type::json_array) {
		logger_error(JsonLogger, "Json::remove(): type error, Json is not a json_array type");
		return false;
	}
	int size = m_value.m_array->size();
	if (_index < 0 || _index >= size) {
		logger_error(JsonLogger, "Json::remove(): value out of range <%d>", _index);
		return false;
	}
	(*(m_value.m_array))[_index].clear();
	m_value.m_array->erase(m_value.m_array->begin() + _index);   // 删除下标为_index的元素
	return true;
}


std::string Json::toString()
{
	std::stringstream ss;   // 内部<<运算符的重载函数，可以连接各种类型的数据
	switch (m_type)
	{
	case Type::json_null:   // null
		ss << "null";
		break;
	case Json::Type::json_bool:     // true false
		m_value.m_bool == true ? ss << "true" : ss << "false";
		break;
	case Type::json_int:             // 1234
		ss << m_value.m_int;   
		break;
	case Json::Type::json_double:    // 123.456
		// ss << m_value.m_double;   // ss默认只存两位小数
		ss << std::fixed << std::setprecision(8) << m_value.m_double;;  // 要先用std::fixed取消默认输出3位小数
		break;
	case Json::Type::json_string:    // "abcdef"
		ss << "\"" << *m_value.m_string << "\"";
		break;
	case Json::Type::json_array:     // [json, json, json, json]
	{
		ss << "[";
		for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it) {
			if (it != m_value.m_array->begin()) {
				ss << ", ";
			}
			ss << it->toString();
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
			ss << "\"" << it->first << "\"" << ":" << it->second.toString();
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


void Json::deepCopy(const Json& _json)
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
			(*(m_value.m_array))[i].deepCopy((*(_json.m_value.m_array))[i]);
		}
		break;
	}
	case Type::json_object:
	{
		m_value.m_object = new std::map<std::string, Json>();
		for (auto it = _json.m_value.m_object->begin(); it != _json.m_value.m_object->end(); ++it) {  // 通常拷贝整个map中的数据时，用迭代器遍历
			(*(m_value.m_object))[it->first].deepCopy(it->second);  // map中[]运算符重载，当没有找到key值时，则自动创建一个新的                                                                     
		}
		break;
	}
	default:
		break;
	}
}


bool Json::save(const std::string& _filename)
{
	FILE* fp;
	if ((fp = fopen(_filename.c_str(), "w")) == nullptr) {
		logger_error(JsonLogger, "Json::save(): open file fild <%s>", _filename.c_str());
		return false;
	}
	if (this->getType() == Type::json_null) {
		logger_error(JsonLogger, "Json::save(): null data, json is null");
		return false;
	}
	std::string str = std::move(this->toString());
	fwrite(str.c_str(), 1, str.size(), fp);
	fclose(fp);
	return true;
}



// =============================================================================================================
// class JsonParser

JsonParser::JsonParser() : m_string(nullptr), m_index(0)
{}

JsonParser::~JsonParser()
{
	if (m_string != nullptr) {
		free(m_string);  // 这里不能用delete
		m_string = nullptr;
	}
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


bool JsonParser::open(const std::string& _filename)
{
	clear();

	FILE* fp;
	if ((fp = fopen(_filename.c_str(), "r")) == nullptr) {
		logger_error(JsonLogger, "Json::save(): open file fild <%s>", _filename.c_str());
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


char JsonParser::getNextValidChar()
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
	char ch = getNextValidChar();
	switch (ch)
	{
	case 'n':
		return parseNull();
	case 't':
	case 'f':
		return parseBool();
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
		return parseNumber();
	case '"':
		return Json(parseString());
	case '[':
		return parseArray();
	case '{':
		return parseObject();
	default:  // 取到'\0'了，或者格式错了
		logger_error(JsonLogger, "JsonParser::parse(): parse failed, invalid char");
		return Json(Json::Type::json_null);
	}
}

Json JsonParser::parseNull()
{
	if (strncmp(&m_string[m_index], "null", 4) == 0) {
		m_index += 4;  // 跳过null     
		return Json(Json::Type::json_null);
	}
	else {
		logger_error(JsonLogger, "JsonParser::parseNull: parse failed, json_null");
		return Json(Json::Type::json_null);
	}
}


Json JsonParser::parseBool()
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
		logger_error(JsonLogger, "JsonParser::parseBool: parse failed, json_bool");
		return Json(Json::Type::json_null);
	}
}



Json JsonParser::parseNumber()
{
	// 科学计数法说明
	// -123.445e-2    -123.445e+2    123.445E2  123.445E+
	// -123   -123.123

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
	// 处理科学技数法和小数
	double res = strtod(startPtr, &endPtr);
	while (m_string[m_index] != *endPtr) {
		++m_index;
	}
	return Json(res);
}


std::string JsonParser::parseString()
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
		logger_error(JsonLogger, "JsonParser::parseString: parse failed, json_string");
		return "";
	}
	else {
		++m_index;  // 跳过结尾的"号
		return std::string(m_string, startPos, m_index - startPos - 1);
	}

}


Json JsonParser::parseArray()
{
	Json arr(Json::Type::json_array);
	++m_index;  // 跳过开头的'['
	char ch = getNextValidChar();

	// 空数组
	if (ch == ']') {
		++m_index; // 跳过最后的']'
		return arr;
	}
	// 非空数组
	while (m_string[m_index] != '\0') {
		arr.append(std::move(parse()));
		ch = getNextValidChar();
		if (ch == ']') {
			++m_index;  // 跳过最后的']'
			return arr;
		}
		else if (ch == ',' ) {
			++m_index;  // 跳过','
			ch = getNextValidChar();
		}
		else {
			break;
		}
	}
	logger_error(JsonLogger, "JsonParser::parseArray: parse failed, json_array");
	return Json(Json::Type::json_null);
}



Json JsonParser::parseObject()
{
	Json obj(Json::Type::json_object);
	std::string key;
	++m_index;  // 跳过开头的'{'
	char ch = getNextValidChar();

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
		key = std::move(parseString());

		ch = getNextValidChar();
		if (ch != ':') {  
			break;
		}
		++m_index; // 跳过':'

		ch = getNextValidChar();
		obj[key] = std::move(parse());

		ch = getNextValidChar();
		if (ch == '}') {
			++m_index;  // 跳过最后的'}'
			return obj;
		}

		if (ch != ',') {
			break;
		}
		++m_index;  // 跳过','
		ch = getNextValidChar();
	}
	logger_error(JsonLogger, "JsonParser::parseObject: parse failed, json_object");
	return Json(Json::Type::json_null);
}


void JsonParser::clear()
{
	if (m_string != nullptr) {
		free(m_string);  // 这里不能用delete
		m_string = nullptr;
		m_index = 0;
	}
}

bool JsonParser::isReadEndOfFile()
{
	getNextValidChar();  // 移动到'\0'
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
		m_string = nullptr;
	}
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

	char ch = getNextValidChar();
	if (ch != '{') {
		logger_error(JsonLogger, "JsonReader::operator[](): read failed, Json is not a json_object type");
		m_index = strlen(m_string);
		return *this;
	}
	++m_index;    // 跳过开头的'{'
	std::stack<char> stack;

	while (m_string[m_index] != '\0') {
		ch = getNextValidChar();
		switch(ch)
		{
		case '}': {  // 这一层找完了没找到，时 m_string[m_index] = '}'，如果再进行 parse 会引发错误
			return *this;
		}
		case '"': {
			if (_key.compare(parseString()) == 0) {   // 找到了，且 parseString 结束后m_index 指向 '"'的后面，但不知道是否指向':' 
				ch = getNextValidChar(); // 取到':'
				++m_index;               // 跳过':'
				return *this;
			}
			ch = getNextValidChar(); // 取到':'
			break;   // 退出时 m_string[m_index] = ':' 之后还要跳过':'
		}
		case '{': {  // 跳过所有子json_object
			stack.push('{');
			while (stack.empty() == false) {
				++m_index;
				ch = getNextValidChar();
				if (ch == '}') {
					stack.pop();
				}
				else if (ch == '{') {
					stack.push('{');
				}
			}
			break;  // 退出时 m_string[m_index] = '}' 之后还要跳过'}'
		}
		case '[': { // 跳过所有子json_array
			stack.push('[');
			while (stack.empty() == false) {
				++m_index;
				ch = getNextValidChar();
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
	return *this;   // 此时 m_string[m_index] = '\0' 如果再进行 parse 会引发错误
}


JsonReader& JsonReader::operator[](int _index)
{
	 // [1,2,3,[4,5,6],7,{"k1":123},8]

	char ch = getNextValidChar();
	if (ch != '[') {
		logger_error(JsonLogger, "JsonReader::operator[](): read failed, Json is not a json_array type");
		m_index = strlen(m_string);
		return *this;
	}
	++m_index;    // 跳过开头的'['
	std::stack<char> stack;

	while (m_string[m_index] != '\0') {
		ch = getNextValidChar();

		if (_index == 0) {   // 找到了
			return *this;
		}

		switch (ch)
		{
		case ']': {  // 这一层找完了没找到，时 m_string[m_index] = ']'，如果再进行 parse 会引发错误
			return *this;
		}
		case ',': {
			--_index;
			break;   // 退出时 m_string[m_index] = ',' 之后还要跳过','
		}
		case '{': {  // 跳过所有子json_object
			stack.push('{');
			while (stack.empty() == false) {
				++m_index;
				ch = getNextValidChar();
				if (ch == '}') {
					stack.pop();
				}
				else if (ch == '{') {
					stack.push('{');
				}
			}
			break;  // 退出时 m_string[m_index] = '}' 之后还要跳过'}'
		}
		case '[': { // 跳过所有子json_array
			stack.push('[');
			while (stack.empty() == false) {
				++m_index;
				ch = getNextValidChar();
				if (ch == ']') {
					stack.pop();
				}
				else if (ch == '[') {
					stack.push('[');
				}
			}
			break;  // 退出时 m_string[m_index] = ']' 之后还要跳过']'
		}
		default:
			break;
		}
		++m_index;  // 跳过指定的字符
	}
	return *this;   // 此时 m_string[m_index] = '\0' 如果进行再 parse 会引发错误
}


Json JsonReader::startRead()
{
	if (m_string == nullptr) {
		logger_error(JsonLogger, "JsonReader::startRead(): null pointer, <m_string> is nullptr");
		return Json(Json::Type::json_null);
	}
	else {
		Json json = std::move(parse());
		m_index = 0;
		return std::move(json);
	}
}