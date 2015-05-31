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
    
    inline int GetFileName(char* path, int cbModule,
            const char* lpszModuleName) {
        return 0;
    }
}

#endif /* MINI_LOG_TOOLS_H_MINI_LOGCPP_ */
