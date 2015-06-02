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
#include "threadLock.h"

namespace mini_log {
    
    class CMiniLog {
    public:
        CMiniLog();
        ~CMiniLog();

        /*
         *写日志接口
         * 
         * @param log_level 日志级别
         * @param file_name 如果希望打印文件名，设置成宏__FILE__，否则设置为NULL
         * @param fn_name 如果希望打印函数名称，设置成宏__FUNCTION__，否则设置为NULL
         * @param line 如果希望打印行号，设置成宏__LINE__，否则设置为NULL
         * @param fmt 格式字符串
         */
        void LogEx(LOG_LEVEL log_level, const char *file_name,
                const char *fn_name, unsigned int line, const char * fmt, ...);
        /*
         *设置日志界别
         *
         * @param log_level 日志级别
         *
         */
        inline void SetLogLevel(int log_level);

        /*
         *设置日志存放路径
         *
         * @param file_path 日志存放路径
         *
         */
        inline void SetLogFilePath(const char *file_path);

        /*
         *设置打印日志时，不滚动日志文件
         *
         */
        inline void CloseShiftLog();

        /**
         * 是否需要滚动日志
         *
         * @return true日志可以滚动，日志不能滚动
         */
        inline bool GetLogShift() const;

        /**
         * 设置日志滚动模式
         *
         * @param bLogShift true日志文件可以滚动，false日志文件不能滚动
         *
         */
        inline void SetLogShift(bool bLogShift);

        /*
         *设置日志前缀名
         *
         * @param pre_name 日志前缀名
         */
        inline void SetLogNameFmt(const char *pre_name);

        /*
         *设置日志前缀名和日志内容格式
         *
         * @param mode 日志内容格式
         */
        inline void SetLogNameFmt(unsigned int mode);

        /*
         *设置日志内容格式
         *
         * @param mode 日志名前缀格式
         */
        inline void SetLogContentFmt(unsigned int mode);

        /*
         *设置缓写缓冲区大小，打开缓写模式(默认情况下，缓写模式是关闭的)
         *
         * @param delay_log_size 缓写缓冲区大小
         */
        inline void SetWMode(unsigned int delay_log_size);

        /*
         *设置单个日志文件大小和日志滚动个数，打开日志滚动模式(默认情况下，不滚动打日志)
         *
         * @param  log_save_cnt 日志滚动个数，默认为0，不滚动
         */
        inline void SetLogNum(unsigned int log_save_cnt = 0);

        /*
         *设置单个日志文件大小和日志滚动个数，打开日志滚动模式(默认情况下，不滚动打日志)
         *
         * @param  log_max_size 单个日志文件大小
         */
        inline void SetLogSize(unsigned int log_max_size);

    private:
        
        /**
         * 日志滚动操作函数
         *
         * @param sFilePrefixName 日志文件名前缀
         *
         */
        int ShiftLogFiles(const char *sFilePrefixName);

        /**
         * 设置缓写模式
         *
         * @param bSaveDelay true打开缓写模式，false关闭缓写模式
         * @param uiDelayLogSize 缓写缓冲区大小
         *
         */
        inline void SetSaveDelay(bool bSaveDelay, unsigned int uiDelayLogSize);

        /*
         * 设置日志内容的前缀
         * 
         * @param log_content 输出的缓存内存
         * @param max_len 缓存的大小
         * @param log_level 日志级别
         * @param file_name 文件名
         * @param fn_name 函数名
         * @param line 行号
         */
        int LogContentPrefix(char *log_content, int max_len,
                LOG_LEVEL log_level, const char *file_name, const char *fn_name,
                unsigned int line);

        /*
         * 设置日志文件名
         * 
         * @param file_name 文件名缓存
         * @param max_len 文件名最大长度
         * @param log_level 日志级别
         */
        int LogFileName(char *file_name, int max_len, LOG_LEVEL log_level);

        /*
         * 输出日志级别
         * 
         * @param buffer 输出缓存
         * @param buff_max_len 输出缓存大小
         * @param buff_cur_len 当前缓存输出的长度
         * @param fmt 输出格式
         * @param log_level 日志级别
         */
        inline void SprintLogLevel(char *buffer, int buff_max_len,
                int& buff_cur_len, const char *fmt, unsigned int log_level);

        /**
         * 设置日志文件名前缀
         *
         * @param sFilePrefixName 日志文件名前缀
         *
         */
        inline void SetFilePrefixName(const char *sFilePrefixName);

        /**
         * 将日志内容写入文件
         *
         * @param sLogContent 待写入的日志内容
         * @param uilen 内容字符串的字节数
         *
         * @return 0成功，-1失败
         */
        int Log(const char *sLogContent, unsigned int uilen);

        /**
         * 是否打开缓写模式
         *
         * @return true缓写模式已打开，false缓写模式已关闭
         */
        inline bool GetSaveDelay() const;

    private:
        static const char** level_str;
        int _log_level;							//日志的级别
        
        bool _bSaveDelay;						//是否打开缓写模式
        char *_sData;							//缓写模式下的日志缓冲区
        unsigned int _uiCurWrite;				//缓写模式下，当前缓冲区大小
        unsigned int _uiDelayLogSize;			//缓写模式下，日志缓冲区容量
        
        bool _bLogShift;						//是否需要滚动日志
        long _uiLogMaxSize;				        //单个日志文件的极限大小，以B为单位
        unsigned int _uiLogSaveCount;			//滚动模式下最多保存的日志个数
        
        unsigned int _emNameFmt;                //日志文件名前缀
        unsigned int _emContentFmt;             //日志内容前缀
        
        pid_t _iProcessID;                      //进程ID
        char _sProcessName[LOG_MAX_PATH];       //进程名
        char _sFilePrefixName[LOG_MAX_PATH];    //日志文件名前缀
        char _sFilePath[LOG_MAX_PATH];          //日志文件路径
        
        ThreadMutexLock _lock;
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
