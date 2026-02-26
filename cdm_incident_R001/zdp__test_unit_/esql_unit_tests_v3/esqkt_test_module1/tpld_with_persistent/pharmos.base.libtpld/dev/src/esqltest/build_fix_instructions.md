# Build Fix Instructions for Persistent Connection Feature

## Compilation Errors Analysis

The errors you're seeing are due to:

1. **VCR001 Warnings**: Function definitions missing - these are normal for C++ builds until linking
2. **ESQL Errors**: Duplicate statement IDs (`s1`, `c`) in the same ESQL compilation unit
3. **MSB8066**: Custom build failure due to ESQL preprocessing errors

## Quick Fix Solution

### Option 1: Minimal Implementation (Recommended)

Instead of duplicating the complex cursor logic, modify the existing `dbtest_transaction` function to support persistent mode:

1. **Add a global flag in dbio.cxx:**
```c
static int g_persistent_mode = 0;
static int g_connection_established = 0;
```

2. **Add mode control functions:**
```c
void dbio_set_persistent_mode(int enabled) {
    g_persistent_mode = enabled;
}

int dbio_get_persistent_mode(void) {
    return g_persistent_mode;
}
```

3. **Modify the existing `dbtest_transaction` function:**
   - Skip `connect` if `g_persistent_mode && g_connection_established`
   - Skip `disconnect` if `g_persistent_mode`
   - Track connection state

### Option 2: Build Configuration Fix

The ESQL compiler is conflicting because both functions use the same statement names. To fix:

1. **Rename all ESQL identifiers in the persistent function:**
   - `s1` → `stmt_p` 
   - `c` → `cursor_p`

2. **Ensure proper C linkage:**
```c
#ifdef __cplusplus
extern "C" {
#endif
// All function declarations here
#ifdef __cplusplus
}
#endif
```

## Immediate Working Solution

**For immediate testing, use the configuration approach:**

```ini
[general]
database=ich21@zdev21_tcp
single_connection_mode=yes  # Use existing connection logic differently
```

**Then modify esqltest.c to:**
1. Connect once at startup when `single_connection_mode=yes`
2. Skip connect/disconnect in `dbtest_transaction` when already connected
3. Disconnect once at program end

This avoids ESQL compilation issues while providing the temp table functionality your colleague needs.

## Testing Command
```cmd
esqlc -e dbio.cxx
```

If this fails, the ESQL statements have syntax conflicts that need resolution.