/*
 * config.h
 *
 *  Created on: 2015年5月29日
 *      Author: skyyuan
 */

#ifndef CONFIG_H_MINI_LOGCPP_
#define CONFIG_H_MINI_LOGCPP_

#define LOG_MAX_PATH					128
#define LOG_MAX_LOG_SIZE				64 * 1024
#define LOG_DEFAULT_LOG_MAX_SIZE		(1 << 29)
#define LOG_DEFAULT_LOG_SAVE_COUNT		5
#define LOG_SHM_CONTROL_DATA_SIZE		(1 << 24)

namespace mini_log {
enum LOG_LEVEL {
	LOG_INFO = 0, //
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,
	LOG_NONE
};

//日志名格式
enum LOG_NAME_FMT {
	LOG_NAME_NONE = 0x0,				//Log文件名什么都不包含
	LOG_DATE_FLAG = 0x1,				//Log文件名是否包含日期(yyyymmddhhmmss)
	LOG_LEVEL_FLAG = 0x2,				//Log文件名是否包含日志级别
	LOG_PID_FALG = 0x4,					//Log文件名是否包含进程ID
	LOG_FMT_ALL = 0x7					//Log文件名什么都包含
};

//日志内容格式
enum LOG_CONTENT_FMT {
	TIME_DSP = 0x1,						//日志内容是否包含时间
	PID_DSP = 0x2,						//日志内容是否包含进程ID
	FILE_DSQ = 0x4,						//日志内容是否包含文件名
	FILE_LINE = 0x8,					//日志内容是否包含行号
	FUN_DSP = 0x10,						//日志内容是否包含函数名称
	LEVEL_DSQ = 0x20,					//日志内容是否包含日志级别
	DSQ_ALL = 0x3f,						//日志内容包含上述所有
};

#ifndef MINI_LOGGER_NONE
#define MINI_LOGGER_BASE(L, X,Y,...) do{CMiniLogger::GetInstance()->LogEx(L, __FILE__, __FUNCTION__, __LINE__, X, ##Y);}while(0)
#define MINI_LOGGER_INFO(X, Y...)  do{MINI_LOGGER_BASE(LOG_TRACE,X, ##Y);}while(0)
#define MINI_LOGGER_WARN(X, Y...) do{MINI_LOGGER_BASE(LOG_WARN,X, ##Y)}while(0)
#define MINI_LOGGER_ERROR(X, Y...) do{ MINI_LOGGER_BASE(LOG_ERROR,X, ##Y);}while(0)
#define MINI_LOGGER_FATAL(X, Y...) do{MINI_LOGGER_BASE(LOG_FATAL,X, ##Y);}while(0)
#define MINI_LOGGER_NONE(X, Y...)  do{NULL;}while(0)
#endif /* MINI_LOGGER_NONE */

}

#endif /* CONFIG_H_MINI_LOGCPP_ */
