
# Complete H24 Mode Archiving Decision Tree
**Comprehensive analysis of all possible archiving scenarios in H24 mode**

**Analysis Date:** January 16, 2025  
**Context:** Complete decision tree for H24 mode archiving logic

---

## H24 Mode Archiving Logic - Complete Decision Tree

```
H24 Mode Archiving Logic:
├── Check kdauftragstat[1,4] == "0000" (Primary Status)
│   ├── YES: Continue to H24 flag check
│   │   ├── Check kdauftragstat[5] == '1' (Erfasst/Captured Flag)
│   │   │   ├── YES: Skip order (Erfasst overrides all H24 logic)
│   │   │   │   └── RESULT: ❌ NOT ARCHIVED - "Erfasst flag active"
│   │   │   └── NO: Continue to H24 flag processing
│   │   │       ├── Check kdauftragstat[9] == '1' (Completed/Invoiced)
│   │   │       │   ├── YES: Archive immediately (ignores date)
│   │   │       │   │   └── RESULT: ✅ ARCHIVED IMMEDIATELY - "H24 Complete flag active"
│   │   │       │   └── NO: Check cancel flag
│   │   │       │       ├── Check kdauftragstat[10] == '1' (Cancelled/Storniert)
│   │   │       │       │   ├── YES: Archive immediately (ignores date)
│   │   │       │       │   │   └── RESULT: ✅ ARCHIVED IMMEDIATELY - "H24 Cancel flag active"
│   │   │       │       │   └── NO: Fall back to date logic
│   │   │       │       │       ├── Apply business day logic (if enabled)
│   │   │       │       │       │   ├── Business Day Logic Enabled
│   │   │       │       │       │   │   ├── Current Time: 08:00-18:00 (Emergency Window)
│   │   │       │       │       │   │   │   ├── Check datumauslieferung <= previous_business_day
│   │   │       │       │       │   │   │   │   ├── YES: Archive with emergency protection
│   │   │       │       │       │   │   │   │   │   └── RESULT: ✅ ARCHIVED - "Emergency window, previous day logic"
│   │   │       │       │       │   │   │   │   └── NO: Skip (too recent for emergency)
│   │   │       │       │       │   │   │   │       └── RESULT: ❌ NOT ARCHIVED - "Too recent for emergency window"
│   │   │       │       │       │   │   │   └── Current Time: 18:00-08:00 (Normal Window)
│   │   │       │       │       │   │   │       ├── Check datumauslieferung <= current_business_day
│   │   │       │       │       │   │   │       │   ├── YES: Archive with normal logic
│   │   │       │       │       │   │   │       │   │   └── RESULT: ✅ ARCHIVED - "Normal window, current day logic"
│   │   │       │       │       │   │   │       │   └── NO: Skip (too recent for normal)
│   │   │       │       │       │   │   │       │       └── RESULT: ❌ NOT ARCHIVED - "Too recent for normal archiving"
│   │   │       │       │       │   │   └── Weekend Execution
│   │   │       │       │       │   │       ├── Check datumauslieferung <= friday_cutoff
│   │   │       │       │       │   │       │   ├── YES: Archive with weekend logic
│   │   │       │       │       │   │       │   │   └── RESULT: ✅ ARCHIVED - "Weekend execution, Friday cutoff"
│   │   │       │       │       │   │       │   └── NO: Skip (too recent for weekend)
│   │   │       │       │       │   │       │       └── RESULT: ❌ NOT ARCHIVED - "Too recent for weekend archiving"
│   │   │       │       │       │   └── Business Day Logic Disabled
│   │   │       │       │       │       ├── Check datumauslieferung <= current_date_minus_retention
│   │   │       │       │       │       │   ├── YES: Archive with original logic
│   │   │       │       │       │       │   │   └── RESULT: ✅ ARCHIVED - "Original date logic applied"
│   │   │       │       │       │       │   └── NO: Skip (retention period not met)
│   │   │       │       │       │       │       └── RESULT: ❌ NOT ARCHIVED - "Retention period not satisfied"
│   │   │       │       │       └── Apply cutoff logic (if enabled instead of business day)
│   │   │       │       │           ├── Current Time < cutoff_time (e.g., 06:30)
│   │   │       │       │           │   ├── Check datumauslieferung <= previous_business_day
│   │   │       │       │           │   │   ├── YES: Archive with cutoff protection
│   │   │       │       │           │   │   │   └── RESULT: ✅ ARCHIVED - "Cutoff protection, previous day logic"
│   │   │       │       │           │   │   └── NO: Skip (too recent for cutoff)
│   │   │       │       │           │   │       └── RESULT: ❌ NOT ARCHIVED - "Too recent for cutoff window"
│   │   │       │       │           └── Current Time >= cutoff_time
│   │   │       │       │               ├── Check datumauslieferung <= current_business_day
│   │   │       │       │               │   ├── YES: Archive with normal cutoff logic
│   │   │       │       │               │   │   └── RESULT: ✅ ARCHIVED - "Normal cutoff operation"
│   │   │       │       │               │   └── NO: Skip (too recent for normal cutoff)
│   │   │       │       │               │       └── RESULT: ❌ NOT ARCHIVED - "Too recent for normal cutoff"
│   └── NO: Skip order (invalid primary status)
│       └── RESULT: ❌ NOT ARCHIVED - "Invalid primary status (not 0000)"
```

