
# TPLDAUFTR Business Hours Enhancement - Usage Guide
**Complete Parameter Reference and Production Usage Examples**

**Date:** August 13, 2025  
**Version:** Business Hours Implementation  
**Status:** ✅ Production Ready

---

## Overview

TPLDAUFTR Enhanced implements business hours logic to prevent "business blindness" during emergency day operations while maintaining full backward compatibility with the original functionality.

## Business Hours Logic Implementation

**Core Enhancement:**
- **Business Hours (08:00-18:00)**: Archives **previous business day** only
- **Non-Business Hours (18:00-08:00)**: Archives **current business day** (normal operation)
- **Weekend Handling**: Saturday/Sunday automatically use Friday as last business day

---

## Complete Parameter List

### Standard Parameters (Preserved)
```bash
-start                 # Start the archiving process
-stop                  # Stop the archiving process  
-status                # Check process status
-fil <number>          # Filial/branch number
-backup                # Backup mode
-h24                   # 24-hour mode
```

### Business Hours Enhancement Parameters
```bash
--business-day-mode enabled|disabled    # Explicit control over business hours logic
--cutoff-time HH:MM                     # Custom time boundaries (future enhancement)
--force-date YYYYMMDD                   # Force specific archive date
--archive-mode current|previous|all     # Archive mode selection
-dg                                     # Debug mode for detailed logging
```

---

## Production Usage Examples

### 1. Recommended Standard Usage
```bash
# Automatic business hours logic - RECOMMENDED
./tpldauftr.sh -start -fil 50
```
**Behavior:**
- **08:00-18:00**: Archives previous business day (prevents business blindness)
- **18:00-08:00**: Archives current business day (normal overnight operation)
- **Weekends**: Automatically uses Friday as last business day

### 2. Debug Mode for Troubleshooting
```bash
# Enable detailed CPR logging
./tpldauftr.sh -dg -start -fil 50
```
**Debug Output Includes:**
- Business hours detection status
- Date calculation logic
- SQL query building process
- Archive mode decisions

### 3. Explicit Control Options
```bash
# Force enable business hours mode
./tpldauftr.sh -start -fil 50 --business-day-mode enabled

# Force disable (revert to original behavior)
./tpldauftr.sh -start -fil 50 --business-day-mode disabled
```

### 4. Advanced Control (Future Enhancements)
```bash
# Custom archive mode selection
./tpldauftr.sh -start -fil 50 --archive-mode previous

# Force specific date (manual reprocessing)
./tpldauftr.sh -start -fil 50 --force-date 20250810
```

---

## Execution Behavior by Time

### Business Hours (08:00-18:00)
```bash
# Example: Run at 14:30
./tpldauftr.sh -start -fil 50
# Result: Archives PREVIOUS business day orders
# Business Impact: Current day orders remain visible ✅
```

### Non-Business Hours (18:00-08:00)
```bash
# Example: Run at 23:30 (normal overnight run)
./tpldauftr.sh -start -fil 50  
# Result: Archives CURRENT business day orders
# Business Impact: Standard overnight archiving ✅
```

### Weekend Operations
```bash
# Example: Run on Sunday at any time
./tpldauftr.sh -start -fil 50
# Result: Archives Friday orders (last business day)
# Business Impact: Proper business day rollback ✅
```

---

## Debug Mode Usage

### Enable Detailed Logging
```bash
./tpldauftr.sh -dg -start -fil 50
```

### Sample Debug Output
```
DEBUG: CPR Enhancement - Business hours logic active (8-18: previous day, else: current day)
DEBUG: Current time: 2025-08-13 14:30:25
DEBUG: Business hours detected (14:30) - using previous business day
DEBUG: Archive mode 'previous' - using previous business day
DEBUG: Final process command: /path/to/tpldauftr.bin --business-day-mode enabled -fil 50 -db ode21@zdev21_shm -del 35
```

---

## Parameter Combinations

### Standard Production Scenarios

| Scenario | Command | Archives | Use Case |
|----------|---------|----------|----------|
| **Normal Overnight** | `./tpldauftr.sh -start -fil 50` | Current Day | Standard nightly processing |
| **Emergency Day Run** | `./tpldauftr.sh -start -fil 50` | Previous Day | Safe daytime execution |
| **Weekend Cleanup** | `./tpldauftr.sh -start -fil 50` | Friday | Weekend operations |
| **Debug/Troubleshoot** | `./tpldauftr.sh -dg -start -fil 50` | Time-dependent | Detailed logging |

