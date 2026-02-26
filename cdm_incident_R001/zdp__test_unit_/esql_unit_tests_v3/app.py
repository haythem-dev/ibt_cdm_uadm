import streamlit as st
import tempfile
import os
import shutil
from pathlib import Path
import pandas as pd
from utils.package_extractor import PackageExtractor
from utils.file_analyzer import FileAnalyzer
from utils.code_analyzer import CodeAnalyzer
from utils.visualizations import create_visualizations

# Page configuration
st.set_page_config(
    page_title="Package Analysis Tool",
    page_icon="📦",
    layout="wide",
    initial_sidebar_state="expanded"
)

# Initialize session state
if 'extracted_path' not in st.session_state:
    st.session_state.extracted_path = None
if 'analysis_data' not in st.session_state:
    st.session_state.analysis_data = None
if 'package_name' not in st.session_state:
    st.session_state.package_name = None

def reset_analysis():
    """Reset all analysis data and temporary files"""
    if st.session_state.extracted_path and os.path.exists(st.session_state.extracted_path):
        shutil.rmtree(st.session_state.extracted_path, ignore_errors=True)
    st.session_state.extracted_path = None
    st.session_state.analysis_data = None
    st.session_state.package_name = None

def main():
    st.title("📦 Package Analysis Tool")
    st.markdown("Upload and analyze package files to extract insights about structure, dependencies, and composition.")
    
    # Sidebar for upload and basic controls
    with st.sidebar:
        st.header("Package Upload")
        
        # File uploader
        uploaded_file = st.file_uploader(
            "Choose a package file",
            type=['zip', 'tar', 'gz', 'whl', 'egg'],
            help="Supported formats: ZIP, TAR, GZ, WHL (Python wheels), EGG"
        )
        
        if uploaded_file is not None:
            if st.button("Analyze Package", type="primary"):
                reset_analysis()  # Clear previous analysis
                
                with st.spinner("Extracting and analyzing package..."):
                    try:
                        # Create temporary directory for extraction
                        temp_dir = tempfile.mkdtemp()
                        
                        # Save uploaded file temporarily
                        temp_file_path = os.path.join(temp_dir, uploaded_file.name)
                        with open(temp_file_path, 'wb') as f:
                            f.write(uploaded_file.getvalue())
                        
                        # Extract package
                        extractor = PackageExtractor()
                        extracted_path = extractor.extract_package(temp_file_path, temp_dir)
                        
                        if extracted_path:
                            st.session_state.extracted_path = extracted_path
                            st.session_state.package_name = uploaded_file.name
                            
                            # Analyze files
                            analyzer = FileAnalyzer()
                            st.session_state.analysis_data = analyzer.analyze_directory(extracted_path)
                            
                            st.success("Package analyzed successfully!")
                        else:
                            st.error("Failed to extract package. Please check the file format.")
                            
                        # Clean up temporary file
                        os.remove(temp_file_path)
                        
                    except Exception as e:
                        st.error(f"Error analyzing package: {str(e)}")
        
        if st.session_state.analysis_data:
            st.header("Analysis Options")
            
            # Export options
            if st.button("Export Analysis Results"):
                try:
                    # Create analysis report
                    report_data = {
                        'Package Name': st.session_state.package_name,
                        'Total Files': st.session_state.analysis_data['total_files'],
                        'Total Size': st.session_state.analysis_data['total_size_mb'],
                        'File Types': len(st.session_state.analysis_data['file_types']),
                        'Directory Depth': st.session_state.analysis_data['max_depth']
                    }
                    
                    df = pd.DataFrame([report_data])
                    csv = df.to_csv(index=False)
                    
                    st.download_button(
                        label="Download Analysis Report (CSV)",
                        data=csv,
                        file_name=f"{st.session_state.package_name}_analysis_report.csv",
                        mime="text/csv"
                    )
                except Exception as e:
                    st.error(f"Error creating export: {str(e)}")
            
            if st.button("Clear Analysis"):
                reset_analysis()
                st.rerun()
    
    # Main content area
    if st.session_state.analysis_data is None:
        st.info("👆 Upload a package file from the sidebar to begin analysis")
        
        # Show supported formats
        col1, col2 = st.columns(2)
        
        with col1:
            st.subheader("Supported Package Formats")
            st.markdown("""
            - **ZIP files** (.zip)
            - **TAR archives** (.tar, .tar.gz, .tgz)
            - **Python wheels** (.whl)
            - **Python eggs** (.egg)
            - **Compressed archives** (.gz)
            """)
        
        with col2:
            st.subheader("Analysis Features")
            st.markdown("""
            - 📁 Interactive file tree browser
            - 📊 File type and size distribution
            - 📈 Package composition visualizations
            - 🔍 Code metrics and analysis
            - 📋 Dependency detection
            - 📤 Export analysis results
            """)
    
    else:
        # Display analysis results
        display_analysis_results()

