
# Informix Database Tables: Business Day Logic vs Retention Period Analysis

## Executive Summary

The TPLDAUFTR archiving process involves **two distinct filtering mechanisms** that work on different database tables and serve different purposes in your Informix environment.

## 1. Database Tables Involved

### 1.1 Primary Order Tables
```sql
-- Main order processing tables
KDAUFTRAG          -- Active customer orders (source)
AKDAUFTRAG         -- Archived customer orders (target)
KDAUFTRAGPOS       -- Order line items
KDAUFTRAGBEM       -- Order remarks
KDAUFTRAGWERTE     -- Order values
```

### 1.2 Related Processing Tables
```sql
-- Cascaded archiving tables (processed with each order)
KDAUFTRAGDEBTREL   -- Debt relations
KDAUFTRAGRECHART   -- Recharge data
KDAUFTRAGDISCACC   -- Discount accounts
KDAUFTRAGPROLINK   -- Product links
KDAUFTRAGPOSRAB    -- Position discounts
KDAUFTRAGPOSCHARGE -- Position charges
```

## 2. Business Day Logic (CPR Enhancement)

### 2.1 Database Impact
```sql
-- Business Day Logic determines WHICH orders to process
-- Affects: Selection criteria for KDAUFTRAG → AKDAUFTRAG transfer

-- Without CPR (Original):
SELECT * FROM KDAUFTRAG 
WHERE vertriebszentrumnr = ? 
AND kdauftragstat = '0000'
AND datumauslieferung <= ?;  -- Simple date cutoff

-- With CPR (Enhanced):
SELECT * FROM KDAUFTRAG 
WHERE vertriebszentrumnr = ? 
AND kdauftragstat = '0000'
AND datumauslieferung <= ?   -- Business day calculated cutoff
AND additional_business_day_filters;
```

### 2.2 Business Day Calculation Tables
```sql
-- System tables used for business day calculations
SYSMASTER:SYSDUAL     -- Current system date/time
CALENDAR_TABLE        -- Business day definitions (if exists)
HOLIDAY_TABLE         -- Holiday exclusions (if exists)
```

### 2.3 Time-Based Logic Impact
| Execution Time | Target Date Calculation | Database Query Impact |
|----------------|------------------------|----------------------|
| **Before 6 AM** | Previous business day | `WHERE datumauslieferung <= [previous_business_day]` |
| **After 6 AM** | Current business day | `WHERE datumauslieferung <= [current_business_day]` |
| **Weekend Run** | Friday rollback | `WHERE datumauslieferung <= [last_friday]` |

## 3. Retention Period (35-Day Policy)

### 3.1 Database Impact
```sql
-- Retention Period determines HOW LONG archived data is kept
-- Affects: Cleanup operations on AKDAUFTRAG and related tables

-- Retention cleanup query:
DELETE FROM AKDAUFTRAG 
WHERE datumarchivierung <= CURRENT - 35 UNITS DAY;

-- Cascaded cleanup on related tables:
DELETE FROM AKDAUFTRAGPOS 
WHERE kdauftragnr NOT IN (SELECT kdauftragnr FROM AKDAUFTRAG);
```

### 3.2 Retention Policy Tables
```sql
-- Tables affected by retention cleanup
AKDAUFTRAG         -- Main archived orders (35+ days deleted)
AKDAUFTRAGPOS      -- Archived positions (cascade deleted)
AKDAUFTRAGBEM      -- Archived remarks (cascade deleted)
AKDAUFTRAGWERTE    -- Archived values (cascade deleted)
```

## 4. Combined Database Operation Flow

### 4.1 Phase 1: Business Day Selection
```sql
-- Step 1: Calculate business day cutoff
DECLARE business_day_cutoff DATE;
SET business_day_cutoff = calculate_business_day(CURRENT);

-- Step 2: Select orders for archiving based on business day
INSERT INTO temp_orders_to_archive 
SELECT kdauftragnr FROM KDAUFTRAG 
WHERE vertriebszentrumnr = 50
AND kdauftragstat = '0000'
AND datumauslieferung <= business_day_cutoff;
```

