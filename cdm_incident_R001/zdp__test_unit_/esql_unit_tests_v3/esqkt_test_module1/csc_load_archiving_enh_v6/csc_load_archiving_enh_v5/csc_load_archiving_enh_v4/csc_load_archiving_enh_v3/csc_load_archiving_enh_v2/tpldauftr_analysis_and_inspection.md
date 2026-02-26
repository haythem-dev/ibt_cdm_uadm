# tpldauftr Script Analysis & Pre-Launch Table Inspection

## 1. Script Process Flow Analysis

### 1.1 What tpldauftr Does (Step-by-Step)

Based on the CPR description, the tpldauftr script performs the following operations:

#### **Phase 1: Initialization & Validation**
1. **Parameter Processing**
   - Reads command line parameters
   - Determines business day logic (normal vs emergency run)
   - Validates database connectivity
   - Sets archiving date range

2. **Business Day Calculation** 
   - **Normal Run (≤23:30)**: Archives current business day closed orders
   - **Late/Emergency Run (>00:00 next day)**: Archives previous business day only
   - **Critical Logic**: Prevents same-day blindness during emergency runs

#### **Phase 2: Data Selection & Validation**
3. **Order Selection**
   - Identifies closed orders based on calculated business day
   - Validates order status and completeness
   - Checks for existing archive records (avoid duplicates)
   - Validates referential integrity

4. **Dependency Check**
   - Verifies all related child records are also closed
   - Checks foreign key constraints
   - Validates business rules compliance

#### **Phase 3: Archive Execution**
5. **Data Movement**
   - Copies records from source tables to archive tables
   - Maintains referential integrity during transfer
   - Handles transaction rollback on failures

6. **Cleanup & Verification**
   - Removes archived records from source tables
   - Updates status flags and timestamps
   - Logs archiving statistics
   - Validates archive completeness

### 1.2 Enhanced Business Day Logic (Post-CPR)
```
IF current_time <= 23:30 THEN
    archive_date = current_business_day
ELSE IF current_time > 00:00 AND current_date > original_business_day THEN
    archive_date = previous_business_day  -- Prevents same-day blindness
ELSE
    archive_date = current_business_day
END IF
```

## 2. Comprehensive Table Impact Analysis

### 2.1 Primary Tables (Direct Impact)

#### **kdauftrag** (Source Orders Table)
- **Impact**: Records will be deleted after archiving
- **Critical Fields**: order_id, status, close_date, archived_flag
- **Risk Level**: HIGH

#### **akdauftrag** (Archive Orders Table)  
- **Impact**: New records will be inserted
- **Critical Fields**: order_id, archive_date, original_close_date
- **Risk Level**: MEDIUM

### 2.2 Secondary Tables (Potential Dependencies)

#### **kdaufpos** (Order Line Items)
- **Relationship**: Child records of kdauftrag
- **Impact**: Line items archived with parent orders
- **Critical Fields**: order_id, line_id, status

#### **akdaufpos** (Archive Order Line Items)
- **Relationship**: Archive destination for kdaufpos
- **Impact**: Receives archived line items
- **Critical Fields**: order_id, line_id, archive_date

#### **kdaufhist** (Order History/Status Changes)
- **Relationship**: Historical tracking of order changes
- **Impact**: History records may need archiving
- **Critical Fields**: order_id, change_date, status_from, status_to

#### **akdaufhist** (Archive Order History)
- **Relationship**: Archive destination for order history
- **Impact**: Receives historical records
- **Critical Fields**: order_id, change_date, archive_date

### 2.3 Reference Tables (Indirect Impact)

#### **kdaufref** (Order References/Links)
- **Relationship**: Cross-references between orders
- **Impact**: Reference integrity must be maintained
- **Critical Fields**: source_order_id, target_order_id, ref_type

#### **kdaufstat** (Order Status Tracking)
- **Relationship**: Current status of all orders
- **Impact**: Status records updated during archiving
- **Critical Fields**: order_id, current_status, last_update

#### **kdauflog** (Order Processing Log)
- **Relationship**: Audit trail of order processing
- **Impact**: Log entries may reference archived orders
- **Critical Fields**: order_id, log_date, process_type, user_id

