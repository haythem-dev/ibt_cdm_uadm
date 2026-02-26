
# LoadCSCArticlePrognosis Module Validation Plan
## Using esqltest SQL Testing Framework

**Analysis Date:** 2025-01-27  
**Module:** `pharmos.outbound.csc_load/dev/src/loadcscarticleprognosis`  
**Testing Framework:** esqltest (pharmos.base.libtpld)  
**Database:** ich21@zdev21_tcp

---

## EXECUTIVE SUMMARY

The `loadcscarticleprognosis` module is a critical component of the pharmos CSC_LOAD package responsible for transferring article prognosis data from inbound to outbound databases. This validation plan covers comprehensive testing of all module use cases using the esqltest framework with temporary tables and incremental loading simulation.

---

## MODULE ANALYSIS

### Core Functionality
The module performs the following operations:
1. **Incremental Data Loading** - Only processes records newer than last update
2. **MERGE Operations** - Updates existing records, inserts new ones
3. **Data Cleanup** - Removes outdated records from target table
4. **Batch Processing** - Commits every 50 records for performance
5. **Error Handling** - Manages database connectivity and transaction failures

### Key Tables
- **Source**: `artprog` (inbound database)
- **Target**: `cscarticleprognosis` (outbound database)
- **Temporary**: `tmp_artprog` (processing buffer)

### Data Fields
- `filialnr` (branch number) - Primary key component
- `artikel_nr` (article number) - Primary key component  
- `datum` (date) - Primary key component
- `prog_s` (prognosis value)
- `wprog_s` (weekly prognosis value)
- `lastupdatedate` (last update timestamp)

---

## VALIDATION TEST PHASES

### Phase 1: Table Structure Validation
- Create temporary tables mimicking source and target structures
- Validate table creation and constraints
- Test index creation and performance

### Phase 2: Data Loading Simulation
- Simulate inbound `artprog` data with various scenarios
- Test incremental loading logic with different date ranges
- Validate batch processing with large datasets

### Phase 3: MERGE Operation Testing
- Test INSERT operations for new records
- Test UPDATE operations for existing records
- Validate MERGE logic with mixed scenarios

### Phase 4: Data Cleanup Validation
- Test removal of outdated records
- Validate cursor-based deletion logic
- Test batch commit behavior (every 50 records)

### Phase 5: Error Handling & Edge Cases
- Test with empty source data
- Test with duplicate records
- Test transaction rollback scenarios
- Test large dataset performance

### Phase 6: Business Logic Validation
- Test branch-specific filtering
- Validate date-based incremental processing
- Test prognosis value calculations

---

## TECHNICAL IMPLEMENTATION DETAILS

### Temporary Table Strategy
- `temp_artprog_source`: Simulates inbound `artprog` table
- `temp_cscarticleprognosis`: Simulates target table
- `temp_processing_log`: Tracks processing statistics

### Date Handling
- Uses current date as `lastupdatedate` default
- Implements incremental loading based on `max(lastupdatedate)`
- Tests various date scenarios (past, current, future)

### Branch Testing
- Tests single branch processing (parameter-driven)
- Validates multi-branch data isolation
- Tests branch-specific cleanup logic

### Performance Validation
- Tests with datasets of varying sizes (10, 100, 1000+ records)
- Validates batch processing efficiency
- Measures transaction commit frequency

---

## SUCCESS CRITERIA

1. **Data Integrity**: All records processed without corruption
2. **Incremental Logic**: Only new/updated records processed
3. **MERGE Accuracy**: Existing records updated, new records inserted
4. **Cleanup Efficiency**: Outdated records removed correctly
5. **Performance**: Batch processing meets timing requirements
6. **Error Recovery**: Graceful handling of database issues

---

## RISK MITIGATION

### Data Loss Prevention
- Comprehensive backup simulation before cleanup
- Transaction rollback testing
- Validation of record count consistency

### Performance Optimization
- Index usage validation
- Batch size optimization testing
- Memory usage monitoring

### Error Handling
- Connection failure simulation
- Transaction timeout testing
- Rollback scenario validation
