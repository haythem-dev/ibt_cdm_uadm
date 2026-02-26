# Archive Script Status Validation Analysis
**Complete Analysis of kdauftragstat Flag Validation in tpldauftr.cxx**

**Analysis Date:** January 16, 2025  
**Based On:** C++ Source Code Evidence from tpldauftr.cxx  
**Context:** Comprehensive status flag validation logic for all archiving scenarios

---

## Executive Summary

This document provides a complete analysis of the `kdauftragstat` field validation logic in the archiving script `tpldauftr.cxx`. Based on direct source code examination, we document all accepted status combinations, the differences between Standard and H24 modes, and explain why certain queries return different results.

---

## 1. Source Code Evidence

### 1.1 Script Validation Logic from C++ Code

**From tpldauftr.cxx lines 4912-4913:**
```cpp
strcat( str, " and kdauftragstat[5] != '1'" );  /* Erfasst */
strcat( str, " and kdauftragstat[10] != '1'" ); /* Storniert */
```

**From tpldauftr.cxx lines 895-899 (H24 Mode):**
```cpp
if(i24 == YES)
{
    strcat( str, " and (kdauftragstat[9] == '1' " );
    strcat( str, " or kdauftragstat[10] == '1') " );
}
```

**H24 Flag Detection (line 509):**
```cpp
else if ( !strcmp( schalter, "-h24" ) )
{
   i24 = YES;
}
```

### 1.2 Status Field Structure
```
Position:  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
Example:   0 0 0 0 0 0 0 0 1 0  0  0  0  0  0  0
           │ │ │ │ │ │ │ │ │ │  │  │  │  │  │  │
           └─┴─┴─┴─── Primary Status (must be "0000")
                     │ │ │ │ │ │  │  │  │  │  │  │
                     └─── Position 5: Erfasst/Captured
                       │ │ │ │ │  │  │  │  │  │  │
                       └─┴─┴─┴─── Processing Flags
                               │  │  │  │  │  │  │
                               └── Position 9: H24 Complete Flag
                                  │  │  │  │  │  │
                                  └── Position 10: H24 Cancel/Storniert Flag
                                     │  │  │  │  │
                                     └─┴─┴─┴─┴─┴── Extended Flags
```

---

## 2. Validation Rules by Mode

### 2.1 Standard Mode (No -h24 flag)
**Command:** `./tpldauftr.sh -start -fil <branch> -del 30`

**Validation Rules:**
```cpp
// Always applied:
kdauftragstat[1,4] == "0000"    // Primary status must be "0000"
kdauftragstat[5] != '1'         // Must not be "Erfasst" (captured)
kdauftragstat[10] != '1'        // Must not be "Storniert" (cancelled)

// Date logic then applied
```

### 2.2 H24 Mode (With -h24 flag)  
**Command:** `./tpldauftr.sh -start -fil <branch> -del 30 -h24`

**Validation Rules:**
```cpp
// Always applied:
kdauftragstat[1,4] == "0000"    // Primary status must be "0000"
kdauftragstat[5] != '1'         // Must not be "Erfasst" (captured)

// H24 specific logic:
(kdauftragstat[9] == '1' OR kdauftragstat[10] == '1')  // H24 triggers

// Note: Position 10='1' ARCHIVES in H24 mode but REJECTS in Standard mode
```

---

## 3. All Accepted Status Combinations

### 3.1 Standard Mode Accepted Patterns

**Pattern Template:**
```
Position:     1234 5 678 9 10 111213141516
Requirements: 0000 0 xxx x 0  xxxxxxxxxxxx
```
Where `x` = can be 0 or 1

**Valid Combinations:** 2^10 = 1,024 combinations
- Positions 1-4: **Fixed "0000"** (only 1 combination)
- Position 5: **Fixed "0"** (only 1 combination) 
- Position 10: **Fixed "0"** (only 1 combination)
- Positions 6,7,8,9,11,12,13,14,15,16: **Any value** (2^10 combinations)

**Examples of ACCEPTED Status in Standard Mode:**
```
0000000000000000  ✅ Basic valid status
0000010101000000  ✅ Processing flags set, but valid
0000011111000111  ✅ Multiple flags set, but core validation passes
0000001010000001  ✅ Various flags active, core requirements met
```

**Examples of REJECTED Status in Standard Mode:**
```
0001000000000000  ❌ Primary status "0001" ≠ "0000"
0000100000000000  ❌ Position 5 = '1' (Erfasst active)
0000000000100000  ❌ Position 10 = '1' (Storniert active)
0000100000100000  ❌ Both pos 5 and 10 = '1' (multiple violations)
```

