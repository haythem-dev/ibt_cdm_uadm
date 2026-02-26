# Complete tpldauftr Script Analysis - Based on Original Source Code

## Executive Summary

The tpldauftr (Order Archiving) script is a comprehensive C++ application that performs daily archiving/deletion of closed customer orders and related data from an Informix database. This analysis is based on the original source files and successful execution on August 13, 2025.

## 1. Script Architecture & Execution Flow

### 1.1 Shell Wrapper (tpldauftr.sh)
The shell script acts as a parameter processor and launcher:

```bash
# Execution Command Structure:
./tpldauftr.sh -start -fil 50

# Translated to Binary Call:
/software/wss/de/ksc/load/bin/tpldauftr.bin -fil 50 -db ode21@zdev21_shm -del 35
```

#### **Parameter Processing:**
- **-fil 50**: Filial number (branch/store ID = 50)
- **-del 35**: Retention period (35 days minimum)
- **-db ode21@zdev21_shm**: Database connection string
- **-h24**: 24-hour processing flag (optional)
- **-dg**: Debug mode (optional)

### 1.2 C++ Binary Logic (tpldauftr.cxx)

#### **Phase 1: Initialization & Validation**
1. **Parameter Validation**
   - Validates numeric parameters (-fil, -del)
   - Ensures minimum 30-day retention
   - Establishes database connection

2. **Pre-Processing Checks**
   - `PruefeFaktur()`: Verifies all orders are invoiced
   - `PruefeAuftrag()`: Checks all orders are completed
   - `PruefeVBAuftrag()`: Validates VB orders are processed
   - `UpdateKzrestored()`: Updates restoration flags

#### **Phase 2: Date Calculation Logic**
**Current Implementation (Pre-CPR):**
```cpp
// No business day enhancement - processes based on retention period only
lDatumAktuell = current_date;
target_date = lDatumAktuell - iNumberOfDay; // 35 days back
```

**Key Finding**: The original version does NOT have the business day logic enhancement mentioned in your CPR. It simply processes orders older than the retention period (35 days).

#### **Phase 3: Archival Processing**
The script processes orders in this sequence:

1. **Archive Historical Data** (`DeleteOld()`)
2. **Delete CSV/Log Files** (`DeleteOldCSV()`)
3. **Process Main Orders** (loop through `kdauftrag`)
4. **Process Related Tables** (cascade deletion)
5. **Handle Special Cases** (VB orders, orphaned records)

## 2. Complete Table Impact Analysis

### 2.1 Primary Order Tables

#### **kdauftrag** → **akdauftrag** (Main Orders)
- **Selection Criteria**: `datumauslieferung <= target_date AND kdauftragstat = "0000"`
- **Business Impact**: Orders become invisible to business users
- **Records Processed**: Based on -fil parameter (branch filter)

#### **Associated Order Tables** (Cascaded Processing):
```cpp
// Processing sequence from source code:
DeleteKdauftragDebtrel(&num, &z);      // Debt relations
DeleteKdauftragRechart(&num, &z);      // Recharge data  
DeleteKdauftragDiscacc(&num, &z);      // Discount accounts
DeleteKdauftragProlink(&num, &z);      // Product links
DeleteKdauftragPos(&num, &z);          // Order positions
DeleteKdauftragPosRab(&num, &z);       // Position discounts
DeleteKdauftragPosCharge(&num, &z);    // Position charges
DeleteKdauftragPosChange(&num, &z);    // Position changes
DeleteKdauftragPosRechart(&num, &z);   // Position recharges
DeleteKdauftragPosPromo(&num, &z);     // Position promotions
DeleteKdauftragPosRefund(&num, &z);    // Position refunds
DeleteKdauftragPosXml(&num, &z);       // XML data
DeleteKdauftragPosEreignis(&num, &z);  // Position events
DeleteKdauftragPosTour(&num, &z);      // Tour data
DeleteKdauftragPartnerFunction(&num, &z); // Partner functions
DeleteKdauftragBem(&num, &z);          // Remarks
DeleteKdauftragWerte(&num, &z);        // Values
DeleteOrderRelationNo(&num, &z);       // Order relations
DeleteDispoArtikel(&num, &z);          // Disposition articles
DeleteKoeinheit(&num, &z);             // Cost units
DeleteKdauftraginfo(&num, &z);         // Order info
DeleteOrderRemarks(&num, &z);          // Order remarks
DeleteOrderPositionDiscount(&num, &z); // Position discounts
DeleteCscOrderFlags(&num, &z);         // CSC flags
DeleteCscOrderPosSplittInfo(&num, &z); // Split info
DeleteCscOrderIBTPickingData(&num, &z); // Picking data
DeleteOrderPositionDiscountProcessed(&num, &z); // Processed discounts
```

