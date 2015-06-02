/*
 * config.h
 *
 *  Created on: 2015年5月29日
 *      Author: skyyuan
 */

#ifndef CONFIG_H_MINI_LOGCPP_
#define CONFIG_H_MINI_LOGCPP_

#include <stdlib.h>
#include <cstdio>
#include <iosfwd>
#include <istream>
#include <sstream>
#include <ostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <deque>
#include <math.h>
#include <utility>
#include <cassert> //Assertions
#include <iomanip> //Standard manipulators
#include <memory>
#include <stddef.h> // Common definitions size_t
#include <cstddef> //  Common definitions size_t     
#include <sys/types.h> //Primitive System Data Types
#include <sys/stat.h>  //File Characteristics
#include <stdarg.h> //Variable arguments
#include <unistd.h> //Symbolic Constants
#include <algorithm>
#include <functional> 

using namespace std;

#define LOG_MAX_PATH                    128
#define LOG_MAX_LOG_SIZE                64 * 1024
#define LOG_DEFAULT_LOG_MAX_SIZE        (1 << 29)
#define LOG_DEFAULT_LOG_SAVE_COUNT      5
#define LOG_SHM_CONTROL_DATA_SIZE       (1 << 24)

#define LOG_NAME_DEFAULT_FLAG (LOG_NAME_DATE_FLAG | LOG_NAME_LEVEL_FLAG)
#define LOG_CONTENT_DEFAULT_FLAG  (LOG_CONTENT_TIME_FLAG | LOG_CONTENT_FILE_FLAG | LOG_CONTENT_LINE_FLAG | LOG_CONTENT_FUN_FLAG)

#ifndef MINI_LOGGER_NONE
#define MINI_LOGGER_BASE(L, X,Y,...) do{CMiniLogger::GetInstance()->LogEx(L, __FILE__, __FUNCTION__, __LINE__, X, ##Y);}while(0)
#define MINI_LOGGER_INFO(X, Y...)  do{MINI_LOGGER_BASE(LOG_TRACE,X, ##Y);}while(0)
#define MINI_LOGGER_WARN(X, Y...) do{MINI_LOGGER_BASE(LOG_WARN,X, ##Y)}while(0)
#define MINI_LOGGER_ERROR(X, Y...) do{ MINI_LOGGER_BASE(LOG_ERROR,X, ##Y);}while(0)
#define MINI_LOGGER_FATAL(X, Y...) do{MINI_LOGGER_BASE(LOG_FATAL,X, ##Y);}while(0)
#define MINI_LOGGER_NONE(X, Y...)  do{NULL;}while(0)
#endif /* MINI_LOGGER_NONE */

#ifndef SAFE_MINI_LOGGER_NONE
#define SAFE_MINI_LOGGER_BASE(logger,L, X,Y,...) do{logger->LogEx(L, __FILE__, __FUNCTION__, __LINE__, X, ##Y);}while(0)
#define SAFE_MINI_LOGGER_INFO(logger,X, Y...)  do{MINI_LOGGER_BASE(logger,LOG_TRACE,X, ##Y);}while(0)
#define SAFE_MINI_LOGGER_WARN(logger,X, Y...) do{MINI_LOGGER_BASE(logger,LOG_WARN,X, ##Y)}while(0)
#define SAFE_MINI_LOGGER_ERROR(logger,X, Y...) do{ MINI_LOGGER_BASE(logger,LOG_ERROR,X, ##Y);}while(0)
#define SAFE_MINI_LOGGER_FATAL(logger,X, Y...) do{MINI_LOGGER_BASE(logger,LOG_FATAL,X, ##Y);}while(0)
#define SAFE_MINI_LOGGER_NONE(logger,X, Y...)  do{NULL;}while(0)
#endif /* SAFE_MINI_LOGGER_NONE */

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
        LOG_NAME_NONE_FLAG = 0x0,				//Log文件名什么都不包含
        LOG_NAME_DATE_FLAG = 0x1,				//Log文件名是否包含日期(yyyymmddhhmmss)
        LOG_NAME_LEVEL_FLAG = 0x2,				//Log文件名是否包含日志级别
        LOG_NAME_PID_FALG = 0x4,			    //Log文件名是否包含进程ID
        LOG_NAME_ALL_FLAG = 0x7				//Log文件名什么都包含
    };
    
//日志内容格式 LOG_CONTENT_FORMAT
    enum LOG_CONTENT_FMT {
        LOG_CONTENT_TIME_FLAG = 0x1,					//日志内容是否包含时间
        LOG_CONTENT_PID_FLAG = 0x2,					//日志内容是否包含进程ID
        LOG_CONTENT_FILE_FLAG = 0x4,					//日志内容是否包含文件名
        LOG_CONTENT_LINE_FLAG = 0x8,					//日志内容是否包含行号
        LOG_CONTENT_FUN_FLAG = 0x16,				    //日志内容是否包含函数名称
        LOG_CONTENT_LEVEL_FLAG = 0x32,					//日志内容是否包含日志级别
        LOG_CONTENT_ALL_FLAG = 0x3f,					//日志内容包含上述所有
    };

}

#endif /* CONFIG_H_MINI_LOGCPP_ */
