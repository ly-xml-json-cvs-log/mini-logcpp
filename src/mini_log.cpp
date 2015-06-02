/*
 * mini_log.cpp
 *
 *  Created on: 2015-5-29
 *      Author: tiankonguse
 */

#include "mini_log.h"

namespace mini_log {
    
    const char *sLogLevel[] = { "INFO", "WARN", "ERROR", "FATAL", "" };
    const char** CMiniLog::level_str = sLogLevel;
    
    CMiniLog::CMiniLog() {
        
        SetLogLevel(LOG_ERROR);
        SetLogNameFmt(LOG_NAME_DEFAULT_FLAG);
        SetLogContentFmt(LOG_CONTENT_DEFAULT_FLAG);
        
        //默认缓存关闭
        _bSaveDelay = false;
        _uiDelayLogSize = 0;
        _sData = NULL;
        _uiCurWrite = 0;
        
        //默认情况下，最大单个Log文件大小为1G，滚动模式打日志, 文件数目为5
        SetLogNum(LOG_DEFAULT_LOG_SAVE_COUNT);
        SetLogSize(LOG_DEFAULT_LOG_MAX_SIZE);
        
        memset(_sFilePath, 0, LOG_MAX_PATH);
        memset(_sFilePrefixName, 0, LOG_MAX_PATH);
        memset(_sProcessName, 0, LOG_MAX_PATH);
        
        //默认情况下日志文件存放在/var/log/目录下
        strncpy(_sFilePath, "/var/log/", LOG_MAX_PATH);
        
        //获得进程ID和进程名
        _iProcessID = getpid();
        GetProcessName(_sProcessName, LOG_MAX_PATH);
        
        //默认情况下日志前缀为进程名
        strncpy(_sFilePrefixName, _sProcessName, sizeof(_sFilePrefixName) - 1);
        
        _bLogShift = true;
        _uiLogMaxSize = LOG_DEFAULT_LOG_MAX_SIZE;
        _uiLogSaveCount = LOG_DEFAULT_LOG_SAVE_COUNT;
        
        _lock.init();
    }
    
    CMiniLog::~CMiniLog() {
        if (_sData != NULL) {
            delete[] _sData;
            _sData = NULL;
        }
    }
    
    void CMiniLog::LogEx(LOG_LEVEL log_level, const char *file_name,
            const char *fn_name, unsigned int line, const char * fmt, ...) {
        va_list argList;
        va_start(argList, fmt);
        
        int len = 0;
        char sFileName[LOG_MAX_PATH];
        char sBuff[LOG_MAX_LOG_SIZE];
        
        if (log_level < _log_level) {
            return;
        }
        
        len = LogContentPrefix(sBuff, (LOG_MAX_LOG_SIZE - 2), log_level,
                file_name, fn_name, line);
        
        if (len > 0) {
            len += vsnprintf(sBuff + len, (LOG_MAX_LOG_SIZE - 2) - len - 1, fmt,
                    argList);
        }
        
        //如果缓冲区溢出了，len记录的缓冲区大小就无效了，必须重新求取下sBuff的长度，注意之
        len = strlen(sBuff);
        strncat(sBuff + len, "\r\n", 2);
        len += 2;
        
        if (LogFileName(sFileName, LOG_MAX_PATH, log_level) < 0) {
            return;
        } else {
            SetFilePrefixName(sFileName);
        }
        
        Log(sBuff, len);
        
        va_end(argList);
    }
    
    int CMiniLog::Log(const char *sLogContent, unsigned int uilen) {
        //如果打开了滚动开关
        if (GetLogShift() == true) {
            int iRet = ShiftLogFiles(_sFilePrefixName);
            if (iRet < 0) {
                return -1;
            }
        }
        
        //如果打开了缓写开关，并且缓写缓冲区未溢出
        if (GetSaveDelay() == true && (_uiCurWrite + uilen) < _uiDelayLogSize) {
            memcpy(_sData + _uiCurWrite, sLogContent, uilen);
            _uiCurWrite += uilen;
        } else {
            char sLogFileName[256] = { 0 };
            snprintf(sLogFileName, sizeof(sLogFileName) - 1, "%s.log",
                    _sFilePrefixName);
            FILE * f = fopen(sLogFileName, "a+b");
            
            if (0 == f || (ferror(f) != 0)) {
                return -1;
            }
            
            if (GetSaveDelay() == true && _uiCurWrite > 0) {
                fwrite(_sData, 1, _uiCurWrite, f);
                _uiCurWrite = 0;
                memset(_sData, 0, sizeof(char) * _uiDelayLogSize);
            } else {
                fwrite(sLogContent, 1, uilen, f);
            }
            
            fclose(f);
        }
        return 0;
    }
    
