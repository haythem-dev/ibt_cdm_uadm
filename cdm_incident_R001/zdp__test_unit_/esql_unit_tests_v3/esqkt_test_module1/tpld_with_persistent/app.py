import streamlit as st
import os
from utils.file_handler import FileHandler
from utils.analyzer import PackageAnalyzer

def main():
    """Main application entry point"""
    st.set_page_config(
        page_title="Package Analyzer",
        page_icon="📦",
        layout="wide",
        initial_sidebar_state="expanded"
    )
    
    # Initialize session state
    if 'uploaded_file' not in st.session_state:
        st.session_state.uploaded_file = None
    if 'analysis_results' not in st.session_state:
        st.session_state.analysis_results = None
    if 'file_handler' not in st.session_state:
        st.session_state.file_handler = FileHandler()
    if 'analyzer' not in st.session_state:
        st.session_state.analyzer = PackageAnalyzer()
    
    # Main header
    st.title("📦 Package Analyzer")
    st.markdown("---")
    
    # Sidebar navigation
    st.sidebar.title("Navigation")
    page = st.sidebar.selectbox(
        "Choose a page",
        ["Upload Package", "Analysis Results", "About"]
    )
    
    # Page routing
    if page == "Upload Package":
        render_upload_page()
    elif page == "Analysis Results":
        render_analysis_page()
    elif page == "About":
        render_about_page()

def render_upload_page():
    """Render the package upload page"""
    st.header("Upload Package for Analysis")
    
    # File upload section
    st.subheader("Select Package File")
    uploaded_file = st.file_uploader(
        "Choose a package file",
        type=['zip', 'tar', 'gz', 'tgz', 'tar.gz', 'rar', '7z', 'whl', 'egg'],
        help="Supported formats: ZIP, TAR, GZ, RAR, 7Z, Python Wheels, etc."
    )
    
    if uploaded_file is not None:
        # Display file information
        st.success(f"File uploaded successfully: {uploaded_file.name}")
        
        col1, col2, col3 = st.columns(3)
        with col1:
            st.metric("File Name", uploaded_file.name)
        with col2:
            st.metric("File Size", f"{uploaded_file.size / 1024:.2f} KB")
        with col3:
            st.metric("File Type", uploaded_file.type or "Unknown")
        
        # Store uploaded file in session state
        st.session_state.uploaded_file = uploaded_file
        
        # Process file button
        if st.button("Process Package", type="primary"):
            with st.spinner("Processing package..."):
                try:
                    # Process the uploaded file
                    processing_result = st.session_state.file_handler.process_file(uploaded_file)
                    
                    if processing_result['success']:
                        st.success("Package processed successfully!")
                        st.info("Navigate to 'Analysis Results' to view the analysis.")
                        
                        # Store processing results
                        st.session_state.processing_result = processing_result
                    else:
                        st.error(f"Failed to process package: {processing_result['error']}")
                        
                except Exception as e:
                    st.error(f"Error processing package: {str(e)}")
    
    else:
        st.info("Please upload a package file to begin analysis.")
        
        # Display supported formats
        st.subheader("Supported Package Formats")
        formats = {
            "Archive Formats": ["ZIP", "TAR", "GZ", "TGZ", "RAR", "7Z"],
            "Python Packages": ["Wheel (.whl)", "Egg (.egg)"],
            "Source Distributions": ["tar.gz", "zip"]
        }
        
        for category, format_list in formats.items():
            st.write(f"**{category}:** {', '.join(format_list)}")