## Summary of All Possible H24 Outcomes

### Immediate Archive Scenarios (H24 Flags Override Date Logic)
1. **H24 Complete Flag Active** (`kdauftragstat[9] == '1'`)
   - ✅ Archives immediately regardless of date
   - ✅ Ignores business day logic
   - ✅ Ignores retention periods

2. **H24 Cancel Flag Active** (`kdauftragstat[10] == '1'`)
   - ✅ Archives immediately regardless of date
   - ✅ Ignores business day logic
   - ✅ Ignores retention periods

### Rejection Scenarios (Always Skip)
3. **Invalid Primary Status** (`kdauftragstat[1,4] != "0000"`)
   - ❌ Always rejected
   - ❌ H24 flags ignored

4. **Erfasst Flag Active** (`kdauftragstat[5] == '1'`)
   - ❌ Always rejected
   - ❌ H24 flags ignored (Erfasst overrides everything)

### Date-Dependent Scenarios (No H24 Flags Active)
5. **Business Day Logic + Emergency Window** (08:00-18:00)
   - ✅/❌ Depends on previous business day comparison
   - Protection against emergency day blindness

6. **Business Day Logic + Normal Window** (18:00-08:00)
   - ✅/❌ Depends on current business day comparison
   - Normal archiving operation

7. **Weekend Execution**
   - ✅/❌ Depends on Friday cutoff comparison
   - Weekend safety logic

8. **Business Day Disabled (Original Logic)**
   - ✅/❌ Depends on original retention calculation
   - No business day protection

9. **Cutoff Logic** (Alternative to business day)
   - ✅/❌ Depends on configurable cutoff time
   - More precise time-based protection

## Risk Assessment by Scenario

### ✅ Safe Scenarios (Proper Business Protection)
- H24 flags active (immediate archive of completed/cancelled orders)
- Business day emergency window (prevents day-of archiving)
- Erfasst flag protection (prevents archiving of captured orders)

### ⚠️ Medium Risk Scenarios
- Normal business day operation (current day archiving)
- Weekend operation with Friday cutoff
- Cutoff logic with proper timing

### 🔴 High Risk Scenarios  
- Business day logic disabled during business hours
- Invalid status combinations that bypass all protections
- Orders that fall through date logic gaps

## Implementation Notes

1. **Erfasst Flag Priority**: Position 5 = '1' always blocks archiving, even with H24 flags
2. **H24 Flag Priority**: Positions 9 & 10 = '1' always trigger immediate archiving
3. **Date Logic Fallback**: Only applies when no H24 flags are active
4. **Business Day Protection**: Independent of H24 mode, can be disabled separately
5. **Weekend Handling**: Automatically uses Friday as reference point

This complete decision tree covers all possible combinations of status flags, execution modes, and timing scenarios in the H24 archiving system.
