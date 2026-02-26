import os
import ast
from pathlib import Path
from collections import defaultdict
import re

class CodeAnalyzer:
    """Class for analyzing code files and extracting metrics"""
    
    def __init__(self):
        self.code_extensions = {
            '.py': self._analyze_python_file,
            '.js': self._analyze_javascript_file,
            '.jsx': self._analyze_javascript_file,
            '.ts': self._analyze_typescript_file,
            '.tsx': self._analyze_typescript_file,
            '.java': self._analyze_java_file,
            '.cpp': self._analyze_cpp_file,
            '.c': self._analyze_c_file,
            '.h': self._analyze_c_file,
            '.php': self._analyze_php_file,
            '.rb': self._analyze_ruby_file,
            '.go': self._analyze_go_file
        }
    
    def analyze_code_files(self, directory_path):
        """
        Analyze all code files in a directory
        
        Args:
            directory_path (str): Path to directory to analyze
            
        Returns:
            dict: Code analysis results
        """
        analysis_results = {
            'python_files': [],
            'javascript_files': [],
            'other_code_files': [],
            'total_loc': 0,
            'total_functions': 0,
            'total_classes': 0,
            'languages_found': set(),
            'imports_analysis': defaultdict(int),
            'complexity_metrics': {}
        }
        
        try:
            self._analyze_directory_recursive(directory_path, analysis_results)
            analysis_results['languages_found'] = list(analysis_results['languages_found'])
            analysis_results['imports_analysis'] = dict(analysis_results['imports_analysis'])
            
        except Exception as e:
            print(f"Error in code analysis: {str(e)}")
            analysis_results['error'] = str(e)
        
        return analysis_results
    
    def _analyze_directory_recursive(self, directory_path, results):
        """Recursively analyze code files in directory"""
        try:
            for item in os.listdir(directory_path):
                item_path = os.path.join(directory_path, item)
                
                if os.path.isfile(item_path):
                    self._analyze_code_file(item_path, results)
                elif os.path.isdir(item_path):
                    self._analyze_directory_recursive(item_path, results)
                    
        except PermissionError:
            print(f"Permission denied accessing: {directory_path}")
        except Exception as e:
            print(f"Error analyzing directory {directory_path}: {str(e)}")
    
    def _analyze_code_file(self, file_path, results):
        """Analyze a single code file"""
        try:
            file_extension = Path(file_path).suffix.lower()
            
            if file_extension in self.code_extensions:
                analyzer_func = self.code_extensions[file_extension]
                file_analysis = analyzer_func(file_path)
                
                if file_analysis:
                    # Update totals
                    results['total_loc'] += file_analysis.get('lines_of_code', 0)
                    results['total_functions'] += len(file_analysis.get('functions', []))
                    results['total_classes'] += len(file_analysis.get('classes', []))
                    
                    # Add language to found languages
                    results['languages_found'].add(file_analysis.get('language', 'unknown'))
                    
                    # Store file analysis based on type
                    if file_extension == '.py':
                        results['python_files'].append(file_analysis)
                        # Track imports
                        for imp in file_analysis.get('imports', []):
                            results['imports_analysis'][imp] += 1
                    elif file_extension in ['.js', '.jsx', '.ts', '.tsx']:
                        results['javascript_files'].append(file_analysis)
                    else:
                        results['other_code_files'].append(file_analysis)
                        
        except Exception as e:
            print(f"Error analyzing code file {file_path}: {str(e)}")
    
    def _analyze_python_file(self, file_path):
        """Analyze Python file using AST"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            # Basic metrics
            lines = content.split('\n')
            lines_of_code = len([line for line in lines if line.strip() and not line.strip().startswith('#')])
            
            analysis = {
                'filename': os.path.basename(file_path),
                'filepath': file_path,
                'language': 'Python',
                'lines_of_code': lines_of_code,
                'total_lines': len(lines),
                'functions': [],
                'classes': [],
                'imports': [],
                'docstrings': 0,
                'comments': len([line for line in lines if line.strip().startswith('#')])
            }
            
            # Parse AST for detailed analysis
            try:
                tree = ast.parse(content)
                
                for node in ast.walk(tree):
                    if isinstance(node, ast.FunctionDef):
                        analysis['functions'].append(node.name)
                    elif isinstance(node, ast.ClassDef):
                        analysis['classes'].append(node.name)
                    elif isinstance(node, (ast.Import, ast.ImportFrom)):
                        if isinstance(node, ast.Import):
                            for alias in node.names:
                                analysis['imports'].append(alias.name)
                        else:  # ImportFrom
                            if node.module:
                                analysis['imports'].append(node.module)
                    elif isinstance(node, ast.Expr) and isinstance(node.value, ast.Constant):
                        if isinstance(node.value.value, str):
                            analysis['docstrings'] += 1
                            
            except SyntaxError:
                print(f"Syntax error in Python file: {file_path}")
            
            return analysis
            
        except Exception as e:
            print(f"Error analyzing Python file {file_path}: {str(e)}")
            return None
    
    def _analyze_javascript_file(self, file_path):
        """Basic analysis of JavaScript/JSX files"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            lines = content.split('\n')
            lines_of_code = len([line for line in lines if line.strip() and not line.strip().startswith('//')])
            
            # Simple regex patterns for basic analysis
            function_pattern = r'\b(?:function\s+(\w+)|const\s+(\w+)\s*=\s*(?:\([^)]*\)\s*=>|\([^)]*\)\s*=>\s*{)|(\w+)\s*:\s*function|\bfunction\s*\*?\s*(\w+))'
            class_pattern = r'\bclass\s+(\w+)'
            import_pattern = r'\b(?:import\s+.*?from\s+[\'"]([^\'"]+)[\'"]|import\s+[\'"]([^\'"]+)[\'"]|require\s*\(\s*[\'"]([^\'"]+)[\'"]\s*\))'
            
            functions = re.findall(function_pattern, content)
            classes = re.findall(class_pattern, content)
            imports = re.findall(import_pattern, content)
            
            # Flatten function matches (regex returns tuples)
            function_names = [name for match in functions for name in match if name]
            import_names = [name for match in imports for name in match if name]
            
            analysis = {
                'filename': os.path.basename(file_path),
                'filepath': file_path,
                'language': 'JavaScript' if file_path.endswith(('.js', '.jsx')) else 'TypeScript',
                'lines_of_code': lines_of_code,
                'total_lines': len(lines),
                'functions': function_names,
                'classes': classes,
                'imports': import_names,
                'comments': len([line for line in lines if line.strip().startswith('//')])
            }
            
            return analysis
            
        except Exception as e:
            print(f"Error analyzing JavaScript file {file_path}: {str(e)}")
            return None
    
    def _analyze_typescript_file(self, file_path):
        """Analyze TypeScript files (similar to JavaScript)"""
        return self._analyze_javascript_file(file_path)
    
    def _analyze_java_file(self, file_path):
        """Basic analysis of Java files"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            lines = content.split('\n')
            lines_of_code = len([line for line in lines if line.strip() and not line.strip().startswith('//')])
            
            # Simple patterns for Java
            class_pattern = r'\b(?:public\s+|private\s+|protected\s+)?class\s+(\w+)'
            method_pattern = r'\b(?:public\s+|private\s+|protected\s+|static\s+)*\w+\s+(\w+)\s*\([^)]*\)\s*{'
            import_pattern = r'\bimport\s+([^;]+);'
            
            classes = re.findall(class_pattern, content)
            methods = re.findall(method_pattern, content)
            imports = re.findall(import_pattern, content)
            
            analysis = {
                'filename': os.path.basename(file_path),
                'filepath': file_path,
                'language': 'Java',
                'lines_of_code': lines_of_code,
                'total_lines': len(lines),
                'functions': methods,
                'classes': classes,
                'imports': imports,
                'comments': len([line for line in lines if line.strip().startswith('//')])
            }
            
            return analysis
            
        except Exception as e:
            print(f"Error analyzing Java file {file_path}: {str(e)}")
            return None
    
    def _analyze_cpp_file(self, file_path):
        """Basic analysis of C++ files"""
        return self._analyze_c_file(file_path, language='C++')
    
    def _analyze_c_file(self, file_path, language='C'):
        """Basic analysis of C/C++ files"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            lines = content.split('\n')
            lines_of_code = len([line for line in lines if line.strip() and not line.strip().startswith('//')])
            
            # Simple patterns
            function_pattern = r'\b\w+\s+(\w+)\s*\([^)]*\)\s*{'
            include_pattern = r'#include\s*[<"]([^>"]+)[>"]'
            
            functions = re.findall(function_pattern, content)
            includes = re.findall(include_pattern, content)
            
            analysis = {
                'filename': os.path.basename(file_path),
                'filepath': file_path,
                'language': language,
                'lines_of_code': lines_of_code,
                'total_lines': len(lines),
                'functions': functions,
                'classes': [],  # Would need more complex parsing for C++ classes
                'imports': includes,
                'comments': len([line for line in lines if line.strip().startswith('//')])
            }
            
            return analysis
            
        except Exception as e:
            print(f"Error analyzing {language} file {file_path}: {str(e)}")
            return None
    
    def _analyze_php_file(self, file_path):
        """Basic analysis of PHP files"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            lines = content.split('\n')
            lines_of_code = len([line for line in lines if line.strip() and not line.strip().startswith('//')])
            
            function_pattern = r'\bfunction\s+(\w+)'
            class_pattern = r'\bclass\s+(\w+)'
            include_pattern = r'\b(?:include|require)(?:_once)?\s*\(?[\'"]([^\'"]+)[\'"]'
            
            functions = re.findall(function_pattern, content)
            classes = re.findall(class_pattern, content)
            includes = re.findall(include_pattern, content)
            
            return {
                'filename': os.path.basename(file_path),
                'filepath': file_path,
                'language': 'PHP',
                'lines_of_code': lines_of_code,
                'total_lines': len(lines),
                'functions': functions,
                'classes': classes,
                'imports': includes,
                'comments': len([line for line in lines if line.strip().startswith('//')])
            }
            
        except Exception as e:
            print(f"Error analyzing PHP file {file_path}: {str(e)}")
            return None
    
    def _analyze_ruby_file(self, file_path):
        """Basic analysis of Ruby files"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            lines = content.split('\n')
            lines_of_code = len([line for line in lines if line.strip() and not line.strip().startswith('#')])
            
            function_pattern = r'\bdef\s+(\w+)'
            class_pattern = r'\bclass\s+(\w+)'
            require_pattern = r'\brequire\s+[\'"]([^\'"]+)[\'"]'
            
            functions = re.findall(function_pattern, content)
            classes = re.findall(class_pattern, content)
            requires = re.findall(require_pattern, content)
            
            return {
                'filename': os.path.basename(file_path),
                'filepath': file_path,
                'language': 'Ruby',
                'lines_of_code': lines_of_code,
                'total_lines': len(lines),
                'functions': functions,
                'classes': classes,
                'imports': requires,
                'comments': len([line for line in lines if line.strip().startswith('#')])
            }
            
        except Exception as e:
            print(f"Error analyzing Ruby file {file_path}: {str(e)}")
            return None
    
    def _analyze_go_file(self, file_path):
        """Basic analysis of Go files"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            lines = content.split('\n')
            lines_of_code = len([line for line in lines if line.strip() and not line.strip().startswith('//')])
            
            function_pattern = r'\bfunc\s+(\w+)'
            struct_pattern = r'\btype\s+(\w+)\s+struct'
            import_pattern = r'\bimport\s+[\'"]([^\'"]+)[\'"]'
            
            functions = re.findall(function_pattern, content)
            structs = re.findall(struct_pattern, content)
            imports = re.findall(import_pattern, content)
            
            return {
                'filename': os.path.basename(file_path),
                'filepath': file_path,
                'language': 'Go',
                'lines_of_code': lines_of_code,
                'total_lines': len(lines),
                'functions': functions,
                'classes': structs,  # Go uses structs instead of classes
                'imports': imports,
                'comments': len([line for line in lines if line.strip().startswith('//')])
            }
            
        except Exception as e:
            print(f"Error analyzing Go file {file_path}: {str(e)}")
            return None
