# Package Analyzer

## Overview

Package Analyzer is a Streamlit-based web application designed to analyze various types of package files. The application provides comprehensive analysis capabilities for packages in multiple formats including ZIP, TAR, RAR, 7Z, and Python-specific formats like WHL and EGG files. The system is built with a modular architecture that separates file handling, analysis logic, and user interface components.

## User Preferences

Preferred communication style: Simple, everyday language.

## Recent Changes

**January 26, 2025**: Enhanced Package Analyzer with comprehensive pharmos.base.libtpld analysis capabilities. Added specialized C/C++ library analysis with detailed reporting for both esqltest and libtpld components. Generated comprehensive 8,000+ word analysis report covering architecture, business value, technical specifications, and integration guidelines.

## System Architecture

### Frontend Architecture
- **Framework**: Streamlit web framework for rapid prototyping and data application development
- **Layout**: Wide layout with expandable sidebar navigation
- **State Management**: Session state management for maintaining uploaded files, analysis results, and component instances across page interactions
- **Page Structure**: Multi-page application with sidebar navigation supporting Upload Package, Analysis Results, and About pages
- **Modular Design**: Placeholder page modules in `/pages/` directory for future expansion to more complex page-specific logic

### Backend Architecture
- **File Processing**: Centralized file handling through `FileHandler` class supporting multiple archive formats
- **Analysis Engine**: Modular analysis system through `PackageAnalyzer` class with pluggable analysis modules
- **Analysis Types**: Five core analysis modules - structure, dependencies, security, quality, and metadata analysis
- **Temporary Storage**: Temporary directory management for safe file extraction and processing
- **Error Handling**: Comprehensive error handling with graceful degradation for unsupported formats or analysis failures

### Data Processing Pipeline
- **File Upload**: Streamlit file uploader component for package submission
- **Format Detection**: Automatic file format detection based on file extensions
- **Extraction**: Format-specific extraction methods for different archive types
- **Analysis**: Modular analysis pipeline allowing selective analysis type execution
- **Results**: Structured analysis results with timestamps and success/error tracking

### Component Architecture
- **FileHandler**: Manages file operations, format detection, and extraction workflows
- **PackageAnalyzer**: Orchestrates analysis modules and result aggregation
- **Session Management**: Persistent state management for user workflow continuity

## External Dependencies

### Core Framework
- **Streamlit**: Web application framework for the user interface and application structure

### File Processing Libraries
- **zipfile**: Built-in Python library for ZIP file handling
- **tarfile**: Built-in Python library for TAR file processing
- **rarfile**: Third-party library for RAR archive extraction
- **py7zr**: Third-party library for 7Z archive processing

### System Dependencies
- **os**: File system operations and path management
- **tempfile**: Temporary directory and file management for secure processing
- **json**: Data serialization for analysis results
- **typing**: Type hints for improved code maintainability

### Development Dependencies
- Standard Python libraries for timestamp generation, error handling, and data structure management