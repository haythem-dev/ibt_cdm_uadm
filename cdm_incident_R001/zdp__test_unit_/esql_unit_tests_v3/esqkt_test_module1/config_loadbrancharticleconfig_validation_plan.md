# LoadBranchArticleConfig Module Validation Plan
## Using esqltest SQL Testing Framework

**Analysis Date:** 2025-08-27  
**Module:** `pharmos.outbound.csc_load/dev/src/loadbrancharticleconfig`  
**Testing Framework:** esqltest (pharmos.base.libtpld)  
**Database:** ich21@zdev21_tcp

---

## EXECUTIVE SUMMARY

The `loadbrancharticleconfig` module is a critical component of the pharmos CSC_LOAD package responsible for loading and managing IBT (Internet-Based Trading) branch article configurations. This validation plan covers comprehensive testing of all module use cases using the esqltest framework with temporary tables and persistent connections.

---

## MODULE ANALYSIS

### Core Functionality
Based on source code analysis, `loadbrancharticleconfig` performs:

1. **IBT Branch Article Configuration Loading**
   - Loads article configuration data for specific branches
   - Manages IBT type associations and assortment types
   - Handles article-branch relationships

2. **Table Operations**
   - Primary table: `IBTBRANCHARTICLECONFIG`
   - Processing table: `PUIBTBRANCHARTICLECONFIG` 
   - Article local processing with WAFO type updates

3. **Key Data Structures**
   ```c
   struct N_IBTBRANCHARTICLECONFIG {
       long articleno;              // Article number
       short branchno;              // Branch number
       short ibttypeid;             // IBT type identifier
       short ibtassortmenttypeid;   // IBT assortment type
       short ibtonlyflag;           // IBT-only flag
       datetime lastchangedatetime; // Last change timestamp
   }

   struct N_PUIBTBRANCHARTICLECONFIG {
       long ARTICLENO;              // Article number
       short BRANCHNO;              // Branch number
       short IBTROLETYPEID;         // IBT role type
       short ASSORTMENTTYPEID;      // Assortment type
       datetime LASTCHANGEDATETIME; // Last change timestamp
       short RELEVANCEFLAG;         // Relevance flag
   }
   ```

### Processing Flow
1. **PrepareLoadTables()** - Initialize loading environment
2. **StartLoadingIBTBranchArticle()** - Begin main processing
3. **LoadTables()** - Execute table loading operations
4. **AfterLoadTables()** - Cleanup and finalization

---

## VALIDATION USE CASES

### UC1: Basic Table Creation and Structure Validation
**Objective:** Validate temporary table creation matching production schema
**Coverage:** Table structure, constraints, data types

### UC2: Article Configuration Data Loading
**Objective:** Test insertion of article-branch configurations
**Coverage:** INSERT operations, data validation, constraint checking

### UC3: Branch-Specific Article Management
**Objective:** Test branch-specific article filtering and processing
**Coverage:** Branch number filtering, article associations

### UC4: IBT Type Management
**Objective:** Validate IBT type assignments and updates
**Coverage:** Type ID management, assortment type processing

### UC5: Data Synchronization Testing
**Objective:** Test synchronization between source and target tables
**Coverage:** PUIBT to IBT data flow, transformation rules

### UC6: Update Operations
**Objective:** Test article configuration updates
**Coverage:** UPDATE statements, timestamp handling, flag management

### UC7: Batch Processing Validation
**Objective:** Test batch processing with commit intervals
**Coverage:** Transaction management, batch sizes, commit frequency

### UC8: Error Handling and Recovery
**Objective:** Validate error conditions and recovery mechanisms
**Coverage:** SQL error handling, rollback scenarios, data consistency

### UC9: Performance and Load Testing
**Objective:** Test performance with realistic data volumes
**Coverage:** Large dataset processing, memory usage, execution time

### UC10: Data Quality and Integrity
**Objective:** Validate data quality checks and constraints
**Coverage:** Data validation rules, referential integrity, business rules

---

## TECHNICAL TESTING APPROACH

