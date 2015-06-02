/*
 * mini_log_tools.h
 *
 *  Created on: 2015年5月29日
 *      Author: skyyuan
 */

#ifndef MINI_LOG_TOOLS_H_MINI_LOGCPP_
#define MINI_LOG_TOOLS_H_MINI_LOGCPP_

#include "config.h"

#define SPRINT(buff, buff_len, len, fmt, args...)   do{if(len < buff_len){len += snprintf(buff + len, buff_len - len - 1, fmt, ##args);}}while(0)

namespace mini_log {
    
    inline void SprintTime(char *buffer, int buff_max_len, int& buff_cur_len,
            const char *fmt, long tv_sec = 0, long tv_usec = 0) {
        time_t now;
        
        if (tv_sec <= 0 && tv_usec <= 0) {
            now = time(NULL);
        } else {
            now = tv_sec;
        }
        struct tm tmm;
        localtime_r(&now, &tmm);
        
        if (tv_usec > 0) {
            SPRINT(buffer, buff_max_len, buff_cur_len, fmt, tmm.tm_year + 1900,
                    tmm.tm_mon + 1, tmm.tm_mday, tmm.tm_hour, tmm.tm_min,
                    tmm.tm_sec, tv_usec);
        } else {
            SPRINT(buffer, buff_max_len, buff_cur_len, fmt, tmm.tm_year + 1900,
                    tmm.tm_mon + 1, tmm.tm_mday, tmm.tm_hour, tmm.tm_min,
                    tmm.tm_sec);
        }
        
    }
    
    inline void SprintDate(char *buffer, int buff_max_len, int& buff_cur_len,
            const char *fmt) {
        time_t now = time(NULL);
        struct tm tmm;
        localtime_r(&now, &tmm);
        
        SPRINT(buffer, buff_max_len, buff_cur_len, fmt, tmm.tm_year + 1900,
                tmm.tm_mon + 1, tmm.tm_mday);
        
    }
    inline int GetModuleFileName(char* lpszModulePath, int cbModule,
            const char* lpszModuleName) {
        int len = 0;
        
        memset(lpszModulePath, 0, sizeof(char) * cbModule);
        
        //取得当前路径
        char szCurPath[LOG_MAX_PATH] = { 0 };
        !getcwd(szCurPath, LOG_MAX_PATH);
        
        char szCmdPath[LOG_MAX_PATH] = { 0 };
        char szCmdLineFile[LOG_MAX_PATH] = { 0 };
        
        snprintf(szCmdLineFile, sizeof(szCmdLineFile) - 1, "/proc/%d/cmdline",
                getpid());
        FILE* f = fopen(szCmdLineFile, "r");
        if (f != 0) {
            len = fread(szCmdPath, 1, LOG_MAX_PATH, f);
            fclose(f);
        }
        
        if (szCmdPath[0] != '/' && szCurPath[0] == '/') {
            char szTmp[LOG_MAX_PATH] = { 0 };
            if (szCmdPath[0] == '.' && szCmdPath[1] == '/') {
                snprintf(szTmp, sizeof(szTmp) - 1, "%s/%s", szCurPath,
                        &szCmdPath[2]);
            } else {
                snprintf(szTmp, sizeof(szTmp) - 1, "%s/%s", szCurPath,
                        szCmdPath);
            }
            strncpy(szCmdPath, szTmp, sizeof(szCmdPath) - 1);
        }
        
        switch (szCmdPath[0]) {
            case '.':
                snprintf(lpszModulePath, cbModule - 1, "%s%s", szCurPath,
                        &szCmdPath[1]);
                break;
            default:
                strncpy(lpszModulePath, szCmdPath, cbModule - 1);
                break;
        }
        
        len = strlen(lpszModulePath);
        
        return len;
        
    }
    
    inline int GetProcessName(char* lpszProcessName, int) {
        char szPath[LOG_MAX_PATH] = { 0 };
        int len = GetModuleFileName(szPath, LOG_MAX_PATH, 0);
        int size = len;
        //取得路径
        for (; len > 0; len--) {
            if (szPath[len] == '/' || szPath[len] == '\\') {
                len++;
                break;
            }
        }
        
        int l = size - len;
        strncpy(lpszProcessName, &szPath[len], l);
        return l;
    }
    
    inline int GetFileName(char* path, int cbModule,
            const char* lpszModuleName) {
        return 0;
    }
}

#endif /* MINI_LOG_TOOLS_H_MINI_LOGCPP_ */
