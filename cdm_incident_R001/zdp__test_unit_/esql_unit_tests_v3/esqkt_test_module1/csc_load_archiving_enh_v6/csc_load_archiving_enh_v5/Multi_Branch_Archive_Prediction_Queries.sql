-- =============================================================================
-- MULTI-BRANCH ARCHIVE PREDICTION QUERIES
-- Based on actual tpldauftr.cxx script logic analysis
-- Extended to cover all branches with per-branch results
-- =============================================================================
-- Branch List: 5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82
-- Cutoff Date: 20250815 (example)
-- Analysis Date: January 16, 2025
-- =============================================================================

-- =============================================================================
-- SCENARIO 1: STANDARD MODE (No H24 flag) - ALL BRANCHES
-- Command: ./tpldauftr.sh -start -fil <branch> -del 30
-- Logic: Date-based archiving with standard status validation
-- =============================================================================

SELECT 
    'STANDARD MODE' as execution_mode,
    'Date-based archiving (no H24 flags)' as description,
    filialnr as branch_number,
    
    -- Branch name lookup (add branch names for better readability)
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
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
    filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250815  -- Business day cutoff
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'  -- Primary status must be "0000"
    AND SUBSTR(kdauftragstat, 5, 1) != '1'    -- Not "Erfasst" 
    -- NO H24 flag conditions in standard mode
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- =============================================================================
-- SCENARIO 2: H24 MODE (Status-flag-based archiving) - ALL BRANCHES
-- Command: ./tpldauftr.sh -start -fil <branch> -del 30 -h24
-- Logic: H24 flags override date logic for immediate archiving
-- =============================================================================

SELECT 
    'H24 MODE' as execution_mode,
    'Status-flag-based archiving (H24 flags active)' as description,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
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
    filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
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
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- =============================================================================
-- SCENARIO 3: BUSINESS DAY DISABLED (Original behavior) - ALL BRANCHES
-- Command: ./tpldauftr.sh -start -fil <branch> -del 30 --business-day-mode disabled
-- Logic: Original date logic without business day filtering
-- =============================================================================

SELECT 
    'BUSINESS DAY DISABLED' as execution_mode,
    'Original date logic (no business day filtering)' as description,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
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
    filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250815  -- Still uses current cutoff but no business day logic
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- =============================================================================
-- SCENARIO 4: H24 + BUSINESS DAY DISABLED (Most aggressive) - ALL BRANCHES
-- Command: ./tpldauftr.sh -start -fil <branch> -del 30 -h24 --business-day-mode disabled
-- Logic: H24 flags + original date logic (no business day protection)
-- =============================================================================

