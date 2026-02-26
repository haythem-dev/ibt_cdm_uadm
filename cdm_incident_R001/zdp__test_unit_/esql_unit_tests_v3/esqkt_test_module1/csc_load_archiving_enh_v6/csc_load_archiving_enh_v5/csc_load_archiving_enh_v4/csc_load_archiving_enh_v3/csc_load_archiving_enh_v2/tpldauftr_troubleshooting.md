# tpldauftr Script Failure Analysis & Troubleshooting

## Failure Summary (August 13, 2025 21:35:45)

### Error Details:
- **Return Code**: 2 (Technical problems preventing process start)
- **Process**: tpldauftr.bin with enhanced business day mode
- **Database**: ode21@zdev21_shm
- **Parameters**: -fil 50 -del 35
- **PID**: 2163370 (wrapper), 54526378 (main process)

### Log Analysis:
```
Command: ./tpldauftr.sh -start -fil 50
Enhanced Parameter: --business-day-mode enabled -fil 50 -db ode21@zdev21_shm -del 35
Status: Process could not be started - technical problems
Log Entry: tpldauft Main gestartet mit Tagen:35 fuer DB: ode21@zdev21_shm->0/0
```

## Root Cause Analysis

### 1. Database Connection Issues
The log shows `ode21@zdev21_shm->0/0` which suggests database connection problems.

#### Diagnostic Queries:
```sql
-- Check database connectivity
SELECT CURRENT FROM sysmaster:sysdual;

-- Verify database status
SELECT 
    name,
    is_online,
    is_logged,
    flags
FROM sysmaster:sysdatabases 
WHERE name = 'ode21';

-- Check shared memory configuration
SELECT 
    cf_effective as parameter_name,
    cf_original as original_value,
    cf_configured as configured_value
FROM sysconfig 
WHERE cf_name IN ('SHMVIRTSIZE', 'SHMADD', 'SHMTOTAL');
```

### 2. Process Permission Issues
#### Check File Permissions:
```bash
# Verify executable permissions
ls -la /software/wss/de/ksc/load/bin/tpldauftr.bin
ls -la /software/wss/de/ksc/load/proc/tpldauftr.sh

# Check directory permissions
ls -ld /software/wss/de/ksc/load/bin/
ls -ld /software/wss/de/ksc/load/proc/

# Verify user access
id h.benabdelaziz
groups h.benabdelaziz
```

### 3. Environment Configuration
#### Check Required Environment Variables:
```bash
# Database environment
echo $INFORMIXDIR
echo $INFORMIXSERVER
echo $ONCONFIG
echo $INFORMIXSQLHOSTS

# Application environment
echo $PATH | grep -E "(informix|ksc|load)"
echo $LD_LIBRARY_PATH

# Process limits
ulimit -a
```

### 4. Shared Memory Issues
#### Diagnose Shared Memory Problems:
```bash
# Check shared memory usage
ipcs -m | grep informix

# Check semaphores
ipcs -s | grep informix

# System limits
cat /proc/sys/kernel/shmmax
cat /proc/sys/kernel/shmall

# Current usage
free -h
```

## Immediate Troubleshooting Steps

### Step 1: Database Connectivity Test
```bash
# Test basic database connection
dbaccess ode21@zdev21_shm - <<EOF
SELECT 'Connection successful', CURRENT FROM sysmaster:sysdual;
EOF

# Check if database is in quiescent mode
onstat -d | grep ode21

# Verify server status
onstat -
```

### Step 2: Process Environment Validation
```bash
# Source environment files
source /software/wss/de/ksc/load/proc/set_env.sh  # if exists
source ~/.profile

# Test binary execution directly
/software/wss/de/ksc/load/bin/tpldauftr.bin --help

# Check for missing libraries
ldd /software/wss/de/ksc/load/bin/tpldauftr.bin
```

### Step 3: Parameter Validation
```bash
# Test with minimal parameters
./tpldauftr.sh -start -fil 10

# Test database parameter separately
./tpldauftr.sh -start -db ode21@zdev21_shm

# Validate business day mode parameter
./tpldauftr.sh -start --business-day-mode enabled
```

