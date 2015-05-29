/*
 * mini_log.h
 *
 *  Created on: 2015年5月29日
 *      Author: skyyuan
 */

#ifndef MINI_LOG_H_MINI_LOGCPP_
#define MINI_LOG_H_MINI_LOGCPP_

#include "config.h"
#include "mini_log_tools.h"

namespace mini_log {

class CMiniLog {
public:
	CMiniLog();
	~CMiniLog();

	void Log(LOG_LEVEL log_level, const char * fmt, ...);
	void LogEx(LOG_LEVEL log_level, const char *file_name, const char *fn_name,
			unsigned int line, const char * fmt, ...);
	/*
	 *设置日志界别
	 *
	 * @param log_level 日志级别
	 *
	 */
	inline void SetLogLevel(int log_level) {
		_log_level = log_level;
	}

	/*
	 *设置日志存放路径
	 *
	 * @param file_path 日志存放路径
	 *
	 */
	inline void SetLogFilePath(const char *file_path) {
		if (file_path != NULL && strlen(file_path) > 0) {
			memset(_sFilePath, 0, sizeof(_sFilePath));
			strncpy(_sFilePath, file_path, sizeof(_sFilePath));
		}
	}

	/*
	 *设置打印日志时，不滚动日志文件
	 *
	 */
	inline void CloseShiftLog() {
		_cInnerLogger.SetLogShift(false);
	}

	/*
	 *设置日志前缀名和日志内容格式
	 *
	 * @param pre_name 日志前缀名
	 * @param mode 日志内容格式
	 */
	inline void SetLogNameFmt(const char *pre_name, unsigned int mode) {
		memset(_sFilePrefixName, 0, ISD_MAX_PATH);

		if (pre_name == NULL) {
			strncpy(_sFilePrefixName, _sProcessName,
					sizeof(_sFilePrefixName) - 1);
		} else {
			strncpy(_sFilePrefixName, pre_name, ISD_MAX_PATH - 1);
		}

		if (mode > 0)
			_emNameFmt = mode;
		else
			_emNameFmt = LOG_NAME_NONE;
	}

	/*
	 *设置日志内容格式
	 *
	 * @param mode 日志前缀名
	 */
	inline void SetLogContentFmt(unsigned int mode) {
		if (mode > 0) {
			_emContentFmt = mode;
		} else {
			_emContentFmt = DSQ_ALL;
		}
	}

	/*
	 *设置缓写缓冲区大小，打开缓写模式(默认情况下，缓写模式是关闭的)
	 *
	 * @param delay_log_size 缓写缓冲区大小
	 */
	inline void SetWMode(unsigned int delay_log_size) {
		if (delay_log_size > 0) {
			_cInnerLogger.SetSaveDelay(true, delay_log_size);
		}
	}

	/*
	 *设置单个日志文件大小和日志滚动个数，打开日志滚动模式(默认情况下，不滚动打日志)
	 *
	 * @param  log_max_size 单个日志文件大小
	 * @param  log_save_cnt 日志滚动个数，默认为0，不滚动
	 */
	inline void SetLogNum(unsigned int log_save_cnt = 0) {
		if (log_save_cnt > 0) {
			_bLogShift = true;
			_uiLogSaveCount = log_save_cnt;
		}
	}
	inline void SetLogSize(unsigned int log_max_size) {
		if (log_max_size > 0) {
			_uiLogMaxSize = log_max_size;
		}
	}

private:

	int _log_level;							//日志的级别
	bool _bSaveDelay;						//是否打开缓写模式
	char *_sData;							//缓写模式下的日志缓冲区
	unsigned int _uiCurWrite;				//缓写模式下，当前缓冲区大小
	unsigned int _uiDelayLogSize;			//缓写模式下，日志缓冲区容量

	bool _bLogShift;						//是否需要滚动日志
	long _uiLogMaxSize;				//单个日志文件的极限大小，以B为单位
	unsigned int _uiLogSaveCount;			//滚动模式下最多保存的日志个数

	char _sFilePrefixName[LOG_MAX_PATH];		//日志文件名前缀
};

class CMiniLogger {
public:
	/**
	 * 指向CISDLog实例的一个指针。该实例全程唯一。
	 */
	static CMiniLog *GetInstance() {
		if (_pInstance == 0)
			_pInstance = new CMiniLog();

		return _pInstance;
	}

	/**
	 * 默认构造函数
	 */
	CMiniLogger() {
	}

	/**
	 * 析构函数
	 */
	~CMiniLogger() {
	}

public:
	static CMiniLog *_pInstance;
};

inline void ISD_LOGGER_INIT(int log_level, const char *file_path = NULL) {
	CMiniLogger::GetInstance()->SetLogLevel(log_level);
	CMiniLogger::GetInstance()->SetLogFilePath(file_path);
}

inline CMiniLog *ISD_LOGGER_GET_INSTANCE() {
	return CMiniLogger::GetInstance();
}

}

#endif /* MINI_LOG_H_MINI_LOGCPP_ */