### 2.4 System Tables (Monitoring Impact)

#### **archive_control** (Archive Process Control)
- **Relationship**: Controls and tracks archive processes
- **Impact**: Process status and parameters
- **Critical Fields**: process_name, last_run, next_run, status

#### **archive_stats** (Archive Statistics)
- **Relationship**: Statistical tracking of archiving
- **Impact**: Updated with archiving metrics
- **Critical Fields**: process_date, records_archived, table_name

## 3. Pre-Launch Inspection Queries (Informix)

### 3.1 Primary Table Health Check

```sql
-- kdauftrag Source Table Analysis
SELECT 
    'kdauftrag' as table_name,
    COUNT(*) as total_records,
    COUNT(CASE WHEN status = 'CLOSED' THEN 1 END) as closed_orders,
    COUNT(CASE WHEN status = 'CLOSED' AND archived_flag = 'N' THEN 1 END) as ready_for_archive,
    COUNT(CASE WHEN status = 'CLOSED' AND archived_flag = 'Y' THEN 1 END) as already_archived,
    MIN(close_date) as earliest_close,
    MAX(close_date) as latest_close
FROM kdauftrag
WHERE close_date IS NOT NULL;

-- akdauftrag Archive Table Status
SELECT 
    'akdauftrag' as table_name,
    COUNT(*) as total_archived,
    MIN(archive_date) as first_archive,
    MAX(archive_date) as last_archive,
    COUNT(DISTINCT DATE(archive_date)) as archive_days
FROM akdauftrag;
```

### 3.2 Business Day Target Analysis

```sql
-- Today's Target Orders (13/08/2025)
SELECT 
    'Target for archiving' as description,
    close_date::DATE as business_day,
    COUNT(*) as orders_to_archive,
    MIN(close_date) as first_close,
    MAX(close_date) as last_close,
    COUNT(CASE WHEN archived_flag = 'Y' THEN 1 END) as already_archived_conflict
FROM kdauftrag 
WHERE status = 'CLOSED'
AND close_date::DATE = CASE 
    WHEN CURRENT::TIME <= '23:30:00' THEN '2025-08-13'::DATE
    ELSE '2025-08-12'::DATE  -- Previous day for late runs
END
AND (archived_flag = 'N' OR archived_flag IS NULL)
GROUP BY close_date::DATE;

-- Previous Day Safety Check
SELECT 
    'Previous day check' as description,
    close_date::DATE as business_day,
    COUNT(*) as potential_orders,
    COUNT(CASE WHEN archived_flag = 'N' THEN 1 END) as unarchived
FROM kdauftrag 
WHERE status = 'CLOSED'
AND close_date::DATE = '2025-08-12'::DATE
GROUP BY close_date::DATE;
```

### 3.3 Referential Integrity Validation

```sql
-- Order Line Items Consistency
SELECT 
    k.order_id,
    COUNT(p.line_id) as line_items,
    k.status as order_status,
    k.close_date,
    CASE 
        WHEN COUNT(CASE WHEN p.status != 'CLOSED' THEN 1 END) > 0 
        THEN 'OPEN_LINES_EXIST'
        ELSE 'ALL_LINES_CLOSED'
    END as line_status
FROM kdauftrag k
LEFT JOIN kdaufpos p ON k.order_id = p.order_id
WHERE k.status = 'CLOSED'
AND k.close_date::DATE >= '2025-08-12'::DATE
GROUP BY k.order_id, k.status, k.close_date
HAVING COUNT(CASE WHEN p.status != 'CLOSED' THEN 1 END) > 0;

-- Cross-Reference Validation
SELECT 
    r.source_order_id,
    r.target_order_id,
    r.ref_type,
    k1.status as source_status,
    k2.status as target_status,
    CASE 
        WHEN k1.status = 'CLOSED' AND k2.status != 'CLOSED' 
        THEN 'REFERENCE_INTEGRITY_ISSUE'
        ELSE 'OK'
    END as integrity_status
FROM kdaufref r
JOIN kdauftrag k1 ON r.source_order_id = k1.order_id
JOIN kdauftrag k2 ON r.target_order_id = k2.order_id
WHERE k1.status = 'CLOSED'
AND k1.close_date::DATE >= '2025-08-12'::DATE
AND k2.status != 'CLOSED';
```

