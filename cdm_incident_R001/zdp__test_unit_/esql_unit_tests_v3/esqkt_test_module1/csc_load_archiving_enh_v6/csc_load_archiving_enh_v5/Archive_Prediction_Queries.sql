-- =============================================================================
-- ENHANCED ARCHIVE PREDICTION QUERIES
-- Based on actual tpldauftr.cxx script logic analysis
-- =============================================================================
-- Branch: 44 (example)
-- Cutoff Date: 20250815 (example)
-- Analysis Date: January 16, 2025
-- =============================================================================

-- =============================================================================
-- SCENARIO 1: STANDARD MODE (No H24 flag)
-- Command: ./tpldauftr.sh -start -fil 44 -del 30
-- Logic: Date-based archiving with standard status validation
-- =============================================================================

SELECT 
    'STANDARD MODE' as execution_mode,
    'Date-based archiving (no H24 flags)' as description,
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    -- Status breakdown for analysis
    SUBSTR(kdauftragstat, 1, 4) as primary_status,
    SUBSTR(kdauftragstat, 5, 1) as erfasst_flag,
    SUBSTR(kdauftragstat, 9, 1) as h24_complete_flag,
    SUBSTR(kdauftragstat, 10, 1) as h24_cancel_flag,
    
    -- Reason for archiving/rejection
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        ELSE 'ELIGIBLE: Standard Date Logic Applied'
    END as archive_reason
    
FROM kdauftrag
WHERE 
    filialnr = 44
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250815  -- Business day cutoff
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'  -- Primary status must be "0000"
    AND SUBSTR(kdauftragstat, 5, 1) != '1'    -- Not "Erfasst" 
    -- NO H24 flag conditions in standard mode
    
GROUP BY kdauftragstat
ORDER BY kdauftragstat;

-- =============================================================================
-- SCENARIO 2: H24 MODE (Status-flag-based archiving)
-- Command: ./tpldauftr.sh -start -fil 44 -del 30 -h24
-- Logic: H24 flags override date logic for immediate archiving
-- =============================================================================

SELECT 
    'H24 MODE' as execution_mode,
    'Status-flag-based archiving (H24 flags active)' as description,
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    -- Status breakdown
    SUBSTR(kdauftragstat, 1, 4) as primary_status,
    SUBSTR(kdauftragstat, 5, 1) as erfasst_flag,
    SUBSTR(kdauftragstat, 9, 1) as h24_complete_flag,
    SUBSTR(kdauftragstat, 10, 1) as h24_cancel_flag,
    
    -- H24 specific reason
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' THEN 'H24 IMMEDIATE: Complete Flag Active'
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 'H24 IMMEDIATE: Cancel Flag Active'
        ELSE 'H24 DATE DEPENDENT: No H24 flags, falls back to date logic'
    END as h24_archive_reason
    
