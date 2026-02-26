import os
import zipfile
import tarfile
import tempfile
import shutil
from pathlib import Path

class PackageExtractor:
    """Class for extracting various package formats"""
    
    def __init__(self):
        self.supported_formats = {
            '.zip': self._extract_zip,
            '.whl': self._extract_zip,  # Python wheels are ZIP files
            '.egg': self._extract_zip,  # Python eggs are ZIP files
            '.tar': self._extract_tar,
            '.gz': self._extract_tar,
            '.tgz': self._extract_tar,
            '.tar.gz': self._extract_tar,
            '.tar.bz2': self._extract_tar,
            '.tbz2': self._extract_tar
        }
    
    def extract_package(self, package_path, extract_to):
        """
        Extract a package to the specified directory
        
        Args:
            package_path (str): Path to the package file
            extract_to (str): Directory to extract to
            
        Returns:
            str: Path to the extracted contents, None if failed
        """
        try:
            file_path = Path(package_path)
            file_extension = self._get_file_extension(file_path)
            
            if file_extension not in self.supported_formats:
                raise ValueError(f"Unsupported file format: {file_extension}")
            
            # Create extraction directory
            extract_dir = os.path.join(extract_to, f"extracted_{file_path.stem}")
            os.makedirs(extract_dir, exist_ok=True)
            
            # Extract using appropriate method
            extractor = self.supported_formats[file_extension]
            success = extractor(package_path, extract_dir)
            
            if success:
                return extract_dir
            else:
                # Clean up on failure
                shutil.rmtree(extract_dir, ignore_errors=True)
                return None
                
        except Exception as e:
            print(f"Error extracting package: {str(e)}")
            return None
    
    def _get_file_extension(self, file_path):
        """Get the appropriate file extension for extraction method selection"""
        file_path_str = str(file_path).lower()
        
        # Check for compound extensions first
        compound_extensions = ['.tar.gz', '.tar.bz2']
        for ext in compound_extensions:
            if file_path_str.endswith(ext):
                return ext
        
        # Check for single extensions
        return file_path.suffix.lower()
    
    def _extract_zip(self, package_path, extract_dir):
        """Extract ZIP-based files (ZIP, WHL, EGG)"""
        try:
            with zipfile.ZipFile(package_path, 'r') as zip_ref:
                # Check if the ZIP file is valid
                zip_ref.testzip()
                
                # Extract all files
                zip_ref.extractall(extract_dir)
                return True
                
        except zipfile.BadZipFile:
            print("Error: Invalid or corrupted ZIP file")
            return False
        except Exception as e:
            print(f"Error extracting ZIP file: {str(e)}")
            return False
    
    def _extract_tar(self, package_path, extract_dir):
        """Extract TAR-based files (TAR, GZ, TGZ, etc.)"""
        try:
            # Determine the mode based on file extension
            file_path_str = str(package_path).lower()
            
            if file_path_str.endswith(('.tar.gz', '.tgz')):
                mode = 'r:gz'
            elif file_path_str.endswith(('.tar.bz2', '.tbz2')):
                mode = 'r:bz2'
            elif file_path_str.endswith('.gz') and not file_path_str.endswith('.tar.gz'):
                # Handle .gz files that aren't .tar.gz
                import gzip
                return self._extract_gzip(package_path, extract_dir)
            else:
                mode = 'r'
            
            with tarfile.open(package_path, mode) as tar_ref:
                # Security check: ensure no path traversal
                def is_safe_path(path, base_path):
                    return os.path.commonpath([os.path.realpath(os.path.join(base_path, path)), base_path]) == base_path
                
                members = tar_ref.getmembers()
                safe_members = []
                
                for member in members:
                    if is_safe_path(member.name, extract_dir):
                        safe_members.append(member)
                    else:
                        print(f"Skipping potentially unsafe path: {member.name}")
                
                # Extract safe members
                tar_ref.extractall(extract_dir, members=safe_members)
                return True
                
        except tarfile.TarError as e:
            print(f"Error: Invalid or corrupted TAR file: {str(e)}")
            return False
        except Exception as e:
            print(f"Error extracting TAR file: {str(e)}")
            return False
    
    def _extract_gzip(self, package_path, extract_dir):
        """Extract standalone GZIP files"""
        try:
            import gzip
            
            file_path = Path(package_path)
            output_filename = file_path.stem
            output_path = os.path.join(extract_dir, output_filename)
            
            with gzip.open(package_path, 'rb') as gz_file:
                with open(output_path, 'wb') as output_file:
                    shutil.copyfileobj(gz_file, output_file)
            
            return True
            
        except Exception as e:
            print(f"Error extracting GZIP file: {str(e)}")
            return False
    
    def is_supported_format(self, file_path):
        """Check if the file format is supported"""
        file_extension = self._get_file_extension(Path(file_path))
        return file_extension in self.supported_formats
    
    def get_supported_extensions(self):
        """Get list of supported file extensions"""
        return list(self.supported_formats.keys())