### Testing Environment
- **Database:** ich21@zdev21_tcp (development environment)
- **Connection Type:** Persistent connections for transaction consistency
- **Table Strategy:** Temporary tables to avoid production data interference
- **Cleanup Strategy:** Automatic cleanup after each test cycle

### Test Data Strategy
- **Volume Testing:** 1000+ article configurations per test
- **Branch Coverage:** Multiple branches (10, 20, 30, 40, 50)
- **Article Range:** Articles 100001-105000 for testing
- **Type Variations:** Multiple IBT types and assortment types
- **Time Ranges:** Current and historical timestamp testing

### Transaction Management
- **Commit Intervals:** Every 50 records (matching module behavior)
- **Transaction Isolation:** Work unit boundaries for consistency
- **Rollback Testing:** Deliberate error injection for rollback validation

---

## ESQLTEST CONFIGURATION STRUCTURE

The validation uses the following test structure:

1. **Setup Phase (Transactions 00-05)**
   - Create temporary tables
   - Initialize test data
   - Set up reference data

2. **Core Testing Phase (Transactions 06-20)**
   - Article configuration loading tests
   - Branch-specific processing
   - Type management validation

3. **Synchronization Phase (Transactions 21-25)**
   - Source to target data flow
   - Transformation validation
   - Data consistency checks

4. **Update Phase (Transactions 26-30)**
   - Configuration updates
   - Timestamp handling
   - Flag management

5. **Performance Phase (Transactions 31-35)**
   - Batch processing tests
   - Large dataset handling
   - Commit interval validation

6. **Cleanup Phase (Transactions 36-40)**
   - Table cleanup
   - Connection management
   - Resource deallocation

---

## EXPECTED OUTCOMES

### Success Criteria
1. **Functional Validation**
   - All table operations execute successfully
   - Data transformations work correctly
   - Business rules are enforced

2. **Performance Validation**
   - Processing times within acceptable limits
   - Memory usage stays within bounds
   - Commit intervals function properly

3. **Data Integrity**
   - No data corruption or loss
   - Referential integrity maintained
   - Consistent timestamps and flags

### Key Metrics
- **Processing Speed:** >500 records/second for bulk operations
- **Memory Usage:** <100MB for typical batch sizes
- **Transaction Success Rate:** 100% for valid data
- **Error Recovery:** 100% successful rollback on errors

---

## RISK MITIGATION

### Data Safety
- Exclusive use of temporary tables
- No production data modification
- Automatic cleanup procedures

### Performance Impact
- Development database usage only
- Off-peak testing windows
- Resource monitoring during tests

### Test Reliability
- Repeatable test scenarios
- Consistent test data
- Automated validation checks

---

## IMPLEMENTATION TIMELINE

### Phase 1: Test Configuration (Days 1-2)
- Complete INI configuration file
- Validate esqltest framework setup
- Initial connectivity testing

### Phase 2: Core Validation (Days 3-5)
- Execute basic functionality tests
- Validate data loading operations
- Test error handling scenarios

### Phase 3: Advanced Testing (Days 6-8)
- Performance and load testing
- Data synchronization validation
- Comprehensive regression testing

### Phase 4: Documentation (Days 9-10)
- Test results analysis
- Validation report preparation
- Recommendations and findings

---

## TECHNICAL SPECIFICATIONS

### Database Configuration
```
Database: ich21@zdev21_tcp
Connection: Persistent (persistent_connection=1)
Timeout: 300 seconds
Commit Interval: 50 records
Separator: semicolon (;)
```

### Table Naming Convention
```
temp_ibtbrancharticleconfig    - Main configuration table
temp_puibtbrancharticleconfig  - Processing/staging table
temp_article_reference         - Article reference data
temp_branch_reference          - Branch reference data
```

### Test Data Ranges
```
Article Numbers: 100001-105000
Branch Numbers: 10, 20, 30, 40, 50
IBT Type IDs: 1-10
Assortment Type IDs: 1-5
```

This comprehensive validation plan ensures thorough testing of the loadbrancharticleconfig module while maintaining data integrity and providing meaningful validation results.