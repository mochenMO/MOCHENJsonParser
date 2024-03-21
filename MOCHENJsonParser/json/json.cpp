#include "json.h"

using namespace mochen::json;
using namespace mochen;

// =============================================================================================================
// ȫ����

// ���� Logger ��
log::Logger* createJsonLogger()   // ��ʼ�� JsonLogger �ĺ��� (ע��ú���ֻ�ܱ�����һ��)
{
	static log::Logger JsonLogger("JsonLogger");
	// std::shared_ptr<log::FileLogAppender> fileLogAppender = std::make_shared<log::FileLogAppender>("json");
	// JsonLogger.addAppender(fileLogAppender);
	return &JsonLogger;
}

inline log::Logger* mochen::json::getJsonLogger()  // ���JsonLogger�ĺ�����д�ú�����Ϊ��Ϊ���ļ������JsonLogger��׼����ע��������ռ䣩
{
	static log::Logger* temp = createJsonLogger();
	return temp;
}

log::Logger* JsonLogger = getJsonLogger();


// ����һ���յ�Json���ڴ�����Ѱ���Ϊ�յ����
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
	 clear();  // ��ֵ��������ص�ʵ�ֹ��̣������ԭ�е����ݺ���Դ���ٽ��и�ֵ 
	 deepCopy(_json);
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

Json& Json::operator[](const std::string& _key) // ͨ��[]��������أ�û�б߽����һᴴ����ֵ�� 
{
	// ���� json_null �� json_object �ʹ�����ͬʱ�������NullJson����
	if ((m_type != Type::json_object && m_type != Type::json_null)
		|| this == getNullJson())     // ��ֹ����NullJson����Ӷ�Ӱ�췵��ֵ (������������û᷵��NullJson)
	{
		logger_error(JsonLogger, "Json::operator[](): type error, Json is not a json_object type");
		return *getNullJson();        // ע�⣺���ﲻ��ֱ�ӷ���Json(Type::json_null)����Ϊ���������������޸��������ͣ����շ��صľͲ�������NullJson��
	}
	if (m_type == Type::json_null) {
		(*this) = Json(Type::json_object);
	}
	return (*(m_value.m_object))[_key];

}


Json& Json::operator[](int _index) // ͨ��[]��������أ�û�б߽����һᴴ����ֵ��
{
	// ���� json_null �� json_array �ʹ�����ͬʱ�������NullJson����
	if ((m_type != Type::json_array && m_type != Type::json_null)
		|| this == getNullJson())  // ��ֹ����NullJson����Ӷ�Ӱ�췵��ֵ (������������û᷵��NullJson)
	{
		logger_error(JsonLogger, "Json::operator[](): type error, Json is not a json_array type");
		return *getNullJson();   // ע�⣺���ﲻ��ֱ�ӷ���Json(Type::json_null)����Ϊ���������������޸��������ͣ����շ��صľͲ�������NullJson��
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


Json& Json::at(const std::string& _key)   // ͨ��at�����б߽��鵫���ᴴ����ֵ  
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


Json& Json::at(int _index)  // ͨ��at�����б߽��鵫���ᴴ����ֵ  
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
	(*(m_value.m_object))[_key] = _json;   // map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�
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
	(*(m_value.m_object))[_key] = std::move(_json);   // map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�
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
	m_value.m_array->erase(m_value.m_array->begin() + _index);   // ɾ���±�Ϊ_index��Ԫ��
	return true;
}


std::string Json::toString()
{
	std::stringstream ss;   // �ڲ�<<����������غ������������Ӹ������͵�����
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
		// ss << m_value.m_double;   // ssĬ��ֻ����λС��
		ss << std::fixed << std::setprecision(8) << m_value.m_double;;  // Ҫ����std::fixedȡ��Ĭ�����3λС��
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
		for (int i = 0; i < size; ++i) {    // ͨ����������vector�е�����ʱ�����±����
			(*(m_value.m_array))[i].deepCopy((*(_json.m_value.m_array))[i]);
		}
		break;
	}
	case Type::json_object:
	{
		m_value.m_object = new std::map<std::string, Json>();
		for (auto it = _json.m_value.m_object->begin(); it != _json.m_value.m_object->end(); ++it) {  // ͨ����������map�е�����ʱ���õ���������
			(*(m_value.m_object))[it->first].deepCopy(it->second);  // map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�                                                                     
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
		free(m_string);  // ���ﲻ����delete
		m_string = nullptr;
	}
}

JsonParser::JsonParser(const std::string& _string)
{
	JsonParser(_string.c_str());
}


JsonParser::JsonParser(const char* _string)
{
	m_string = (char*)malloc(sizeof(char) * (strlen(_string) + 1));   // +1�����β��\0
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
	m_string = (char*)malloc(sizeof(char) * (strlen(_string) + 1));   // +1�����β��\0
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
	int size = ftell(fp);     // �����ļ��Ĵ�С
	fseek(fp, 0, SEEK_SET);

	m_string = (char*)malloc(sizeof(char) * (size + 1));// +1�����β��\0
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
	default:  // ȡ��'\0'�ˣ����߸�ʽ����
		logger_error(JsonLogger, "JsonParser::parse(): parse failed, invalid char");
		return Json(Json::Type::json_null);
	}
}