FROM kdauftrag
WHERE 
    filialnr = 44
    AND datumauslieferung > 0
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'  -- Primary status must be "0000"
    AND SUBSTR(kdauftragstat, 5, 1) != '1'    -- Not "Erfasst"
    -- H24 specific condition (from C++ code):
    AND (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1')
    -- Note: Date filtering still applies but H24 flags can override
    AND (
        (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') -- H24 immediate
        OR datumauslieferung <= 20250815  -- Or meets date criteria
    )
    
GROUP BY kdauftragstat
ORDER BY kdauftragstat;

-- =============================================================================
-- SCENARIO 3: BUSINESS DAY DISABLED (Original behavior)
-- Command: ./tpldauftr.sh -start -fil 44 -del 30 --business-day-mode disabled
-- Logic: Original date logic without business day filtering
-- =============================================================================

SELECT 
    'BUSINESS DAY DISABLED' as execution_mode,
    'Original date logic (no business day filtering)' as description,
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    -- Status breakdown
    SUBSTR(kdauftragstat, 1, 4) as primary_status,
    SUBSTR(kdauftragstat, 5, 1) as erfasst_flag,
    
    -- Simple reason (no H24, no business day logic)
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        ELSE 'ELIGIBLE: Original Date Logic'
    END as archive_reason
    
FROM kdauftrag
WHERE 
    filialnr = 44
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250815  -- Still uses current cutoff but no business day logic
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    
GROUP BY kdauftragstat
ORDER BY kdauftragstat;

-- =============================================================================
-- SCENARIO 4: H24 + BUSINESS DAY DISABLED (Most aggressive)
-- Command: ./tpldauftr.sh -start -fil 44 -del 30 -h24 --business-day-mode disabled
-- Logic: H24 flags + original date logic (no business day protection)
-- =============================================================================

SELECT 
    'H24 + BD DISABLED' as execution_mode,
    'H24 flags + original date logic (most aggressive)' as description,
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    -- Status breakdown
    SUBSTR(kdauftragstat, 1, 4) as primary_status,
    SUBSTR(kdauftragstat, 5, 1) as erfasst_flag,
    SUBSTR(kdauftragstat, 9, 1) as h24_complete_flag,
    SUBSTR(kdauftragstat, 10, 1) as h24_cancel_flag,
    
    -- Combined H24 + BD disabled reason
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' THEN 'H24 IMMEDIATE: Complete Flag (ignores date)'
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 'H24 IMMEDIATE: Cancel Flag (ignores date)'
        ELSE 'ORIGINAL DATE LOGIC: No H24 flags, no BD protection'
    END as archive_reason
    
FROM kdauftrag
WHERE 
    filialnr = 44
    AND datumauslieferung > 0
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    -- H24 condition OR date condition (more aggressive)
    AND (
        (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') -- H24 immediate
        OR datumauslieferung <= 20250815  -- Or meets original date criteria
    )
    
GROUP BY kdauftragstat
ORDER BY kdauftragstat;

-- =============================================================================
-- WEEKEND SCENARIOS: Saturday Execution
-- Weekend execution requires different date calculations
-- =============================================================================

-- WEEKEND SCENARIO A: Saturday Standard Mode
-- On Saturday, business day logic would typically point to Friday
SELECT 
    'WEEKEND SATURDAY STANDARD' as execution_mode,
    'Saturday execution with business day logic (Friday cutoff)' as description,
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        ELSE 'ELIGIBLE: Friday Cutoff Applied'
    END as weekend_reason
    
FROM kdauftrag
WHERE 
    filialnr = 44
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250813  -- Friday (assuming 20250815 is Sunday, Friday would be 20250813)
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    
GROUP BY kdauftragstat
ORDER BY kdauftragstat;

-- WEEKEND SCENARIO B: Saturday H24 Mode
SELECT 
    'WEEKEND SATURDAY H24' as execution_mode,
    'Saturday H24 execution (H24 flags override weekend logic)' as description,
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    SUBSTR(kdauftragstat, 9, 1) as h24_complete_flag,
    SUBSTR(kdauftragstat, 10, 1) as h24_cancel_flag,
    
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' THEN 'H24 IMMEDIATE: Complete Flag (overrides weekend)'
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 'H24 IMMEDIATE: Cancel Flag (overrides weekend)'
        ELSE 'WEEKEND DATE LOGIC: Falls back to Friday cutoff'
    END as weekend_h24_reason
    
FROM kdauftrag
WHERE 
    filialnr = 44
    AND datumauslieferung > 0
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    AND (
        (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') -- H24 immediate
        OR datumauslieferung <= 20250813  -- Friday cutoff for weekend
    )
    
GROUP BY kdauftragstat
ORDER BY kdauftragstat;

-- WEEKEND SCENARIO C: Sunday Execution
-- On Sunday, business day logic would point to Friday
SELECT 
    'WEEKEND SUNDAY STANDARD' as execution_mode,
    'Sunday execution with business day logic (Friday cutoff)' as description,
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        ELSE 'ELIGIBLE: Friday Cutoff Applied'
    END as sunday_reason
    
FROM kdauftrag
WHERE 
    filialnr = 44
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250813  -- Friday (2 days back from Sunday)
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    
GROUP BY kdauftragstat
ORDER BY kdauftragstat;

-- =============================================================================
-- COMPREHENSIVE SUMMARY QUERY
-- Compare all execution modes side by side
-- =============================================================================

SELECT 
    'EXECUTION MODE COMPARISON' as analysis_type,
    
    -- Standard Mode Count
    (SELECT COUNT(*) FROM kdauftrag 
     WHERE filialnr = 44 AND datumauslieferung > 0 AND datumauslieferung <= 20250815
     AND SUBSTR(kdauftragstat, 1, 4) = '0000' AND SUBSTR(kdauftragstat, 5, 1) != '1') as standard_mode_count,
    
    -- H24 Mode Count (immediate archives only)
    (SELECT COUNT(*) FROM kdauftrag 
     WHERE filialnr = 44 AND datumauslieferung > 0
     AND SUBSTR(kdauftragstat, 1, 4) = '0000' AND SUBSTR(kdauftragstat, 5, 1) != '1'
     AND (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1')) as h24_immediate_count,
    
    -- Total H24 eligible (immediate + date-dependent)
    (SELECT COUNT(*) FROM kdauftrag 
     WHERE filialnr = 44 AND datumauslieferung > 0
     AND SUBSTR(kdauftragstat, 1, 4) = '0000' AND SUBSTR(kdauftragstat, 5, 1) != '1'
     AND ((SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') 
          OR datumauslieferung <= 20250815)) as h24_total_count,
    
    -- Rejected by Erfasst flag
    (SELECT COUNT(*) FROM kdauftrag 
     WHERE filialnr = 44 AND datumauslieferung > 0 AND datumauslieferung <= 20250815
     AND SUBSTR(kdauftragstat, 5, 1) = '1') as rejected_erfasst_count,
    
    -- Rejected by Primary Status
    (SELECT COUNT(*) FROM kdauftrag 
     WHERE filialnr = 44 AND datumauslieferung > 0 AND datumauslieferung <= 20250815
     AND SUBSTR(kdauftragstat, 1, 4) != '0000') as rejected_primary_count,
    
    -- Weekend Friday cutoff count
    (SELECT COUNT(*) FROM kdauftrag 
     WHERE filialnr = 44 AND datumauslieferung > 0 AND datumauslieferung <= 20250813
     AND SUBSTR(kdauftragstat, 1, 4) = '0000' AND SUBSTR(kdauftragstat, 5, 1) != '1') as weekend_friday_count;

-- =============================================================================
-- VALIDATION QUERY
-- Find orders that would behave differently between modes
-- =============================================================================

SELECT 
    'BEHAVIOR DIFFERENCES' as analysis_type,
    kdauftragnr,
    kdauftragstat,
    datumauslieferung,
    
    -- Show what each mode would do
    CASE WHEN SUBSTR(kdauftragstat, 1, 4) = '0000' AND SUBSTR(kdauftragstat, 5, 1) != '1' 
              AND datumauslieferung <= 20250815
         THEN 'ARCHIVE' ELSE 'SKIP' END as standard_mode,
         
    CASE WHEN SUBSTR(kdauftragstat, 1, 4) = '0000' AND SUBSTR(kdauftragstat, 5, 1) != '1'
              AND (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1')
         THEN 'H24_IMMEDIATE'
         WHEN SUBSTR(kdauftragstat, 1, 4) = '0000' AND SUBSTR(kdauftragstat, 5, 1) != '1' 
              AND datumauslieferung <= 20250815
         THEN 'H24_DATE_LOGIC' 
         ELSE 'SKIP' END as h24_mode,
    
    -- Highlight differences
    CASE 
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1'
        THEN 'H24_ADVANTAGE: Would archive immediately in H24 mode'
        WHEN datumauslieferung > 20250815 
        THEN 'STANDARD_BLOCKED: Too recent for standard mode'
        ELSE 'SAME_BEHAVIOR'
    END as mode_difference
    
FROM kdauftrag
WHERE filialnr = 44 
  AND datumauslieferung > 0
  AND SUBSTR(kdauftragstat, 1, 4) = '0000'
  AND SUBSTR(kdauftragstat, 5, 1) != '1'
  -- Show orders where modes behave differently
  AND (
      (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') -- H24 flags
      OR datumauslieferung > 20250815 -- Beyond standard cutoff
  )
ORDER BY kdauftragstat, kdauftragnr
LIMIT 100;