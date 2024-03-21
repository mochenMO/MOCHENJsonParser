#include "log.h"

using namespace mochen::log;

// =============================================================================================================
// ȫ����

// ����ȫ�ֵ� defauleLogEventManager (ע�⣺����ȫ�ֺ���ʱҲҪ���������ռ�)
inline LogEventManager* mochen::log::getDefaultLogEventManager() // ע����C++11�����ϰ汾��׼�У��涨��̬�ֲ�������ʼ�����̰߳�ȫ��
{
	static LogEventManager defauleLogEventManager{};
	return &defauleLogEventManager;
}

// ȫ�ֺ���
inline Logger* mochen::log::getDefaultLogger()
{
	static Logger defauleLogger("defauleLogger", LogLevel::debug, std::make_shared<ConsoleLogAppender>());
	return &defauleLogger;
}


// ����ȫ�ֵ� logLevelString
const char* logLevelString[5] = { "debug","info","warn","error","fatal" };


// =============================================================================================================
// ��ʽ������


// [��-��-�� ʱ:��:��][��־������][��־�ȼ�][�ļ���]:[�к�][��־��Ϣ]
extern inline void mochen::log::logFormatFuntion_detailed(std::stringstream& _ss, LogEvent& _logEvent)
{
	char timeString[24] = { 0 };
	struct tm* pt = nullptr;     // ע�� pt ָ����ڴ�ռ���ϵͳ���������ֶ��ͷ�

	// ����ʱ��
	pt = localtime(&_logEvent.m_timestamp);
	// [0000-00-00 00:00:00]
	sprintf(timeString, "%d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d",
		pt->tm_year + 1900,
		pt->tm_mon + 1,
		pt->tm_mday,
		pt->tm_hour,
		pt->tm_min,
		pt->tm_sec);

	// [��-��-�� ʱ:��:��][��־������][��־�ȼ�][�ļ���]:[�к�][��־��Ϣ]
	_ss << "[" << timeString << "]";
	_ss << "[" << *(_logEvent.m_loggername) << "]";
	_ss << "[" << logLevelString[(int)_logEvent.m_LogLevel] << "]";
	_ss << "[" << _logEvent.m_filename << "]:";
	_ss << "[" << _logEvent.m_line << "]";
	_ss << "[" << _logEvent.m_content << "]";
	_ss << "\n";
}


// [ʱ:��:��][��־�ȼ�][��Ҫ���ļ���]:[�к�][��־��Ϣ]
extern inline void mochen::log::logFormatFuntion_briefly(std::stringstream& _ss, LogEvent& _logEvent)
{
	char timeString[24] = { 0 };
	struct tm* pt = nullptr;     // ע�� pt ָ����ڴ�ռ���ϵͳ���������ֶ��ͷ�

	// ��ȡ�ļ���
	const char* temp = _logEvent.m_filename + strlen(_logEvent.m_filename);   // �� temp �Ƶ� _logEvent.m_filename �Ľ�β
	while (temp != _logEvent.m_filename) {
		if (*temp == '\\') {
			++temp;
			break;
		}
		--temp;
	}

	// ����ʱ��
	pt = localtime(&_logEvent.m_timestamp);
	// [00:00:00]
	sprintf(timeString, "%0.2d:%0.2d:%0.2d",
		pt->tm_hour,
		pt->tm_min,
		pt->tm_sec);

	// [ʱ:��:��][��־�ȼ�][��Ҫ���ļ���]:[�к�][��־��Ϣ]
	_ss << "[" << timeString << "]";
	_ss << "[" << logLevelString[(int)_logEvent.m_LogLevel] << "]";
	_ss << "[" << temp << "]:";
	_ss << "[" << _logEvent.m_line << "]";
	_ss << "[" << _logEvent.m_content << "]";
	_ss << "\n";
}


// =============================================================================================================
// class LogAppender

LogAppender::LogAppender()
{
	m_pLogFormatFuntion = &logFormatFuntion_briefly;
}

PLogFormatFuntion LogAppender::getFormatFuntion()
{
	return m_pLogFormatFuntion;
}

void LogAppender::setFormatFuntion(PLogFormatFuntion _pLogFormatFuntion)
{
	m_pLogFormatFuntion = _pLogFormatFuntion;
}



// =============================================================================================================
// class ConsoleLogAppender

ConsoleLogAppender::ConsoleLogAppender()
{
	m_pLogFormatFuntion = &logFormatFuntion_briefly;
}

ConsoleLogAppender::ConsoleLogAppender(PLogFormatFuntion _pLogFormatFuntion)
{
	m_pLogFormatFuntion = _pLogFormatFuntion;
}

