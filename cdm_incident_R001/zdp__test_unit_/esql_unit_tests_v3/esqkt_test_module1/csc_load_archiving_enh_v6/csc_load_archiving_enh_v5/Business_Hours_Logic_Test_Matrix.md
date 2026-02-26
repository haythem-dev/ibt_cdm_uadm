# Business Hours Logic Test Matrix

## Current Business Hours Configuration
- **Business Hours**: 08:00 - 18:00 (8 AM to 6 PM)
- **Business Logic**: 
  - **Within business hours**: Archive PREVIOUS business day
  - **Outside business hours**: Archive CURRENT business day

## Test Scenarios Matrix

### Scenario 1: Monday During Business Hours
**Test Context**: Monday 10:00 AM
**Expected Logic**: Archive PREVIOUS business day (Friday)
**Weekend Rollback**: None needed (Friday is business day)
**Expected Cutoff**: Friday

### Scenario 2: Monday Outside Business Hours  
**Test Context**: Monday 19:00 PM (after 18:00)
**Expected Logic**: Archive CURRENT business day (Monday)
**Weekend Rollback**: None needed (Monday is business day)
**Expected Cutoff**: Monday

### Scenario 3: Saturday During "Business Hours"
**Test Context**: Saturday 10:00 AM
**Expected Logic**: Archive PREVIOUS business day (Friday)
**Weekend Rollback**: Previous day = Friday, no rollback needed
**Expected Cutoff**: Friday

### Scenario 4: Saturday Outside Business Hours
**Test Context**: Saturday 19:00 PM
**Expected Logic**: Archive CURRENT business day
**Weekend Rollback**: Current day = Saturday → rollback to Friday
**Expected Cutoff**: Friday

### Scenario 5: Sunday During "Business Hours" (Our Previous Test)
**Test Context**: Sunday 17:52 PM (within 8-18)
**Expected Logic**: Archive PREVIOUS business day (Saturday)
**Weekend Rollback**: Saturday → Friday
**Expected Cutoff**: Friday ✅ (Already tested)

### Scenario 6: Sunday Outside Business Hours
**Test Context**: Sunday 19:00 PM
**Expected Logic**: Archive CURRENT business day
**Weekend Rollback**: Current day = Sunday → rollback to Friday  
**Expected Cutoff**: Friday

### Scenario 7: Friday End of Business Hours (Edge Case)
**Test Context**: Friday 17:59 PM (last minute of business hours)
**Expected Logic**: Archive PREVIOUS business day (Thursday)
**Weekend Rollback**: None needed (Thursday is business day)
**Expected Cutoff**: Thursday

### Scenario 8: Friday Just After Business Hours (Edge Case)
**Test Context**: Friday 18:01 PM (just after business hours)
**Expected Logic**: Archive CURRENT business day (Friday)
**Weekend Rollback**: None needed (Friday is business day)
**Expected Cutoff**: Friday

## Critical Edge Cases to Test

### Edge Case A: Business Hours Boundary
- **17:59** vs **18:01** on same day should yield different results
- **07:59** vs **08:01** on same day should yield different results

### Edge Case B: Weekend Business Hours
- Saturday/Sunday at 10:00 should still respect business hours logic
- Weekend + business hours = archive previous day with weekend rollback

### Edge Case C: Cross-Week Scenarios
- Sunday 19:00 → Current day rollback (Sun → Fri)
- Monday 07:00 → Current day (Mon, no rollback)

## Test Implementation Strategy

### Method 1: Simulate Different Times
Create test cases that mock `time()` function to return specific timestamps

### Method 2: Manual Hour Override
Temporarily modify business hours configuration for testing

### Method 3: Force Date Testing
Use existing `--force-date` parameter to test specific scenarios