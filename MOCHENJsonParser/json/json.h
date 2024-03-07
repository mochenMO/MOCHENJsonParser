/*// ��δ���������
* 1.��ô���û�ҵ������⣬
*
*
*
*/


/*// ��ѧ����֪ʶ
* 1.��union����class�������Լ�洢�ռ�
* 2.��class enum ���� enum����������ʽת����int
* 3.����ֻ�ܴ��const���ݣ�ͬʱbegin��end���ص��ǵ�������ʵ�壬��������
* 4.map��[]��������أ���û���ҵ�keyֵʱ�����Զ�����һ���µ�
* 5.ͨ����������vector�е�����ʱ�����±��������������map�е�����ʱ���õ���������
* 6.��ֵ��������ص�ʵ�ֹ��̣������ԭ�е����ݺ���Դ���ٽ��и�ֵ
* 7.����ת����������أ��Զ��Է���ֵ��������ת��
* 8.ͨ��[]��������أ�û�б߽����һᴴ����ֵ��ͨ��at�����б߽��鵫���ᴴ����ֵ
* 9.����ȫ�ֵľ�̬�������Ҿ�̬�����Ĵ�����Ӧ�ڳ�Ա�����н���
* 10.����д��std::string������дһ��const char* �����غ������Դ˽������bool���غ�string����ת�����غ����������ĳ�ͻ���⡣
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

	Json& operator[](const std::string& _key);   // ͨ��[]��������أ�û�б߽����һᴴ����ֵ��
	Json& operator[](const char* _key);
	Json& operator[](int _index);               

	Json& at(const std::string& _key);           // ͨ��at�����б߽��鵫���ᴴ����ֵ
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

	operator int&();   // ����ת�����������, operator T&(); �������ú�ֵ���������͵�ת��
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
//	Json& find(const std::string& _key);         // �ᴴ���µ�ֵ�������½���ֵ�������ֵ�����
//	Json& find(int _index);
//
//	Json& operator[](const std::string& _key);   // �ᴴ���µ�ֵ�������½���ֵ�������ֵ�����
//	Json& operator[](int _index);
//
//};













};

};






#endif // !_MOCHEN_JSON_H_










