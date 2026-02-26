import { useState } from 'react';
import { Package } from 'lucide-react';
import FileUploadZone from './FileUploadZone';
import FileExplorer from './FileExplorer';
import AnalysisDashboard from './AnalysisDashboard';
import CodeViewer from './CodeViewer';
import ThemeToggle from './ThemeToggle';

interface FileNode {
  id: string;
  name: string;
  type: 'file' | 'folder';
  size?: number;
  children?: FileNode[];
  path: string;
}

interface AnalysisResult {
  packageName: string;
  version?: string;
  size: number;
  fileCount: number;
  folderCount: number;
  vulnerabilities: number;
  dependencies: string[];
  description?: string;
  author?: string;
  license?: string;
}

export default function AppLayout() {
  const [packageLoaded, setPackageLoaded] = useState(false);
  const [selectedFile, setSelectedFile] = useState<string | undefined>(undefined);
  const [isAnalyzing, setIsAnalyzing] = useState(false);

  // todo: remove mock functionality - these would come from actual package analysis
  const mockFiles: FileNode[] = [
    {
      id: 'package-json',
      name: 'package.json',
      type: 'file',
      size: 1024,
      path: '/package.json'
    },
    {
      id: 'src',
      name: 'src',
      type: 'folder',
      path: '/src',
      children: [
        {
          id: 'components',
          name: 'components',
          type: 'folder',
          path: '/src/components',
          children: [
            {
              id: 'button-tsx',
              name: 'Button.tsx',
              type: 'file',
              size: 2048,
              path: '/src/components/Button.tsx'
            },
            {
              id: 'card-tsx',
              name: 'Card.tsx',
              type: 'file',
              size: 1536,
              path: '/src/components/Card.tsx'
            },
            {
              id: 'input-tsx',
              name: 'Input.tsx',
              type: 'file',
              size: 1280,
              path: '/src/components/Input.tsx'
            }
          ]
        },
        {
          id: 'utils',
          name: 'utils',
          type: 'folder',
          path: '/src/utils',
          children: [
            {
              id: 'helpers-ts',
              name: 'helpers.ts',
              type: 'file',
              size: 896,
              path: '/src/utils/helpers.ts'
            }
          ]
        },
        {
          id: 'app-tsx',
          name: 'App.tsx',
          type: 'file',
          size: 4096,
          path: '/src/App.tsx'
        },
        {
          id: 'index-tsx',
          name: 'index.tsx',
          type: 'file',
          size: 512,
          path: '/src/index.tsx'
        }
      ]
    },
    {
      id: 'readme-md',
      name: 'README.md',
      type: 'file',
      size: 2560,
      path: '/README.md'
    },
    {
      id: 'tsconfig-json',
      name: 'tsconfig.json',
      type: 'file',
      size: 640,
      path: '/tsconfig.json'
    }
  ];

  const mockAnalysisResult: AnalysisResult = {
    packageName: 'my-react-package',
    version: '1.2.4',
    size: 8394752, // ~8MB
    fileCount: 42,
    folderCount: 8,
    vulnerabilities: 1,
    dependencies: [
      'react@^18.2.0',
      'typescript@^5.0.0',
      'lodash@^4.17.21',
      'axios@^1.3.0'
    ],
    description: 'A modern React component library with TypeScript support for building user interfaces.',
    author: 'Development Team',
    license: 'MIT'
  };

  const mockFileContents: Record<string, string> = {
    '/package.json': `{
  "name": "my-react-package",
  "version": "1.2.4",
  "description": "A modern React component library",
  "main": "dist/index.js",
  "scripts": {
    "build": "tsc",
    "test": "jest",
    "dev": "vite"
  },
  "dependencies": {
    "react": "^18.2.0",
    "typescript": "^5.0.0"
  },
  "devDependencies": {
    "vite": "^4.0.0",
    "jest": "^29.0.0"
  }
}`,
    '/src/components/Button.tsx': `import React from 'react';

interface ButtonProps {
  children: React.ReactNode;
  variant?: 'primary' | 'secondary';
  size?: 'sm' | 'md' | 'lg';
  onClick?: () => void;
  disabled?: boolean;
}

export const Button: React.FC<ButtonProps> = ({
  children,
  variant = 'primary',
  size = 'md',
  onClick,
  disabled = false
}) => {
  const baseClasses = 'rounded font-medium transition-colors';
  const variantClasses = {
    primary: 'bg-blue-600 text-white hover:bg-blue-700',
    secondary: 'bg-gray-200 text-gray-900 hover:bg-gray-300'
  };
  const sizeClasses = {
    sm: 'px-3 py-1 text-sm',
    md: 'px-4 py-2',
    lg: 'px-6 py-3 text-lg'
  };

  return (
    <button
      className={\`\${baseClasses} \${variantClasses[variant]} \${sizeClasses[size]}\`}
      onClick={onClick}
      disabled={disabled}
    >
      {children}
    </button>
  );
};`,
    '/README.md': `# My React Package

A modern React component library built with TypeScript.

## Installation

\`\`\`bash
npm install my-react-package
\`\`\`

## Usage

\`\`\`tsx
import { Button } from 'my-react-package';

function App() {
  return (
    <Button variant="primary" onClick={() => alert('Hello!')}>
      Click me
    </Button>
  );
}
\`\`\`

## Components

- **Button**: A versatile button component
- **Card**: Container component for content
- **Input**: Form input with validation

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request`
  };

  const handleFileUpload = (file: File) => {
    console.log('Package uploaded:', file.name);
    setIsAnalyzing(true);
    
    // Simulate analysis process
    setTimeout(() => {
      setIsAnalyzing(false);
      setPackageLoaded(true);
    }, 3000);
  };

  const handleFileSelect = (file: FileNode) => {
    setSelectedFile(file.id);
    console.log('File selected for viewing:', file.path);
  };

  const handleNewPackage = () => {
    setPackageLoaded(false);
    setSelectedFile(undefined);
    setIsAnalyzing(false);
    console.log('Starting new package analysis');
  };

  const getCurrentFileContent = () => {
    if (!selectedFile) return undefined;
    
    const findFile = (files: FileNode[]): FileNode | undefined => {
      for (const file of files) {
        if (file.id === selectedFile) return file;
        if (file.children) {
          const found = findFile(file.children);
          if (found) return found;
        }
      }
      return undefined;
    };

    const file = findFile(mockFiles);
    if (!file) return undefined;

    return {
      fileName: file.name,
      content: mockFileContents[file.path] || `// Content of ${file.name}\n// This file contains ${file.size} bytes`,
      language: file.name.endsWith('.tsx') || file.name.endsWith('.ts') ? 'typescript' :
                file.name.endsWith('.json') ? 'json' :
                file.name.endsWith('.md') ? 'markdown' : 'text',
      fileSize: file.size
    };
  };

  const fileContent = getCurrentFileContent();

  return (
    <div className="h-screen flex flex-col bg-background">
      {/* Header */}
      <header className="border-b px-6 py-4 flex items-center justify-between bg-background/80 backdrop-blur">
        <div className="flex items-center gap-3">
          <Package className="h-8 w-8 text-primary" />
          <div>
            <h1 className="text-xl font-bold">Package Analyzer</h1>
            <p className="text-sm text-muted-foreground">
              Analyze and explore package contents
            </p>
          </div>
        </div>
        <div className="flex items-center gap-2">
          {packageLoaded && (
            <button
              className="text-sm text-primary hover:text-primary/80 font-medium px-3 py-1 rounded hover-elevate"
              onClick={handleNewPackage}
              data-testid="button-new-package"
            >
              New Package
            </button>
          )}
          <ThemeToggle />
        </div>
      </header>

      {/* Main Content */}
      {!packageLoaded ? (
        <main className="flex-1 flex items-center justify-center p-6">
          <div className="max-w-md w-full">
            <FileUploadZone onFileSelect={handleFileUpload} />
          </div>
        </main>
      ) : (
        <main className="flex-1 overflow-hidden">
          <div className="h-full grid grid-cols-12 gap-4 p-4">
            {/* File Explorer */}
            <div className="col-span-3">
              <FileExplorer 
                files={mockFiles}
                onFileSelect={handleFileSelect}
                selectedFile={selectedFile}
              />
            </div>
            
            {/* Code Viewer */}
            <div className="col-span-6">
              {isAnalyzing ? (
                <AnalysisDashboard isAnalyzing={true} />
              ) : (
                <CodeViewer 
                  fileName={fileContent?.fileName}
                  content={fileContent?.content}
                  language={fileContent?.language}
                  fileSize={fileContent?.fileSize}
                />
              )}
            </div>
            
            {/* Analysis Dashboard */}
            <div className="col-span-3">
              <AnalysisDashboard 
                analysisResult={!isAnalyzing ? mockAnalysisResult : undefined}
                isAnalyzing={isAnalyzing}
              />
            </div>
          </div>
        </main>
      )}
    </div>
  );
}