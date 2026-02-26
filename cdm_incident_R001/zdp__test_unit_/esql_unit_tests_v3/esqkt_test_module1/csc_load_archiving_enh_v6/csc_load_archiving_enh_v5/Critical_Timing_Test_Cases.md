# Critical Timing Test Cases for Business Hours Logic

## Test Scenarios to Validate

### 1. **Boundary Hour Tests** (Most Critical)

#### Test A: Exactly 8:00 AM (Business Hours Start)
```bash
# Simulate Monday 8:00 AM
Expected: Within business hours → Archive PREVIOUS day (Sunday → Friday rollback)
```

#### Test B: Exactly 6:00 PM (18:00 - Business Hours End)  
```bash
# Simulate Monday 18:00 PM
Expected: Outside business hours → Archive CURRENT day (Monday, no rollback)
```

#### Test C: One Minute Before Business Hours End
```bash
# Simulate Monday 17:59 PM
Expected: Within business hours → Archive PREVIOUS day (Sunday → Friday rollback)
```

#### Test D: One Minute After Business Hours End
```bash
# Simulate Monday 18:01 PM  
Expected: Outside business hours → Archive CURRENT day (Monday, no rollback)
```

### 2. **Weekend Timing Combinations**

#### Test E: Saturday 10:00 AM (Weekend + Business Hours)
```bash
Expected Logic:
- Within "business hours" but on weekend
- Strategy: Archive PREVIOUS day (Friday)
- Rollback: Previous day = Friday (no rollback needed)
- Result: Friday cutoff
```

#### Test F: Saturday 19:00 PM (Weekend + After Business Hours)
```bash
Expected Logic:
- Outside business hours and on weekend
- Strategy: Archive CURRENT day (Saturday)
- Rollback: Current day = Saturday → Friday
- Result: Friday cutoff
```

#### Test G: Sunday 8:00 AM (Weekend + Business Hours Start)
```bash
Expected Logic:
- Exactly at business hours start but on weekend
- Strategy: Archive PREVIOUS day (Saturday)
- Rollback: Saturday → Friday  
- Result: Friday cutoff
```

#### Test H: Sunday 18:00 PM (Weekend + Business Hours End)
```bash
Expected Logic:
- Exactly at business hours end and on weekend
- Strategy: Archive CURRENT day (Sunday)
- Rollback: Sunday → Friday (-2 days)
- Result: Friday cutoff
```

### 3. **Cross-Week Edge Cases**

#### Test I: Sunday 19:00 PM → Monday Processing
```bash
Current: Sunday 19:00 PM (outside business hours)
Strategy: Archive CURRENT day (Sunday)
Rollback: Sunday → Friday (previous week)
Critical Check: Ensure correct week calculation
```

#### Test J: Friday 17:59 PM vs Friday 18:01 PM
```bash
17:59: Within business hours → Archive Thursday
18:01: Outside business hours → Archive Friday
Critical: Same day, different outcomes based on 2-minute difference
```

### 4. **Month/Year Boundary Tests**

#### Test K: End of Month Rollback
```bash
# Example: Saturday March 1st → Should rollback to Friday Feb 28th
Test mktime() handles month boundaries correctly
```

#### Test L: End of Year Rollback  
```bash
# Example: Sunday January 1st → Should rollback to Friday December 29th (prev year)
Test year transition handling
```

## Validation Commands

### Real-Time Testing
```bash
# Test current time behavior
./tpldauftr.sh -dg -start -fil 5

# Force specific times for testing
./tpldauftr.sh -dg -start -fil 5 --force-date 20250815  # Friday
./tpldauftr.sh -dg -start -fil 5 --force-date 20250816  # Saturday  
./tpldauftr.sh -dg -start -fil 5 --force-date 20250817  # Sunday
```

### Test Data for Each Scenario
```sql
-- Test orders spanning the boundary dates
INSERT INTO kdauftrag (kdauftragnr, datumauslieferung, vertriebszentrumnr) VALUES
(880100001, 20250814, 5),  -- Thursday (should be archived in most scenarios)
(880100002, 20250815, 5),  -- Friday (boundary case)
(880100003, 20250816, 5),  -- Saturday (should be kept in most scenarios)
(880100004, 20250817, 5);  -- Sunday (should be kept in most scenarios)
```

## Expected Logging Output for Validation

### Successful Boundary Detection:
```
=== BUSINESS HOURS BOUNDARY CHECK ===
Current hour: 18, minute: 00
Business hours: 08:00 to 18:00 (start <= hour < end)
Boundary evaluation: 18 >= 08 AND 18 < 18 = FALSE (outside)
BOUNDARY: Exactly at business hours end (18:00) - considered OUTSIDE
```

### Weekend + Timing Combination:
```
WEEKEND DETECTED: Current day is Saturday
=== BUSINESS HOURS LOGIC ===
Strategy: Archive PREVIOUS business day
Initial target date: 2025-08-15 (Friday)
NO WEEKEND ROLLBACK: Target Friday is a business day
```

## Critical Issues to Watch For

1. **Hour 18 Logic**: Ensure `hour < 18` (not `hour <= 18`)
2. **Weekend Edge Cases**: Weekend + business hours combinations
3. **mktime() Rollback**: Proper date arithmetic across month/year boundaries
4. **Timezone Handling**: System time vs business time zones
5. **Daylight Saving**: Date calculations during DST transitions

This comprehensive testing ensures the business hours logic works correctly across all timing scenarios and edge cases.