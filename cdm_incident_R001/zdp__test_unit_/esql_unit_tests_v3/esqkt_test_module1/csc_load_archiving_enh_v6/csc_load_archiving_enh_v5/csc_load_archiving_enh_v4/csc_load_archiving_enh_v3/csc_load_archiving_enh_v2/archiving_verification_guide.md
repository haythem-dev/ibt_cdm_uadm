# Order Archiving Process - Initial Verification Guide

## Project Overview
This guide covers the initial verification steps required before launching the order archiving script (tpldauftrag) that archives closed orders from `kdauftrag` to `akdauftrag`.

### CPR Enhancement Summary
- **Issue**: Emergency daytime runs were archiving current business day orders, making them invisible to business users
- **Solution**: Enhanced logic to distinguish between normal night runs (archive same day) vs late runs (archive previous business day only)
- **Critical Time**: 23:30 normal run vs 8:00+ late run behavior

## Pre-Launch Verification Checklist

### 1. Database Connection Verification
First, verify database connectivity and basic system status:

```sql
-- Check database connection and current timestamp
SELECT CURRENT FROM sysmaster:sysdual;

-- Verify database is online
SELECT name, is_online FROM sysmaster:sysdatabases 
WHERE name IN ('your_database_name');
```

### 2. Core Table Status Verification

#### 2.1 Main Order Tables Status
```sql
-- Check kdauftrag table status and record count
SELECT 
    COUNT(*) as total_records,
    MIN(creation_date) as earliest_order,
    MAX(creation_date) as latest_order,
    COUNT(CASE WHEN status = 'CLOSED' THEN 1 END) as closed_orders
FROM kdauftrag;

-- Check akdauftrag archive table status
SELECT 
    COUNT(*) as archived_records,
    MIN(archive_date) as earliest_archive,
    MAX(archive_date) as latest_archive
FROM akdauftrag;
```

#### 2.2 Business Day Analysis
```sql
-- Analyze closed orders by business day (last 7 days from 13/08/2025)
SELECT 
    close_date::DATE as business_day,
    COUNT(*) as closed_orders,
    COUNT(CASE WHEN archived_flag = 'Y' THEN 1 END) as already_archived,
    COUNT(CASE WHEN archived_flag = 'N' OR archived_flag IS NULL THEN 1 END) as pending_archive
FROM kdauftrag 
WHERE close_date >= '2025-08-06'::DATE
AND status = 'CLOSED'
GROUP BY close_date::DATE
ORDER BY 1 DESC;
```

#### 2.3 Current Business Day Status
```sql
-- Check current business day closed orders (critical for emergency runs)
SELECT 
    COUNT(*) as todays_closed_orders,
    MIN(close_date) as first_close_today,
    MAX(close_date) as last_close_today
FROM kdauftrag 
WHERE DATE(close_date) = CURRENT
AND status = 'CLOSED'
AND (archived_flag = 'N' OR archived_flag IS NULL);
```

### 3. System Parameters Verification

#### 3.1 Archive Process Parameters
```sql
-- Check last archive run details
SELECT 
    process_name,
    last_run_date,
    last_run_time,
    business_day_processed,
    records_archived,
    run_status
FROM archive_process_log 
WHERE process_name = 'tpldauftrag'
ORDER BY last_run_date DESC, last_run_time DESC
LIMIT 5;
```

#### 3.2 Business Day Configuration
```sql
-- Verify business day calculation logic
SELECT 
    CURRENT as current_timestamp,
    DATE(CURRENT) as current_date,
    CASE 
        WHEN EXTRACT(HOUR FROM CURRENT) >= 23 THEN DATE(CURRENT)
        ELSE DATE(CURRENT) - 1 UNITS DAY
    END as target_business_day_normal,
    DATE(CURRENT) - 1 UNITS DAY as target_business_day_late
FROM sysmaster:sysdual;
```

### 4. Data Integrity Checks

#### 4.1 Referential Integrity
```sql
-- Check for orphaned records or integrity issues
SELECT 
    k.order_id,
    k.status,
    k.close_date,
    a.archive_date,
    CASE 
        WHEN a.order_id IS NOT NULL THEN 'ALREADY_ARCHIVED'
        WHEN k.status = 'CLOSED' THEN 'READY_FOR_ARCHIVE'
        ELSE 'NOT_ELIGIBLE'
    END as archive_status
FROM kdauftrag k
LEFT JOIN akdauftrag a ON k.order_id = a.order_id
WHERE k.status = 'CLOSED'
AND DATE(k.close_date) = CURRENT - 1 UNITS DAY
ORDER BY k.close_date;
```

#### 4.2 Lock Status Check
```sql
-- Check for table locks that might interfere with archiving
SELECT 
    tabname,
    type,
    owner,
    waiter
FROM sysmaster:syslocks 
WHERE tabname IN ('kdauftrag', 'akdauftrag')
AND type IN ('S', 'X', 'U');
```

### 5. Critical Decision Matrix

| Current Time | Target Business Day | Action Required |
|--------------|-------------------|-----------------|
| Before 23:30 | Current Date | Archive current day closed orders |
| After 23:30 same day | Current Date | Archive current day closed orders |
| After 00:00 next day | Previous Date | Archive previous day closed orders only |
| Emergency run (8:00+ AM) | Previous Date | Archive previous day closed orders only |

### 6. Pre-Launch Safety Verification

#### 6.1 Backup Verification
```sql
-- Verify recent backup exists
SELECT 
    name,
    level,
    began,
    ended,
    status
FROM sysmaster:sysbackup
WHERE name LIKE '%kdauftrag%'
AND began >= CURRENT - 1 UNITS DAY
ORDER BY began DESC;
```

#### 6.2 Space Check
```sql
-- Check available space for archive operations
SELECT 
    name,
    size,
    used,
    (size - used) as available_space,
    ROUND(((used * 100.0) / size), 2) as usage_percent
FROM sysmaster:syschunks
WHERE chknum IN (
    SELECT chknum FROM systables 
    WHERE tabname IN ('kdauftrag', 'akdauftrag')
);
```

## Launch Decision Criteria

### ✅ Safe to Proceed When:
- All tables are online and accessible
- No blocking locks detected
- Backup completed within last 24 hours
- Available space > 20%
- Business day logic correctly calculated
- No pending archival conflicts detected

### ⚠️ Review Required When:
- Current day has closed orders and time > 00:00
- Unusual lock patterns detected
- Previous archive run failed
- Space usage > 80%

### ❌ Do Not Proceed When:
- Tables are offline or inaccessible
- Critical locks detected
- No recent backup available
- Space usage > 90%
- Data integrity issues found

## Emergency Rollback Plan

If issues occur during archiving:

1. **Stop Process**: Kill archive process immediately
2. **Verify Data**: Check both source and target tables
3. **Restore if Needed**: Use most recent backup
4. **Document Issues**: Log all findings for analysis

## Contact Information

- **Database Team**: [Contact Details]
- **Business Users**: [Contact Details]  
- **Emergency Escalation**: [Contact Details]

---

**Created**: August 13, 2025  
**Last Updated**: [Update Date]  
**Version**: 1.0