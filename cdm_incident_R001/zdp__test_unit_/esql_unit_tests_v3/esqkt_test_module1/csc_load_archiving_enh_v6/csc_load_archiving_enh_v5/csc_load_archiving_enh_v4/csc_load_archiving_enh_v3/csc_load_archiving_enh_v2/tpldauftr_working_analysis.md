# tpldauftr Working Script Analysis - Post-Execution Report

## Successful Execution Summary (August 13, 2025 21:41:42)

### Execution Details:
- **Status**: ✅ SUCCESS (RC: 0 - Application started)
- **Command**: `./tpldauftr.sh -start -fil 50`
- **Database**: ode21@zdev21_shm  
- **Parameters**: -fil 50 -del 35
- **Process ID**: 33882598
- **Version**: Original files (pre-CPR enhancement)

### Key Differences from Failed Run:
| Aspect | Failed Run (21:35:45) | Successful Run (21:41:42) |
|--------|----------------------|---------------------------|
| Parameters | `--business-day-mode enabled -fil 50` | `-fil 50 -db ode21@zdev21_shm -del 35` |
| Return Code | RC: 2 (Technical problems) | RC: 0 (Success) |
| Enhancement | CPR business day mode | Original version |
| Binary | Enhanced tpldauftr.bin | Original tpldauftr.bin |

## What the Working Script Actually Does

### 1. Script Execution Flow (Based on Successful Run)

#### **Phase 1: Initialization**
```bash
# tpldauftr.sh wrapper calls:
/software/wss/de/ksc/load/proc/printStartStop -start -fil 50

# Which executes:
/software/wss/de/ksc/load/bin/tpldauftr.bin -fil 50 -db ode21@zdev21_shm -del 35
```

#### **Phase 2: Parameter Processing**
- **-fil 50**: Filter parameter (likely batch size or limit)
- **-db ode21@zdev21_shm**: Database connection string
- **-del 35**: Deletion/retention parameter (possibly days)

#### **Phase 3: Database Operations** 
Based on typical archiving patterns, the script likely:
1. Connects to ode21@zdev21_shm database
2. Identifies closed orders for archiving
3. Moves records from kdauftrag to akdauftrag
4. Processes in batches of 50 records (-fil 50)
5. Applies 35-day retention logic (-del 35)

### 2. Tables Actually Impacted (Refined Analysis)

Since the script ran successfully, here are the most likely impacted tables:

#### **Primary Archive Tables**
```sql
-- Main orders archiving (most certain)
SOURCE: kdauftrag  → DESTINATION: akdauftrag

-- Order line items (highly probable)  
SOURCE: kdaufpos   → DESTINATION: akdaufpos

-- Order status history (probable)
SOURCE: kdaufhist  → DESTINATION: akdaufhist
```

#### **Reference/Control Tables**
```sql
-- Archive process tracking
archive_log, archive_control, arch_statistics

-- Cross-references (if maintaining integrity)
kdaufref, kdauflink

-- Status tracking
kdaufstat, order_status_log
```

### 3. Post-Execution Inspection Queries

Now that we know the script ran successfully, use these queries to see what actually happened:

#### **3.1 Archive Activity Detection**
```sql
-- Check for new archive records (last 1 hour)
SELECT 
    'Recent archive activity' as check_type,
    COUNT(*) as new_archives,
    MIN(archive_date) as first_archive,
    MAX(archive_date) as last_archive
FROM akdauftrag 
WHERE archive_date >= CURRENT - 1 UNITS HOUR;

-- Compare archive counts before/after
SELECT 
    'Archive volume comparison' as metric,
    COUNT(CASE WHEN archive_date::DATE = '2025-08-13' THEN 1 END) as today_archives,
    COUNT(CASE WHEN archive_date::DATE = '2025-08-12' THEN 1 END) as yesterday_archives,
    COUNT(CASE WHEN archive_date >= CURRENT - 1 UNITS HOUR THEN 1 END) as last_hour
FROM akdauftrag;
```

#### **3.2 Source Table Changes**
```sql
-- Check for reduction in source table
SELECT 
    'kdauftrag changes' as table_name,
    COUNT(*) as total_orders,
    COUNT(CASE WHEN status = 'CLOSED' THEN 1 END) as closed_orders,
    COUNT(CASE WHEN archived_flag = 'Y' THEN 1 END) as marked_archived,
    COUNT(CASE WHEN archived_flag = 'N' OR archived_flag IS NULL THEN 1 END) as not_archived
FROM kdauftrag;

-- Identify specific orders processed
SELECT 
    order_id,
    status,
    close_date,
    archived_flag,
    last_update
FROM kdauftrag 
WHERE last_update >= CURRENT - 1 UNITS HOUR
AND status = 'CLOSED'
ORDER BY last_update DESC;
```

