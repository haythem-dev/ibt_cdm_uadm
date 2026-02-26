# TPLDAUFTR Enhanced Test Suite
**Comprehensive Testing Strategy and Test Cases**

**Date:** August 12, 2025  
**Version:** Enhanced CPR Implementation  
**Purpose:** Validate business day enhancement and ensure backward compatibility

---

## Test Suite Overview

### Testing Objectives
1. **Functional Validation** - All new parameters work correctly
2. **Business Logic** - 6 AM cutoff and weekend handling
3. **Backward Compatibility** - Original functionality preserved
4. **Edge Cases** - Boundary conditions and error scenarios
5. **Integration** - Works with existing systems

### Testing Environment
- **Test System:** Non-production database
- **Test Data:** Sample orders with various timestamps
- **Test Period:** Multiple business days and weekends
- **Validation:** Compare actual vs expected archive dates

---

## Test Categories

## 1. Parameter Validation Tests

### Test 1.1: Business Day Mode Parameter
```bash
# Test Case: Enable business day mode
./tpldauftr -start -fil 1 --business-day-mode enabled
Expected: Mode activated, success message

# Test Case: Disable business day mode  
./tpldauftr -start -fil 1 --business-day-mode disabled
Expected: Mode deactivated, original behavior

# Test Case: Invalid value
./tpldauftr -start -fil 1 --business-day-mode invalid
Expected: Error message, usage help
```

### Test 1.2: Cutoff Time Parameter
```bash
# Test Case: Valid time format
./tpldauftr -start -fil 1 --cutoff-time 06:30
Expected: Cutoff set to 6:30 AM

# Test Case: Single digit format
./tpldauftr -start -fil 1 --cutoff-time 5:15
Expected: Cutoff set to 5:15 AM

# Test Case: Invalid time
./tpldauftr -start -fil 1 --cutoff-time 25:00
Expected: Error message, invalid time

# Test Case: Invalid format
./tpldauftr -start -fil 1 --cutoff-time 6:30PM
Expected: Error message, format help
```

### Test 1.3: Force Date Parameter
```bash
# Test Case: Valid date
./tpldauftr -start -fil 1 --force-date 20250801
Expected: Archive date set to Aug 1, 2025

# Test Case: Invalid format
./tpldauftr -start -fil 1 --force-date 2025-08-01
Expected: Error message, format help

# Test Case: Invalid date
./tpldauftr -start -fil 1 --force-date 20251301
Expected: Error message, invalid date
```

### Test 1.4: Archive Mode Parameter
```bash
# Test Case: Previous mode
./tpldauftr -start -fil 1 --archive-mode previous
Expected: Always archive previous business day

# Test Case: Current mode
./tpldauftr -start -fil 1 --archive-mode current
Expected: Always archive current business day

# Test Case: All mode
./tpldauftr -start -fil 1 --archive-mode all
Expected: Archive all orders regardless of date

# Test Case: Invalid mode
./tpldauftr -start -fil 1 --archive-mode tomorrow
Expected: Error message, valid options listed
```

---

## 2. Business Logic Tests

### Test 2.1: 6 AM Cutoff Logic
```bash
# Setup: Set system time and run tests

# Test Case: Run at 4:30 AM (Emergency scenario)
# Set time: 04:30
./tpldauftr -start -fil 1
Expected: Archives previous business day orders

# Test Case: Run at 8:00 AM (Normal scenario)  
# Set time: 08:00
./tpldauftr -start -fil 1
Expected: Archives current business day orders

# Test Case: Run exactly at 6:00 AM (Boundary)
# Set time: 06:00
./tpldauftr -start -fil 1
Expected: Archives current business day orders (>= cutoff)
```