    inline bool CMiniLog::GetSaveDelay() const {
        return _bSaveDelay;
    }
    
    int CMiniLog::ShiftLogFiles(const char *sFilePrefixName) {
        struct stat stStat;
        char sLogFileName[256] = { 0 };
        char sLogNewFileName[256] = { 0 };
        
        snprintf(sLogFileName, sizeof(sLogFileName) - 1, "%s.log",
                sFilePrefixName);
        
        if (stat(sLogFileName, &stStat) < 0) {
            return 0;
        }
        
        if (stStat.st_size < _uiLogMaxSize) {
            return 0;
        }
        
        ThreadAutoLock myLock(&_lock);
        
        snprintf(sLogFileName, sizeof(sLogFileName) - 1, "%s%u.log",
                sFilePrefixName, _uiLogSaveCount - 1);
        if (access(sLogFileName, F_OK) == 0) {
            if (remove(sLogFileName) < 0) {
                return -1;
            }
        }
        
        for (int i = _uiLogSaveCount - 2; i >= 0; i--) {
            if (i == 0) {
                snprintf(sLogFileName, sizeof(sLogFileName) - 1, "%s.log",
                        sFilePrefixName);
            } else {
                snprintf(sLogFileName, sizeof(sLogFileName) - 1, "%s%u.log",
                        sFilePrefixName, i);
            }
            
            if (access(sLogFileName, F_OK) == 0) {
                snprintf(sLogNewFileName, sizeof(sLogNewFileName) - 1,
                        "%s%u.log", sFilePrefixName, i + 1);
                if (rename(sLogFileName, sLogNewFileName) < 0) {
                    return -1;
                }
            }
        }
        
        return 0;
    }
    
    void CMiniLog::SetFilePrefixName(const char *sFilePrefixName) {
        memset(_sFilePrefixName, 0, LOG_MAX_PATH);
        strncpy(_sFilePrefixName, sFilePrefixName, LOG_MAX_PATH - 1);
    }
    
    void CMiniLog::SprintLogLevel(char *buffer, int buff_max_len,
            int& buff_cur_len, const char *fmt, unsigned int log_level) {
        //不输出级别
        if (log_level > LOG_FATAL) {
            return;
        }
        SPRINT(buffer, buff_max_len, buff_cur_len, fmt, level_str[log_level]);
    }
    
    int CMiniLog::LogFileName(char *file_name, int max_len,
            LOG_LEVEL log_level) {
        int len = 0;
        int iFilePath = 0;
        
        if ((iFilePath = strlen(_sFilePath)) > 0
                && _sFilePath[iFilePath - 1] == '/') {
            SPRINT(file_name, max_len, len, "%s%s", _sFilePath,
                    _sFilePrefixName);
        } else if (iFilePath > 0) {
            SPRINT(file_name, max_len, len, "%s/%s", _sFilePath,
                    _sFilePrefixName);
        } else {
            SPRINT(file_name, max_len, len, "%s", _sFilePrefixName);
        }
        
        switch (_emNameFmt) {
            case LOG_NAME_NONE_FLAG: {
                break;
            }
            default: {
                if (_emNameFmt & LOG_NAME_PID_FALG) {
                    SPRINT(file_name, max_len, len, "%d", getpid());
                }
                if (_emNameFmt & LOG_NAME_LEVEL_FLAG) {
                    SprintLogLevel(file_name, max_len, len, "_%s", log_level);
                }
                if (_emNameFmt & LOG_NAME_DATE_FLAG) {
                    SprintDate(file_name, max_len, len, "_%04d-%02d-%02d");
                }
                break;
            }
                
        }
        
        return strlen(file_name);
    }
    
