# ESQL Unit Testing Framework - INI File Comprehensive Guide

## Overview

This document provides a complete reference for creating and configuring INI files for the ESQL unit testing framework used in the Informix database upgrade project. The framework uses configuration files to define test scenarios, database connections, and validation criteria for pharmaceutical modules.

## INI File Structure

### General Configuration Section

Every INI file must begin with a `[general]` section that defines global test parameters:

```ini
[general]
database=ich21@zdev21_tcp
test_capture_entries_count=10
persistent_connection=1
separator=;
timeout=300
commit_interval=50
```

#### General Section Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `database` | String | Required | Informix database connection string (format: `database@server_connection`) |
| `test_capture_entries_count` | Integer | 0 | Number of sample entries to capture for debugging |
| `persistent_connection` | Integer | 0 | Use persistent database connection (1=yes, 0=no) |
| `separator` | String | `;` | Field separator for result output |
| `timeout` | Integer | 120 | Transaction timeout in seconds |
| `commit_interval` | Integer | 50 | Number of operations before automatic commit |

### Transaction Sections

Each test operation is defined in a numbered transaction section:

```ini
[transaction00]
sql=CREATE TEMP TABLE temp_example (id INT, name VARCHAR(50));

[transaction01]
sql=INSERT INTO temp_example VALUES (1, 'Test Data');
```

#### Transaction Numbering Rules

- **Sequential numbering**: Must start from `00` and increment: `transaction00`, `transaction01`, `transaction02`, etc.
- **Zero-padding**: Always use two digits with leading zeros
- **No gaps**: Every number in sequence must be present
- **Maximum**: Framework supports up to `transaction99`

#### Transaction Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `sql` | String | Required | SQL statement to execute |
| `header` | String | Optional | Column headers for output formatting |
| `separator` | String | Global | Override global separator for this transaction |
| `limit` | Integer | Unlimited | Limit number of result rows returned |
| `skip` | Boolean | false | Skip this transaction (values: true/false, 1/0) |
| `sleep` | Integer | 0 | Sleep duration in seconds after execution |
| `repeat` | Integer | 1 | Number of times to repeat this transaction |

## SQL Statement Guidelines

### Informix Compatibility Requirements

#### Database References
All table references must include the full database qualifier:
```sql
-- Correct
FROM ich21@zdev21_tcp:systables

-- Incorrect
FROM systables
```

#### Temporary Tables
Use explicit temporary table creation:
```sql
CREATE TEMP TABLE ich21@zdev21_tcp:temp_tablename ( 
    column1 datatype,
    column2 datatype
);
```

#### Date Functions
Use Informix-specific date functions:
```sql
-- Current timestamp
lastchangedatetime DATETIME YEAR TO SECOND DEFAULT CURRENT

-- Date comparisons
WHERE date_field >= TODAY
WHERE date_field <= (TODAY + 30)
```

#### String Concatenation
Use proper Informix concatenation syntax:
```sql
-- Correct
SELECT (column1 || ';' || column2 || ';' || column3) AS result

-- For NULL handling
SELECT (NVL(column1, 'DEFAULT') || ';' || column2) AS result
```

### Result Formatting Standards

#### Single Result Format
For queries returning formatted results:
```sql
sql=SELECT (field1 || ';' || field2 || ';' || field3) AS result 
FROM table_name 
ORDER BY field1;
header=field1;field2;field3
```

#### Multiple Row Results
Use consistent field separators and ordering:
```sql
sql=SELECT (a.id || ';' || a.name || ';' || b.category || ';' || a.status) AS result 
FROM table_a a, table_b b 
WHERE a.category_id = b.id 
ORDER BY a.id, b.category;
header=id;name;category;status
limit=50
```

#### Count and Summary Queries
Format summary information consistently:
```sql
sql=SELECT (COUNT(*) || ';TOTAL_RECORDS') AS result 
FROM target_table
UNION ALL
SELECT (COUNT(*) || ';ACTIVE_RECORDS') AS result 
FROM target_table WHERE status = 'ACTIVE';
header=count;description
```

## Test Data Management

### Reference Data Setup
Create comprehensive reference data for testing:

```ini
# Create reference tables first
[transaction00]
sql=CREATE TEMP TABLE ich21@zdev21_tcp:temp_branches (
    branchno SMALLINT NOT NULL PRIMARY KEY,
    branchname VARCHAR(40),
    country VARCHAR(3),
    active_flag SMALLINT DEFAULT 1
);

# Load reference data with individual INSERT statements
[transaction01]
sql=INSERT INTO ich21@zdev21_tcp:temp_branches 
(branchno, branchname, country, active_flag) VALUES 
(10, 'Central Pharmacy Berlin', 'DE', 1);

[transaction02]
sql=INSERT INTO ich21@zdev21_tcp:temp_branches 
(branchno, branchname, country, active_flag) VALUES 
(20, 'Regional Pharmacy Munich', 'DE', 1);
```