### 3.4 Archive Destination Readiness

```sql
-- Archive Table Space and Conflicts
SELECT 
    'akdauftrag readiness' as check_type,
    COUNT(*) as existing_records,
    COUNT(CASE WHEN archive_date::DATE = '2025-08-13' THEN 1 END) as todays_archives,
    COUNT(CASE WHEN archive_date::DATE = '2025-08-12' THEN 1 END) as yesterday_archives
FROM akdauftrag;

-- Duplicate Prevention Check
SELECT 
    a.order_id,
    a.archive_date,
    k.status as current_status,
    k.close_date,
    'DUPLICATE_RISK' as warning
FROM akdauftrag a
JOIN kdauftrag k ON a.order_id = k.order_id
WHERE k.status = 'CLOSED'
AND k.close_date::DATE >= '2025-08-12'::DATE;
```

### 3.5 System Resource Validation

```sql
-- Table Lock Detection
SELECT 
    'LOCK_CHECK' as check_type,
    tabname as table_name,
    type as lock_type,
    owner,
    waiter,
    'POTENTIAL_BLOCKING' as warning
FROM sysmaster:syslocks 
WHERE tabname IN ('kdauftrag', 'akdauftrag', 'kdaufpos', 'akdaufpos')
AND type IN ('X', 'S', 'U');

-- Database Space Check
SELECT 
    'SPACE_CHECK' as check_type,
    t.tabname,
    c.size as chunk_size,
    c.used as used_space,
    (c.size - c.used) as available_space,
    ROUND(((c.used * 100.0) / c.size), 2) as usage_percent
FROM systables t
JOIN syschunks c ON t.partnum = c.partnum
WHERE t.tabname IN ('kdauftrag', 'akdauftrag', 'kdaufpos', 'akdaufpos')
AND c.size > 0;
```

### 3.6 Process Control Validation

```sql
-- Archive Process Status
SELECT 
    process_name,
    last_run_date,
    last_run_time,
    status,
    records_processed,
    CASE 
        WHEN status = 'RUNNING' THEN 'PROCESS_ALREADY_ACTIVE'
        WHEN status = 'FAILED' THEN 'PREVIOUS_FAILURE_NEEDS_REVIEW'
        ELSE 'READY'
    END as process_status
FROM archive_control 
WHERE process_name = 'tpldauftr'
ORDER BY last_run_date DESC, last_run_time DESC;

-- Recent Archive Statistics
SELECT 
    process_date,
    table_name,
    records_archived,
    process_duration,
    status
FROM archive_stats 
WHERE process_date >= '2025-08-06'::DATE
AND table_name IN ('kdauftrag', 'kdaufpos')
ORDER BY process_date DESC, table_name;
```

## 4. Critical Pre-Launch Checklist

### ✅ Mandatory Checks (MUST PASS)
- [ ] No active locks on source/target tables
- [ ] No running archive processes
- [ ] Sufficient disk space (>20% available)
- [ ] All closed orders have closed line items
- [ ] No duplicate archive candidates
- [ ] Database backup completed in last 24 hours

### ⚠️ Warning Conditions (REVIEW REQUIRED)
- [ ] Cross-reference integrity issues
- [ ] Previous archive process failures
- [ ] Unusual volume of orders to archive
- [ ] Emergency run during business hours

### 🚫 Blocking Conditions (DO NOT PROCEED)
- [ ] Active exclusive locks detected
- [ ] Archive process already running
- [ ] Disk space <10% available
- [ ] Referential integrity violations
- [ ] Missing archive destination tables

## 5. Emergency Rollback Procedures

### Immediate Actions if Issues Detected:
1. **Stop Process**: `kill -9 <tpldauftr_pid>`
2. **Check Status**: Run integrity validation queries
3. **Assess Damage**: Count missing vs archived records
4. **Restore Options**: 
   - Rollback transaction (if still active)
   - Restore from backup (if process completed)
   - Manual data recovery (last resort)

---
**Created**: August 13, 2025  
**Analysis Date**: August 13, 2025  
**Target Business Day**: August 12/13, 2025 (depending on run time)