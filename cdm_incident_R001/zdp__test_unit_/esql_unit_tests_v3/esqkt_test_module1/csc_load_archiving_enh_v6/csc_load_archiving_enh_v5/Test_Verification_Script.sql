-- Test Verification Script for Order of Execution Fix
-- Run this script before and after the tpldauftr execution to verify fix

-- 1. Clear any existing test data
DELETE FROM kdauftrag WHERE kdauftragnr IN (880100005, 880100006);

-- 2. Insert test orders (same as original test)
INSERT INTO kdauftrag (
    kdauftragnr,
    kdauftragstat,
    datumauslieferung,
    vertriebszentrumnr,
    herkunftfiliale,
    kzrestored
) VALUES
(880100006, '0000000001000000', 20250817, 5, 1, '0'),  -- August 17 (Sunday)
(880100005, '0000000001000000', 20250816, 5, 1, '0');  -- August 16 (Saturday)

-- 3. Verify test data before script execution
SELECT 
    kdauftragnr,
    datumauslieferung,
    CASE 
        WHEN datumauslieferung = 20250817 THEN 'Sunday Aug 17'
        WHEN datumauslieferung = 20250816 THEN 'Saturday Aug 16'  
        ELSE 'Other date'
    END as day_description,
    vertriebszentrumnr
FROM kdauftrag 
WHERE kdauftragnr IN (880100005, 880100006) 
ORDER BY kdauftragnr;

-- Expected result: 2 rows showing both orders

-- 4. After running: ./tpldauftr.sh -dg -start -fil 5
-- Run this query to verify the fix worked:

SELECT 
    COUNT(*) as remaining_orders,
    'Expected: 2 orders (both should be kept because they are after Friday Aug 15)' as expected_result
FROM kdauftrag 
WHERE kdauftragnr IN (880100005, 880100006);

-- 5. Detailed verification - show which orders remain
SELECT 
    kdauftragnr,
    datumauslieferung,
    CASE 
        WHEN datumauslieferung = 20250817 THEN 'Sunday Aug 17 - SHOULD BE KEPT (> Friday Aug 15)'
        WHEN datumauslieferung = 20250816 THEN 'Saturday Aug 16 - SHOULD BE KEPT (> Friday Aug 15)'  
        ELSE 'Other date'
    END as status_explanation
FROM kdauftrag 
WHERE kdauftragnr IN (880100005, 880100006) 
ORDER BY kdauftragnr;

-- Expected after fix: Both orders should remain because:
-- - Cutoff date: Friday August 15, 2025 (20250815)
-- - Order 880100005: August 16 (20250816) > August 15 → KEEP
-- - Order 880100006: August 17 (20250817) > August 15 → KEEP

-- Success criteria:
-- Before fix: 0 orders remaining (both incorrectly archived)
-- After fix: 2 orders remaining (both correctly kept)