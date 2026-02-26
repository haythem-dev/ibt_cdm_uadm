
# H24 and Business Day Criteria Matrix Analysis
**Complete archiving criteria breakdown for all execution scenarios**

**Analysis Date:** January 16, 2025  
**Context:** Deep dive into archiving criteria applied in each scenario

---

## Executive Summary

The enhanced tpldauftr script applies **different combinations of criteria** depending on the flags used. Here's the complete breakdown of what gets checked in each scenario.

---

## Scenario 1: No Flags (Default Enhanced Mode)
```bash
./tpldauftr.sh -start -fil 50
# Parameters passed: --business-day-mode enabled -fil 50
```

### Applied Criteria:
| Criterion | Applied | Logic |
|-----------|---------|-------|
| **Date Check** | ✅ YES | Business day logic (08:00-18:00 = previous day, else current) |
| **Status Check** | ✅ YES | Standard status validation |
| **H24 Mode** | ❌ NO | Standard processing only |
| **Business Day Protection** | ✅ YES | Active protection against business blindness |

### Detailed Logic Flow:
```cpp
// Time-based date selection
if (current_hour >= 8 && current_hour < 18) {
    // EMERGENCY: 08:00-18:00
    target_date = getPreviousBusinessDay();
} else {
    // NORMAL: 18:00-08:00
    target_date = getCurrentBusinessDay();
}

// Status validation (standard)
if (kdauftragstat passes standard validation) {
    // Archive eligible orders
}

// Final query
SELECT * FROM kdauftrag 
WHERE filialnr = 50 
  AND datumauslieferung <= target_date
  AND [standard_status_criteria]
```

---

## Scenario 2: H24 Only
```bash
./tpldauftr.sh -start -fil 50 -h24
# Parameters passed: -h24 --business-day-mode enabled -fil 50
```

### Applied Criteria:
| Criterion | Applied | Logic |
|-----------|---------|-------|
| **Date Check** | ✅ YES | Business day logic (08:00-18:00 = previous day, else current) |
| **Status Check** | ✅ YES | Enhanced status validation with H24 logic |
| **H24 Mode** | ✅ YES | 24-hour processing enabled |
| **Business Day Protection** | ✅ YES | Active protection + H24 capabilities |

### Detailed Logic Flow:
```cpp
// Same time-based date selection as Scenario 1
if (current_hour >= 8 && current_hour < 18) {
    target_date = getPreviousBusinessDay();
} else {
    target_date = getCurrentBusinessDay();
}

// Enhanced status validation with H24
if (h24_mode_enabled) {
    // Apply H24-specific order processing logic
    // Broader status acceptance criteria
}

// Same business day protection + H24 processing
SELECT * FROM kdauftrag 
WHERE filialnr = 50 
  AND datumauslieferung <= target_date
  AND [h24_enhanced_status_criteria]
```

---

## Scenario 3: Business Day Disabled
```bash
./tpldauftr.sh -start -fil 50 --business-day-mode disabled
# Parameters passed: --business-day-mode disabled -fil 50
```

### Applied Criteria:
| Criterion | Applied | Logic |
|-----------|---------|-------|
| **Date Check** | ❌ NO | Original behavior - no time-based logic |
| **Status Check** | ✅ YES | Standard status validation |
| **H24 Mode** | ❌ NO | Standard processing only |
| **Business Day Protection** | ❌ NO | **DISABLED** - Original archiving behavior |

### Detailed Logic Flow:
```cpp
// NO time-based date selection - uses original logic
target_date = getCurrentBusinessDay(); // Always current day

// Standard status validation (original)
if (kdauftragstat passes original validation) {
    // Archive eligible orders
}

// Original query behavior
SELECT * FROM kdauftrag 
WHERE filialnr = 50 
  AND datumauslieferung <= CURRENT_DATE
  AND [original_status_criteria]
```

**⚠️ WARNING:** This scenario creates risk of business blindness during business hours!

---

## Scenario 4: H24 + Business Day Disabled
```bash
./tpldauftr.sh -start -fil 50 -h24 --business-day-mode disabled
# Parameters passed: -h24 --business-day-mode disabled -fil 50
```

### Applied Criteria:
| Criterion | Applied | Logic |
|-----------|---------|-------|
| **Date Check** | ❌ NO | Original behavior - no time-based logic |
| **Status Check** | ✅ YES | Enhanced status validation with H24 logic |
| **H24 Mode** | ✅ YES | 24-hour processing enabled |
| **Business Day Protection** | ❌ NO | **DISABLED** - Original + H24 behavior |

