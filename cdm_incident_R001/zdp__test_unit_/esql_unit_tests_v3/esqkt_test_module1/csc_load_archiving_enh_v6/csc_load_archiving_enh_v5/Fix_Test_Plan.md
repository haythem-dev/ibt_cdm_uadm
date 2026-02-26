# Fix Test Plan - Order of Execution Correction

## Problem Identified
The business day enhancement was calculating the cutoff date **AFTER** SQL queries were already prepared, causing the filter to use `datumauslieferung <= 0` instead of the correct cutoff date.

## Fix Applied
**Before (Wrong Order):**
1. `PrepareAll(iFilialNr)` - builds SQL with `g_business_day_cutoff_date = 0`
2. `calculate_business_day_cutoff()` - sets `g_business_day_cutoff_date = 20250815`

**After (Correct Order):**
1. `calculate_business_day_cutoff()` - sets `g_business_day_cutoff_date = 20250815`
2. `PrepareAll(iFilialNr)` - builds SQL with correct cutoff date

## Test Scenario
**Test Date:** Sunday, August 17, 2025 at 17:52 (within business hours 8-18)
**Expected Cutoff:** Friday, August 15, 2025 (`20250815`)

**Test Orders:**
- Order 1: `kdauftragnr=880100006`, `datumauslieferung=20250817` (August 17)
- Order 2: `kdauftragnr=880100005`, `datumauslieferung=20250816` (August 16)

## Expected Results After Fix

### Weekend Logic Calculation (Should Remain Same)
```
Current Date: 2025-08-17 (Sunday)
Current time 17:52 is within business hours (08:00-18:00)
Strategy: Archive PREVIOUS business day
Initial target date: 2025-08-16 (Saturday)  
WEEKEND ROLLBACK: Target is Saturday, rolling back 1 day to Friday
Final cutoff date: 2025-08-15 (Friday) = 20250815
```

### SQL Query Generation (Should Change)
**Before Fix:**
```sql
SELECT ... FROM kdauftrag WHERE vertriebszentrumnr=5 and datumauslieferung <= 0
-- This archived everything because 0 means "archive all dates"
```

**After Fix:**
```sql
SELECT ... FROM kdauftrag WHERE vertriebszentrumnr=5 and datumauslieferung <= 20250815
-- This only archives orders up to August 15 (Friday)
```

### Archiving Behavior (Should Change)
**Before Fix:**
- Order from August 17 → Archived (WRONG - should be kept)
- Order from August 16 → Archived (WRONG - should be kept)
- Total archived: 2 orders

**After Fix:**
- Order from August 17 (`20250817`) > cutoff (`20250815`) → **Kept** ✅
- Order from August 16 (`20250816`) > cutoff (`20250815`) → **Kept** ✅  
- Total archived: 0 orders

### Expected Log Changes
**New logs should show:**
```
CPR LOG: Preparing SQL queries with business day cutoff: 20250815
CPR LOG: Applied business day filter to subquery: datumauslieferung <= 20250815
CPR LOG: Total Orders Archived: 0 (via g_total_orders_archived counter)
```

## Test Commands
```bash
# Re-insert test data
INSERT INTO kdauftrag (kdauftragnr, kdauftragstat, datumauslieferung, vertriebszentrumnr, herkunftfiliale, kzrestored) 
VALUES (880100006, '0000000001000000', 20250817, 5, 1, '0'),
       (880100005, '0000000001000000', 20250816, 5, 1, '0');

# Run fixed script  
./tpldauftr.sh -dg -start -fil 5

# Verify orders still exist (should be 2 orders)
SELECT COUNT(*) FROM kdauftrag WHERE kdauftragnr IN (880100005, 880100006);
```

## Success Criteria
1. ✅ Weekend logic calculation remains correct
2. ✅ SQL queries built with correct cutoff date (`<= 20250815`)
3. ✅ Orders with dates after Friday (Aug 15) are **NOT** archived
4. ✅ Total archived count shows 0 orders
5. ✅ Both test orders remain in database after script execution

This fix ensures the business day enhancement works as designed: respecting weekend rollback rules while properly filtering records based on the calculated cutoff date.