void ConsoleLogAppender::log(const char* _massage)
{
	printf("%s", _massage);
}


// =============================================================================================================
// class FileLogAppender

FileLogAppender::FileLogAppender()
{
	m_pLogFormatFuntion = &logFormatFuntion_detailed;
	m_filename = "";
	m_fp = nullptr;
	m_maxSize = M_FILEMAXSIZE;
}


FileLogAppender::FileLogAppender(const std::string& _filename, int _maxSize)
{
	m_pLogFormatFuntion = &logFormatFuntion_detailed;
	m_maxSize = _maxSize;
	m_filename = _filename;

	std::string temp = _filename;
	temp += ".txt";

	if ((m_fp = fopen(temp.c_str(), "a")) == nullptr) {
		throw std::logic_error("FileLogAppender::FileLogAppender(): open file filded");    // std::logic_error�쳣�����л��Զ������ļ������кŵȵ�����Ϣ
	}
}


FileLogAppender::~FileLogAppender()
{
	clear();
}


FileLogAppender::FileLogAppender(FileLogAppender&& _value) noexcept
{
	m_filename = _value.m_filename;
	m_fp = _value.m_fp;

	_value.m_filename = "";
	_value.m_fp = nullptr;
}


void FileLogAppender::operator=(FileLogAppender&& _value) noexcept
{
	clear();

	m_filename = _value.m_filename;
	m_fp = _value.m_fp;

	_value.m_filename = "";
	_value.m_fp = nullptr;
}


void FileLogAppender::log(const char* _message)
{
	if (m_fp == nullptr) {
		throw std::logic_error("FileLogAppender::log(): null pointer, <m_fp> is nullptr");
	}

	if (getFileSize() >= m_maxSize) {
		scrolling();  // ����
	}

	fwrite(_message, strlen(_message), 1, m_fp);
	fflush(m_fp);
}

void FileLogAppender::clear()
{
	if (m_fp != nullptr) {
		fclose(m_fp);
		m_fp = nullptr;
	}
	m_filename = "";
}

void FileLogAppender::open(const std::string& _filename)
{
	clear();

	m_filename = _filename;
	std::string temp = _filename;
	temp += ".txt";

	if ((m_fp = fopen(temp.c_str(), "a")) == nullptr) {
		throw std::logic_error("FileLogAppender::open(): open file filded");    // std::logic_error�쳣�����л��Զ������ļ������кŵȵ�����Ϣ
	}
}


inline std::string FileLogAppender::getFilename()
{
	return m_filename;
}


inline int FileLogAppender::getFileSize()
{
	int size = 0;
	fseek(m_fp, 0, SEEK_END);    // �ƶ��ļ�ָ�뵽�ļ���β
	size = ftell(m_fp);          // ��ȡ��ǰ�ļ�ָ���λ�ã����ļ���С����λ�ֽ�
	fseek(m_fp, 0, SEEK_SET);    // �ƶ��ļ�ָ�뵽�ļ���ͷ
	return size;
}

inline void FileLogAppender::setFileSize(int _size)
{
	m_maxSize = _size;
}

void FileLogAppender::scrolling()
{
	if (m_fp != nullptr) {   // ����û��ʵ��setFilename���������� m_fp != nullptr ʱ m_filename != ""
		fclose(m_fp);
		m_fp = nullptr;
	}
	else {
		throw std::logic_error("FileLogAppender::scrolling(): null pointer, <m_fp> is nullptr");
	}

	std::string temp = m_filename;
	temp += "_log_";
	char timeString[24] = { 0 };

	time_t timestamp = time(nullptr);
	struct tm* pt = localtime(&timestamp);    // ע�� pt ָ����ڴ�ռ���ϵͳ���������ֶ��ͷ�

	// 0000-00-00#00-00-00
	sprintf(timeString, "%d-%0.2d-%0.2d#%0.2d-%0.2d-%0.2d",
		pt->tm_year + 1900,
		pt->tm_mon + 1,
		pt->tm_mday,
		pt->tm_hour,
		pt->tm_min,
		pt->tm_sec);

	// m_filename_log_0000-00-00#00-00-00
	temp += timeString;
	temp += ".txt";

	if ((m_fp = fopen(temp.c_str(), "a")) == nullptr) {
		throw std::logic_error("FileLogAppender::scrolling(): open file failded");    // std::logic_error�쳣�����л��Զ������ļ������кŵȵ�����Ϣ
	}
}



// =============================================================================================================
// class LogEventManager

