
-- =====================================================================================
-- Comprehensive TPLDAUFTR Archiving Analysis Query
-- Based on actual kdauftragstat values found in the database
-- =====================================================================================

-- 1. Order Status Distribution Analysis
SELECT 
    'Status Distribution' as analysis_type,
    kdauftragstat,
    COUNT(*) as order_count,
    ROUND((COUNT(*) * 100.0 / (SELECT COUNT(*) FROM kdauftrag)), 2) as percentage
FROM kdauftrag 
GROUP BY kdauftragstat
ORDER BY order_count DESC;

-- 2. Branch 50 Specific Analysis (your target branch)
SELECT 
    'Branch 50 Analysis' as analysis_type,
    vertriebszentrumnr as branch,
    kdauftragstat as status,
    COUNT(*) as order_count,
    MIN(datumauslieferung) as earliest_delivery,
    MAX(datumauslieferung) as latest_delivery
FROM kdauftrag 
WHERE vertriebszentrumnr = 50
GROUP BY vertriebszentrumnr, kdauftragstat
ORDER BY order_count DESC;

-- 3. Determine Archiving Criteria Based on Actual Status Values
-- Since TPLDAUFTR looks for kdauftragstat = '0000', let's check what that translates to
SELECT 
    'Archive Eligibility Check' as check_type,
    kdauftragstat,
    COUNT(*) as total_orders,
    COUNT(CASE WHEN datumauslieferung IS NOT NULL THEN 1 END) as with_delivery_date,
    COUNT(CASE WHEN datumauslieferung <= CURRENT - 35 UNITS DAY THEN 1 END) as retention_eligible,
    COUNT(CASE WHEN vertriebszentrumnr = 50 THEN 1 END) as branch_50_orders,
    -- Check if these might be the "closed" orders TPLDAUFTR is looking for
    CASE 
        WHEN kdauftragstat LIKE '%0000%' THEN 'POTENTIAL_ARCHIVE_CANDIDATE'
        ELSE 'NOT_ARCHIVE_CANDIDATE'
    END as archive_potential
FROM kdauftrag 
GROUP BY kdauftragstat
ORDER BY total_orders DESC;

-- 4. Business Day Logic Analysis (based on your CPR enhancement)
WITH business_day_calc AS (
    SELECT 
        CASE 
            WHEN CURRENT::DATETIME::TIME < '06:00:00' THEN 
                -- Emergency mode: use previous business day only
                CASE 
                    WHEN WEEKDAY(CURRENT::DATE) = 1 THEN CURRENT::DATE - 3 UNITS DAY  
                    WHEN WEEKDAY(CURRENT::DATE) = 0 THEN CURRENT::DATE - 2 UNITS DAY  
                    ELSE CURRENT::DATE - 1 UNITS DAY                                  
                END
            ELSE 
                -- Normal mode: use current business day
                CASE 
                    WHEN WEEKDAY(CURRENT::DATE) = 1 THEN CURRENT::DATE - 3 UNITS DAY  
                    WHEN WEEKDAY(CURRENT::DATE) = 0 THEN CURRENT::DATE - 2 UNITS DAY  
                    ELSE CURRENT::DATE                                                
                END
        END as cutoff_date,
        CURRENT::DATE - 35 UNITS DAY as retention_cutoff
)

SELECT 
    'Potential Archive Candidates' as analysis_type,
    k.kdauftragnr as order_number,
    k.vertriebszentrumnr as branch,
    k.kdauftragstat as status,
    k.datumauslieferung::DATE as delivery_date,
    bdc.cutoff_date as business_day_cutoff,
    bdc.retention_cutoff as retention_policy_cutoff,
    
    -- Determine eligibility based on different criteria
    CASE 
        WHEN k.vertriebszentrumnr = 50 
             AND k.datumauslieferung IS NOT NULL 
             AND k.datumauslieferung::DATE <= bdc.retention_cutoff 
        THEN 'RETENTION_POLICY_ELIGIBLE'
        
        WHEN k.vertriebszentrumnr = 50 
             AND k.datumauslieferung IS NOT NULL 
             AND k.datumauslieferung::DATE <= bdc.cutoff_date 
        THEN 'BUSINESS_DAY_ELIGIBLE'
        
        ELSE 'NOT_ELIGIBLE'
    END as eligibility_status,
    
    -- Check if already archived
    CASE 
        WHEN EXISTS (SELECT 1 FROM akdauftrag a WHERE a.kdauftragnr = k.kdauftragnr) 
        THEN 'ALREADY_ARCHIVED' 
        ELSE 'NOT_ARCHIVED' 
    END as archive_status

FROM kdauftrag k
CROSS JOIN business_day_calc bdc
WHERE k.vertriebszentrumnr = 50
  AND k.datumauslieferung IS NOT NULL
ORDER BY k.datumauslieferung DESC, k.kdauftragnr;

-- 5. Summary Report
SELECT 
    'Archive Summary' as report_type,
    COUNT(*) as total_orders_branch_50,
    COUNT(CASE WHEN kdauftragstat = '0000000001000000' THEN 1 END) as status_type_1,
    COUNT(CASE WHEN kdauftragstat = '0000100000000000' THEN 1 END) as status_type_2,  
    COUNT(CASE WHEN kdauftragstat = '0000100000000001' THEN 1 END) as status_type_3,
    COUNT(CASE WHEN datumauslieferung <= CURRENT - 35 UNITS DAY THEN 1 END) as retention_eligible,
    COUNT(CASE WHEN datumauslieferung IS NULL THEN 1 END) as missing_delivery_date,
    
    -- Potential archiving impact
    'Status analysis needed to determine which represents closed orders' as next_step
    
FROM kdauftrag 
WHERE vertriebszentrumnr = 50;

-- 6. Archive Table Check
SELECT 
    'Archive Table Status' as check_type,
    COUNT(*) as total_archived_orders,
    COUNT(CASE WHEN datumarchivierung::DATE = CURRENT::DATE THEN 1 END) as archived_today,
    COUNT(CASE WHEN datumarchivierung >= CURRENT - 1 UNITS HOUR THEN 1 END) as archived_last_hour,
    MIN(datumarchivierung) as first_archive,
    MAX(datumarchivierung) as last_archive
FROM akdauftrag;