### Test 2.2: Weekend Handling
```bash
# Test Case: Saturday emergency run
# Set time: Saturday 04:00
./tpldauftr -start -fil 1
Expected: Archives Friday's orders

# Test Case: Sunday emergency run
# Set time: Sunday 05:00  
./tpldauftr -start -fil 1
Expected: Archives Friday's orders

# Test Case: Monday emergency run
# Set time: Monday 04:00
./tpldauftr -start -fil 1
Expected: Archives Friday's orders
```

### Test 2.3: Custom Cutoff Time
```bash
# Test Case: Custom cutoff before run time
# Set cutoff: 05:30, Run time: 06:00
./tpldauftr -start -fil 1 --cutoff-time 05:30
Expected: Archives current business day (after cutoff)

# Test Case: Custom cutoff after run time
# Set cutoff: 07:00, Run time: 06:00
./tpldauftr -start -fil 1 --cutoff-time 07:00
Expected: Archives previous business day (before cutoff)
```

---

## 3. Implicit Activation Tests

### Test 3.1: Filial 1 Auto-Activation
```bash
# Test Case: Fil 1 without explicit mode
./tpldauftr -start -fil 1
Expected: Business day mode auto-enabled

# Test Case: Fil 1 with explicit disable
./tpldauftr -start -fil 1 --business-day-mode disabled
Expected: Original behavior, mode disabled
```

### Test 3.2: Other Filials Behavior
```bash
# Test Case: Fil 2 without explicit mode
./tpldauftr -start -fil 2
Expected: Original behavior, mode disabled

# Test Case: Fil 2 with explicit enable
./tpldauftr -start -fil 2 --business-day-mode enabled
Expected: Business day mode enabled
```

---

## 4. Backward Compatibility Tests

### Test 4.1: Original Parameters
```bash
# Test Case: Standard original command
./tpldauftr -start -fil 1 -dg
Expected: Works exactly as original version

# Test Case: All original parameters
./tpldauftr -dg -del -db testdb -fil 1 -h24
Expected: All original functionality preserved
```

### Test 4.2: No Parameter Changes
```bash
# Test Case: Legacy script compatibility
./old_script_calling_tpldauftr.sh
Expected: Works without modification

# Test Case: Cron job compatibility
# Original cron: 0 23 * * * /path/to/tpldauftr -start -fil 1
Expected: Continues to work unchanged
```

---

## 5. Integration Tests

### Test 5.1: Database Integration
```bash
# Test Case: Verify correct orders archived
# Run: ./tpldauftr -start -fil 1 --force-date 20250801
# Validate: Check database for archived orders with correct dates
Expected: Only orders matching business logic archived

# Test Case: SQL query validation
# Check: Archive date calculation in SQL statements
Expected: Correct date filters applied
```

### Test 5.2: Shell Script Integration
```bash
# Test Case: Enhanced shell script
./tpldauftr_enhanced.sh -fil 1 --business-day-mode enabled
Expected: Parameters passed correctly to binary

# Test Case: Error handling
./tpldauftr_enhanced.sh -fil 1 --invalid-param
Expected: Error propagated correctly
```

---

## 6. Edge Cases and Error Scenarios

### Test 6.1: System Clock Issues
```bash
# Test Case: System clock before epoch
# Set time: Jan 1, 1970
./tpldauftr -start -fil 1
Expected: Graceful error handling

# Test Case: Future date
# Set time: Year 2030
./tpldauftr -start -fil 1
Expected: Normal operation with future date
```

### Test 6.2: Resource Constraints
```bash
# Test Case: Low memory conditions
# Limit memory and run
./tpldauftr -start -fil 1
Expected: Graceful degradation or appropriate error

# Test Case: Database connection issues
# Disconnect database and run
./tpldauftr -start -fil 1
Expected: Clear error message, no corruption
```

### Test 6.3: Concurrent Execution
```bash
# Test Case: Multiple instances
./tpldauftr -start -fil 1 &
./tpldauftr -start -fil 1 &
Expected: Proper locking or error handling

# Test Case: Interrupted execution
# Start and kill process
./tpldauftr -start -fil 1
# CTRL+C after partial execution
Expected: Clean state, no partial corruption
```