### 2.2 Special Processing Tables

#### **VB Orders (Verbund)**
```cpp
// Special handling for VB orders:
DeleteKdverbund(&num, &z);        // VB headers
DeleteKdverbundPos(&num, &z);     // VB positions  
DeleteKdverbundPosRab(&num, &z);  // VB position discounts
```

#### **System Tables**
```cpp
DeleteKdTourDruck(&z);           // Tour printing
DeleteKoauftrag(&z);             // Cost orders
DeleteFkwarteschlange(&z);       // Invoice queue
DeleteDailyFiles(&z);            // Daily files
DeletePhoneHistory(&z);          // Phone history
```

## 3. Comprehensive Inspection Queries (Post-Execution)

### 3.1 Execution Verification (August 13, 2025 21:41:42)

```sql
-- Verify script execution impact
SELECT 
    'Execution Summary' as check_type,
    COUNT(*) as total_orders_in_system,
    COUNT(CASE WHEN datumauslieferung <= CURRENT - 35 UNITS DAY THEN 1 END) as retention_eligible,
    COUNT(CASE WHEN kdauftragstat = '0000' THEN 1 END) as closed_orders,
    COUNT(CASE WHEN vertriebszentrumnr = 50 THEN 1 END) as branch_50_orders
FROM kdauftrag;

-- Check recent archival activity (since 21:40:00)
SELECT 
    'Recent Activity' as activity_type,
    COUNT(*) as records_archived,
    MIN(datumarchivierung) as first_archive,
    MAX(datumarchivierung) as last_archive  
FROM akdauftrag 
WHERE datumarchivierung >= '2025-08-13 21:40:00';
```

### 3.2 Table-by-Table Impact Assessment

```sql
-- Position tables verification
SELECT 
    'kdauftragpos' as table_name,
    COUNT(*) as current_records,
    COUNT(CASE WHEN kdauftragnr IN (
        SELECT kdauftragnr FROM akdauftrag 
        WHERE datumarchivierung >= '2025-08-13 21:40:00'
    ) THEN 1 END) as should_be_archived
FROM kdauftragpos;

-- Remarks and additional data
SELECT 
    'kdauftragbem' as table_name,
    COUNT(*) as current_records,
    COUNT(CASE WHEN kdauftragnr IN (
        SELECT kdauftragnr FROM akdauftrag 
        WHERE datumarchivierung >= '2025-08-13 21:40:00'  
    ) THEN 1 END) as should_be_archived
FROM kdauftragbem;

-- CSC specific tables
SELECT 
    'cscorderflags' as table_name,
    COUNT(*) as current_records,
    COUNT(CASE WHEN order_id IN (
        SELECT kdauftragnr FROM akdauftrag 
        WHERE datumarchivierung >= '2025-08-13 21:40:00'
    ) THEN 1 END) as should_be_archived
FROM cscorderflags;
```

### 3.3 Branch 50 Specific Analysis

