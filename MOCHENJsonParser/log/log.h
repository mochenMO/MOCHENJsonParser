/*// ������˵��
* 1.����־ϵͳ�Ĺ���ģʽ
* 
*		                 +--------------+
*		�߳�1 ---------->|              |   LogEventManager�첽�߳�    
*		�߳�2 ---------->|  ��(����д��) | -------------------------->
*		�߳�n ---------->|              |
*		                 +--------------+
* 
* 2.LogEventManager������LogEventQueueָ��ʵ�������ṹ��������
*	(1).LogEventQueue�����Զ����˫������ʵ�ֵġ�
*   (2).read,write,delete ����ָ���Ӧ���ֲ����������ֲ������Ⱥ�˳�����ν��У���write��read��delete��
*	    �� read ���Ե��� write, delete = read->prev���˴�֮�䲻���ͻ��
*   
*		 delete             read               write
*		   |                 |                   |
*		+--+--+       +------+-----+      +------+-----+ 
*		| ... | <---->| LogEvent01 |<---->| LogEvent02 | ----> nullptr 
*		+-----+       +------------+      +------------+ 
* 
* 3.ע��Logger��Loggername�Ǿ���Ψһ�Եģ������ά�������ԡ�
* 4.�������ĿҪʹ�ã�����־�⣬��Ѹ���־�������.cpp�ļ��У�ͬʱ����һ��ȫ�ֱ����������Ҫ��¶��ȫ�ֱ�����
*   �����������Ӧ��ͷ�ļ���������ȫ�ֱ�����
* 5.Ҫ�������ϵͳ���浱ǰʹ�õ��ļ���������ʵ����д���ܣ���Ȼһ���ļ����ˣ�ÿ���������򶼻ᷢ���ļ�������
*/




/*// ��δ���������
* 1.
* 
*/


/*// ��ѧ����֪ʶ
* 1.
* 
*/



#pragma once
#ifndef _MOCHEN_LOG_H_
#define _MOCHEN_LOG_H_

#define _CRT_SECURE_NO_WARNINGS


#include <stdexcept>  // throw std::logic_error 
#include <memory>     // std::share_ptr
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <list>
#include <map>

#include <stdarg.h>
#include <time.h>

namespace mochen
{

namespace log
{

// ��־����
enum class LogLevel
{
	debug = 0,
	info,
	warn,
	error,
	fatal
};

// ��־���Ŀ�ĵ�
class LogAppender
{
public:
	enum class Type
	{
		withoutLogAppender = 0,
		ConsoleLogAppender,
		FileLogAppender
	};
protected:
	Type m_type;
public:
	LogAppender();
	virtual ~LogAppender() {}                    // ������������Ҫʵ�ֺ����Ķ��壬���ֻд����˵�������ܳ������Ӵ���

	virtual void log(const char* _massage) = 0;  // ���麯��
	inline Type getType();
};


class ConsoleLogAppender : public LogAppender
{
public:
	ConsoleLogAppender(LogLevel _level = LogLevel::debug);

	void log(const char* _massage) override;
};

// ����ȫ�ֺ�����������ͷ�ļ�������ȫ�ֱ������ҷ��ص�ָ�����ͣ�������ÿ������ƶ���غ���
extern inline std::shared_ptr<LogAppender>* getDefaultLogAppender();


class FileLogAppender : public LogAppender
{
private:
	std::string m_filename;
	FILE* m_fp;
	int m_maxSize;
#define M_FILEMAXSIZE 409600
public:
	FileLogAppender();
	FileLogAppender(const std::string& _filename, int _maxSize = M_FILEMAXSIZE, LogLevel _level = LogLevel::debug);
	~FileLogAppender();

	FileLogAppender(const FileLogAppender& _value) = delete;  // ���ܿ�������Ϊʵ�ֹ�������ʱҪ�ı��ļ�����
	FileLogAppender(FileLogAppender&& _value) noexcept ;

	void operator=(const FileLogAppender& _value) = delete;
	void operator=(FileLogAppender&& _value) noexcept;

	void log(const char* _massage) override;
	void clear();
	void open(const std::string& _filename);
	// inline std::string setFilename();      // Ϊ��m_filename��m_fp��ƥ�䣬��ֹ�û�ֱ�Ӹ��ļ���������ͨ��openȥ���ļ���
	inline std::string getFilename();