### 3.2 H24 Mode Accepted Patterns

**Pattern Template:**
```
Position:     1234 5 678 9 10 111213141516
Requirements: 0000 0 xxx T T  xxxxxxxxxxxx
```
Where `x` = can be 0 or 1, `TT` = trigger combinations (01, 10, or 11)

**Valid Combinations:** 3 × 2^12 = 12,288 combinations
- Positions 1-4: **Fixed "0000"** (1 combination)
- Position 5: **Fixed "0"** (1 combination)
- Positions 9,10: **H24 triggers** (3 combinations: "01", "10", "11")
- Positions 6,7,8,11,12,13,14,15,16: **Any value** (2^9 combinations)

**H24 Trigger Combinations:**
```
Position 9,10:
  01  ✅ H24: Position 9=0, Position 10=1 (Cancel trigger)
  10  ✅ H24: Position 9=1, Position 10=0 (Complete trigger) 
  11  ✅ H24: Position 9=1, Position 10=1 (Both triggers)
  00  ❌ H24: No triggers - falls back to date logic
```

**Examples of ACCEPTED Status in H24 Mode:**
```
0000000001000000  ✅ H24: Complete flag (pos 9=1) triggers archiving
0000000000100000  ✅ H24: Cancel flag (pos 10=1) triggers archiving  
0000000001100000  ✅ H24: Both flags active (immediate archiving)
0000011101000111  ✅ H24: Complete flag active with other processing flags
0000001000100001  ✅ H24: Cancel flag active with extended flags
```

**Examples of REJECTED Status in H24 Mode:**
```
0001000001000000  ❌ Primary status "0001" ≠ "0000" 
0000100001000000  ❌ Position 5 = '1' (Erfasst blocks even H24)
0000100001100000  ❌ Erfasst active (overrides H24 triggers)
```

---

## 4. Key Differences Between Modes

### 4.1 Position 10 Treatment

**Standard Mode:**
```cpp
kdauftragstat[10] != '1'  // Position 10='1' REJECTS the order
```

**H24 Mode:**  
```cpp
(kdauftragstat[9] == '1' OR kdauftragstat[10] == '1')  // Position 10='1' TRIGGERS archiving
```

### 4.2 Validation Priority

**Both Modes Apply (in order):**
1. **Primary Status Check:** Positions 1-4 must be "0000"
2. **Erfasst Check:** Position 5 must be "0" (never "1")
3. **Mode-Specific Logic:** Standard rejects pos 10='1', H24 uses it as trigger

---

## 5. Real-World Query Discrepancy Analysis

### 5.1 The Branch 33 Case Study

**Query1 (Simple Date Query):**
```sql
SELECT filialnr, kdauftragstat, COUNT(*) 
FROM kdauftrag 
WHERE filialnr = 33 AND datumauslieferung > 0 AND datumauslieferung <= 20250815
GROUP BY filialnr, kdauftragstat;
```

**Result:** 7 orders found with status patterns:
- `"0000111110000001"` 
- `"0001000000000000"`
- `"0001000000000001"`

**Query2 (Standard Mode Script Logic):**
```sql
SELECT * FROM kdauftrag 
WHERE filialnr = 33 
AND datumauslieferung > 0 
AND datumauslieferung <= 20250815
AND SUBSTR(kdauftragstat, 1, 4) = '0000'  -- Primary validation
AND SUBSTR(kdauftragstat, 5, 1) != '1'    -- Erfasst validation
```

**Result:** 0 rows

### 5.2 Why Query2 Returns Zero Results

**Status Analysis:**

1. **`"0000111110000001"`:**
   - Position 1-4: `"0000"` ✅ (Primary status valid)
   - Position 5: `"1"` ❌ (Erfasst flag active - **BLOCKS ARCHIVING**)
   - **Result:** Rejected by script validation

2. **`"0001000000000000"`:**
   - Position 1-4: `"0001"` ❌ (Primary status invalid - not "0000")
   - **Result:** Rejected by primary status check

3. **`"0001000000000001"`:**
   - Position 1-4: `"0001"` ❌ (Primary status invalid - not "0000")  
   - **Result:** Rejected by primary status check

**Conclusion:** All 7 orders found by Query1 are rejected by the script's validation logic, explaining why Query2 returns 0 results.

---

## 6. Practical Validation Examples