### Test Scenario Data
Design test data to cover all business rule scenarios:

```ini
# Normal case
[transaction10]
sql=INSERT INTO temp_articles VALUES (100001, 'Normal Article', 'Y', 0, NULL);

# Exclusion case
[transaction11]
sql=INSERT INTO temp_articles VALUES (100002, 'Excluded Article', 'Y', 1, 'RESTRICTED');

# Edge case
[transaction12]
sql=INSERT INTO temp_articles VALUES (100003, 'Edge Case Article', 'N', 0, NULL);
```

## Advanced Testing Patterns

### Multi-Phase Testing Structure

Organize complex tests into logical phases:

```ini
# ==============================================================================
# PHASE 1: SETUP - Create tables and reference data
# ==============================================================================

[transaction00]
sql=CREATE TEMP TABLE...

# ==============================================================================
# PHASE 2: DATA LOADING - Insert test scenarios
# ==============================================================================

[transaction10]
sql=INSERT INTO...

# ==============================================================================
# PHASE 3: BUSINESS LOGIC TESTING - Validate core functionality
# ==============================================================================

[transaction20]
sql=SELECT...

# ==============================================================================
# PHASE 4: CLEANUP - Drop temporary tables
# ==============================================================================

[transaction90]
sql=DROP TABLE...
```

### Rule-Based Testing
Test individual business rules systematically:

```ini
# Test Rule 1: Narcotics exclusion
[transaction30]
sql=SELECT (article_no || ';' || article_name || ';' || narcotics_flag || ';NARCOTICS_RULE') AS result
FROM temp_articles a, temp_exclusion_rules r
WHERE r.rule_id = 1 AND r.activation_flag = 1
  AND a.narcotics_flag = CAST(r.parameter_value AS SMALLINT)
ORDER BY a.article_no;
header=article_no;article_name;narcotics_flag;rule_type

# Test Rule 2: UN Number exclusion  
[transaction31]
sql=SELECT (article_no || ';' || article_name || ';' || NVL(un_no, 'NULL') || ';UN_NO_RULE') AS result
FROM temp_articles a, temp_exclusion_rules r
WHERE r.rule_id = 2 AND r.activation_flag = 1
  AND a.un_no IS NOT NULL
ORDER BY a.article_no;
header=article_no;article_name;un_no;rule_type
```

### Performance Testing Patterns
Include performance validation in large-scale tests:

```ini
# Generate large dataset for performance testing
[transaction50]
sql=INSERT INTO temp_performance_test
SELECT (100000 + (MOD(rowid, 1000) + 1)),
       ('Generated Item ' || (MOD(rowid, 1000) + 1)),
       (MOD(rowid, 5) + 1),
       CURRENT
FROM temp_reference_data
WHERE rowid <= 10;

# Performance test with aggregation
[transaction51]
sql=SELECT (category || ';' || COUNT(*) || ';' || AVG(value) || ';' || MAX(timestamp)) AS result
FROM temp_performance_test
GROUP BY category
ORDER BY COUNT(*) DESC;
header=category;count;average_value;latest_timestamp
limit=20
```

## Error Handling and Validation

### Expected Failures
Test error conditions with intentional failures:

```ini
# Test constraint violation (should fail)
[transaction60]
sql=INSERT INTO temp_articles (article_no, article_name) 
VALUES (100001, 'Duplicate Article');
# This should fail due to primary key constraint - validates error handling
```

### Data Validation Checks
Include comprehensive validation queries:

```ini
# Data consistency check
[transaction70]
sql=SELECT ('CONSISTENCY_CHECK;' ||
           COUNT(DISTINCT a.article_no) || ';' ||
           COUNT(DISTINCT e.article_no) || ';' ||
           CASE WHEN COUNT(DISTINCT a.article_no) = COUNT(DISTINCT e.article_no)
                THEN 'CONSISTENT' ELSE 'INCONSISTENT' END) AS result
FROM temp_articles a
LEFT JOIN temp_export_permissions e ON a.article_no = e.article_no;
header=check_type;article_count;permission_count;status
```

### NULL Handling Tests
Validate NULL value processing:

