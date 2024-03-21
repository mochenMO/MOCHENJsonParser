/*// ������˵��
* 1.ע����getBool, getInt���ຯ����ȡֵʱ��������Ͳ�ƥ���������ֵ��δ֪�ġ�
* 2.��������ݵ�������������������ܵİ���ȷ�����ݽ����������������������־
* 
* 
*/





/*// ��δ���������
* 1.��ô���û�ҵ������⣬
*
*
*
*/


/*// ��ѧ����֪ʶ
* 1.��union����class�������Լ�洢�ռ�
* 2.��class enum ���� enum����������ʽת����int
* 3.����ֻ�ܴ��const���ݣ�ͨ����ֵ���ͣ�ͬʱbegin��end���ص��ǵ�������ʵ�壬�������á�
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

#define _CRT_SECURE_NO_WARNINGS

#include "../log/log.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>    // std::stringstream
#include <iomanip>    // std::setprecision ��չ std::stringstream �洢��С������
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

	bool getBool();   // ע�⣬�����Ͳ�ƥ��ʱ�����ֵ��δ֪��
	int	getInt();                              
	double getDouble();                        
	std::string getString();    // ע�ⷵ��ֵ����
	std::vector<Json> getArray();              
	std::map<std::string, Json> getObject();   

	bool isNull();
	bool isBool();   
	bool isInt();    
	bool isDouble(); 
	bool isString(); 
	bool isArray();  
	bool isObject();

	bool append(const Json& _json);      // ���������� m_value.m_array
	bool append(Json&& _json) noexcept;  // ���������� m_value.m_array

	bool insert(int _index, const Json& _json);
	bool insert(int _index, Json&& _json) noexcept;
	bool insert(const std::string& _key, const Json& _json);   // const char*����ƥ��int��˲�����д�����غ���
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
	std::string parseString();  // ������Json����Ϊjosn_object��keyҲ��std::string���������Json��Ҫ�����ڴ沢�ͷ�
	Json parseArray();           
	Json parseObject();          

	void clear();
	bool isReadEndOfFile();    
};


// ʹ�õ�ǰ����������֤Josn�ļ��е���������ȷ�Ŀɶ���
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


extern inline log::Logger* getJsonLogger();   // �ɹ������ļ�ʹ��









};

};






#endif // !_MOCHEN_JSON_H_