    int CMiniLog::LogContentPrefix(char *log_content, int max_len,
            LOG_LEVEL log_level, const char *file_name, const char *fn_name,
            unsigned int line) {
        
        int len = 0;
        
        //日志内容输出日期时间
        if (_emContentFmt & LOG_CONTENT_TIME_FLAG) {
            SprintTime(log_content, max_len, len,
                    "%04d-%02d-%02d %02d:%02d:%02d");
        }
        
        if (_emContentFmt & LOG_CONTENT_PID_FLAG) {
            SPRINT(log_content, max_len, len, "[%d]", getpid());
        }
        
        if ((_emContentFmt & LOG_CONTENT_FILE_FLAG) && file_name != NULL) {
            SPRINT(log_content, max_len, len, "/%s/", file_name);
        }
        
        if ((_emContentFmt & LOG_CONTENT_FUN_FLAG) && fn_name != NULL) {
            SPRINT(log_content, max_len, len, "{%s}", fn_name);
        }
        
        if ((_emContentFmt & LOG_CONTENT_LINE_FLAG) && line > 0) {
            SPRINT(log_content, max_len, len, "(%d)", line);
        }
        
        if (_emContentFmt & LOG_CONTENT_LEVEL_FLAG) {
            SprintLogLevel(log_content, max_len, len, "_%-6s", log_level);
        }
        
        SPRINT(log_content, max_len, len, "%s", "  ");
        
        return strlen(log_content);
    }
    
    void CMiniLog::SetLogLevel(int log_level) {
        _log_level = log_level;
    }
    
    void CMiniLog::SetLogFilePath(const char *file_path) {
        if (file_path != NULL && strlen(file_path) > 0) {
            memset(_sFilePath, 0, sizeof(_sFilePath));
            strncpy(_sFilePath, file_path, sizeof(_sFilePath) - 1);
        }
    }
    
    void CMiniLog::CloseShiftLog() {
        _bLogShift = false;
    }
    
    bool CMiniLog::GetLogShift() const {
        return _bLogShift;
    }
    
    void CMiniLog::SetLogShift(bool bLogShift) {
        _bLogShift = bLogShift;
    }
    
    void CMiniLog::SetLogNameFmt(const char *pre_name) {
        memset(_sFilePrefixName, 0, sizeof(_sFilePrefixName));
        if (pre_name == NULL) {
            strncpy(_sFilePrefixName, _sProcessName,
                    sizeof(_sFilePrefixName) - 1);
        } else {
            strncpy(_sFilePrefixName, pre_name, sizeof(_sFilePrefixName) - 1);
        }
    }
    
    void CMiniLog::SetLogNameFmt(unsigned int mode) {
        if (mode > 0) {
            _emNameFmt = mode;
        } else {
            _emNameFmt = LOG_NAME_NONE_FLAG;
        }
    }
    
    void CMiniLog::SetLogContentFmt(unsigned int mode) {
        if (mode > 0) {
            _emContentFmt = mode;
        } else {
            _emContentFmt = LOG_CONTENT_ALL_FLAG;
        }
    }
    
    void CMiniLog::SetSaveDelay(bool bSaveDelay, unsigned int uiDelayLogSize) {
        _bSaveDelay = bSaveDelay;
        
        _uiDelayLogSize = uiDelayLogSize;
        
        if (_sData != NULL) {
            delete[] _sData;
            _sData = NULL;
        }
        
        _sData = new char[uiDelayLogSize];
    }
    
    void CMiniLog::SetWMode(unsigned int delay_log_size) {
        if (delay_log_size > 0) {
            SetSaveDelay(true, delay_log_size);
        }
    }
    
    void CMiniLog::SetLogNum(unsigned int log_save_cnt) {
        if (log_save_cnt > 0) {
            _bLogShift = true;
            _uiLogSaveCount = log_save_cnt;
        }
    }
    
    void CMiniLog::SetLogSize(unsigned int log_max_size) {
        if (log_max_size > 0) {
            _uiLogMaxSize = log_max_size;
        }
    }

}
