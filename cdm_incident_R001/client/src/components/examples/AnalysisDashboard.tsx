import AnalysisDashboard from '../AnalysisDashboard';

export default function AnalysisDashboardExample() {
  // todo: remove mock functionality
  const mockAnalysisResult = {
    packageName: 'react-ui-components',
    version: '2.1.3',
    size: 15728640, // 15MB
    fileCount: 247,
    folderCount: 18,
    vulnerabilities: 2,
    dependencies: [
      'react@^18.2.0',
      'typescript@^5.0.0',
      'styled-components@^5.3.0',
      'lodash@^4.17.21'
    ],
    description: 'A comprehensive React UI component library with TypeScript support and modern styling capabilities.',
    author: 'UI Team',
    license: 'MIT'
  };

  return (
    <div style={{ height: '500px' }}>
      <AnalysisDashboard analysisResult={mockAnalysisResult} />
    </div>
  );
}