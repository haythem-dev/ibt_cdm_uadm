import plotly.express as px
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import pandas as pd

def create_visualizations(analysis_data):
    """
    Create various visualizations for package analysis
    
    Args:
        analysis_data (dict): Analysis data from FileAnalyzer
        
    Returns:
        dict: Dictionary containing plotly figures
    """
    try:
        visualizations = {
            'file_type_chart': create_file_type_chart(analysis_data),
            'size_distribution_chart': create_size_distribution_chart(analysis_data),
            'directory_chart': create_directory_structure_chart(analysis_data),
            'largest_files_chart': create_largest_files_chart(analysis_data)
        }
        
        return visualizations
        
    except Exception as e:
        print(f"Error creating visualizations: {str(e)}")
        return {
            'file_type_chart': None,
            'size_distribution_chart': None,
            'directory_chart': None,
            'largest_files_chart': None
        }

def create_file_type_chart(analysis_data):
    """Create pie chart of file type distribution"""
    try:
        file_types = analysis_data.get('file_types', {})
        
        if not file_types:
            return None
        
        # Convert to DataFrame for easier manipulation
        df = pd.DataFrame(list(file_types.items()), columns=['Extension', 'Count'])
        df = df.sort_values('Count', ascending=False)
        
        # Group small categories together
        if len(df) > 10:
            top_extensions = df.head(9)
            others_count = df.tail(-9)['Count'].sum()
            
            if others_count > 0:
                others_row = pd.DataFrame([['Others', others_count]], columns=['Extension', 'Count'])
                df = pd.concat([top_extensions, others_row], ignore_index=True)
        
        # Create pie chart
        fig = px.pie(
            df, 
            values='Count', 
            names='Extension',
            title='File Type Distribution',
            hover_data={'Count': True}
        )
        
        fig.update_traces(textposition='inside', textinfo='percent+label')
        fig.update_layout(showlegend=True, height=500)
        
        return fig
        
    except Exception as e:
        print(f"Error creating file type chart: {str(e)}")
        return None

def create_size_distribution_chart(analysis_data):
    """Create histogram of file size distribution"""
    try:
        file_sizes = analysis_data.get('file_sizes', [])
        
        if not file_sizes:
            return None
        
        # Convert bytes to KB for better readability
        file_sizes_kb = [size / 1024 for size in file_sizes if size > 0]
        
        if not file_sizes_kb:
            return None
        
        # Create histogram
        fig = px.histogram(
            x=file_sizes_kb,
            nbins=min(50, len(set(file_sizes_kb))),
            title='File Size Distribution',
            labels={'x': 'File Size (KB)', 'y': 'Number of Files'},
            marginal='box'  # Add box plot on top
        )
        
        fig.update_layout(
            showlegend=False,
            height=500,
            xaxis_title='File Size (KB)',
            yaxis_title='Number of Files'
        )
        
        return fig
        
    except Exception as e:
        print(f"Error creating size distribution chart: {str(e)}")
        return None

def create_directory_structure_chart(analysis_data):
    """Create treemap visualization of directory structure"""
    try:
        file_details = analysis_data.get('file_details', [])
        
        if not file_details:
            return None
        
        # Prepare data for treemap
        df = pd.DataFrame(file_details)
        
        # Group by directory
        df['directory'] = df['path'].apply(lambda x: '/'.join(x.split('/')[:-1]) if '/' in x else 'root')
        
        # Aggregate by directory
        dir_stats = df.groupby('directory').agg({
            'size': 'sum',
            'name': 'count'
        }).reset_index()
        
        dir_stats.columns = ['Directory', 'Total Size (bytes)', 'File Count']
        dir_stats['Size (MB)'] = dir_stats['Total Size (bytes)'] / (1024 * 1024)
        
        # Create treemap
        fig = px.treemap(
            dir_stats,
            path=['Directory'],
            values='Total Size (bytes)',
            title='Directory Size Distribution',
            hover_data={'File Count': True, 'Size (MB)': ':.2f'}
        )
        
        fig.update_layout(height=500)
        
        return fig
        
    except Exception as e:
        print(f"Error creating directory structure chart: {str(e)}")
        return None