```sql
-- Verify branch 50 processing (from -fil 50 parameter)
SELECT 
    'Branch 50 Analysis' as analysis_type,
    vertriebszentrumnr as branch,
    COUNT(*) as total_orders,
    COUNT(CASE WHEN kdauftragstat = '0000' THEN 1 END) as closed_orders,
    COUNT(CASE WHEN datumauslieferung <= CURRENT - 35 UNITS DAY THEN 1 END) as retention_eligible,
    MIN(datumauslieferung) as oldest_delivery,
    MAX(datumauslieferung) as newest_delivery
FROM kdauftrag 
WHERE vertriebszentrumnr = 50
GROUP BY vertriebszentrumnr;

-- Check what was actually processed for branch 50
SELECT 
    'Archived from Branch 50' as result_type,
    COUNT(*) as archived_orders,
    MIN(datumauslieferung) as oldest_archived,
    MAX(datumauslieferung) as newest_archived
FROM akdauftrag 
WHERE datumarchivierung >= '2025-08-13 21:40:00'
AND original_vertriebszentrumnr = 50;
```

### 3.4 Retention Policy Verification

```sql
-- Verify 35-day retention policy application
SELECT 
    'Retention Policy Check' as policy_check,
    CASE 
        WHEN datumauslieferung <= CURRENT - 35 UNITS DAY THEN 'Within Policy (>35 days)'
        WHEN datumauslieferung > CURRENT - 35 UNITS DAY THEN 'Outside Policy (<35 days)'
        ELSE 'Unknown'
    END as retention_status,
    COUNT(*) as order_count,
    MIN(datumauslieferung) as earliest_delivery,
    MAX(datumauslieferung) as latest_delivery
FROM akdauftrag 
WHERE datumarchivierung >= '2025-08-13 21:40:00'
GROUP BY 
    CASE 
        WHEN datumauslieferung <= CURRENT - 35 UNITS DAY THEN 'Within Policy (>35 days)'
        WHEN datumauslieferung > CURRENT - 35 UNITS DAY THEN 'Outside Policy (<35 days)'
        ELSE 'Unknown'
    END;
```

## 4. Key Findings & CPR Implications

### 4.1 Original vs Enhanced Version

| Aspect | Original Version | Enhanced Version (CPR) |
|--------|------------------|------------------------|
| **Business Day Logic** | No time-based logic | `--business-day-mode enabled` |
| **Execution Success** | ✅ Works (RC: 0) | ❌ Failed (RC: 2) |
| **Date Calculation** | Fixed retention period | Time-sensitive calculation |
| **Emergency Handling** | No special handling | Prevents same-day blindness |

### 4.2 CPR Enhancement Requirements

**Current Issue**: The original version processes orders based purely on retention period (35 days), not business day logic.

**CPR Goal**: Add time-sensitive business day calculation:
- **Normal runs (≤23:30)**: Archive current business day
- **Late/Emergency runs (>00:00)**: Archive previous business day only

### 4.3 Root Cause of Enhancement Failure

The enhanced version failed because:
1. **New Parameter**: `--business-day-mode enabled` not recognized by binary
2. **Modified Logic**: Time-based calculation changes not properly integrated
3. **Database Connection**: Enhanced version may have connection string issues

## 5. Recommended Action Plan

### 5.1 Immediate Steps
1. **Verify Current Archival Results** using inspection queries above
2. **Compare Original vs Enhanced** binary for parameter differences  
3. **Test Enhanced Version** with corrected parameters
4. **Validate Business Logic** in both versions

### 5.2 CPR Implementation Strategy
1. **Fix Enhanced Binary** to properly handle business day parameters
2. **Add Time Logic** for emergency vs normal run detection
3. **Maintain Compatibility** with existing shell wrapper
4. **Test Thoroughly** before production deployment

## 6. Success Metrics

### ✅ Original Version Success Indicators:
- RC: 0 (successful execution)
- Database connection established
- Parameter processing completed
- Archival process initiated

### 🔍 Next Steps:
- Run inspection queries to verify actual archival impact
- Analyze why enhanced version failed
- Implement proper business day logic
- Ensure emergency run safety

---
**Analysis Date**: August 13, 2025  
**Based on**: Original source files and successful execution  
**Execution Time**: August 13, 2025 21:41:42  
**Status**: Original works, Enhancement needs debugging