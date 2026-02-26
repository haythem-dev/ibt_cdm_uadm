# Package Analysis Tool

## Overview

This is a Streamlit-based web application designed to analyze various package formats and provide comprehensive insights about their structure, dependencies, and composition. The tool supports multiple package formats including ZIP, TAR, Python wheels (WHL), Python eggs (EGG), and compressed archives. It extracts packages, analyzes their contents, and provides detailed visualizations and metrics about file structure, code complexity, and package composition.

## User Preferences

Preferred communication style: Simple, everyday language.

## System Architecture

### Frontend Architecture
- **Framework**: Streamlit web framework for rapid prototyping and data application development
- **Layout**: Wide layout with expandable sidebar for controls and main area for analysis results
- **State Management**: Uses Streamlit's session state to maintain analysis data across interactions
- **User Interface**: File uploader component for package input with support for multiple archive formats

### Backend Architecture
- **Modular Design**: Organized into utility modules for specific analysis tasks
- **Package Extraction**: Supports multiple archive formats through format-specific extractors
- **Analysis Pipeline**: Three-stage analysis process covering file structure, code metrics, and visualization generation
- **Temporary File Management**: Uses temporary directories for safe package extraction and cleanup

### Core Analysis Components

#### Package Extractor (`utils.package_extractor`)
- Handles extraction of various package formats (ZIP, TAR, WHL, EGG, compressed archives)
- Uses appropriate Python libraries (zipfile, tarfile) based on file extension detection
- Implements safe extraction to temporary directories with cleanup capabilities

#### File Analyzer (`utils.file_analyzer`)
- Performs comprehensive directory structure analysis
- Calculates file statistics including size distribution, file types, and directory depth
- Uses MIME type detection for file classification
- Generates detailed metadata for each file including hash calculations

#### Code Analyzer (`utils.code_analyzer`)
- Language-specific code analysis for multiple programming languages
- Supports Python, JavaScript, TypeScript, Java, C/C++, PHP, Ruby, and Go
- Extracts metrics like lines of code, function count, class count, and complexity
- Performs import/dependency analysis to understand code relationships

#### Visualization Engine (`utils.visualizations`)
- Uses Plotly for interactive data visualizations
- Creates multiple chart types: pie charts for file distribution, histograms for size analysis
- Implements responsive design with appropriate handling of large datasets
- Groups smaller categories together for cleaner visualization

### Data Processing Strategy
- **File Type Classification**: Extension-based classification with MIME type fallback
- **Size Analysis**: Byte-level precision with MB conversion for human readability
- **Code Metrics**: AST parsing for accurate code structure analysis
- **Error Handling**: Comprehensive exception handling throughout the analysis pipeline

### Performance Considerations
- **Memory Management**: Uses generators and streaming where possible for large files
- **Temporary Storage**: Automatic cleanup of extracted files to prevent disk space issues
- **Lazy Loading**: Analysis data is computed on-demand and cached in session state

## External Dependencies

### Core Framework Dependencies
- **Streamlit**: Web application framework for data applications
- **Pandas**: Data manipulation and analysis library for handling analysis results
- **Plotly**: Interactive visualization library for charts and graphs

### File Processing Libraries
- **zipfile**: Built-in Python library for ZIP archive handling
- **tarfile**: Built-in Python library for TAR archive handling
- **mimetypes**: Built-in Python library for MIME type detection
- **pathlib**: Modern path handling for cross-platform compatibility

### Code Analysis Dependencies
- **ast**: Abstract Syntax Tree parsing for Python code analysis
- **re**: Regular expressions for pattern matching in various file types
- **hashlib**: Cryptographic hashing for file integrity and identification

### System Integration
- **tempfile**: Secure temporary file and directory creation
- **shutil**: High-level file operations including recursive directory removal
- **os**: Operating system interface for file system operations

### Data Structure Libraries
- **collections.defaultdict**: Efficient counting and grouping operations
- **collections**: Additional data structures for analysis aggregation