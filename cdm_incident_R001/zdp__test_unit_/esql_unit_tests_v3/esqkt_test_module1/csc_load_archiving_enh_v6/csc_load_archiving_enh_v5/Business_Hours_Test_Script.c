/*
 * Business Hours Logic Test Script
 * Simulates different time scenarios to verify business day calculations
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

// Mock the global variables from tpldauftr.cxx
static int g_business_hours_start = 8;
static int g_business_hours_end = 18;
static long g_business_day_cutoff_date = 0;

// Test logging function
static void test_log(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

// Simulate the business day calculation logic
static long simulate_business_day_cutoff(struct tm* mock_time) {
    char* weekday_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    printf("\n=== BUSINESS DAY CALCULATION TEST ===\n");
    printf("Mock Date: %04d-%02d-%02d (%s)\n", 
           mock_time->tm_year + 1900, mock_time->tm_mon + 1, mock_time->tm_mday,
           weekday_names[mock_time->tm_wday]);
    printf("Mock Time: %02d:%02d:%02d\n", 
           mock_time->tm_hour, mock_time->tm_min, mock_time->tm_sec);
    
    struct tm cutoff_tm = *mock_time;
    int current_hour = mock_time->tm_hour;
    
    printf("Business Hours: %02d:00 - %02d:00\n", g_business_hours_start, g_business_hours_end);
    printf("Current Hour: %02d\n", current_hour);
    
    if (current_hour >= g_business_hours_start && current_hour < g_business_hours_end) {
        printf("=== BUSINESS HOURS LOGIC ===\n");
        printf("Within business hours - Archive PREVIOUS business day\n");
        
        cutoff_tm.tm_mday -= 1;
        mktime(&cutoff_tm);
        
        printf("Previous day target: %04d-%02d-%02d (%s)\n",
               cutoff_tm.tm_year + 1900, cutoff_tm.tm_mon + 1, cutoff_tm.tm_mday,
               weekday_names[cutoff_tm.tm_wday]);
        
        // Weekend rollback logic
        if (cutoff_tm.tm_wday == 0) {  // Sunday -> Friday
            printf("WEEKEND ROLLBACK: Sunday -> Friday (-2 days)\n");
            cutoff_tm.tm_mday -= 2;
            mktime(&cutoff_tm);
        } else if (cutoff_tm.tm_wday == 6) {  // Saturday -> Friday
            printf("WEEKEND ROLLBACK: Saturday -> Friday (-1 day)\n");
            cutoff_tm.tm_mday -= 1;
            mktime(&cutoff_tm);
        } else {
            printf("NO ROLLBACK: Target is business day\n");
        }
    } else {
        printf("=== NON-BUSINESS HOURS LOGIC ===\n");
        printf("Outside business hours - Archive CURRENT business day\n");
        
        printf("Current day target: %04d-%02d-%02d (%s)\n",
               cutoff_tm.tm_year + 1900, cutoff_tm.tm_mon + 1, cutoff_tm.tm_mday,
               weekday_names[cutoff_tm.tm_wday]);
        
        // Weekend rollback for current day
        if (cutoff_tm.tm_wday == 0) {  // Sunday -> Friday
            printf("WEEKEND ROLLBACK: Sunday -> Friday (-2 days)\n");
            cutoff_tm.tm_mday -= 2;
            mktime(&cutoff_tm);
        } else if (cutoff_tm.tm_wday == 6) {  // Saturday -> Friday
            printf("WEEKEND ROLLBACK: Saturday -> Friday (-1 day)\n");
            cutoff_tm.tm_mday -= 1;
            mktime(&cutoff_tm);
        } else {
            printf("NO ROLLBACK: Current day is business day\n");
        }
    }
    
    printf("FINAL CUTOFF: %04d-%02d-%02d (%s)\n",
           cutoff_tm.tm_year + 1900, cutoff_tm.tm_mon + 1, cutoff_tm.tm_mday,
           weekday_names[cutoff_tm.tm_wday]);
    
    // Convert to internal date format (YYYYMMDD)
    long cutoff_date = (cutoff_tm.tm_year + 1900) * 10000 + 
                       (cutoff_tm.tm_mon + 1) * 100 + 
                       cutoff_tm.tm_mday;
    
    printf("Internal Format: %ld\n", cutoff_date);
    return cutoff_date;
}

int main() {
    struct tm test_time;
    
    printf("BUSINESS HOURS LOGIC VERIFICATION TESTS\n");
    printf("========================================\n");
    
    // Test Case 1: Monday 10:00 AM (business hours)
    memset(&test_time, 0, sizeof(test_time));
    test_time.tm_year = 125; // 2025
    test_time.tm_mon = 7;    // August (0-based)
    test_time.tm_mday = 18;  // Monday, Aug 18, 2025
    test_time.tm_hour = 10;
    test_time.tm_min = 0;
    test_time.tm_wday = 1;   // Monday
    printf("\n--- TEST 1: Monday 10:00 AM ---");
    simulate_business_day_cutoff(&test_time);
    
    // Test Case 2: Monday 19:00 PM (after business hours)
    test_time.tm_hour = 19;
    printf("\n--- TEST 2: Monday 19:00 PM ---");
    simulate_business_day_cutoff(&test_time);
    
    // Test Case 3: Saturday 10:00 AM (weekend, business hours)
    test_time.tm_mday = 16;  // Saturday, Aug 16, 2025
    test_time.tm_hour = 10;
    test_time.tm_wday = 6;   // Saturday
    printf("\n--- TEST 3: Saturday 10:00 AM ---");
    simulate_business_day_cutoff(&test_time);
    
    // Test Case 4: Saturday 19:00 PM (weekend, after business hours)
    test_time.tm_hour = 19;
    printf("\n--- TEST 4: Saturday 19:00 PM ---");
    simulate_business_day_cutoff(&test_time);
    
    // Test Case 5: Sunday 17:52 PM (our previous test case)
    test_time.tm_mday = 17;  // Sunday, Aug 17, 2025
    test_time.tm_hour = 17;
    test_time.tm_min = 52;
    test_time.tm_wday = 0;   // Sunday
    printf("\n--- TEST 5: Sunday 17:52 PM (Previous Test) ---");
    simulate_business_day_cutoff(&test_time);
    
    // Test Case 6: Sunday 19:00 PM (weekend, after business hours)
    test_time.tm_hour = 19;
    test_time.tm_min = 0;
    printf("\n--- TEST 6: Sunday 19:00 PM ---");
    simulate_business_day_cutoff(&test_time);
    
    // Test Case 7: Friday 17:59 PM (last minute of business hours)
    test_time.tm_mday = 15;  // Friday, Aug 15, 2025
    test_time.tm_hour = 17;
    test_time.tm_min = 59;
    test_time.tm_wday = 5;   // Friday
    printf("\n--- TEST 7: Friday 17:59 PM (Edge Case) ---");
    simulate_business_day_cutoff(&test_time);
    
    // Test Case 8: Friday 18:01 PM (just after business hours)
    test_time.tm_hour = 18;
    test_time.tm_min = 1;
    printf("\n--- TEST 8: Friday 18:01 PM (Edge Case) ---");
    simulate_business_day_cutoff(&test_time);
    
    printf("\n========================================\n");
    printf("BUSINESS HOURS LOGIC TESTS COMPLETE\n");
    
    return 0;
}