#### **3.3 Business Day Logic Verification**
```sql
-- Analyze which business day was processed
SELECT 
    close_date::DATE as business_day_processed,
    COUNT(*) as orders_archived,
    MIN(close_date) as earliest_close,
    MAX(close_date) as latest_close
FROM akdauftrag 
WHERE archive_date >= CURRENT - 1 UNITS HOUR
GROUP BY close_date::DATE
ORDER BY business_day_processed DESC;

-- Check if current day (13/08) or previous day (12/08) was archived
SELECT 
    CASE 
        WHEN close_date::DATE = '2025-08-13' THEN 'Current Day (13/08)'
        WHEN close_date::DATE = '2025-08-12' THEN 'Previous Day (12/08)'
        ELSE 'Other Day: ' || close_date::DATE
    END as day_category,
    COUNT(*) as archived_count
FROM akdauftrag 
WHERE archive_date >= CURRENT - 1 UNITS HOUR
GROUP BY close_date::DATE;
```

#### **3.4 Filter and Deletion Parameter Impact**
```sql
-- Verify -fil 50 parameter (batch processing)
SELECT 
    'Batch analysis' as metric,
    COUNT(*) as total_processed,
    CASE 
        WHEN COUNT(*) <= 50 THEN 'Single batch (≤50)'
        WHEN COUNT(*) <= 100 THEN 'Two batches (51-100)'
        ELSE 'Multiple batches (>100)'
    END as batch_estimate
FROM akdauftrag 
WHERE archive_date >= CURRENT - 1 UNITS HOUR;

-- Verify -del 35 parameter (retention logic)
SELECT 
    'Retention check' as metric,
    MIN(close_date) as oldest_order_archived,
    (CURRENT::DATE - MIN(close_date::DATE)) as days_old,
    CASE 
        WHEN (CURRENT::DATE - MIN(close_date::DATE)) <= 35 
        THEN 'Within 35-day retention'
        ELSE 'Outside 35-day retention'
    END as retention_status
FROM akdauftrag 
WHERE archive_date >= CURRENT - 1 UNITS HOUR;
```

### 4. Process Log Analysis

#### **4.1 Check Archive Process Logs**
```sql
-- Look for process execution records
SELECT 
    process_name,
    execution_date,
    start_time,
    end_time,
    records_processed,
    status,
    parameters_used
FROM archive_process_log 
WHERE execution_date = '2025-08-13'
AND start_time >= '21:40:00'
ORDER BY start_time DESC;

-- Check for any error conditions
SELECT 
    log_level,
    log_message,
    log_timestamp,
    process_id
FROM archive_execution_log
WHERE log_timestamp >= '2025-08-13 21:40:00'
AND process_id IN (33882598, 54526378)
ORDER BY log_timestamp;
```

### 5. Business Day Behavior Analysis

Since this was the original version (without CPR enhancement), the business day logic was:

#### **Expected Behavior at 21:41:42**
- **Time**: 21:41:42 (before 23:30 cutoff)
- **Expected**: Should archive current day (13/08/2025) closed orders
- **Original Logic**: No emergency vs normal run distinction

#### **Verification Query**
```sql
-- Confirm which day was actually processed
SELECT 
    'Business day verification' as analysis,
    CASE 
        WHEN MAX(close_date::DATE) = '2025-08-13' 
        THEN 'Processed current day (expected for 21:41 run)'
        WHEN MAX(close_date::DATE) = '2025-08-12' 
        THEN 'Processed previous day (unexpected for 21:41 run)'
        ELSE 'Processed other day: ' || MAX(close_date::DATE)
    END as business_day_behavior
FROM akdauftrag 
WHERE archive_date >= CURRENT - 1 UNITS HOUR;
```

## 6. Key Findings and Next Steps

### ✅ What We Confirmed:
- Original tpldauftr script executes successfully
- Database connection to ode21@zdev21_shm works
- Parameters -fil 50 and -del 35 are accepted
- Process completed with RC: 0

### 🔍 What to Investigate:
- Actual records archived (run inspection queries above)
- Which business day was processed (13/08 vs 12/08)
- Impact of -fil 50 and -del 35 parameters
- Differences between original and enhanced versions

### ⚠️ CPR Enhancement Issues:
- Enhanced version with `--business-day-mode enabled` failed
- Original version works but may not have the business day fix
- Need to analyze why enhancement breaks execution

### 📋 Recommended Actions:
1. **Run inspection queries** to see what the script actually archived
2. **Compare original vs enhanced** binary for parameter differences  
3. **Test enhanced version** with corrected parameters
4. **Validate business day logic** in both versions

---
**Execution Date**: August 13, 2025 21:41:42  
**Analysis Date**: August 13, 2025 21:45:00  
**Status**: Original script successful, enhancement needs debugging