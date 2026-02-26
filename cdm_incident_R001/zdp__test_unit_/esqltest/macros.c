#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <unistd.h>
#else
#include <process.h>
#define getpid _getpid
#endif

static int getRND(int min, int max) {
    static int seeded = 0;
    if (!seeded) {
        seeded = 1;
        struct timespec ts;
#if defined(_WIN32) || defined(_WIN64)
        ts.tv_sec = (long)time(NULL);
        ts.tv_nsec = 0;
#else
        clock_gettime(CLOCK_REALTIME, &ts);
#endif
        srand((unsigned int)(ts.tv_sec ^ ts.tv_nsec ^ getpid()));
    }
    if (max < min) return min;
    return min + (int)((double)rand() / ((double)RAND_MAX + 1) * (max - min + 1));
}

void expand_macros(char* str, size_t str_size, int iteration) {
    // Expand $(TODAY)
    const char* macro_today = "$(TODAY)";
    while (1) {
        char* pos_today = strstr(str, macro_today);
        if (!pos_today) break;
        char date_str[9];
        time_t t = time(NULL);
        struct tm tm_info;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm_info, &t);
#else
        localtime_r(&t, &tm_info);
#endif
        strftime(date_str, sizeof(date_str), "%Y%m%d", &tm_info);
        char temp[1024];
        size_t prefix_len = pos_today - str;
        snprintf(temp, sizeof(temp), "%.*s%s%s",
                 (int)prefix_len, str, date_str, pos_today + strlen(macro_today));
        strncpy(str, temp, str_size - 1);
        str[str_size - 1] = '\0';
    }
    // Expand $(NOW)
    const char* macro_now = "$(NOW)";
    while (1) {
        char* pos_now = strstr(str, macro_now);
        if (!pos_now) break;
        char now_str[20];
        time_t t = time(NULL);
        struct tm tm_info;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm_info, &t);
#else
        localtime_r(&t, &tm_info);
#endif
        strftime(now_str, sizeof(now_str), "%Y.%m.%d %H:%M:%S", &tm_info);
        char temp[1024];
        size_t prefix_len = pos_now - str;
        snprintf(temp, sizeof(temp), "%.*s%s%s",
                 (int)prefix_len, str, now_str, pos_now + strlen(macro_now));
        strncpy(str, temp, str_size - 1);
        str[str_size - 1] = '\0';
    }
    // Expand $(ITERATION)
    const char* macro_iter = "$(ITERATION)";
    while (1) {
        char* pos_iter = strstr(str, macro_iter);
        if (!pos_iter) break;
        char iter_str[16];
        snprintf(iter_str, sizeof(iter_str), "%d", iteration);
        char temp[1024];
        size_t prefix_len = pos_iter - str;
        snprintf(temp, sizeof(temp), "%.*s%s%s",
                 (int)prefix_len, str, iter_str, pos_iter + strlen(macro_iter));
        strncpy(str, temp, str_size - 1);
        str[str_size - 1] = '\0';
    }
    // Expand $(RND:min,max)
    while (1) {
        char* macro_rnd = strstr(str, "$(RND:");
        if (!macro_rnd) break;
        char* comma = strchr(macro_rnd, ',');
        char* close_paren = strchr(macro_rnd, ')');
        if (!comma || !close_paren || comma > close_paren) break;
        char min_str[16] = {0}, max_str[16] = {0};
        int min = 0, max = 0;
        char* min_start = macro_rnd + 6;
        size_t min_len = comma - min_start;
        size_t max_len = close_paren - (comma + 1);
        if (min_len > 0 && min_len < sizeof(min_str)) {
            strncpy(min_str, min_start, min_len);
            min_str[min_len] = '\0';
            min = atoi(min_str);
        }
        if (max_len > 0 && max_len < sizeof(max_str)) {
            strncpy(max_str, comma + 1, max_len);
            max_str[max_len] = '\0';
            max = atoi(max_str);
        }
        int rnd = getRND(min, max);
        char rnd_str[16];
        snprintf(rnd_str, sizeof(rnd_str), "%d", rnd);
        char temp[1024];
        size_t prefix_len = macro_rnd - str;
        snprintf(temp, sizeof(temp), "%.*s%s%s",
                 (int)prefix_len, str, rnd_str, close_paren + 1);
        strncpy(str, temp, str_size - 1);
        str[str_size - 1] = '\0';
    }
    // Expand $(STRFTIME:fmt)
    while (1) {
        char* macro_strftime = strstr(str, "$(STRFTIME:");
        if (!macro_strftime) break;
        char* close_paren = strchr(macro_strftime, ')');
        if (!close_paren) break;
        size_t fmt_len = close_paren - (macro_strftime + 11);
        if (fmt_len <= 0 || fmt_len >= 64) break;
        char fmt[64] = {0};
        strncpy(fmt, macro_strftime + 11, fmt_len);
        fmt[fmt_len] = '\0';
        char time_str[128] = {0};
        time_t t = time(NULL);
        struct tm tm_info;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm_info, &t);
#else
        localtime_r(&t, &tm_info);
#endif
        strftime(time_str, sizeof(time_str), fmt, &tm_info);
        char temp[1024];
        size_t prefix_len = macro_strftime - str;
        snprintf(temp, sizeof(temp), "%.*s%s%s",
                 (int)prefix_len, str, time_str, close_paren + 1);
        strncpy(str, temp, str_size - 1);
        str[str_size - 1] = '\0';
    }
    // Expand $(ENV:var)
    while (1) {
        char* macro_env = strstr(str, "$(ENV:");
        if (!macro_env) break;
        char* close_paren = strchr(macro_env, ')');
        if (!close_paren) break;
        size_t var_len = close_paren - (macro_env + 6);
        if (var_len <= 0 || var_len >= 64) break;
        char var[64] = {0};
        strncpy(var, macro_env + 6, var_len);
        var[var_len] = '\0';
        const char* env_val = getenv(var);
        char env_buf[256] = "";
        if (env_val) {
            size_t env_len = strlen(env_val);
            size_t j = 0;
            for (size_t i = 0; i < env_len && j < sizeof(env_buf) - 1; ++i) {
                env_buf[j++] = isalnum((unsigned char)env_val[i]) ? env_val[i] : '_';
            }
            env_buf[j] = '\0';
        }
        char temp[1024];
        size_t prefix_len = macro_env - str;
        snprintf(temp, sizeof(temp), "%.*s%s%s",
                 (int)prefix_len, str, env_val ? env_buf : "", close_paren + 1);
        strncpy(str, temp, str_size - 1);
        str[str_size - 1] = '\0';
    }
    // Expand $(PID)
    while (1) {
        char* macro_pid = strstr(str, "$(PID)");
        if (!macro_pid) break;
        char pid_str[32];
        snprintf(pid_str, sizeof(pid_str), "%d", (int)getpid());
        char temp[1024];
        size_t prefix_len = macro_pid - str;
        snprintf(temp, sizeof(temp), "%.*s%s%s",
                 (int)prefix_len, str, pid_str, macro_pid + strlen("$(PID)"));
        strncpy(str, temp, str_size - 1);
        str[str_size - 1] = '\0';
    }
}
