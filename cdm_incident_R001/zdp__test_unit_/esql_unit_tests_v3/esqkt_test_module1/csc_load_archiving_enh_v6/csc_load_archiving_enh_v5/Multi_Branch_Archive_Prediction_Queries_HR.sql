
-- =============================================================================
-- MULTI-BRANCH ARCHIVE PREDICTION QUERIES - CROATIA (HR)
-- Based on actual tpldauftr.cxx script logic analysis
-- Extended to cover all HR branches with per-branch results
-- =============================================================================
-- HR Branch List: 71,72,73,74,75,77,99
-- Cutoff Date: 20250815 (example)
-- Analysis Date: January 16, 2025
-- =============================================================================

-- =============================================================================
-- SCENARIO 1: STANDARD MODE (No H24 flag) - HR BRANCHES
-- Command: ./tpldauftr.sh -start -fil <branch> -del 30
-- Logic: Date-based archiving with standard status validation
-- =============================================================================

SELECT 
    'STANDARD MODE HR' as execution_mode,
    'Date-based archiving (no H24 flags) - Croatia' as description,
    filialnr as branch_number,
    
    -- Branch name lookup for Croatia
    CASE filialnr
        WHEN 71 THEN 'ZDEV21 Zagreb'
        WHEN 72 THEN 'ZDEV21 Osijek'
        WHEN 73 THEN 'ZDEV21 Split'
        WHEN 74 THEN 'ZDEV21 Rijeka'
        WHEN 75 THEN 'ZDEV21 Varazdin'
        WHEN 77 THEN 'ZDEV21 Velica Gorica'
        WHEN 99 THEN 'ZDEV21 Pseudofiliale zur Ueberwachung - Kroatia'
        ELSE 'Unknown HR Branch'
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
    filialnr IN (71,72,73,74,75,77,99)
    AND datumauslieferung > 0
    AND datumauslieferung <= 20250815  -- Business day cutoff
    -- Script validation conditions:
    AND SUBSTR(kdauftragstat, 1, 4) = '0000'  -- Primary status must be "0000"
    AND SUBSTR(kdauftragstat, 5, 1) != '1'    -- Not "Erfasst" 
    -- NO H24 flag conditions in standard mode
    
GROUP BY filialnr, kdauftragstat
ORDER BY filialnr, kdauftragstat;

-- =============================================================================
-- SCENARIO 2: H24 MODE (Status-flag-based archiving) - HR BRANCHES
-- Command: ./tpldauftr.sh -start -fil <branch> -del 30 -h24
-- Logic: H24 flags override date logic for immediate archiving
-- =============================================================================

SELECT 
    'H24 MODE HR' as execution_mode,
    'Status-flag-based archiving (H24 flags active) - Croatia' as description,
    filialnr as branch_number,
    
    -- Branch name lookup for Croatia
    CASE filialnr
        WHEN 71 THEN 'ZDEV21 Zagreb'
        WHEN 72 THEN 'ZDEV21 Osijek'
        WHEN 73 THEN 'ZDEV21 Split'
        WHEN 74 THEN 'ZDEV21 Rijeka'
        WHEN 75 THEN 'ZDEV21 Varazdin'
        WHEN 77 THEN 'ZDEV21 Velica Gorica'
        WHEN 99 THEN 'ZDEV21 Pseudofiliale zur Ueberwachung - Kroatia'
        ELSE 'Unknown HR Branch'
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
    filialnr IN (71,72,73,74,75,77,99)
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
-- COMPREHENSIVE SUMMARY QUERY - HR BRANCHES
-- Compare all execution modes side by side per branch
-- =============================================================================

SELECT 
    'EXECUTION MODE COMPARISON HR' as analysis_type,
    filialnr as branch_number,
    
    -- Branch name lookup for Croatia
    CASE filialnr
        WHEN 71 THEN 'ZDEV21 Zagreb'
        WHEN 72 THEN 'ZDEV21 Osijek'
        WHEN 73 THEN 'ZDEV21 Split'
        WHEN 74 THEN 'ZDEV21 Rijeka'
        WHEN 75 THEN 'ZDEV21 Varazdin'
        WHEN 77 THEN 'ZDEV21 Velica Gorica'
        WHEN 99 THEN 'ZDEV21 Pseudofiliale zur Ueberwachung - Kroatia'
        ELSE 'Unknown HR Branch'
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
    
    -- Rejected by Erfasst flag
    (SELECT COUNT(*) FROM kdauftrag k4
     WHERE k4.filialnr = k.filialnr AND k4.datumauslieferung > 0 AND k4.datumauslieferung <= 20250815
     AND SUBSTR(k4.kdauftragstat, 5, 1) = '1') as rejected_erfasst_count
     
FROM (SELECT DISTINCT filialnr FROM kdauftrag 
      WHERE filialnr IN (71,72,73,74,75,77,99)) k
ORDER BY filialnr;