def display_analysis_results():
    """Display comprehensive analysis results"""
    data = st.session_state.analysis_data
    
    # Package overview
    st.header(f"Analysis Results: {st.session_state.package_name}")
    
    # Key metrics
    col1, col2, col3, col4 = st.columns(4)
    
    with col1:
        st.metric("Total Files", data['total_files'])
    
    with col2:
        st.metric("Total Size", f"{data['total_size_mb']:.2f} MB")
    
    with col3:
        st.metric("File Types", len(data['file_types']))
    
    with col4:
        st.metric("Max Depth", data['max_depth'])
    
    # Create tabs for different analysis views
    tab1, tab2, tab3, tab4, tab5 = st.tabs([
        "📁 File Structure", 
        "📊 Statistics", 
        "📈 Visualizations", 
        "🔍 Code Analysis",
        "📋 Dependencies"
    ])
    
    with tab1:
        display_file_structure()
    
    with tab2:
        display_statistics()
    
    with tab3:
        display_visualizations()
    
    with tab4:
        display_code_analysis()
    
    with tab5:
        display_dependencies()

def display_file_structure():
    """Display interactive file tree structure"""
    st.subheader("File Tree Structure")
    
    if st.session_state.extracted_path:
        try:
            # Create expandable file tree
            file_tree = create_file_tree(st.session_state.extracted_path)
            
            # Display as expandable structure
            for item in file_tree:
                display_tree_item(item, 0)
                
        except Exception as e:
            st.error(f"Error displaying file structure: {str(e)}")

def create_file_tree(root_path):
    """Create a nested file tree structure"""
    tree = []
    
    try:
        root = Path(root_path)
        for item in sorted(root.iterdir()):
            if item.is_file():
                tree.append({
                    'name': item.name,
                    'type': 'file',
                    'size': item.stat().st_size,
                    'path': str(item)
                })
            elif item.is_dir():
                tree.append({
                    'name': item.name,
                    'type': 'directory',
                    'children': create_file_tree(str(item)),
                    'path': str(item)
                })
    except Exception as e:
        st.error(f"Error creating file tree: {str(e)}")
    
    return tree

def display_tree_item(item, depth):
    """Display a single tree item with proper indentation"""
    indent = "  " * depth
    
    if item['type'] == 'file':
        size_kb = item['size'] / 1024
        st.text(f"{indent}📄 {item['name']} ({size_kb:.1f} KB)")
    else:
        # Directory
        with st.expander(f"{indent}📁 {item['name']}", expanded=(depth < 2)):
            for child in item.get('children', []):
                display_tree_item(child, depth + 1)

def display_statistics():
    """Display detailed package statistics"""
    st.subheader("Package Statistics")
    data = st.session_state.analysis_data
    
    # File type breakdown
    st.write("**File Types Distribution:**")
    file_types_df = pd.DataFrame(list(data['file_types'].items()), columns=['Extension', 'Count'])
    file_types_df = file_types_df.sort_values('Count', ascending=False)
    st.dataframe(file_types_df, use_container_width=True)
    
    # Size distribution
    if 'file_sizes' in data:
        st.write("**File Size Analysis:**")
        sizes = data['file_sizes']
        size_stats = {
            'Total Files': len(sizes),
            'Average Size (KB)': sum(sizes) / len(sizes) / 1024 if sizes else 0,
            'Largest File (KB)': max(sizes) / 1024 if sizes else 0,
            'Smallest File (KB)': min(sizes) / 1024 if sizes else 0
        }
        
        stats_df = pd.DataFrame([size_stats])
        st.dataframe(stats_df, use_container_width=True)

