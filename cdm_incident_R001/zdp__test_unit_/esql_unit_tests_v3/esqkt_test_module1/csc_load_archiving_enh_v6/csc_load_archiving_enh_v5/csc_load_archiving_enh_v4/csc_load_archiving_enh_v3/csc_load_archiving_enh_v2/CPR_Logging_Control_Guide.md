
# CPR Enhancement Logging Control Guide

## Overview

The CPR business day enhancement includes detailed logging for debugging purposes. By default, these logs are hidden during normal execution to keep the output clean.

## Normal Execution (No CPR Logs)

```bash
# Standard execution - CPR logs are hidden
./tpldauftr.sh -start -fil 50

# Explicit business day mode - still no verbose logs
./tpldauftr.sh -start -fil 50 --business-day-mode enabled
```

**Output:** Clean execution with minimal informational messages

## Debug Mode (Full CPR Logs)

```bash
# Enable debug mode to see all CPR logs
./tpldauftr.sh -dg -start -fil 50

# Debug with specific business day settings
./tpldauftr.sh -dg -start -fil 50 --business-day-mode enabled --cutoff-time 06:00
```

**Output:** Detailed CPR logs including:
- Business day mode status
- Cutoff time calculations
- SQL query building
- Date logic decisions

## Log Categories

### Normal Mode Logs (Always Visible)
- Process start/stop messages
- Error messages
- Critical warnings
- Final execution results

### Debug Mode Logs (Only with -dg)
- `CPR LOG: Building main kdauftrag query for filial X`
- `CPR LOG: Business day mode enabled but cutoff_date not set`
- `CPR LOG: Final main query SQL: select...`
- `CPR LOG: Current time: YYYY-MM-DD HH:MM:SS`
- `CPR LOG: Archive mode 'X' - using Y day`

## Environment Variable Control

You can also control debug mode via environment variable:

```bash
# Enable debug mode via environment
export DEBUG=1
./tpldauftr.sh -start -fil 50

# Disable debug mode
export DEBUG=0
./tpldauftr.sh -start -fil 50
```

## Production Recommendations

### For Production Systems:
- **Never use** `-dg` flag unless troubleshooting
- Keep logs clean for automated monitoring
- Use standard execution: `./tpldauftr.sh -start -fil 50`

### For Development/Testing:
- Use `-dg` flag to verify CPR logic
- Monitor cutoff calculations
- Validate SQL query generation

### For Troubleshooting:
- Always enable debug mode first: `-dg`
- Capture full log output for analysis
- Compare normal vs debug execution

## Log File Locations

Normal logs are written to: `$LOG_RESOURCE`
Debug logs are written to: `$LOG_RESOURCE` (same file, more verbose)

## Examples

### Clean Production Run
```bash
./tpldauftr.sh -start -fil 50
# Output: Minimal, clean logs suitable for monitoring
```

### Full Debug Analysis
```bash
./tpldauftr.sh -dg -start -fil 50 --business-day-mode enabled
# Output: Complete CPR enhancement details for troubleshooting
```

This approach ensures production systems remain clean while providing full debugging capabilities when needed.