SELECT 
    'H24 + BD DISABLED' as execution_mode,
    'H24 flags + original date logic (most aggressive)' as description,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
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
    filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
    AND datumauslieferung > 0
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    -- H24 condition OR date condition (more aggressive)
    AND (
        (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') -- H24 immediate
        OR datumauslieferung <= 20250815  -- Or meets original date criteria
    )
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- =============================================================================
-- WEEKEND SCENARIOS: Saturday Execution - ALL BRANCHES
-- Weekend execution requires different date calculations
-- =============================================================================

-- WEEKEND SCENARIO A: Saturday Standard Mode - ALL BRANCHES
-- On Saturday, business day logic would typically point to Friday
SELECT 
    'WEEKEND SATURDAY STANDARD' as execution_mode,
    'Saturday execution with business day logic (Friday cutoff)' as description,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        ELSE 'ELIGIBLE: Friday Cutoff Applied'
    END as weekend_reason
    
FROM kdauftrag
WHERE 
    filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250813  -- Friday (assuming 20250815 is Sunday, Friday would be 20250813)
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- WEEKEND SCENARIO B: Saturday H24 Mode - ALL BRANCHES
SELECT 
    'WEEKEND SATURDAY H24' as execution_mode,
    'Saturday H24 execution (H24 flags override weekend logic)' as description,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
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
    filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
    AND datumauslieferung > 0
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    AND (
        (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') -- H24 immediate
        OR datumauslieferung <= 20250813  -- Friday cutoff for weekend
    )
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- WEEKEND SCENARIO C: Sunday Execution - ALL BRANCHES
-- On Sunday, business day logic would point to Friday
SELECT 
    'WEEKEND SUNDAY STANDARD' as execution_mode,
    'Sunday execution with business day logic (Friday cutoff)' as description,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
    COUNT(*) as orders_to_be_archived,
    kdauftragstat,
    
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'REJECTED: Invalid Primary Status'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'REJECTED: Erfasst Flag Active'
        ELSE 'ELIGIBLE: Friday Cutoff Applied'
    END as sunday_reason
    
FROM kdauftrag
WHERE 
    filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250813  -- Friday (2 days back from Sunday)
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'
    AND SUBSTR(kdauftragstat, 5, 1) != '1'
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- =============================================================================
-- COMPREHENSIVE SUMMARY QUERY - ALL BRANCHES
-- Compare all execution modes side by side per branch
-- =============================================================================

SELECT 
    'EXECUTION MODE COMPARISON' as analysis_type,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
    -- Standard Mode Count
    (SELECT COUNT(*) FROM kdauftrag k1
     WHERE k1.filialnr = k.filialnr AND k1.datumauslieferung > 0 AND k1.datumauslieferung <= 20250815
     AND SUBSTR(k1.kdauftragstat, 1, 4) = '0000' AND SUBSTR(k1.kdauftragstat, 5, 1) != '1') as standard_mode_count,
    
    -- H24 Mode Count (immediate archives only)
    (SELECT COUNT(*) FROM kdauftrag k2
     WHERE k2.filialnr = k.filialnr AND k2.datumauslieferung > 0
     AND SUBSTR(k2.kdauftragstat, 1, 4) = '0000' AND SUBSTR(k2.kdauftragstat, 5, 1) != '1'
     AND (SUBSTR(k2.kdauftragstat, 9, 1) = '1' OR SUBSTR(k2.kdauftragstat, 10, 1) = '1')) as h24_immediate_count,
    
    -- Total H24 eligible (immediate + date-dependent)
    (SELECT COUNT(*) FROM kdauftrag k3
     WHERE k3.filialnr = k.filialnr AND k3.datumauslieferung > 0
     AND SUBSTR(k3.kdauftragstat, 1, 4) = '0000' AND SUBSTR(k3.kdauftragstat, 5, 1) != '1'
     AND ((SUBSTR(k3.kdauftragstat, 9, 1) = '1' OR SUBSTR(k3.kdauftragstat, 10, 1) = '1') 
          OR k3.datumauslieferung <= 20250815)) as h24_total_count,
    
    -- Rejected by Erfasst flag
    (SELECT COUNT(*) FROM kdauftrag k4
     WHERE k4.filialnr = k.filialnr AND k4.datumauslieferung > 0 AND k4.datumauslieferung <= 20250815
     AND SUBSTR(k4.kdauftragstat, 5, 1) = '1') as rejected_erfasst_count,
    
    -- Rejected by Primary Status
    (SELECT COUNT(*) FROM kdauftrag k5
     WHERE k5.filialnr = k.filialnr AND k5.datumauslieferung > 0 AND k5.datumauslieferung <= 20250815
     AND SUBSTR(k5.kdauftragstat, 1, 4) != '0000') as rejected_primary_count,
    
    -- Weekend Friday cutoff count
    (SELECT COUNT(*) FROM kdauftrag k6
     WHERE k6.filialnr = k.filialnr AND k6.datumauslieferung > 0 AND k6.datumauslieferung <= 20250813
     AND SUBSTR(k6.kdauftragstat, 1, 4) = '0000' AND SUBSTR(k6.kdauftragstat, 5, 1) != '1') as weekend_friday_count
     
FROM (SELECT DISTINCT filialnr FROM kdauftrag 
      WHERE filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)) k
ORDER BY filialnr;

-- =============================================================================
-- VALIDATION QUERY - ALL BRANCHES
-- Find orders that would behave differently between modes per branch
-- =============================================================================

SELECT 
    'BEHAVIOR DIFFERENCES' as analysis_type,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
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
WHERE filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
  AND datumauslieferung > 0
  AND SUBSTR(kdauftragstat, 1, 4) = '0000'
  AND SUBSTR(kdauftragstat, 5, 1) != '1'
  -- Show orders where modes behave differently
  AND (
      (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1') -- H24 flags
      OR datumauslieferung > 20250815 -- Beyond standard cutoff
  )
ORDER BY filialnr, kdauftragstat, kdauftragnr
LIMIT 1000;

-- =============================================================================
-- BRANCH-SPECIFIC TOTALS SUMMARY
-- Quick overview of archive counts per branch for each mode
-- =============================================================================

SELECT 
    'BRANCH TOTALS OVERVIEW' as analysis_type,
    filialnr as branch_number,
    
    -- Branch name lookup
    CASE filialnr
        WHEN 5 THEN 'VZ Fuerth DEV'
        WHEN 59 THEN 'Vertriebszentrum Cottbus'
        WHEN 40 THEN 'Vertriebszentrum Bielefeld'
        WHEN 6 THEN 'Vertriebszentrum Berlin'
        WHEN 31 THEN 'Vertriebszentrum Hamburg'
        WHEN 33 THEN 'Vertriebszentrum Weser-Ems'
        WHEN 9 THEN 'Vertriebszentrum Herne'
        WHEN 108 THEN 'Vertriebszentrum Muenster'
        WHEN 46 THEN 'Vertriebszentrum Koeln'
        WHEN 70 THEN 'Vertriebszentrum Goettingen'
        WHEN 78 THEN 'Vertriebszentrum Hannover'
        WHEN 99 THEN 'Pseudofiliale zur Ueberwachung'
        WHEN 56 THEN 'Vertriebszentrum Hanau'
        WHEN 21 THEN 'Hageda Stumpf'
        WHEN 44 THEN 'DEV - Augsburg'
        WHEN 57 THEN 'Vertriebszentrum Muenchen'
        WHEN 51 THEN 'Vertriebszentrum Mannheim'
        WHEN 52 THEN 'Vertriebszentrum Neuhausen'
        WHEN 58 THEN 'Vertriebszentrum Freiburg'
        WHEN 54 THEN 'Vertriebszentrum Bad Kreuznach'
        WHEN 81 THEN 'Vertriebszentrum Leipzig'
        WHEN 82 THEN 'Vertriebszentrum Gotha'
        ELSE 'Unknown Branch'
    END as branch_name,
    
    COUNT(*) as total_orders,
    
    -- Count eligible for standard mode
    SUM(CASE WHEN SUBSTR(kdauftragstat, 1, 4) = '0000' 
                  AND SUBSTR(kdauftragstat, 5, 1) != '1'
                  AND datumauslieferung <= 20250815
             THEN 1 ELSE 0 END) as standard_eligible,
    
    -- Count eligible for H24 immediate archiving
    SUM(CASE WHEN SUBSTR(kdauftragstat, 1, 4) = '0000' 
                  AND SUBSTR(kdauftragstat, 5, 1) != '1'
                  AND (SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1')
             THEN 1 ELSE 0 END) as h24_immediate_eligible,
    
    -- Count rejected by Erfasst
    SUM(CASE WHEN SUBSTR(kdauftragstat, 5, 1) = '1' 
             THEN 1 ELSE 0 END) as rejected_erfasst,
    
    -- Count rejected by primary status
    SUM(CASE WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' 
             THEN 1 ELSE 0 END) as rejected_primary_status
             
FROM kdauftrag
WHERE filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
  AND datumauslieferung > 0
GROUP BY filialnr
ORDER BY filialnr;