Json JsonParser::parseNull()
{
	if (strncmp(&m_string[m_index], "null", 4) == 0) {
		m_index += 4;  // ����null     
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
		m_index += 4;  // ����true    
		return Json(true);
	}
	else if (strncmp(&m_string[m_index], "false", 5) == 0) {
		m_index += 5;  // ����false 
		return Json(false);
	}
	else {
		logger_error(JsonLogger, "JsonParser::parseBool: parse failed, json_bool");
		return Json(Json::Type::json_null);
	}
}



Json JsonParser::parseNumber()
{
	// ��ѧ������˵��
	// -123.445e-2    -123.445e+2    123.445E2  123.445E+
	// -123   -123.123

	char* startPtr = &m_string[m_index];   // ����number����ʼλ��
	char* endPtr = nullptr;                // ����number�Ľ���λ��
	if (m_string[m_index] == '-') {
		++m_index;
	}
	while (m_string[m_index] >= '0' && m_string[m_index] <= '9') {
		++m_index;
	}
	// �ж��ǲ��ǿ�ѧ��������С��
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
	// �����ѧ��������С��
	double res = strtod(startPtr, &endPtr);
	while (m_string[m_index] != *endPtr) {
		++m_index;
	}
	return Json(res);
}


std::string JsonParser::parseString()
{
	// "string"    "123\"string\"2\"233"   
	
	++m_index;  // ������ͷ��"��
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
		++m_index;  // ������β��"��
		return std::string(m_string, startPos, m_index - startPos - 1);
	}

}


Json JsonParser::parseArray()
{
	Json arr(Json::Type::json_array);
	++m_index;  // ������ͷ��'['
	char ch = getNextValidChar();

	// ������
	if (ch == ']') {
		++m_index; // ��������']'
		return arr;
	}
	// �ǿ�����
	while (m_string[m_index] != '\0') {
		arr.append(std::move(parse()));
		ch = getNextValidChar();
		if (ch == ']') {
			++m_index;  // ��������']'
			return arr;
		}
		else if (ch == ',' ) {
			++m_index;  // ����','
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
	++m_index;  // ������ͷ��'{'
	char ch = getNextValidChar();

	// �ն���
	if (ch == '}') {
		++m_index; // ��������'}'
		return obj;
	}
	// �ǿն���
	while (m_string[m_index] != '\0') {
		if (ch != '"') {
			break;
		}
		key = std::move(parseString());

		ch = getNextValidChar();
		if (ch != ':') {  
			break;
		}
		++m_index; // ����':'

		ch = getNextValidChar();
		obj[key] = std::move(parse());

		ch = getNextValidChar();
		if (ch == '}') {
			++m_index;  // ��������'}'
			return obj;
		}

		if (ch != ',') {
			break;
		}
		++m_index;  // ����','
		ch = getNextValidChar();
	}
	logger_error(JsonLogger, "JsonParser::parseObject: parse failed, json_object");
	return Json(Json::Type::json_null);
}


void JsonParser::clear()
{
	if (m_string != nullptr) {
		free(m_string);  // ���ﲻ����delete
		m_string = nullptr;
		m_index = 0;
	}
}

bool JsonParser::isReadEndOfFile()
{
	getNextValidChar();  // �ƶ���'\0'
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
		free(m_string);  // ���ﲻ����delete
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
	++m_index;    // ������ͷ��'{'
	std::stack<char> stack;

	while (m_string[m_index] != '\0') {
		ch = getNextValidChar();
		switch(ch)
		{
		case '}': {  // ��һ��������û�ҵ���ʱ m_string[m_index] = '}'������ٽ��� parse ����������
			return *this;
		}
		case '"': {
			if (_key.compare(parseString()) == 0) {   // �ҵ��ˣ��� parseString ������m_index ָ�� '"'�ĺ��棬����֪���Ƿ�ָ��':' 
				ch = getNextValidChar(); // ȡ��':'
				++m_index;               // ����':'
				return *this;
			}
			ch = getNextValidChar(); // ȡ��':'
			break;   // �˳�ʱ m_string[m_index] = ':' ֮��Ҫ����':'
		}
		case '{': {  // ����������json_object
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
			break;  // �˳�ʱ m_string[m_index] = '}' ֮��Ҫ����'}'
		}
		case '[': { // ����������json_array
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
			break; // �˳�ʱ m_string[m_index] = ']' ֮��Ҫ����']'
		}
		default:
			break;
		}
		++m_index;  // ����ָ�����ַ�
	}
	return *this;   // ��ʱ m_string[m_index] = '\0' ����ٽ��� parse ����������
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
	++m_index;    // ������ͷ��'['
	std::stack<char> stack;

	while (m_string[m_index] != '\0') {
		ch = getNextValidChar();

		if (_index == 0) {   // �ҵ���
			return *this;
		}

		switch (ch)
		{
		case ']': {  // ��һ��������û�ҵ���ʱ m_string[m_index] = ']'������ٽ��� parse ����������
			return *this;
		}
		case ',': {
			--_index;
			break;   // �˳�ʱ m_string[m_index] = ',' ֮��Ҫ����','
		}
		case '{': {  // ����������json_object
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
			break;  // �˳�ʱ m_string[m_index] = '}' ֮��Ҫ����'}'
		}
		case '[': { // ����������json_array
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
			break;  // �˳�ʱ m_string[m_index] = ']' ֮��Ҫ����']'
		}
		default:
			break;
		}
		++m_index;  // ����ָ�����ַ�
	}
	return *this;   // ��ʱ m_string[m_index] = '\0' ��������� parse ����������
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