### Detailed Logic Flow:
```cpp
// NO time-based date selection - uses original logic
target_date = getCurrentBusinessDay(); // Always current day

// Enhanced status validation with H24 (original + H24)
if (h24_mode_enabled) {
    // Apply H24-specific order processing logic
    // Broader status acceptance criteria
}

// Original query + H24 processing
SELECT * FROM kdauftrag 
WHERE filialnr = 50 
  AND datumauslieferung <= CURRENT_DATE
  AND [h24_enhanced_status_criteria]
```

**⚠️ WARNING:** This scenario also creates risk of business blindness during business hours!

---

## Comprehensive Criteria Comparison Table

| Scenario | Date Logic | Time Check | Status Logic | H24 Processing | Business Protection | Risk Level |
|----------|------------|------------|--------------|----------------|-------------------|------------|
| **No Flags** | Business Day | 08:00-18:00 | Standard | ❌ | ✅ Active | 🟢 Low |
| **H24 Only** | Business Day | 08:00-18:00 | Enhanced | ✅ | ✅ Active | 🟢 Low |
| **BD Disabled** | Original | None | Standard | ❌ | ❌ Disabled | 🔴 High |
| **H24 + BD Disabled** | Original | None | Enhanced | ✅ | ❌ Disabled | 🔴 High |

---

## Date Selection Logic Detail

### Business Day Mode Enabled (Scenarios 1 & 2):
```cpp
// Time-sensitive date calculation
struct tm *timeinfo = localtime(&now);
int hour = timeinfo->tm_hour;

if (hour >= 8 && hour < 18) {
    // EMERGENCY WINDOW: Archive previous business day only
    if (weekday == 1) { // Monday
        target_date = current_date - 3; // Friday
    } else if (weekday == 0) { // Sunday  
        target_date = current_date - 2; // Friday
    } else {
        target_date = current_date - 1; // Previous day
    }
} else {
    // NORMAL WINDOW: Archive current business day
    target_date = current_date;
}
```

### Business Day Mode Disabled (Scenarios 3 & 4):
```cpp
// Original behavior - always current day
target_date = current_date; // No time logic applied
```

---

## Status Check Logic Detail

### Standard Status Check (Scenarios 1 & 3):
```cpp
// Original order status validation
if (PruefeAuftrag(order_status)) {
    // Standard archiving criteria
    // Checks basic order completion status
}
```

### Enhanced Status Check with H24 (Scenarios 2 & 4):
```cpp
// H24 enhanced order status validation
if (h24_mode) {
    // Broader status acceptance
    // Additional H24-specific order processing
    // Extended archiving criteria
}
if (PruefeAuftrag(order_status)) {
    // Standard validation still applies
}
```

---

## Impact on Order Selection

### Example: Execution at 10:00 AM

| Scenario | Target Date | Orders Selected | Business Impact |
|----------|-------------|-----------------|-----------------|
| **No Flags** | Previous Day | Yesterday's closed orders | ✅ Safe |
| **H24 Only** | Previous Day | Yesterday's closed orders + H24 processing | ✅ Safe |
| **BD Disabled** | Current Day | Today's closed orders | ⚠️ Business blindness |
| **H24 + BD Disabled** | Current Day | Today's closed orders + H24 processing | ⚠️ Business blindness |

### Example: Execution at 22:00 PM

| Scenario | Target Date | Orders Selected | Business Impact |
|----------|-------------|-----------------|-----------------|
| **No Flags** | Current Day | Today's closed orders | ✅ Normal |
| **H24 Only** | Current Day | Today's closed orders + H24 processing | ✅ Normal |
| **BD Disabled** | Current Day | Today's closed orders | ✅ Normal |
| **H24 + BD Disabled** | Current Day | Today's closed orders + H24 processing | ✅ Normal |

---

## Conclusion

**Key Findings:**
1. **Business Day Protection** only applies in Scenarios 1 & 2
2. **H24 Processing** enhances status validation in Scenarios 2 & 4
3. **Date Logic** is completely independent of H24 flag
4. **Scenarios 3 & 4** revert to original behavior with business blindness risk

**Recommendation:** Use Scenarios 1 or 2 for production to maintain business day protection.