LogEventManager::LogEventManager() :
	m_thread(&LogEventManager::dealLogEvent_threadFuntion, this),  // ע��ʹ�ó�Ա����ָ��ʱҪ��������
	m_isCanExit(false)
{
	m_logEventQueue = (LogEventQueue*)malloc(sizeof(LogEventQueue));
	m_logEventQueue->m_next = nullptr;

	m_ptrWrite = m_logEventQueue;
	m_ptrRead = m_logEventQueue;

	m_LogAppenderListMap = new std::map<std::string, std::list<std::shared_ptr<LogAppender>>>{};
}


LogEventManager::~LogEventManager()
{
	m_isCanExit = true;         // ע�� m_isCanExit = true �Ҵ�������־�������������ݺ�����˳�
	m_thread.join();            // �ȴ���������־��������������
	clearLogEventQueue();
	clearLogAppenderListMap();

	printf("~LogEventManager\n");  // LogEventManagerӦ�������һ��������
	_CrtDumpMemoryLeaks();         // ����ڴ��Ƿ�й¶
}



void LogEventManager::clearLogEventNodeData(LogEventQueue* _node)
{
	delete _node->m_data.m_loggername;
	free(_node->m_data.m_content);        // �ͷ��������Դ
	free(_node);
}

void LogEventManager::clearLogEventQueue()
{
	LogEventQueue* tempNode = m_logEventQueue;
	m_logEventQueue = m_logEventQueue->m_next;
	free(tempNode);      // �Ȱ��ڱ��ڵ����ٵ�����Ϊ�ڱ��ڵ���û�д����ݣ����Բ��� free(m_data.m_content)

	while (m_logEventQueue != nullptr) {
		tempNode = m_logEventQueue;
		m_logEventQueue = m_logEventQueue->m_next;
		clearLogEventNodeData(tempNode);
	}
	m_logEventQueue = nullptr;
}


void LogEventManager::clearLogAppenderListMap()
{
	for (auto mapIt = m_LogAppenderListMap->begin(); mapIt != m_LogAppenderListMap->end(); ++mapIt) {
		// list = mapIt->second
		for (auto vecterIt = mapIt->second.begin(); vecterIt != mapIt->second.end(); ++vecterIt) {
			vecterIt->reset();   // �ͷ�����ָ��
		}
	}
	delete m_LogAppenderListMap;
}


void LogEventManager::addAppender(std::string _loggername, std::shared_ptr<LogAppender> _appender)
{
	m_mutex.lock();  // ע�⣺ǧ������Ǽ�����Ϊ���ṹ�и�Ԫ��֮���й����������Ƕ����� ��������
	(*m_LogAppenderListMap)[_loggername].push_back(_appender);   // map�Ǹ����ӵĺں��࣬�޷���ԭ�Ӳ���
	m_mutex.unlock();
}


inline bool LogEventManager::isFindLogger(const std::string& _loggername)
{
	return (m_LogAppenderListMap->find(_loggername) != m_LogAppenderListMap->end());
}


inline std::map<std::string, std::list<std::shared_ptr<LogAppender>>>* LogEventManager::getLogAppenderListMap()
{
	return m_LogAppenderListMap;
}

void LogEventManager::dealLogEvent_threadFuntion()
{
	LogEventQueue* tempNode = nullptr;    // ��ʱ����ɶ��Ľڵ㣬��ȡ��󣬸���ɾ��
	std::list<std::shared_ptr<LogAppender>>* tempList = nullptr;  // ��ָ�����ͱ�����ÿ��������ƶ�����
	LogEvent tempData = { 0 };
	std::stringstream ss;


	while (m_isCanExit == false || m_ptrRead->m_next != nullptr)  // �� m_ptrRead = m_ptrWrite ʱ m_ptrRead->m_next Ҳ���ܵ��ڴ����ϱ���
	{
		if (m_ptrRead->m_next != nullptr)
		{
			tempNode = m_ptrRead->m_next;
			m_ptrRead = m_ptrRead->m_next;

			tempData = tempNode->m_data;
			tempList = &(*m_LogAppenderListMap)[*(tempData.m_loggername)];

			// ���� tempList �����е� LogAppender
			for (auto it = tempList->begin(); it != tempList->end(); ++it) {  // ������ (*(*it)).getType();
				it->get()->getFormatFuntion()(ss, tempData);
				it->get()->log(ss.str().c_str());
				ss.str("");   // ��� stringstream
			}

			if (m_logEventQueue->m_next != m_ptrRead) {   // ͷɾ������ m_logEventQueue = m_ptrWrite ʱ m_logEventQueue->m_next Ҳ���ܵ��ڴ����ϱ���
				tempNode = m_logEventQueue->m_next;
				m_logEventQueue->m_next = tempNode->m_next;
				clearLogEventNodeData(tempNode);
			}

		}

	}

}


