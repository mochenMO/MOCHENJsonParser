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
* 5.ͨ����������vector�е�����ʱ�����±��������������map�е�����ʱ���õ�����������Ч�ʸ��ߣ��ɶ��Ը��á�
* 
* vector �� push_back ����ݲ�������ָ���Ĺ��캯����map �� [] ����ݲ�������ָ���Ĺ��캯����
* 
* 6.��ֵ��������ص�ʵ�ֹ��̣������ԭ�е����ݺ���Դ���ٽ��и�ֵ
*	// 7.����ת����������أ��Զ��Է���ֵ��������ת��
* 8.ͨ��[]��������غ�����û�б߽����һᴴ����ֵ��ͨ��at�����б߽����Ҳ��ᴴ����ֵ
* 9.����ȫ�ֵľ�̬�������Ҿ�̬�����Ĵ�����Ӧ�ڳ�Ա�����н���
* 10.����const char*����ƥ��bool������const std::string&����˵�ͬʱ��const std::string&��bool�����ذ汾ʱ������дһ��const char*�����ذ汾��
* 11.���һ���������ƶ���������������һ��const class& �汾�����غ���ʱ������дһ�� class&& �����ذ汾��
*    ����ж�һ���������ƶ��������������������ȣ���������ʱ��ҪƵ���Ĳ����ɾ�������ġ����о�����Դ�������࣬���ڴ�أ���Դ�صȣ�����ͳ��滮������Դ����
*    ����ж�һ������Ҫ��д����������������ָ���Ա��
*    ����ж�һ����û�п������������󣿸�������Դ�������࣬���ڴ�أ���Դ�صȣ�����ͳ��滮������Դ���࣬��û�п���������
*    ### �ܽ᣺�����һ����ʱ��Ҫ��ȷ�����Ƿ����ƶ������������Ƿ��п��������������Ƿ���Ҫ��д����������
* 12.�����ȷһ�����������׳��쳣�����齫����Ϊnoexcept�����ƶ��ຯ����ע�⣺���麯���������Ͷ����ж�����noexcept��߿ɶ���
*    �����ȷһ�������᲻���׳��쳣��
*                     a.���ƶ��ຯ�����򲻻��׳��쳣��
*                     b.�������õ�������C++�������STL�⣬����׳��쳣
*                     c.������ֻ�õ���C���Կ⣬�����ϵͳ��window.h��C���Կ��׼�⣬����׳��쳣
*                     b.���ݿ���������ʹ�û�������ȷ�Ļ᲻���׳��쳣��
*                     e.ͨ��ֻ���ƶ��ຯ����Ϊnoexcept���ɡ�
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
	Json(Json&& _json) noexcept;                // ���ƶ��ຯ����Ϊnoexcept,��ߴ�������Ч��
	Json(bool _value);
	Json(int _value);
	Json(double _value);
	Json(const std::string& _value);
	Json(const char* _value);                    // дһ��const char*�����ذ汾�����const char*����ƥ��bool������
	Json(Type _type);

	void operator=(const Json& _json);    
	void operator=(Json&& _json) noexcept;
	void operator=(bool _value);
	void operator=(int _value);
	void operator=(double _value);
	void operator=(const std::string& _value);
	void operator=(const char* _value);          // дһ��const char*�����ذ汾�����const char*����ƥ��bool������

	bool isHaveValue(const std::string& _key);
	bool isHaveValue(int _index);

	Json& operator[](const std::string& _key);   // ͨ��[]��������غ�����û�б߽����һᴴ����ֵ��
	Json& operator[](int _index);               

	Json& at(const std::string& _key);           // ͨ��at�����б߽����Ҳ��ᴴ����ֵ
	Json& at(int _index);

	bool get_bool();                             // ע�⣬�����Ͳ�ƥ��ʱ�����ֵ��δ֪��
	int	get_int();
	double get_double();
	std::string& get_string();
	std::vector<Json>& get_array();
	std::map<std::string, Json>& get_object();

	bool is_null();
	bool is_bool();
	bool is_int();
	bool is_double();
	bool is_array();
	bool is_object();

	bool append(const Json& _json);      // ���������� m_value.m_array
	bool append(Json&& _json) noexcept;  // ���������� m_value.m_array

	bool insert(int _index, const Json& _json);
	bool insert(int _index, Json&& _json) noexcept;
	bool insert(const std::string& _key, const Json& _json);   // const char*����ƥ��int��˲�����д�����غ���
	bool insert(const std::string& _key, Json&& _json) noexcept;

	bool remove(const std::string& _key);
	bool remove(int _index);

	std::string to_string();
	Type getType();
	void clear();
	void auxiliary_deep_copy(const Json& _json);
	Json& getNullJson();                                     // ���ڷ��ؿ�ֵ������ȱ�ݲ��Ǻܰ�ȫ

	//operator bool&();
	//operator int&();     // ����ת������������Ǹ����壬��һ�����ϵı����Ի���ȫ�ԺͲ�ȷ���Եò���ʧ
	//operator double&();
	//operator std::string&();
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