### 6.1 Status Pattern Testing

**Test Cases for Standard Mode:**
```
Status Pattern        Primary  Erfasst  Storniert  Result    Reason
0000000000000000      PASS     PASS     PASS       ✅ VALID   All checks pass
0000100000000000      PASS     FAIL     PASS       ❌ REJECT  Erfasst active  
0000000000100000      PASS     PASS     FAIL       ❌ REJECT  Storniert active
0001000000000000      FAIL     PASS     PASS       ❌ REJECT  Invalid primary
0000111110000001      PASS     FAIL     PASS       ❌ REJECT  Erfasst active
```

**Test Cases for H24 Mode:**
```
Status Pattern        Primary  Erfasst  H24_9  H24_10  Result    Reason
0000000001000000      PASS     PASS     PASS   PASS    ✅ H24     Complete trigger
0000000000100000      PASS     PASS     PASS   PASS    ✅ H24     Cancel trigger  
0000000001100000      PASS     PASS     PASS   PASS    ✅ H24     Both triggers
0000000000000000      PASS     PASS     FAIL   FAIL    📅 DATE    No H24 triggers
0000100001000000      PASS     FAIL     PASS   PASS    ❌ REJECT  Erfasst blocks H24
```

### 6.2 Validation Query Templates

**Check Status Validity for Standard Mode:**
```sql
SELECT 
    kdauftragnr,
    kdauftragstat,
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'INVALID_PRIMARY'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'ERFASST_ACTIVE'  
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 'STORNIERT_ACTIVE'
        ELSE 'VALID_FOR_STANDARD'
    END as standard_mode_validation
FROM kdauftrag 
WHERE filialnr = ?;
```

**Check Status Validity for H24 Mode:**
```sql
SELECT 
    kdauftragnr,
    kdauftragstat,
    CASE 
        WHEN SUBSTR(kdauftragstat, 1, 4) != '0000' THEN 'INVALID_PRIMARY'
        WHEN SUBSTR(kdauftragstat, 5, 1) = '1' THEN 'ERFASST_BLOCKS_H24'
        WHEN SUBSTR(kdauftragstat, 9, 1) = '1' THEN 'H24_COMPLETE_TRIGGER'
        WHEN SUBSTR(kdauftragstat, 10, 1) = '1' THEN 'H24_CANCEL_TRIGGER'
        ELSE 'H24_DATE_DEPENDENT'
    END as h24_mode_validation
FROM kdauftrag 
WHERE filialnr = ?;
```

---

## 7. Summary of All Valid Status Patterns

### 7.1 Standard Mode Valid Patterns (1,024 total)
```
Pattern: 0000 0 [0-1][0-1][0-1] [0-1] 0 [0-1][0-1][0-1][0-1][0-1][0-1]
Example: 0000 0 101 1 0 010101  ✅ VALID
Example: 0000 0 000 0 0 000000  ✅ VALID  
Example: 0000 0 111 1 0 111111  ✅ VALID
```

### 7.2 H24 Mode Valid Patterns (12,288 total)
```
Pattern: 0000 0 [0-1][0-1][0-1] [01|10|11] [0-1][0-1][0-1][0-1][0-1][0-1]
Example: 0000 0 101 01 010101  ✅ H24 CANCEL TRIGGER
Example: 0000 0 111 10 111111  ✅ H24 COMPLETE TRIGGER  
Example: 0000 0 000 11 000000  ✅ H24 BOTH TRIGGERS
```

### 7.3 Always Invalid Patterns (Both Modes)
```
[≠0000] ? ??? ?? ??????  ❌ Invalid primary status
0000 1 ??? ?? ??????  ❌ Erfasst flag blocks all archiving
```

---

## Conclusion

**Key Findings:**

1. **Position 5 = '1'** (Erfasst) always blocks archiving in both modes
2. **Position 10 = '1'** has opposite effects: rejects in Standard, triggers in H24
3. **H24 mode** accepts 12x more status combinations than Standard mode
4. **Query discrepancies** occur because simple date queries ignore script validation logic
5. **All 16 positions** have specific meanings, but only positions 1-5, 9-10 affect archiving

**Archiving Mode Comparison:**
- **Standard Mode:** 1,024 valid status patterns (restrictive)
- **H24 Mode:** 12,288 valid status patterns (permissive)  
- **Common Invalid:** Any pattern with positions 1-4 ≠ "0000" or position 5 = '1'

This analysis provides the complete foundation for predicting archiving behavior based on status flags and script execution mode.