### Step 4: Log File Analysis
```bash
# Check for additional error logs
find /software/wss/de/ksc/load -name "*.log" -mtime -1 -exec ls -la {} \;

# Look for core dumps
find /software/wss/de/ksc/load -name "core*" -mtime -1

# Check system logs
tail -50 /var/log/messages | grep -E "(informix|tpldauftr|ode21)"
```

## Specific Error Resolution

### Database Connection 0/0 Error:
This typically indicates:

1. **Shared Memory Not Available**
```bash
# Restart Informix server if needed
onmode -ky  # Shutdown
oninit      # Restart

# Check shared memory cleanup
ipcs -m | grep $(whoami) | awk '{print $2}' | xargs -I{} ipcrm -m {}
```

2. **Database Not Online**
```sql
-- Bring database online
DATABASE ode21;
-- Check table accessibility
SELECT COUNT(*) FROM kdauftrag;
```

3. **Network/Host Issues**
```bash
# Test host connectivity
ping zdev21
telnet zdev21 9088  # Default Informix port

# Check sqlhosts file
cat $INFORMIXDIR/etc/sqlhosts | grep zdev21
```

### Parameter Enhancement Issues:
The CPR enhancement added `--business-day-mode enabled` parameter.

#### Validate Enhancement:
```bash
# Check if binary supports new parameter
strings /software/wss/de/ksc/load/bin/tpldauftr.bin | grep -i "business-day"

# Test without enhancement
/software/wss/de/ksc/load/bin/tpldauftr.bin -fil 50 -db ode21@zdev21_shm -del 35
```

## Recovery Actions

### Option 1: Environment Reset
```bash
# Reset database connection
unset INFORMIXSERVER INFORMIXDIR ONCONFIG
source /opt/informix/etc/informix.env  # Adjust path

# Restart database connection
dbaccess ode21@zdev21_shm - <<EOF
SELECT 'Test connection' FROM sysmaster:sysdual;
EOF
```

### Option 2: Alternative Execution
```bash
# Try direct binary execution
cd /software/wss/de/ksc/load/bin
./tpldauftr.bin -fil 50 -db ode21@zdev21_shm -del 35

# Or without business day enhancement temporarily
./tpldauftr.bin -fil 50 -db ode21@zdev21_shm -del 35 --legacy-mode
```

### Option 3: Manual Parameter Testing
```bash
# Test each parameter individually
./tpldauftr.sh -fil 50    # Test filter parameter
./tpldauftr.sh -del 35    # Test deletion parameter
./tpldauftr.sh -db ode21@zdev21_shm  # Test database parameter
```

## Prevention Measures

### 1. Pre-execution Checklist
```bash
# Create validation script
cat > /software/wss/de/ksc/load/proc/validate_env.sh << 'EOF'
#!/bin/bash
echo "=== Environment Validation ==="
echo "Database: $(dbaccess ode21@zdev21_shm - <<< 'SELECT CURRENT FROM sysmaster:sysdual;')"
echo "Binary: $(ls -la /software/wss/de/ksc/load/bin/tpldauftr.bin)"
echo "Shared Memory: $(ipcs -m | grep informix | wc -l) segments"
echo "Environment: INFORMIXSERVER=$INFORMIXSERVER"
EOF
chmod +x /software/wss/de/ksc/load/proc/validate_env.sh
```

### 2. Enhanced Logging
Add to tpldauftr.sh:
```bash
# Before execution, log environment
echo "DEBUG: INFORMIXSERVER=$INFORMIXSERVER" >> tpldauftr.debug.log
echo "DEBUG: Database test:" >> tpldauftr.debug.log
dbaccess ode21@zdev21_shm - <<< 'SELECT CURRENT FROM sysmaster:sysdual;' >> tpldauftr.debug.log 2>&1
```

## Next Steps

1. **Immediate**: Run database connectivity test
2. **Short-term**: Execute environment validation script
3. **Medium-term**: Implement enhanced logging
4. **Long-term**: Create automated health checks

## Success Indicators

✅ **Ready to Retry When:**
- Database connection returns valid timestamp
- Shared memory segments properly allocated
- Binary executes with --help parameter
- All environment variables properly set
- No core dumps or error logs generated

---
**Failure Time**: August 13, 2025 21:35:45  
**Analysis Date**: August 13, 2025  
**Next Action**: Database connectivity validation