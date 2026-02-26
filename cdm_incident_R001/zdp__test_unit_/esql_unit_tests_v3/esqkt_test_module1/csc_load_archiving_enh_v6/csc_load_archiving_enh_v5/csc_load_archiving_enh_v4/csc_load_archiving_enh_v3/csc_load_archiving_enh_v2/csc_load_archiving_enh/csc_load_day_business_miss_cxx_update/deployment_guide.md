# TPLDAUFTR Business Day Enhancement - Deployment & Testing Guide

**Project:** PHARMOS CSC_LOAD tpldauftr Business Day Enhancement
**Version:** 1.0 - Production Ready
**Date:** August 12, 2025

---

## 1. Deployment Scenarios

### 1.1 Standard Production Deployment

```bash
# Scenario 1: Normal nightly run (23:30)
# Expected: Archives current business day orders (standard behavior)
./tpldauftr -start -fil 1

# Scenario 2: Emergency morning run (08:00)
# Expected: Archives previous business day orders (CPR protection)
./tpldauftr -start -fil 1 --business-day-mode enabled --cutoff-time 06:00

# Scenario 3: Late afternoon run (16:00)
# Expected: Archives current business day orders (normal behavior)
./tpldauftr -start -fil 1 --business-day-mode enabled --cutoff-time 06:00
```

### 1.2 Backward Compatibility Mode

```bash
# Original behavior (no business day logic)
# Use when CPR enhancement needs to be disabled
./tpldauftr -start -fil 1

# With debug logging (original mode)
./tpldauftr -start -fil 1 -dg
```

### 1.3 Enhanced Monitoring Mode

```bash
# Full monitoring with business day logic
./tpldauftr -start -fil 1 --business-day-mode enabled -dg

# Extended logging for troubleshooting
./tpldauftr -start -fil 1 --business-day-mode enabled -dg -backup
```

---

## 2. Country-Specific Deployment Commands

### 2.1 Low-Impact Countries (Phase 1)

```bash
# Switzerland (CH) - Branch 21
./tpldauftr -start -fil 21 --business-day-mode enabled -dg

# Austria (AT) - Branch 11
./tpldauftr -start -fil 11 --business-day-mode enabled -dg

# Bulgaria (BG) - Branch 31
./tpldauftr -start -fil 31 --business-day-mode enabled -dg

# Croatia (HR) - Branch 41
./tpldauftr -start -fil 41 --business-day-mode enabled -dg
```

### 2.2 High-Impact Countries (Phase 2)

```bash
# Germany (DE) - Branch 1
./tpldauftr -start -fil 1 --business-day-mode enabled -dg

# France (FR) - Branch 51
./tpldauftr -start -fil 51 --business-day-mode enabled -dg
```

---

## 3. Emergency Procedures

### 3.1 Immediate Rollback

```bash
# Remove business day logic to revert to original behavior
./tpldauftr -start -fil 1

# Verify original functionality
./tpldauftr -start -fil 1 -dg
```

### 3.2 Binary Rollback (if needed)

```bash
# Replace enhanced binary with original
cp tpldauftr.original tpldauftr_enhanced

# Rebuild from original source
cd tpldauftr/
cp tpldauftr.cxx.original tpldauftr_enhanced.cxx
make clean && make
```

---

## 4. Testing Scenarios

### 4.1 Time-Based Testing

```bash
# Test 1: Before 6 AM (Emergency scenario)
# Simulate time: 05:30 Monday
# Expected: Archive Friday orders (previous business day)
./tpldauftr -start -fil 1 --business-day-mode enabled -dg --cutoff-time 06:00

# Test 2: After 6 AM (Normal scenario)
# Simulate time: 08:00 Monday
# Expected: Archive Monday orders (current business day)
./tpldauftr -start -fil 1 --business-day-mode enabled -dg --cutoff-time 06:00

# Test 3: Weekend emergency
# Simulate time: 05:00 Sunday
# Expected: Archive Friday orders (weekend rollback)
./tpldauftr -start -fil 1 --business-day-mode enabled -dg --cutoff-time 06:00
```

### 4.2 Functionality Testing

```bash
# Test 4: Backward compatibility
# Expected: Exact original behavior
./tpldauftr -start -fil 1
./tpldauftr -start -fil 1 -h24
./tpldauftr -start -fil 1 -backup

# Test 5: Parameter combinations
./tpldauftr -start -fil 1 --business-day-mode enabled -h24
./tpldauftr -start -fil 1 --business-day-mode enabled -backup
./tpldauftr -start -fil 1 --business-day-mode enabled -dg -h24
```

### 4.3 Error Handling Testing

```bash
# Test 6: Invalid parameters
./tpldauftr -start -fil 0 --business-day-mode enabled        # Should fail with error
./tpldauftr -start -fil abc --business-day-mode enabled      # Should fail with error
./tpldauftr --business-day-mode enabled                      # Should fail - no fil parameter

# Test 7: Database connection issues
# Simulate database unavailability and test error handling
./tpldauftr -start -fil 1 --business-day-mode enabled -dg
```

---

## 5. Validation Procedures

### 5.1 Archive Count Validation

