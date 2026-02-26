import os
import mimetypes
from pathlib import Path
from collections import defaultdict
import hashlib

class FileAnalyzer:
    """Class for analyzing file structure and properties"""
    
    def __init__(self):
        # Initialize mime types
        mimetypes.init()
    
    def analyze_directory(self, directory_path):
        """
        Analyze a directory and return comprehensive statistics
        
        Args:
            directory_path (str): Path to directory to analyze
            
        Returns:
            dict: Analysis results containing file statistics and metadata
        """
        analysis_data = {
            'total_files': 0,
            'total_directories': 0,
            'total_size_bytes': 0,
            'total_size_mb': 0.0,
            'file_types': defaultdict(int),
            'file_sizes': [],
            'largest_files': [],
            'directory_structure': {},
            'max_depth': 0,
            'mime_types': defaultdict(int),
            'file_details': []
        }
        
        try:
            self._analyze_recursive(directory_path, directory_path, analysis_data, 0)
            
            # Post-process data
            analysis_data['total_size_mb'] = analysis_data['total_size_bytes'] / (1024 * 1024)
            analysis_data['file_types'] = dict(analysis_data['file_types'])
            analysis_data['mime_types'] = dict(analysis_data['mime_types'])
            
            # Sort largest files
            analysis_data['largest_files'] = sorted(
                analysis_data['largest_files'], 
                key=lambda x: x['size'], 
                reverse=True
            )[:10]  # Top 10 largest files
            
        except Exception as e:
            print(f"Error analyzing directory: {str(e)}")
            analysis_data['error'] = str(e)
        
        return analysis_data
    
    def _analyze_recursive(self, current_path, root_path, analysis_data, depth):
        """Recursively analyze directory contents"""
        try:
            # Update max depth
            analysis_data['max_depth'] = max(analysis_data['max_depth'], depth)
            
            for item in os.listdir(current_path):
                item_path = os.path.join(current_path, item)
                
                if os.path.isfile(item_path):
                    self._analyze_file(item_path, root_path, analysis_data)
                elif os.path.isdir(item_path):
                    analysis_data['total_directories'] += 1
                    # Recurse into subdirectory
                    self._analyze_recursive(item_path, root_path, analysis_data, depth + 1)
                    
        except PermissionError:
            print(f"Permission denied accessing: {current_path}")
        except Exception as e:
            print(f"Error analyzing {current_path}: {str(e)}")
    
    def _analyze_file(self, file_path, root_path, analysis_data):
        """Analyze a single file and update statistics"""
        try:
            file_stat = os.stat(file_path)
            file_size = file_stat.st_size
            
            # Basic counts and sizes
            analysis_data['total_files'] += 1
            analysis_data['total_size_bytes'] += file_size
            analysis_data['file_sizes'].append(file_size)
            
            # File extension analysis
            file_extension = Path(file_path).suffix.lower()
            if not file_extension:
                file_extension = '[no extension]'
            analysis_data['file_types'][file_extension] += 1
            
            # MIME type analysis
            mime_type, _ = mimetypes.guess_type(file_path)
            if mime_type:
                analysis_data['mime_types'][mime_type] += 1
            else:
                analysis_data['mime_types']['unknown'] += 1
            
            # Track largest files
            relative_path = os.path.relpath(file_path, root_path)
            file_info = {
                'name': os.path.basename(file_path),
                'path': relative_path,
                'size': file_size,
                'size_mb': file_size / (1024 * 1024),
                'extension': file_extension,
                'mime_type': mime_type or 'unknown'
            }
            
            analysis_data['largest_files'].append(file_info)
            analysis_data['file_details'].append(file_info)
            
        except Exception as e:
            print(f"Error analyzing file {file_path}: {str(e)}")
    
    def get_file_type_categories(self, file_types):
        """Categorize file types into broader categories"""
        categories = {
            'Code': ['.py', '.js', '.html', '.css', '.java', '.cpp', '.c', '.h', '.php', 
                    '.rb', '.go', '.rs', '.swift', '.kt', '.scala', '.ts', '.jsx', '.tsx'],
            'Documentation': ['.md', '.txt', '.rst', '.doc', '.docx', '.pdf', '.rtf'],
            'Configuration': ['.json', '.yaml', '.yml', '.xml', '.toml', '.ini', '.cfg', '.conf'],
            'Images': ['.png', '.jpg', '.jpeg', '.gif', '.bmp', '.svg', '.ico', '.webp'],
            'Data': ['.csv', '.xlsx', '.xls', '.db', '.sqlite', '.sql', '.parquet'],
            'Archives': ['.zip', '.tar', '.gz', '.rar', '.7z', '.bz2'],
            'Executables': ['.exe', '.dll', '.so', '.dylib', '.bin'],
            'Other': []
        }
        
        categorized = defaultdict(int)
        
        for ext, count in file_types.items():
            category_found = False
            for category, extensions in categories.items():
                if ext.lower() in extensions:
                    categorized[category] += count
                    category_found = True
                    break
            
            if not category_found:
                categorized['Other'] += count
        
        return dict(categorized)
    
    def calculate_file_hash(self, file_path, algorithm='md5'):
        """Calculate hash of a file"""
        try:
            hash_algo = hashlib.new(algorithm)
            
            with open(file_path, 'rb') as f:
                for chunk in iter(lambda: f.read(4096), b""):
                    hash_algo.update(chunk)
            
            return hash_algo.hexdigest()
            
        except Exception as e:
            print(f"Error calculating hash for {file_path}: {str(e)}")
            return None
    
    def find_duplicate_files(self, analysis_data):
        """Find duplicate files based on size and hash"""
        duplicates = defaultdict(list)
        
        # Group by file size first (quick filter)
        size_groups = defaultdict(list)
        for file_info in analysis_data['file_details']:
            if file_info['size'] > 0:  # Skip empty files
                size_groups[file_info['size']].append(file_info)
        
        # For files with same size, calculate hash
        for size, files in size_groups.items():
            if len(files) > 1:
                hash_groups = defaultdict(list)
                for file_info in files:
                    file_hash = self.calculate_file_hash(
                        os.path.join(analysis_data.get('root_path', ''), file_info['path'])
                    )
                    if file_hash:
                        hash_groups[file_hash].append(file_info)
                
                # Add groups with multiple files to duplicates
                for hash_val, duplicate_files in hash_groups.items():
                    if len(duplicate_files) > 1:
                        duplicates[hash_val] = duplicate_files
        
        return dict(duplicates)
