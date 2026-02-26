
-- =====================================================================================
-- COMPREHENSIVE RISK VALIDATION QUERY
-- Identifies all orders at risk across different execution scenarios
-- =====================================================================================

SELECT 
    'RISK_ANALYSIS' as analysis_type,
    filialnr as branch_number,
    
    -- Branch risk classification
    CASE filialnr
        WHEN 5 THEN 'DEV_BRANCH_HIGH_RISK'
        WHEN 6 THEN 'HIGH_VOLUME_HIGH_RISK' 
        WHEN 31 THEN 'HIGH_VOLUME_HIGH_RISK'
        WHEN 44 THEN 'DEV_BRANCH_HIGH_RISK'
        WHEN 57 THEN 'COMPLEX_ORDERS_HIGH_RISK'
        ELSE 'STANDARD_RISK'
    END as branch_risk_profile,
    
    kdauftragnr,
    kdauftragstat,
    datumauslieferung,
    
    -- Risk assessment by mode
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'SAFE_INVALID_PRIMARY'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'SAFE_ERFASST_PROTECTION'
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' THEN 'HIGH_RISK_H24_IMMEDIATE'
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 'HIGH_RISK_H24_CANCEL'
        WHEN datumauslieferung <= 20250815 THEN 'MEDIUM_RISK_DATE_DEPENDENT'
        ELSE 'LOW_RISK_FUTURE_DATE'
    END as risk_classification,
    
    -- Specific risk scenarios
    CASE 
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' AND SUBSTR(kdauftragstat, 10, 1) = '1'
        THEN 'CRITICAL: Both H24 flags active - immediate archive in H24 mode'
        
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1'
        THEN 'HIGH: Complete flag active - could be partial completion'
        
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' 
        THEN 'HIGH: Cancel flag active - immediate archive in H24 mode'
        
        WHEN SUBSTR(kdauftragstat, 5, 1) = '0' AND datumauslieferung <= 20250815
        THEN 'MEDIUM: Past erfasst but within date range - business state unknown'
        
        ELSE 'LOW: Standard protections apply'
    END as risk_detail,
    
    -- Mode-specific predictions
    'SAFE' as standard_mode_prediction,
    
    CASE WHEN SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1'
         THEN 'IMMEDIATE_ARCHIVE' 
         ELSE 'DATE_DEPENDENT' 
    END as h24_mode_prediction,
    
    -- Business day risk
    CASE 
        WHEN EXTRACT(DOW FROM CURRENT_DATE) IN (0, 6) THEN 'WEEKEND_EXECUTION_RISK'
        WHEN EXTRACT(HOUR FROM CURRENT_TIME) BETWEEN 7 AND 9 THEN 'TRANSITION_PERIOD_RISK'
        WHEN EXTRACT(HOUR FROM CURRENT_TIME) BETWEEN 17 AND 19 THEN 'END_OF_DAY_RISK'
        ELSE 'NORMAL_HOURS'
    END as timing_risk

FROM kdauftrag
WHERE filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
  AND datumauslieferung > 0
  
  -- Focus on at-risk orders
  AND (
      SUBSTR(kdauftragstat, 9, 1) = '1'     -- H24 complete flag
      OR SUBSTR(kdauftragstat, 10, 1) = '1' -- H24 cancel flag  
      OR (SUBSTR(kdauftragstat, 5, 1) = '0' AND datumauslieferung <= 20250815) -- Past erfasst + in date range
  )
  
ORDER BY 
    CASE 
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' AND SUBSTR(kdauftragstat, 10, 1) = '1' THEN 1
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' THEN 2
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 3
        ELSE 4
    END,
    filialnr,
    kdauftragnr;

-- Risk summary by branch
SELECT 
    'RISK_SUMMARY_BY_BRANCH' as analysis_type,
    filialnr,
    COUNT(*) as total_at_risk_orders,
    
    SUM(CASE WHEN SUBSTR(kdauftragstat, 9, 1) = '1' THEN 1 ELSE 0 END) as h24_complete_risk,
    SUM(CASE WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 1 ELSE 0 END) as h24_cancel_risk,
    SUM(CASE WHEN SUBSTR(kdauftragstat, 9, 1) = '1' AND SUBSTR(kdauftragstat, 10, 1) = '1' THEN 1 ELSE 0 END) as both_flags_critical_risk,
    
    CASE 
        WHEN SUM(CASE WHEN SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1' THEN 1 ELSE 0 END) > 100
        THEN 'CRITICAL_RISK_BRANCH'
        WHEN SUM(CASE WHEN SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1' THEN 1 ELSE 0 END) > 50  
        THEN 'HIGH_RISK_BRANCH'
        WHEN SUM(CASE WHEN SUBSTR(kdauftragstat, 9, 1) = '1' OR SUBSTR(kdauftragstat, 10, 1) = '1' THEN 1 ELSE 0 END) > 10
        THEN 'MEDIUM_RISK_BRANCH'
        ELSE 'LOW_RISK_BRANCH'
    END as branch_risk_level

FROM kdauftrag
WHERE filialnr IN (5,59,40,6,31,33,9,108,46,70,78,99,56,21,44,57,51,52,58,54,81,82)
  AND datumauslieferung > 0
  AND (
      SUBSTR(kdauftragstat, 9, 1) = '1'
      OR SUBSTR(kdauftragstat, 10, 1) = '1' 
      OR (SUBSTR(kdauftragstat, 5, 1) = '0' AND datumauslieferung <= 20250815)
  )
GROUP BY filialnr
ORDER BY total_at_risk_orders DESC;