def display_visualizations():
    """Display package visualization charts"""
    st.subheader("Package Visualizations")
    
    try:
        viz = create_visualizations(st.session_state.analysis_data)
        
        # File type distribution chart
        if viz['file_type_chart']:
            st.plotly_chart(viz['file_type_chart'], use_container_width=True)
        
        # File size distribution chart
        if viz['size_distribution_chart']:
            st.plotly_chart(viz['size_distribution_chart'], use_container_width=True)
        
        # Directory structure chart
        if viz['directory_chart']:
            st.plotly_chart(viz['directory_chart'], use_container_width=True)
            
    except Exception as e:
        st.error(f"Error creating visualizations: {str(e)}")

def display_code_analysis():
    """Display code analysis results"""
    st.subheader("Code Analysis")
    
    try:
        if st.session_state.extracted_path:
            code_analyzer = CodeAnalyzer()
            code_analysis = code_analyzer.analyze_code_files(st.session_state.extracted_path)
            
            if code_analysis['python_files']:
                st.write("**Python Files Analysis:**")
                
                col1, col2 = st.columns(2)
                
                with col1:
                    st.metric("Python Files", len(code_analysis['python_files']))
                    st.metric("Total Lines of Code", code_analysis['total_loc'])
                
                with col2:
                    st.metric("Functions Found", code_analysis['total_functions'])
                    st.metric("Classes Found", code_analysis['total_classes'])
                
                # Detailed file analysis
                if st.checkbox("Show Detailed Code Analysis"):
                    for file_info in code_analysis['python_files']:
                        with st.expander(f"📄 {file_info['filename']}"):
                            st.write(f"Lines of Code: {file_info['lines_of_code']}")
                            st.write(f"Functions: {len(file_info['functions'])}")
                            st.write(f"Classes: {len(file_info['classes'])}")
                            
                            if file_info['functions']:
                                st.write("Functions:", ", ".join(file_info['functions']))
                            if file_info['classes']:
                                st.write("Classes:", ", ".join(file_info['classes']))
            else:
                st.info("No Python files found for code analysis")
                
    except Exception as e:
        st.error(f"Error in code analysis: {str(e)}")

def display_dependencies():
    """Display dependency analysis"""
    st.subheader("Dependencies Analysis")
    
    try:
        if st.session_state.extracted_path:
            # Look for common dependency files
            dependency_files = find_dependency_files(st.session_state.extracted_path)
            
            if dependency_files:
                st.write("**Found Dependency Files:**")
                for dep_file in dependency_files:
                    st.write(f"📄 {dep_file}")
                    
                    # Try to read and display content
                    try:
                        with open(dep_file, 'r', encoding='utf-8') as f:
                            content = f.read()
                            if content.strip():
                                with st.expander(f"View {os.path.basename(dep_file)}"):
                                    st.code(content, language='text')
                    except Exception as e:
                        st.warning(f"Could not read {dep_file}: {str(e)}")
            else:
                st.info("No standard dependency files found (requirements.txt, setup.py, pyproject.toml, package.json, etc.)")
                
    except Exception as e:
        st.error(f"Error analyzing dependencies: {str(e)}")

def find_dependency_files(root_path):
    """Find common dependency files in the package"""
    dependency_patterns = [
        'requirements.txt', 'requirements-dev.txt', 'requirements-test.txt',
        'setup.py', 'setup.cfg', 'pyproject.toml', 'Pipfile', 'poetry.lock',
        'package.json', 'package-lock.json', 'yarn.lock',
        'Gemfile', 'Gemfile.lock', 'composer.json', 'composer.lock'
    ]
    
    found_files = []
    
    try:
        for root, dirs, files in os.walk(root_path):
            for file in files:
                if file in dependency_patterns:
                    found_files.append(os.path.join(root, file))
    except Exception as e:
        st.error(f"Error searching for dependency files: {str(e)}")
    
    return found_files

if __name__ == "__main__":
    main()