	inline int getFileSize();
	inline void setFileSize(int _size);
	void scrolling();   // ��־�Ĺ������ƣ�Ҫ�������ϵͳ���浱ǰʹ�õ��ļ���������ʵ����д���ܣ�����
};


// ��־�¼�
struct LogEvent
{
	time_t	     m_timestamp;
	std::string *m_loggername; // �����ڴ�����Ҫ�ֶ��ͷ� 
	LogLevel     m_LogLevel;
	const char  *m_filename;  
	int		     m_line;
	char        *m_content;    // �����ڴ�����Ҫ�ֶ��ͷ� 
};


// ��־�¼�������
class LogEventManager
{
private:
	struct LogEventQueue
	{
		LogEvent m_data;
		LogEventQueue* m_next;
		LogEventQueue* m_prev;
	};
private:
	std::thread    m_thread;
	std::mutex     m_mutex;
	LogEventQueue *m_logEventQueue;
	LogEventQueue *m_ptrWrite;
	LogEventQueue *m_ptrRead;
	LogEventQueue *m_ptrDelete;
	bool m_isCanExit;
	std::map<std::string, std::list<std::shared_ptr<LogAppender>>> *m_LogAppenderListMap; // LogAppender������࣬��ֵʱ�޷�����������Ŀ������ƶ����������LogAppender*��ͬʱ��Ϊ����������ڴ���������Ϊ�˷�������ڴ棬������ָ�롣
public:
	LogEventManager();
	~LogEventManager();

	LogEventManager(const LogEventManager& _value) = delete;
	LogEventManager(LogEventManager&& _value) noexcept = delete;

	LogEventManager& operator=(const LogEventManager& _value) = delete;
	LogEventManager& operator=(LogEventManager&& _value) noexcept = delete;
	
	void clearLogEventNodeData(LogEventQueue* _node);
	void clearLogEventQueue();

	void clearLogAppenderListMap();
	void addAppender(std::string _loggername, std::shared_ptr<LogAppender> _appender);   // ע�������Logger::addAppender���ã��ռ��Ƕ����ģ���˲����̰߳�ȫ����
	inline bool isFindLogger(const std::string& _loggername);

	void logFormatter(std::stringstream& _ss, LogEvent& _logEvent);
	void dealLogEvent_threadFuntion();
	void addLogEvent(LogEvent _logEvent);   // �ú�����Ҫ�̰߳�ȫ
};


// ��־��
class Logger
{
private:
	// LogEventManager defauleLogEventManager // ��ȫ�ֵ�defauleLogEventManager
	std::string m_loggername; 
	LogLevel m_level;
public:
	Logger(const std::string& _loggername, LogLevel _level = LogLevel::debug, std::shared_ptr<LogAppender> _appender = (*getDefaultLogAppender()));
	~Logger() = default;   // LogAppender���ͷŽ���LogEventManager

	Logger(const Logger& _logger) = delete;
	Logger(Logger&& _logger) noexcept = delete;

	Logger& operator=(const Logger& _logger) = delete;
	Logger& operator=(Logger&& _logger) noexcept = delete;

	void log(LogLevel _level,const char* _filename, int _line, const char* _format, ...);

	// inline void setLoggerName(const std::string _loggerName);    // ��ֹ�û��� loggerName
	inline std::string getLoggerName();

	bool addAppender(std::shared_ptr<LogAppender> _appender);       // ע�⽨���ڴ����߳�֮ǰ�øú�����������ɳ�ͻ�����
	// bool deleteAppender(....);                                   // ����ʵ�ʿ�������ʱ��ʵ��deleteAppender����

	inline LogLevel getLogLevel();
	inline void setLogLevel(LogLevel _level);
};

// ����ȫ�ֺ�����������ͷ�ļ�������ȫ�ֱ������ҷ��ص�ָ�����ͣ�������ÿ������ƶ���غ���
extern inline Logger* getDefaultLogger();


// ע����Ϊ�꺯��Ҫ�������ļ���չ�������Ժ����������õ��ĺ������߱�������Ҫ���������������ռ���
#define default_debug(_format, ...) \
	mochen::log::getDefaultLogger()->log(mochen::log::LogLevel::debug, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define default_info(_format, ...) \
	mochen::log::getDefaultLogger()->log(mochen::log::LogLevel::info, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define default_warn(_format, ...) \
	mochen::log::getDefaultLogger()->log(mochen::log::LogLevel::warn, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define default_error(_format, ...) \
	mochen::log::getDefaultLogger()->log(mochen::log::LogLevel::error, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define default_fatal(_format, ...) \
	mochen::log::getDefaultLogger()->log(mochen::log::LogLevel::fatal, __FILE__, __LINE__, _format, ##__VA_ARGS__)


#define logger_debug(logger, _format, ...) \
	logger.log(mochen::log::LogLevel::debug, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define logger_info(logger, _format, ...) \
	logger.log(mochen::log::LogLevel::info, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define logger_warn(logger, _format, ...) \
	logger.log(mochen::log::LogLevel::warn, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define logger_error(logger, _format, ...) \
	logger.log(mochen::log::LogLevel::error, __FILE__, __LINE__, _format, ##__VA_ARGS__)

#define logger_fatal(logger, _format, ...) \
	logger.log(mochen::log::LogLevel::fatal, __FILE__, __LINE__, _format, ##__VA_ARGS__)



};

};






#endif // !_MOCHEN_LOG_H_


