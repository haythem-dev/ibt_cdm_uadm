import os
import tempfile
import zipfile
import tarfile
import rarfile
import py7zr
from typing import Dict, Any, List
import streamlit as st

class FileHandler:
    """Handle file operations for package analysis"""
    
    def __init__(self):
        self.temp_dir = None
        self.supported_formats = {
            'zip': self._extract_zip,
            'tar': self._extract_tar,
            'gz': self._extract_tar,
            'tgz': self._extract_tar,
            'rar': self._extract_rar,
            '7z': self._extract_7z,
            'whl': self._extract_zip,  # Python wheels are zip files
            'egg': self._extract_zip   # Python eggs are zip files
        }
    
    def process_file(self, uploaded_file) -> Dict[str, Any]:
        """
        Process an uploaded file and prepare it for analysis
        
        Args:
            uploaded_file: Streamlit uploaded file object
            
        Returns:
            Dict containing processing results
        """
        try:
            # Get file extension
            file_extension = self._get_file_extension(uploaded_file.name)
            
            if file_extension not in self.supported_formats:
                return {
                    'success': False,
                    'error': f"Unsupported file format: {file_extension}"
                }
            
            # Create temporary directory for extraction
            self.temp_dir = tempfile.mkdtemp(prefix="package_analyzer_")
            
            # Save uploaded file to temporary location
            temp_file_path = os.path.join(self.temp_dir, uploaded_file.name)
            with open(temp_file_path, 'wb') as f:
                f.write(uploaded_file.getbuffer())
            
            # Extract the file
            extraction_result = self._extract_file(temp_file_path, file_extension)
            
            if not extraction_result['success']:
                return extraction_result
            
            # Get file structure
            file_structure = self._get_file_structure(extraction_result['extract_path'])
            
            return {
                'success': True,
                'file_name': uploaded_file.name,
                'file_size': uploaded_file.size,
                'file_type': file_extension,
                'temp_path': temp_file_path,
                'extract_path': extraction_result['extract_path'],
                'file_structure': file_structure,
                'file_count': len(file_structure)
            }
            
        except Exception as e:
            return {
                'success': False,
                'error': f"Error processing file: {str(e)}"
            }
    
    def _get_file_extension(self, filename: str) -> str:
        """Extract file extension from filename"""
        if filename.endswith('.tar.gz'):
            return 'gz'
        elif filename.endswith('.tar.bz2'):
            return 'tar'
        else:
            return filename.split('.')[-1].lower()
    
    def _extract_file(self, file_path: str, file_extension: str) -> Dict[str, Any]:
        """Extract file based on its extension"""
        try:
            extract_path = os.path.join(self.temp_dir, 'extracted')
            os.makedirs(extract_path, exist_ok=True)
            
            extractor = self.supported_formats.get(file_extension)
            if extractor:
                extractor(file_path, extract_path)
                return {
                    'success': True,
                    'extract_path': extract_path
                }
            else:
                return {
                    'success': False,
                    'error': f"No extractor available for {file_extension}"
                }
                
        except Exception as e:
            return {
                'success': False,
                'error': f"Extraction failed: {str(e)}"
            }
    
    def _extract_zip(self, file_path: str, extract_path: str):
        """Extract ZIP files"""
        with zipfile.ZipFile(file_path, 'r') as zip_ref:
            zip_ref.extractall(extract_path)
    
    def _extract_tar(self, file_path: str, extract_path: str):
        """Extract TAR files (including tar.gz, tar.bz2)"""
        with tarfile.open(file_path, 'r:*') as tar_ref:
            tar_ref.extractall(extract_path)
    
    def _extract_rar(self, file_path: str, extract_path: str):
        """Extract RAR files"""
        try:
            with rarfile.RarFile(file_path) as rar_ref:
                rar_ref.extractall(extract_path)
        except Exception as e:
            raise Exception(f"RAR extraction requires rarfile library: {str(e)}")
    
    def _extract_7z(self, file_path: str, extract_path: str):
        """Extract 7Z files"""
        try:
            with py7zr.SevenZipFile(file_path, mode='r') as z_ref:
                z_ref.extractall(extract_path)
        except Exception as e:
            raise Exception(f"7Z extraction requires py7zr library: {str(e)}")
    
    def _get_file_structure(self, directory_path: str) -> List[Dict[str, Any]]:
        """Get the file structure of the extracted package"""
        file_structure = []
        
        for root, dirs, files in os.walk(directory_path):
            # Calculate relative path
            rel_root = os.path.relpath(root, directory_path)
            if rel_root == '.':
                rel_root = ''
            
            # Add directories
            for dir_name in dirs:
                full_path = os.path.join(root, dir_name)
                rel_path = os.path.join(rel_root, dir_name) if rel_root else dir_name
                
                file_structure.append({
                    'name': dir_name,
                    'type': 'directory',
                    'path': rel_path,
                    'full_path': full_path,
                    'size': 0
                })
            
            # Add files
            for file_name in files:
                full_path = os.path.join(root, file_name)
                rel_path = os.path.join(rel_root, file_name) if rel_root else file_name
                
                try:
                    file_size = os.path.getsize(full_path)
                except OSError:
                    file_size = 0
                
                file_structure.append({
                    'name': file_name,
                    'type': 'file',
                    'path': rel_path,
                    'full_path': full_path,
                    'size': file_size,
                    'extension': self._get_file_extension(file_name)
                })
        
        return file_structure
    
    def cleanup(self):
        """Clean up temporary files and directories"""
        if self.temp_dir and os.path.exists(self.temp_dir):
            import shutil
            try:
                shutil.rmtree(self.temp_dir)
                self.temp_dir = None
            except Exception as e:
                st.warning(f"Failed to cleanup temporary files: {str(e)}")
    
    def get_file_info(self, file_structure: List[Dict[str, Any]]) -> Dict[str, Any]:
        """Get summary information about the file structure"""
        total_files = len([f for f in file_structure if f['type'] == 'file'])
        total_dirs = len([f for f in file_structure if f['type'] == 'directory'])
        total_size = sum(f['size'] for f in file_structure if f['type'] == 'file')
        
        # Get file type distribution
        extensions = {}
        for file_info in file_structure:
            if file_info['type'] == 'file':
                ext = file_info.get('extension', 'unknown')
                extensions[ext] = extensions.get(ext, 0) + 1
        
        return {
            'total_files': total_files,
            'total_directories': total_dirs,
            'total_size': total_size,
            'file_types': extensions
        }