def render_analysis_page():
    """Render the analysis results page"""
    st.header("Analysis Results")
    
    # Check if we have the pharmos package analysis
    if os.path.exists("pharmos.base.libtpld"):
        st.success("📦 pharmos.base.libtpld package detected in project!")
        
        if st.button("🔍 Analyze pharmos.base.libtpld Package", type="primary"):
            with st.spinner("Analyzing pharmos package components..."):
                # Load and display the comprehensive analysis
                if os.path.exists("pharmos_report.md"):
                    with open("pharmos_report.md", "r") as f:
                        report_content = f.read()
                    
                    st.markdown("---")
                    st.markdown(report_content)
                    
                    # Add download button for the report
                    st.download_button(
                        label="📄 Download Full Analysis Report",
                        data=report_content,
                        file_name="pharmos_analysis_report.md",
                        mime="text/markdown"
                    )
        
        # Show quick package overview
        st.subheader("Package Overview")
        col1, col2, col3 = st.columns(3)
        
        with col1:
            st.metric("Components", "2", help="esqltest + libtpld")
        with col2:
            st.metric("Language", "C/C++", help="Multi-platform C library")
        with col3:
            st.metric("Type", "Enterprise Library", help="Database testing & data conversion")
        
        # Component summary
        st.subheader("Component Summary")
        
        col1, col2 = st.columns(2)
        
        with col1:
            st.markdown("""
            #### 🗃️ ESQLTEST
            **SQL Testing Framework**
            - Database transaction testing
            - INI-based configuration
            - Macro expansion system
            - Performance monitoring
            - Cross-platform support
            """)
        
        with col2:
            st.markdown("""
            #### 🔄 LIBTPLD  
            **Data Processing Library**
            - ASCII/COBOL conversion
            - Legacy data migration
            - EBCDIC encoding support
            - System utilities
            - Logging framework
            """)
        
        # File structure overview
        if st.checkbox("Show File Structure"):
            st.subheader("Package Structure")
            
            # Display directory tree
            base_path = "pharmos.base.libtpld/dev/src"
            if os.path.exists(base_path):
                for root, dirs, files in os.walk(base_path):
                    level = root.replace(base_path, '').count(os.sep)
                    indent = ' ' * 2 * level
                    st.text(f"{indent}{os.path.basename(root)}/")
                    subindent = ' ' * 2 * (level + 1)
                    for file in files[:5]:  # Show first 5 files
                        st.text(f"{subindent}{file}")
                    if len(files) > 5:
                        st.text(f"{subindent}... and {len(files) - 5} more files")
        
        return
    
    # Original analysis for uploaded packages
    if st.session_state.uploaded_file is None:
        st.warning("No package uploaded yet. Please upload a package first.")
        if st.button("Go to Upload Page"):
            st.rerun()
    else:
        st.info("Analysis functionality will be implemented here.")
        st.write(f"**Current Package:** {st.session_state.uploaded_file.name}")
        
        # Placeholder for future analysis features
        st.subheader("Analysis Features (Coming Soon)")
        
        analysis_features = [
            "📄 File Structure Analysis",
            "🔍 Dependency Analysis",
            "🛡️ Security Vulnerability Scan",
            "📊 Code Quality Metrics",
            "📈 Package Statistics",
            "🏷️ Metadata Extraction"
        ]
        
        for feature in analysis_features:
            st.write(f"- {feature}")
        
        # Clear package button
        if st.button("Clear Current Package", type="secondary"):
            st.session_state.uploaded_file = None
            st.session_state.analysis_results = None
            if 'processing_result' in st.session_state:
                del st.session_state.processing_result
            st.success("Package cleared successfully!")
            st.rerun()

def render_about_page():
    """Render the about page"""
    st.header("About Package Analyzer")
    
    st.markdown("""
    ## Overview
    Package Analyzer is a comprehensive tool for analyzing various types of software packages. 
    This application provides insights into package structure, dependencies, security vulnerabilities, 
    and code quality metrics.
    
    ## Features
    - **Multi-format Support**: Analyze various package formats including ZIP, TAR, Python wheels, etc.
    - **Extensible Architecture**: Built with modularity in mind for easy feature additions
    - **User-friendly Interface**: Clean and intuitive web interface powered by Streamlit
    - **Comprehensive Analysis**: Future support for dependency analysis, security scanning, and more
    
    ## How to Use
    1. **Upload**: Navigate to the 'Upload Package' page and select your package file
    2. **Process**: Click 'Process Package' to prepare the file for analysis
    3. **Analyze**: View results in the 'Analysis Results' page (features coming soon)
    
    ## Supported Formats
    - Archive formats: ZIP, TAR, GZ, RAR, 7Z
    - Python packages: Wheels (.whl), Eggs (.egg)
    - Source distributions and more
    
    ## Version
    Current Version: 1.0.0 (Foundation Release)
    """)

if __name__ == "__main__":
    main()