```ini
[transaction75]
sql=SELECT (article_no || ';' ||
           NVL(optional_field, 'NULL_VALUE') || ';' ||
           CASE WHEN optional_field IS NULL THEN 'NULL' ELSE 'NOT_NULL' END) AS result
FROM temp_articles
WHERE article_no BETWEEN 100001 AND 100010
ORDER BY article_no;
header=article_no;field_value;null_status
```

## Best Practices

### 1. Structure and Organization
- Use consistent phase-based organization
- Group related transactions logically
- Include clear section headers with comments
- Number transactions sequentially without gaps

### 2. SQL Quality
- Always use fully qualified table names
- Include proper error handling for expected failures
- Use consistent result formatting across all queries
- Test both positive and negative scenarios

### 3. Data Management
- Create minimal but comprehensive test data
- Include edge cases and boundary conditions
- Clean up all temporary objects
- Validate data integrity at each phase

### 4. Documentation
- Include descriptive comments for each phase
- Document expected results and failure conditions
- Explain complex business logic
- Provide clear test objectives

### 5. Informix Compatibility
- Use Informix-specific date and time functions
- Test database-specific features that might change between versions
- Include performance benchmarks for comparison
- Validate transaction behavior and locking

## Common Patterns and Templates

### Basic CRUD Testing Template
```ini
[general]
database=ich21@zdev21_tcp
persistent_connection=1
separator=;
timeout=300

# CREATE
[transaction00]
sql=CREATE TEMP TABLE ich21@zdev21_tcp:temp_test (id INT, name VARCHAR(50));

# INSERT
[transaction01]
sql=INSERT INTO ich21@zdev21_tcp:temp_test VALUES (1, 'Test');

# SELECT
[transaction02]
sql=SELECT (id || ';' || name) AS result FROM ich21@zdev21_tcp:temp_test;
header=id;name

# UPDATE
[transaction03]
sql=UPDATE ich21@zdev21_tcp:temp_test SET name = 'Updated' WHERE id = 1;

# DELETE
[transaction04]
sql=DELETE FROM ich21@zdev21_tcp:temp_test WHERE id = 1;

# DROP
[transaction05]
sql=DROP TABLE ich21@zdev21_tcp:temp_test;
```

### Complex Business Logic Template
```ini
# Setup exclusion rules
[transaction00]
sql=CREATE TEMP TABLE ich21@zdev21_tcp:temp_rules (
    rule_id INT,
    rule_name VARCHAR(50),
    parameter_value VARCHAR(100),
    activation_flag INT DEFAULT 1
);

# Load test articles
[transaction01]
sql=CREATE TEMP TABLE ich21@zdev21_tcp:temp_articles (
    article_no INT,
    article_name VARCHAR(80),
    test_field VARCHAR(50)
);

# Test business rule application
[transaction02]
sql=SELECT (a.article_no || ';' || a.article_name || ';' ||
           CASE WHEN r.activation_flag = 1 AND a.test_field = r.parameter_value
                THEN 'EXCLUDED' ELSE 'ALLOWED' END) AS result
FROM ich21@zdev21_tcp:temp_articles a,
     ich21@zdev21_tcp:temp_rules r
WHERE r.rule_id = 1
ORDER BY a.article_no;
header=article_no;article_name;result_status
```

## Troubleshooting Common Issues

### Transaction Numbering Errors
- **Problem**: "Transaction not found" or gaps in execution
- **Solution**: Ensure sequential numbering from 00 with no gaps

### Database Connection Issues  
- **Problem**: Connection timeouts or access denied
- **Solution**: Verify database string format: `database@server_connection`

### SQL Syntax Errors
- **Problem**: Informix-specific syntax failures
- **Solution**: Use qualified table names and Informix date functions

### Result Formatting Issues
- **Problem**: Inconsistent output format
- **Solution**: Use standardized concatenation with separators

### Memory and Performance Issues
- **Problem**: Large dataset processing failures
- **Solution**: Use appropriate `commit_interval` and `timeout` values

## Validation Checklist

Before deploying INI files for testing:

- [ ] All transactions numbered sequentially from 00
- [ ] Database connection string correct
- [ ] All SQL statements use qualified table names
- [ ] Consistent result formatting with headers
- [ ] Comprehensive test data covering all scenarios
- [ ] Proper cleanup of temporary objects
- [ ] Error handling for expected failures
- [ ] Performance considerations for large datasets
- [ ] Informix-specific functions tested
- [ ] Documentation and comments complete

## Conclusion

This comprehensive guide provides the foundation for creating robust, maintainable INI files for ESQL unit testing. Following these patterns and best practices ensures reliable testing during database upgrades and system changes, particularly for the critical pharmaceutical business logic in the export article calculator and related modules.