```sql
-- Before running tpldauftr
SELECT COUNT(*) FROM kdauftrag WHERE kzgeschlossen = '1' AND filnr = 1;

-- After running tpldauftr
SELECT COUNT(*) FROM akdauftrag WHERE filnr = 1 AND datumarchiv = TODAY;

-- Validate business day cutoff
SELECT MAX(datumvaluta) FROM akdauftrag WHERE datumarchiv = TODAY;
```

### 5.2 Business Day Logic Validation

```bash
# Check log files for business day calculations
grep "Business day cutoff" /pharmos/logs/tpldauftr.log
grep "Emergency run detected" /pharmos/logs/tpldauftr.log
grep "Normal run detected" /pharmos/logs/tpldauftr.log
```

### 5.3 Performance Validation

```bash
# Measure execution time (should be <1% increase)
time ./tpldauftr -start -fil 1          # Original baseline
time ./tpldauftr -start -fil 1 --business-day-mode enabled      # Enhanced version

# Monitor system resources during execution
top -p $(pgrep tpldauftr)
```

---

## 6. Monitoring and Alerts

### 6.1 Success Indicators

```bash
# Check for successful completion
tail -f /pharmos/logs/tpldauftr.log | grep "STOP.*Return-Code.*0"

# Validate archive counts match expectations
grep "Records archived" /pharmos/logs/tpldauftr.log

# Confirm business day logic activation
grep "Business day mode: ENABLED" /pharmos/logs/tpldauftr.log
```

### 6.2 Error Indicators

```bash
# Check for business day calculation errors
grep "ERROR.*business.*day" /pharmos/logs/tpldauftr.log

# Monitor SQL execution failures
grep "SQL ERROR" /pharmos/logs/tpldauftr.log

# Check for parameter validation failures
grep "Parameter.*invalid" /pharmos/logs/tpldauftr.log
```

---

## 7. Troubleshooting Guide

### 7.1 Common Issues

**Issue:** Business day logic not activating
```bash
# Solution: Verify --business-day-mode enabled parameter is specified
./tpldauftr -start -fil 1 --business-day-mode enabled -dg
# Check logs for "Business day mode: ENABLED"
```

**Issue:** Weekend rollback not working
```bash
# Solution: Check time zone settings and weekend detection
grep "Weekend detected" /pharmos/logs/tpldauftr.log
```

**Issue:** Performance degradation
```bash
# Solution: Monitor and optimize date calculations
grep "Business day calculation time" /pharmos/logs/tpldauftr.log
```

### 7.2 Emergency Contacts

- **Technical Lead:** CPR Implementation Team
- **Database Support:** PINT Team
- **Infrastructure:** AIX Administration Team
- **Business Users:** Country Operations Teams

---

## 8. Sign-off Checklist

### 8.1 Pre-Deployment Validation

- [ ] Enhanced binary compiled successfully
- [ ] All test scenarios executed and passed
- [ ] Backward compatibility verified
- [ ] Performance impact assessed (<1%)
- [ ] Documentation reviewed and approved

### 8.2 Deployment Execution

- [ ] Staging deployment completed
- [ ] User acceptance testing passed
- [ ] Production deployment executed
- [ ] Monitoring enabled and validated
- [ ] Business teams notified

### 8.3 Post-Deployment Validation

- [ ] First emergency run validated (before 6 AM)
- [ ] First normal run validated (after 6 AM)
- [ ] Weekend rollback tested
- [ ] Business user feedback collected
- [ ] Performance metrics within targets

---

## 9. Updated Parameter Documentation

The `-bd` command line parameter has been enhanced to provide more granular control over business day processing. The previous simple `-bd` flag has been superseded by the following options:

```bash
# OLD COMMAND - Implicit business day mode (CPR enhanced behavior by default)
./tpldauftr -start -fil 1

# Explicit business day mode enabled (same as implicit)
./tpldauftr -start -fil 1 --business-day-mode enabled

# Custom cutoff time (default 06:00)
./tpldauftr -start -fil 1 --business-day-mode enabled --cutoff-time 07:00

# Archive previous business day's orders
./tpldauftr -start -fil 1 --archive-mode previous

# Archive current business day's orders
./tpldauftr -start -fil 1 --archive-mode current

# Archive all business days' orders
./tpldauftr -start -fil 1 --archive-mode all

# Force processing for a specific date
./tpldauftr -start -fil 1 --force-date 2025-08-10

# Disable business day logic (original behavior)
./tpldauftr -start -fil 1 --business-day-mode disabled
```

### Business Day Mode Behavior Matrix

| Command | Business Day Logic | Use Case |
|---------|-------------------|----------|
| `./tpldauftr -start -fil 1` | **ENABLED** (implicit) | Standard usage - CPR enhanced by default |
| `./tpldauftr -start -fil 1 --business-day-mode enabled` | **ENABLED** (explicit) | Explicitly request CPR enhancement |
| `./tpldauftr -start -fil 1 --business-day-mode disabled` | **DISABLED** | Callback to original pre-CPR behavior |

**Key Points:**
- **Default behavior changed**: Old commands now use CPR business day logic
- **Backward compatibility**: Use `--business-day-mode disabled` to restore exact original behavior
- **Emergency protection**: Enabled by default for all existing scripts

**Deployment Status:** ✅ Ready for Production
**Approval Authority:** CPR Implementation Team
**Go-Live Authorization:** Pending final staging validation