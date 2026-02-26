import os
import json
from typing import Dict, Any, List, Optional
import streamlit as st

class PackageAnalyzer:
    """Analyze packages for various metrics and insights"""
    
    def __init__(self):
        self.analysis_modules = {
            'structure': self._analyze_structure,
            'dependencies': self._analyze_dependencies,
            'security': self._analyze_security,
            'quality': self._analyze_quality,
            'metadata': self._analyze_metadata
        }
    
    def analyze_package(self, processing_result: Dict[str, Any], analysis_types: List[str] = None) -> Dict[str, Any]:
        """
        Perform comprehensive analysis of a processed package
        
        Args:
            processing_result: Result from FileHandler.process_file()
            analysis_types: List of analysis types to perform
            
        Returns:
            Dict containing analysis results
        """
        if analysis_types is None:
            analysis_types = list(self.analysis_modules.keys())
        
        analysis_results = {
            'package_name': processing_result.get('file_name', 'Unknown'),
            'analysis_timestamp': self._get_timestamp(),
            'analyses': {}
        }
        
        for analysis_type in analysis_types:
            if analysis_type in self.analysis_modules:
                try:
                    analyzer = self.analysis_modules[analysis_type]
                    result = analyzer(processing_result)
                    analysis_results['analyses'][analysis_type] = result
                except Exception as e:
                    analysis_results['analyses'][analysis_type] = {
                        'success': False,
                        'error': str(e)
                    }
            else:
                st.warning(f"Unknown analysis type: {analysis_type}")
        
        return analysis_results
    
    def _analyze_structure(self, processing_result: Dict[str, Any]) -> Dict[str, Any]:
        """Analyze package structure"""
        file_structure = processing_result.get('file_structure', [])
        
        # Basic structure metrics
        total_files = len([f for f in file_structure if f['type'] == 'file'])
        total_dirs = len([f for f in file_structure if f['type'] == 'directory'])
        
        # Directory depth analysis
        max_depth = 0
        for item in file_structure:
            depth = len(item['path'].split(os.sep)) if item['path'] else 0
            max_depth = max(max_depth, depth)
        
        # File size distribution
        file_sizes = [f['size'] for f in file_structure if f['type'] == 'file']
        avg_file_size = sum(file_sizes) / len(file_sizes) if file_sizes else 0
        
        # File type analysis
        file_extensions = {}
        for item in file_structure:
            if item['type'] == 'file':
                ext = item.get('extension', 'no_extension')
                file_extensions[ext] = file_extensions.get(ext, 0) + 1
        
        return {
            'success': True,
            'metrics': {
                'total_files': total_files,
                'total_directories': total_dirs,
                'max_depth': max_depth,
                'average_file_size': avg_file_size,
                'total_size': sum(file_sizes),
                'file_type_distribution': file_extensions
            },
            'insights': self._generate_structure_insights(total_files, total_dirs, max_depth, file_extensions)
        }
    
    def _analyze_dependencies(self, processing_result: Dict[str, Any]) -> Dict[str, Any]:
        """Analyze package dependencies (placeholder for future implementation)"""
        # This would analyze requirements.txt, setup.py, package.json, etc.
        return {
            'success': True,
            'status': 'Not implemented yet',
            'planned_features': [
                'Parse requirements.txt',
                'Analyze setup.py dependencies',
                'Check package.json for Node.js projects',
                'Identify Python imports',
                'Vulnerability scanning of dependencies'
            ]
        }
    
    def _analyze_security(self, processing_result: Dict[str, Any]) -> Dict[str, Any]:
        """Analyze package for security issues (placeholder for future implementation)"""
        return {
            'success': True,
            'status': 'Not implemented yet',
            'planned_features': [
                'Malware scanning',
                'Suspicious file detection',
                'Known vulnerability database checks',
                'Code pattern analysis for security risks',
                'License compliance checking'
            ]
        }
    
    def _analyze_quality(self, processing_result: Dict[str, Any]) -> Dict[str, Any]:
        """Analyze code quality metrics (placeholder for future implementation)"""
        return {
            'success': True,
            'status': 'Not implemented yet',
            'planned_features': [
                'Code complexity analysis',
                'Documentation coverage',
                'Test coverage estimation',
                'Code style compliance',
                'Dead code detection'
            ]
        }
    
    def _analyze_metadata(self, processing_result: Dict[str, Any]) -> Dict[str, Any]:
        """Extract and analyze package metadata (placeholder for future implementation)"""
        return {
            'success': True,
            'status': 'Not implemented yet',
            'planned_features': [
                'Extract setup.py metadata',
                'Parse package.json information',
                'Analyze README files',
                'License detection',
                'Author and maintainer information'
            ]
        }
    
    def _generate_structure_insights(self, total_files: int, total_dirs: int, max_depth: int, file_extensions: Dict[str, int]) -> List[str]:
        """Generate insights about package structure"""
        insights = []
        
        # Size insights
        if total_files > 1000:
            insights.append("Large package with many files - may indicate complex functionality")
        elif total_files < 10:
            insights.append("Small package - likely focused on specific functionality")
        
        # Depth insights
        if max_depth > 6:
            insights.append("Deep directory structure - may indicate complex organization")
        elif max_depth <= 2:
            insights.append("Flat directory structure - simple organization")
        
        # File type insights
        if 'py' in file_extensions and file_extensions['py'] > len(file_extensions) * 0.5:
            insights.append("Python-heavy package")
        
        if 'js' in file_extensions:
            insights.append("Contains JavaScript code")
        
        if any(ext in file_extensions for ext in ['md', 'txt', 'rst']):
            insights.append("Well-documented with README/documentation files")
        
        if 'test' in str(file_extensions) or any('test' in str(ext) for ext in file_extensions):
            insights.append("Includes test files - good development practice")
        
        return insights if insights else ["Standard package structure"]
    
    def _get_timestamp(self) -> str:
        """Get current timestamp for analysis"""
        from datetime import datetime
        return datetime.now().isoformat()
    
    def export_analysis(self, analysis_results: Dict[str, Any], format_type: str = 'json') -> str:
        """Export analysis results in specified format"""
        if format_type == 'json':
            return json.dumps(analysis_results, indent=2)
        else:
            raise ValueError(f"Unsupported export format: {format_type}")
    
    def get_analysis_summary(self, analysis_results: Dict[str, Any]) -> Dict[str, Any]:
        """Get a summary of the analysis results"""
        summary = {
            'package_name': analysis_results.get('package_name', 'Unknown'),
            'analysis_count': len(analysis_results.get('analyses', {})),
            'successful_analyses': 0,
            'failed_analyses': 0,
            'key_metrics': {}
        }
        
        for analysis_name, analysis_result in analysis_results.get('analyses', {}).items():
            if analysis_result.get('success', False):
                summary['successful_analyses'] += 1
                
                # Extract key metrics from structure analysis
                if analysis_name == 'structure' and 'metrics' in analysis_result:
                    summary['key_metrics'].update(analysis_result['metrics'])
            else:
                summary['failed_analyses'] += 1
        
        return summary
