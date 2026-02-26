import CodeViewer from '../CodeViewer';

export default function CodeViewerExample() {
  // todo: remove mock functionality
  const mockCode = `import React from 'react';
import { Button } from './components/ui/button';

interface AppProps {
  title: string;
}

export default function App({ title }: AppProps) {
  const [count, setCount] = React.useState(0);

  const handleClick = () => {
    setCount(prev => prev + 1);
    console.log('Button clicked');
  };

  return (
    <div className="app">
      <h1>{title}</h1>
      <p>Count: {count}</p>
      <Button onClick={handleClick}>
        Click me
      </Button>
    </div>
  );
}`;

  return (
    <div style={{ height: '400px' }}>
      <CodeViewer 
        fileName="App.tsx"
        content={mockCode}
        language="typescript"
        fileSize={1024}
      />
    </div>
  );
}