---

## 7. Performance Tests

### Test 7.1: Processing Time
```bash
# Test Case: Large dataset performance
# Run with 100,000+ orders
time ./tpldauftr -start -fil 1
Expected: Performance similar to original version

# Test Case: Enhanced vs original timing
time ./tpldauftr_original -start -fil 1
time ./tpldauftr_enhanced -start -fil 1 --business-day-mode disabled
Expected: Minimal performance difference
```

### Test 7.2: Memory Usage
```bash
# Test Case: Memory footprint
# Monitor memory during execution
./tpldauftr -start -fil 1
Expected: Memory usage within acceptable limits

# Test Case: Memory leaks
# Run multiple times and monitor
for i in {1..100}; do ./tpldauftr -start -fil 1 --force-date 20250801; done
Expected: No memory leaks detected
```

---

## 8. Automated Test Suite

### Test Script Structure
```bash
#!/bin/bash
# automated_test_suite.sh

run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    
    echo "Running: $test_name"
    result=$(eval "$command" 2>&1)
    
    if [[ "$result" == *"$expected"* ]]; then
        echo "✅ PASS: $test_name"
    else
        echo "❌ FAIL: $test_name"
        echo "Expected: $expected"
        echo "Got: $result"
    fi
}

# Parameter validation tests
run_test "Business Day Mode Enable" \
    "./tpldauftr -start -fil 1 --business-day-mode enabled" \
    "Business day enhancement mode enabled"

run_test "Invalid Archive Mode" \
    "./tpldauftr -start -fil 1 --archive-mode invalid" \
    "Error: Invalid archive mode"

# Add more tests...
```

### Test Data Setup
```sql
-- Create test orders with various timestamps
INSERT INTO orders (order_date, status) VALUES 
('2025-08-10 22:30:00', 'closed'),  -- Friday evening
('2025-08-11 08:00:00', 'closed'),  -- Saturday morning  
('2025-08-12 06:30:00', 'closed'),  -- Monday after cutoff
('2025-08-12 04:30:00', 'closed');  -- Monday before cutoff
```

### Validation Queries
```sql
-- Verify correct archiving behavior
SELECT COUNT(*) FROM archived_orders 
WHERE archive_date = '20250810'  -- Expected for emergency run
AND original_order_date BETWEEN '2025-08-10 00:00:00' AND '2025-08-10 23:59:59';
```

---

## 9. Test Execution Checklist

### Pre-Test Setup
- [ ] Backup production data
- [ ] Set up test database
- [ ] Create test data set
- [ ] Prepare test environment
- [ ] Document baseline behavior

### Test Execution
- [ ] Run parameter validation tests
- [ ] Execute business logic tests
- [ ] Validate backward compatibility
- [ ] Test edge cases
- [ ] Perform integration tests
- [ ] Run performance tests

### Post-Test Validation
- [ ] Verify data integrity
- [ ] Check log files
- [ ] Validate expected outcomes
- [ ] Document test results
- [ ] Report any issues

### Success Criteria
- [ ] All parameter tests pass
- [ ] Business logic works correctly
- [ ] Backward compatibility maintained
- [ ] No data corruption
- [ ] Performance acceptable
- [ ] Error handling appropriate

---

## 10. Continuous Testing

### Daily Validation
```bash
# Daily automated test run
0 1 * * * /path/to/automated_test_suite.sh >> /var/log/tpldauftr_tests.log
```

### Pre-Deployment Testing
```bash
# Complete test suite before production deployment
./run_full_test_suite.sh
if [ $? -eq 0 ]; then
    echo "All tests passed - Ready for deployment"
else
    echo "Tests failed - Do not deploy"
    exit 1
fi
```

This comprehensive test suite ensures the enhanced TPLDAUFTR maintains high quality, reliability, and backward compatibility while delivering the new business day functionality.