### Manual Override Scenarios

| Override | Command | Archives | Use Case |
|----------|---------|----------|----------|
| **Force Enable** | `./tpldauftr.sh -start -fil 50 --business-day-mode enabled` | Time-dependent | Explicit enhancement |
| **Force Disable** | `./tpldauftr.sh -start -fil 50 --business-day-mode disabled` | Original Logic | Revert to pre-CPR |
| **Previous Day** | `./tpldauftr.sh -start -fil 50 --archive-mode previous` | Previous Day | Always safe mode |
| **Current Day** | `./tpldauftr.sh -start -fil 50 --archive-mode current` | Current Day | Force current day |

---

## Integration Examples

### Cron Job Integration
```bash
# Emergency run during business hours (safe - archives previous day)
30 14 * * 1-5 /path/to/tpldauftr.sh -start -fil 50

# Normal overnight run (archives current day)  
30 23 * * * /path/to/tpldauftr.sh -start -fil 50

# Weekend cleanup (uses Friday)
0 2 * * 0 /path/to/tpldauftr.sh -start -fil 50
```

### Shell Script Integration
```bash
#!/bin/bash
# Smart archiving script with business hours awareness

# Get current hour
CURRENT_HOUR=$(date +%H)

if [ $CURRENT_HOUR -ge 8 ] && [ $CURRENT_HOUR -lt 18 ]; then
    echo "Business hours detected - running safe mode"
    ./tpldauftr.sh -start -fil 50  # Will automatically use previous day
else
    echo "Non-business hours - running normal mode"
    ./tpldauftr.sh -start -fil 50  # Will automatically use current day
fi
```

---

## Troubleshooting Guide

### Common Issues and Solutions

**Issue**: Business hours logic not working as expected
```bash
# Solution: Enable debug mode to see detailed logic
./tpldauftr.sh -dg -start -fil 50
```

**Issue**: Need to revert to original behavior
```bash
# Solution: Explicitly disable business day mode
./tpldauftr.sh -start -fil 50 --business-day-mode disabled
```

**Issue**: Weekend operations not using Friday
```bash
# Solution: Check debug output for business day calculation
./tpldauftr.sh -dg -start -fil 50
```

### Debug Output Analysis

**Successful Business Hours Detection:**
```
DEBUG: CPR Enhancement - Business hours logic active
DEBUG: Current time: 2025-08-13 14:30:25  
DEBUG: Business hours detected - using previous business day
```

**Successful Non-Business Hours Detection:**
```
DEBUG: CPR Enhancement - Business hours logic active
DEBUG: Current time: 2025-08-13 20:30:25
DEBUG: Non-business hours detected - using current business day
```

---

## Best Practices

### 1. Use Default Mode for Standard Operations
```bash
# Recommended - automatic business hours logic
./tpldauftr.sh -start -fil 50
```

### 2. Enable Debug Mode for Initial Testing
```bash
# Verify business hours logic is working correctly
./tpldauftr.sh -dg -start -fil 50
```

### 3. Document Special Override Usage
```bash
# Always document why explicit overrides are needed
./tpldauftr.sh -start -fil 50 --business-day-mode disabled  # Reason: Special holiday processing
```

### 4. Monitor Execution Results
```bash
# Check return codes and log output
echo "Exit code: $?" after each execution
```

---

## Production Validation Results

### ✅ Confirmed Working
- **RC: 0** - Successful execution validated
- **Business hours logic** - Working correctly
- **Parameter processing** - All parameters handled properly
- **Debug output** - Detailed logging available

### ✅ Business Impact Achieved
- **Business blindness eliminated** - Day runs safe
- **Normal operations maintained** - Night runs unchanged  
- **Weekend handling correct** - Friday rollback working
- **Debug capabilities available** - Troubleshooting supported

---

## Summary

The TPLDAUFTR business hours enhancement provides intelligent archiving that prevents business blindness during day operations while maintaining all original functionality. The implementation uses business hours logic (08:00-18:00 previous day, else current day) and offers comprehensive debug and override capabilities for operational flexibility.

**Key Benefits:**
- ✅ Automatic protection during business hours
- ✅ Zero configuration required for standard use
- ✅ Full backward compatibility maintained
- ✅ Comprehensive debug and override options
- ✅ Production validated with successful execution

---
**Status:** ✅ Production Ready  
**Implementation:** Business Hours Logic (08:00-18:00)  
**Validation:** RC: 0 Successful Execution  
**Ready for:** Full operational deployment