void LogEventManager::addLogEvent(LogEvent _logEvent)
{
	LogEventQueue* newNode = (LogEventQueue*)malloc(sizeof(LogEventQueue));
	newNode->m_next = nullptr;
	newNode->m_data = _logEvent;

	while (true) // ԭ�Ӳ���ͨ��Ҫ���������
	{
		LogEventQueue* currentWrite = m_ptrWrite;     // m_ptrWrite��ԭ�Ӳ����Ķ�����˿���ֱ�Ӷ�ȡ�����жϣ������� m_ptrWrite = XXXX �� m_ptrWrite->next = XXXX
		LogEventQueue* next = currentWrite->m_next;   // ע�⣺���� m_ptrWrite->m_next

		if (currentWrite == m_ptrWrite) {
			if (next == nullptr) {
				if (InterlockedCompareExchangePointer((PVOID*)(&(currentWrite->m_next)), newNode, nullptr) == nullptr) {   // ע���� currentWrite->m_next ������ m_ptrWrite->next ����Ϊ m_ptrWrite ���ܻ�䡣ͬʱ�� currentWrite == m_ptrWrite ʱ��m_ptrWrite->next Ҳ���ܵ��ڴ����ϱ���
					InterlockedExchangePointer((PVOID*)(&(m_ptrWrite)), newNode); // ע���� newNode ��ֵ��ע�����ʹ m_ptrWrite ������β��Ҳû��ϵ
					break;
				}
			}
		}
		else {
			InterlockedExchangePointer((PVOID*)(&(m_ptrWrite)), next);  // ע���� next ��ֵ
		}
	}
}




// =============================================================================================================
// class Logger

std::string* Logger::m_syncDebugSetting = nullptr;  // ע�⣺��̬��Ա����Ҫ���������ʼ���󣬲����ڳ�Ա�����и�ֵ������ᱨ���޷��������ⲿ����

Logger::Logger(const std::string& _loggername, LogLevel _level, std::shared_ptr<LogAppender> _appender)
{
	// ע�� getDefaultLogEventManager() ��δ��뱣֤�� LogEventManager �� Logger ֮ǰ������ȷ���� LogEventManager ��������
	if (getDefaultLogEventManager()->isFindLogger(_loggername) == true) {
		throw std::logic_error("Logger::Logger(): loggername must be unqiue, <_loggername> is already exists");
	}

	m_level = _level;
	m_loggername = _loggername;
	getDefaultLogEventManager()->addAppender(_loggername, _appender);

	return;
}

bool Logger::setUpSyncDebug()
{
	void* temp = &m_loggername;
	if (m_syncDebugSetting == nullptr) {
		InterlockedCompareExchangePointer((PVOID*)(&m_syncDebugSetting), temp, nullptr);
		return true;
	}
	return false;
}



void Logger::log(LogLevel _level, const char* _filename, int _line, const char* _format, ...)
{
	if ((int)m_level > (int)_level || (m_syncDebugSetting != nullptr && m_syncDebugSetting != &m_loggername)) {
		return;
	}

	va_list args;
	va_start(args, _format);
	int size = vsnprintf(nullptr, 0, _format, args);
	char* buffer = (char*)malloc(sizeof(char) * (size + 1));    // +1�����β��"\0"
	vsprintf(buffer, _format, args);
	va_end(args);

	LogEvent data;
	data.m_timestamp = time(nullptr);
	data.m_loggername = new std::string(m_loggername);
	data.m_LogLevel = _level;
	data.m_filename = _filename;
	data.m_line = _line;
	data.m_content = buffer;

	if (m_syncDebugSetting == nullptr) {
		getDefaultLogEventManager()->addLogEvent(data);
	}
	else {
		std::stringstream ss;
		auto* tempMap = getDefaultLogEventManager()->getLogAppenderListMap();
		std::list<std::shared_ptr<LogAppender>>* tempList = &(*tempMap)[m_loggername];

		for (auto it = tempList->begin(); it != tempList->end(); ++it) {  // ������ (*(*it)).getType();
			it->get()->getFormatFuntion()(ss, data);
			it->get()->log(ss.str().c_str());
			ss.str("");   // ��� stringstream
		}
		free(data.m_content);
		delete data.m_loggername;
	}
}

inline std::string Logger::getLoggerName()
{
	return m_loggername;
}

bool Logger::addAppender(std::shared_ptr<LogAppender> _appender)
{
	getDefaultLogEventManager()->addAppender(m_loggername, _appender);
	return true;
}

inline LogLevel Logger::getLogLevel()
{
	return m_level;
}

inline void Logger::setLogLevel(LogLevel _level)
{
	m_level = _level;
}