### 4.2 Phase 2: Archive Transfer
```sql
-- Step 3: Move selected orders to archive
INSERT INTO AKDAUFTRAG 
SELECT *, CURRENT as datumarchivierung 
FROM KDAUFTRAG 
WHERE kdauftragnr IN (SELECT kdauftragnr FROM temp_orders_to_archive);

-- Step 4: Remove from active table
DELETE FROM KDAUFTRAG 
WHERE kdauftragnr IN (SELECT kdauftragnr FROM temp_orders_to_archive);
```

### 4.3 Phase 3: Retention Cleanup
```sql
-- Step 5: Apply retention policy (independent of business day logic)
DELETE FROM AKDAUFTRAG 
WHERE datumarchivierung <= CURRENT - 35 UNITS DAY;
```

## 5. Database Performance Impact

### 5.1 Index Requirements
```sql
-- Optimal indexes for business day logic
CREATE INDEX idx_kdauftrag_business_day 
ON KDAUFTRAG(vertriebszentrumnr, kdauftragstat, datumauslieferung);

-- Optimal indexes for retention cleanup
CREATE INDEX idx_akdauftrag_retention 
ON AKDAUFTRAG(datumarchivierung);
```

### 5.2 Transaction Log Impact
| Operation | Log Volume | Recovery Impact |
|-----------|------------|-----------------|
| Business Day Archive | Medium (selective) | Low risk |
| Retention Cleanup | High (bulk delete) | Medium risk |
| Combined Operation | High | Plan during maintenance window |

## 6. Key Database Insights

### 6.1 Business Day Logic (Selection Filter)
- **Purpose**: Controls WHICH orders move from KDAUFTRAG → AKDAUFTRAG
- **Timing**: Prevents premature archiving of current day orders
- **Impact**: Maintains business visibility of active orders
- **Tables**: Primarily affects KDAUFTRAG selection criteria

### 6.2 Retention Period (Cleanup Filter)  
- **Purpose**: Controls HOW LONG orders stay in AKDAUFTRAG
- **Timing**: Independent cleanup based on archive date
- **Impact**: Manages database size and performance
- **Tables**: Primarily affects AKDAUFTRAG and related archive tables

### 6.3 Combined Benefit
```sql
-- The two mechanisms work together:
-- Business Day Logic: Smart archiving (prevents blindness)
-- Retention Period: Smart cleanup (prevents bloat)

-- Result: Optimal database management
-- Active orders remain visible when needed
-- Archived orders cleaned up efficiently
-- Database performance maintained
```

## 7. Verification Queries

### 7.1 Business Day Logic Verification
```sql
-- Check what business day was used for last run
SELECT 
    MAX(datumauslieferung) as last_business_day_archived,
    COUNT(*) as orders_archived,
    MAX(datumarchivierung) as archive_timestamp
FROM AKDAUFTRAG 
WHERE datumarchivierung >= CURRENT - 1 UNITS DAY;
```

### 7.2 Retention Policy Verification
```sql
-- Check retention policy application
SELECT 
    MIN(datumarchivierung) as oldest_archive,
    MAX(datumarchivierung) as newest_archive,
    COUNT(*) as total_archived_orders,
    COUNT(CASE WHEN datumarchivierung <= CURRENT - 35 UNITS DAY THEN 1 END) as should_be_cleaned
FROM AKDAUFTRAG;
```

## Conclusion

In your Informix database environment:

- **Business Day Logic** = **Smart Selection** (KDAUFTRAG → AKDAUFTRAG)
- **Retention Period** = **Smart Cleanup** (AKDAUFTRAG maintenance)  
- **Combined Effect** = **Optimal Order Lifecycle Management**

Both mechanisms are essential and work at different stages of the order lifecycle to ensure business continuity and database efficiency.
