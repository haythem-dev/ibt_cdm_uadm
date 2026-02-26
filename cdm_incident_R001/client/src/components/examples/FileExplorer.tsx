import FileExplorer from '../FileExplorer';

export default function FileExplorerExample() {
  // todo: remove mock functionality
  const mockFiles = [
    {
      id: 'src',
      name: 'src',
      type: 'folder' as const,
      path: '/src',
      children: [
        {
          id: 'components',
          name: 'components',
          type: 'folder' as const,
          path: '/src/components',
          children: [
            {
              id: 'button-tsx',
              name: 'Button.tsx',
              type: 'file' as const,
              size: 2048,
              path: '/src/components/Button.tsx'
            },
            {
              id: 'input-tsx',
              name: 'Input.tsx',
              type: 'file' as const,
              size: 1536,
              path: '/src/components/Input.tsx'
            }
          ]
        },
        {
          id: 'app-tsx',
          name: 'App.tsx',
          type: 'file' as const,
          size: 4096,
          path: '/src/App.tsx'
        },
        {
          id: 'main-tsx',
          name: 'main.tsx',
          type: 'file' as const,
          size: 512,
          path: '/src/main.tsx'
        }
      ]
    },
    {
      id: 'package-json',
      name: 'package.json',
      type: 'file' as const,
      size: 1024,
      path: '/package.json'
    },
    {
      id: 'readme-md',
      name: 'README.md',
      type: 'file' as const,
      size: 2560,
      path: '/README.md'
    }
  ];

  return (
    <div style={{ height: '400px', width: '300px' }}>
      <FileExplorer 
        files={mockFiles}
        onFileSelect={(file) => console.log('Selected file:', file.path)}
      />
    </div>
  );
}