def create_largest_files_chart(analysis_data):
    """Create bar chart of largest files"""
    try:
        largest_files = analysis_data.get('largest_files', [])
        
        if not largest_files or len(largest_files) == 0:
            return None
        
        # Take top 15 largest files
        top_files = largest_files[:15]
        
        df = pd.DataFrame(top_files)
        df['size_mb'] = df['size'] / (1024 * 1024)
        
        # Create horizontal bar chart
        fig = px.bar(
            df,
            x='size_mb',
            y='name',
            orientation='h',
            title='Largest Files in Package',
            labels={'size_mb': 'File Size (MB)', 'name': 'File Name'},
            hover_data={'path': True, 'extension': True}
        )
        
        fig.update_layout(
            height=max(400, len(top_files) * 30),
            yaxis={'categoryorder': 'total ascending'}
        )
        
        return fig
        
    except Exception as e:
        print(f"Error creating largest files chart: {str(e)}")
        return None

def create_code_metrics_chart(code_analysis):
    """Create visualization for code metrics"""
    try:
        if not code_analysis or 'python_files' not in code_analysis:
            return None
        
        python_files = code_analysis['python_files']
        
        if not python_files:
            return None
        
        # Prepare data
        df = pd.DataFrame([{
            'filename': f['filename'],
            'lines_of_code': f['lines_of_code'],
            'functions': len(f.get('functions', [])),
            'classes': len(f.get('classes', [])),
            'comments': f.get('comments', 0)
        } for f in python_files])
        
        # Create subplot with secondary y-axis
        fig = make_subplots(
            rows=2, cols=2,
            subplot_titles=('Lines of Code', 'Functions per File', 'Classes per File', 'Comments per File'),
            specs=[[{"secondary_y": False}, {"secondary_y": False}],
                   [{"secondary_y": False}, {"secondary_y": False}]]
        )
        
        # Lines of code
        fig.add_trace(
            go.Bar(x=df['filename'], y=df['lines_of_code'], name='LOC'),
            row=1, col=1
        )
        
        # Functions
        fig.add_trace(
            go.Bar(x=df['filename'], y=df['functions'], name='Functions'),
            row=1, col=2
        )
        
        # Classes
        fig.add_trace(
            go.Bar(x=df['filename'], y=df['classes'], name='Classes'),
            row=2, col=1
        )
        
        # Comments
        fig.add_trace(
            go.Bar(x=df['filename'], y=df['comments'], name='Comments'),
            row=2, col=2
        )
        
        fig.update_layout(
            height=600,
            title_text="Code Metrics Analysis",
            showlegend=False
        )
        
        # Update x-axis labels to be rotated for readability
        fig.update_xaxes(tickangle=45)
        
        return fig
        
    except Exception as e:
        print(f"Error creating code metrics chart: {str(e)}")
        return None

def create_imports_analysis_chart(code_analysis):
    """Create chart showing most commonly imported modules"""
    try:
        imports_data = code_analysis.get('imports_analysis', {})
        
        if not imports_data:
            return None
        
        # Convert to DataFrame and get top imports
        df = pd.DataFrame(list(imports_data.items()), columns=['Module', 'Count'])
        df = df.sort_values('Count', ascending=False).head(20)
        
        if df.empty:
            return None
        
        # Create horizontal bar chart
        fig = px.bar(
            df,
            x='Count',
            y='Module',
            orientation='h',
            title='Most Frequently Imported Modules',
            labels={'Count': 'Number of Imports', 'Module': 'Module Name'}
        )
        
        fig.update_layout(
            height=max(400, len(df) * 25),
            yaxis={'categoryorder': 'total ascending'}
        )
        
        return fig
        
    except Exception as e:
        print(f"Error creating imports analysis chart: {str(e)}")
        return None
