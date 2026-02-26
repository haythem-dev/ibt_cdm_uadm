// First, add the global variable for explicit mode tracking after the existing globals
// Global variables for business day functionality
static int g_business_day_mode = 0;     // 0=disabled, 1=enabled  
static long g_business_day_cutoff_date = 0;  // Calculated archive cutoff date
static int g_explicit_business_day_mode = 0;  // Track if explicitly set by user

