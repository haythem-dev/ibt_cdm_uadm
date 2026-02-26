
-- =====================================================================================
-- TPLDAUFTR Orders Ready for Archiving Detection Query
-- Incorporates CPR Business Day Enhancement Logic
-- =====================================================================================

-- Check current time and calculate business day cutoff
SELECT 
    CASE 
        WHEN CURRENT::DATETIME::TIME < '06:00:00' THEN 'EMERGENCY_MODE'
        ELSE 'NORMAL_MODE'
    END as archive_mode,
    
    CASE 
        WHEN CURRENT::DATETIME::TIME < '06:00:00' THEN 
            -- Emergency mode: use previous business day only
            CASE 
                WHEN WEEKDAY(CURRENT::DATE) = 1 THEN CURRENT::DATE - 3 UNITS DAY  -- Monday -> Friday
                WHEN WEEKDAY(CURRENT::DATE) = 0 THEN CURRENT::DATE - 2 UNITS DAY  -- Sunday -> Friday  
                ELSE CURRENT::DATE - 1 UNITS DAY                                  -- Other days -> previous day
            END
        ELSE 
            -- Normal mode: use current business day
            CASE 
                WHEN WEEKDAY(CURRENT::DATE) = 1 THEN CURRENT::DATE - 3 UNITS DAY  -- Monday -> Friday
                WHEN WEEKDAY(CURRENT::DATE) = 0 THEN CURRENT::DATE - 2 UNITS DAY  -- Sunday -> Friday
                ELSE CURRENT::DATE                                                -- Other days -> current day
            END
    END as business_day_cutoff,
    
    CURRENT::DATETIME as query_timestamp
FROM systables WHERE tabname = 'kdauftrag'
LIMIT 1;

-- =====================================================================================
-- Main Query: Orders Ready for Archiving
-- =====================================================================================

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
        END as cutoff_date
)

SELECT 
    -- Order Identification
    k.kdauftragnr as order_number,
    k.vertriebszentrumnr as branch_number,
    k.datumauslieferung::DATE as delivery_date,
    k.kdauftragstat as order_status,
    
    -- Business Day Logic Validation
    bdc.cutoff_date as business_day_cutoff,
    CASE 
        WHEN k.datumauslieferung::DATE <= bdc.cutoff_date THEN 'READY_FOR_ARCHIVE'
        ELSE 'NOT_READY'
    END as archive_eligibility,
    
    -- Additional Order Details
    k.kundennr as customer_number,
    k.datumerstellung::DATE as creation_date,
    k.datumauslieferung as delivery_datetime,
    
    -- Archive Status Check
    CASE 
        WHEN EXISTS (
            SELECT 1 FROM akdauftrag a 
            WHERE a.kdauftragnr = k.kdauftragnr
        ) THEN 'ALREADY_ARCHIVED'
        ELSE 'NOT_ARCHIVED'
    END as current_archive_status,
    
    -- Risk Assessment
    CASE 
        WHEN k.datumauslieferung::DATE = CURRENT::DATE THEN 'HIGH_RISK_SAME_DAY'
        WHEN k.datumauslieferung::DATE = CURRENT::DATE - 1 UNITS DAY THEN 'MEDIUM_RISK_YESTERDAY'
        ELSE 'LOW_RISK_OLDER'
    END as business_blindness_risk

FROM kdauftrag k
CROSS JOIN business_day_calc bdc

WHERE 
    -- Core archiving criteria
    k.vertriebszentrumnr = 50  -- Replace with your branch number
    AND k.kdauftragstat = '0000'  -- Completed orders only
    AND k.datumauslieferung IS NOT NULL
    AND k.datumauslieferung::DATE <= bdc.cutoff_date
    
    -- Prevent duplicate archiving
    AND NOT EXISTS (
        SELECT 1 FROM akdauftrag a 
        WHERE a.kdauftragnr = k.kdauftragnr
    )
    
    -- Additional safety checks
    AND k.datumauslieferung > 0

ORDER BY 
    archive_eligibility DESC,
    business_blindness_risk ASC,
    k.datumauslieferung ASC,
    k.kdauftragnr ASC;

-- =====================================================================================
-- Summary Statistics Query
-- =====================================================================================

WITH business_day_calc AS (
    SELECT 
        CASE 
            WHEN CURRENT::DATETIME::TIME < '06:00:00' THEN 
                CASE 
                    WHEN WEEKDAY(CURRENT::DATE) = 1 THEN CURRENT::DATE - 3 UNITS DAY  
                    WHEN WEEKDAY(CURRENT::DATE) = 0 THEN CURRENT::DATE - 2 UNITS DAY  
                    ELSE CURRENT::DATE - 1 UNITS DAY                                  
                END
            ELSE 
                CASE 
                    WHEN WEEKDAY(CURRENT::DATE) = 1 THEN CURRENT::DATE - 3 UNITS DAY  
                    WHEN WEEKDAY(CURRENT::DATE) = 0 THEN CURRENT::DATE - 2 UNITS DAY  
                    ELSE CURRENT::DATE                                                
                END
        END as cutoff_date
)

SELECT 
    'ARCHIVING_SUMMARY' as report_type,
    bdc.cutoff_date as business_day_cutoff,
    COUNT(*) as total_orders_ready,
    COUNT(CASE WHEN k.datumauslieferung::DATE = CURRENT::DATE THEN 1 END) as same_day_orders,
    COUNT(CASE WHEN k.datumauslieferung::DATE = CURRENT::DATE - 1 UNITS DAY THEN 1 END) as yesterday_orders,
    COUNT(CASE WHEN k.datumauslieferung::DATE < CURRENT::DATE - 1 UNITS DAY THEN 1 END) as older_orders,
    MIN(k.datumauslieferung::DATE) as oldest_order_date,
    MAX(k.datumauslieferung::DATE) as newest_order_date,
    
    -- Business blindness risk assessment
    CASE 
        WHEN COUNT(CASE WHEN k.datumauslieferung::DATE = CURRENT::DATE THEN 1 END) > 0 
        THEN 'HIGH_RISK: Same day orders detected'
        WHEN COUNT(CASE WHEN k.datumauslieferung::DATE = CURRENT::DATE - 1 UNITS DAY THEN 1 END) > 0 
        THEN 'MEDIUM_RISK: Yesterday orders detected'
        ELSE 'LOW_RISK: Only older orders'
    END as risk_assessment

FROM kdauftrag k
CROSS JOIN business_day_calc bdc

WHERE 
    k.vertriebszentrumnr = 50
    AND k.kdauftragstat = '0000'
    AND k.datumauslieferung IS NOT NULL
    AND k.datumauslieferung::DATE <= bdc.cutoff_date
    AND NOT EXISTS (
        SELECT 1 FROM akdauftrag a 
        WHERE a.kdauftragnr = k.kdauftragnr
    )

GROUP BY bdc.cutoff_date;
