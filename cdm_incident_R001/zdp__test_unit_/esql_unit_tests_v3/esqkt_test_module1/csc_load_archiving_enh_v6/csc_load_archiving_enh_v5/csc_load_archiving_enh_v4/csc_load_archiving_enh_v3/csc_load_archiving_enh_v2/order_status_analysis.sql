
-- =====================================================================================
-- Order Status Analysis - How TPLDAUFTR Interprets Order States
-- Focus: Understanding status field interpretation, not business day logic
-- =====================================================================================

-- 1. Decode the kdauftragstat field positions
SELECT 
    'Status Field Analysis' as analysis_type,
    kdauftragstat,
    LENGTH(kdauftragstat) as status_length,
    -- Extract specific positions that TPLDAUFTR checks
    SUBSTR(kdauftragstat, 9, 1) as position_9_value,
    SUBSTR(kdauftragstat, 10, 1) as position_10_value,
    COUNT(*) as order_count,
    
    -- Determine archiving eligibility based on original script logic
    CASE 
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1' 
        THEN 'ELIGIBLE_FOR_ARCHIVING' 
        ELSE 'NOT_ELIGIBLE_FOR_ARCHIVING'
    END as archive_eligibility
    
FROM kdauftrag 
WHERE vertriebszentrumnr = 50
GROUP BY kdauftragstat
ORDER BY order_count DESC;

-- 2. Status interpretation mapping
SELECT 
    'Status Interpretation' as analysis_type,
    kdauftragstat,
    COUNT(*) as total_orders,
    
    -- Business meaning interpretation (based on common ERP patterns)
    CASE kdauftragstat
        WHEN '0000000001000000' THEN 'COMPLETED_ORDERS'
        WHEN '0000100000000000' THEN 'PROCESSING_ORDERS' 
        WHEN '0000100000000001' THEN 'PROCESSED_WITH_FLAG'
        ELSE 'UNKNOWN_STATUS'
    END as likely_business_meaning,
    
    -- Archive eligibility per TPLDAUFTR logic
    CASE 
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1' 
        THEN 'YES' 
        ELSE 'NO'
    END as will_be_archived
    
FROM kdauftrag 
WHERE vertriebszentrumnr = 50
GROUP BY kdauftragstat
ORDER BY total_orders DESC;

-- 3. Check what's actually in the archive to understand past behavior
SELECT 
    'Archive Pattern Analysis' as analysis_type,
    'Understanding what status orders actually got archived' as purpose,
    COUNT(*) as total_archived_orders,
    MAX(datumarchivierung) as last_archive_date
FROM akdauftrag 
WHERE vertriebszentrumnr = 50;
