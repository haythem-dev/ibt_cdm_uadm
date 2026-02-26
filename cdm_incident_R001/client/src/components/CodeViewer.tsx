import { useState } from 'react';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Button } from '@/components/ui/button';
import { Badge } from '@/components/ui/badge';
import { ScrollArea } from '@/components/ui/scroll-area';
import { 
  Copy, 
  Download, 
  Eye, 
  FileText,
  Maximize2,
  Minimize2
} from 'lucide-react';

interface CodeViewerProps {
  fileName?: string;
  content?: string;
  language?: string;
  fileSize?: number;
  isLoading?: boolean;
}

export default function CodeViewer({ 
  fileName, 
  content, 
  language = 'text',
  fileSize,
  isLoading = false 
}: CodeViewerProps) {
  const [isFullscreen, setIsFullscreen] = useState(false);
  const [showLineNumbers, setShowLineNumbers] = useState(true);

  const formatFileSize = (bytes?: number) => {
    if (!bytes) return '';
    const sizes = ['B', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(1024));
    return Math.round(bytes / Math.pow(1024, i) * 100) / 100 + ' ' + sizes[i];
  };

  const copyToClipboard = () => {
    if (content) {
      navigator.clipboard.writeText(content);
      console.log('Content copied to clipboard');
    }
  };

  const downloadFile = () => {
    if (content && fileName) {
      const blob = new Blob([content], { type: 'text/plain' });
      const url = URL.createObjectURL(blob);
      const a = document.createElement('a');
      a.href = url;
      a.download = fileName;
      document.body.appendChild(a);
      a.click();
      document.body.removeChild(a);
      URL.revokeObjectURL(url);
      console.log('File downloaded:', fileName);
    }
  };

  const getLanguageBadgeColor = (lang: string) => {
    const colors: Record<string, string> = {
      javascript: 'bg-yellow-500',
      typescript: 'bg-blue-500',
      python: 'bg-green-500',
      java: 'bg-orange-500',
      css: 'bg-pink-500',
      html: 'bg-orange-400',
      json: 'bg-green-400',
      markdown: 'bg-blue-400',
      text: 'bg-gray-500'
    };
    return colors[lang.toLowerCase()] || 'bg-gray-500';
  };

  const renderLineNumbers = (content: string) => {
    const lines = content.split('\n');
    return lines.map((_, index) => (
      <div key={index} className="text-right text-xs text-muted-foreground select-none">
        {index + 1}
      </div>
    ));
  };

  const renderContent = (content: string) => {
    const lines = content.split('\n');
    return lines.map((line, index) => (
      <div key={index} className="text-sm font-mono whitespace-pre">
        {line || '\u00A0'} {/* Non-breaking space for empty lines */}
      </div>
    ));
  };

  if (isLoading) {
    return (
      <Card className="h-full">
        <CardHeader>
          <CardTitle className="flex items-center gap-2">
            <FileText className="h-5 w-5 animate-pulse" />
            Loading file...
          </CardTitle>
        </CardHeader>
        <CardContent>
          <div className="space-y-2">
            {[...Array(8)].map((_, i) => (
              <div key={i} className="h-4 bg-muted animate-pulse rounded" />
            ))}
          </div>
        </CardContent>
      </Card>
    );
  }

  if (!fileName || !content) {
    return (
      <Card className="h-full flex items-center justify-center">
        <CardContent className="text-center">
          <FileText className="h-12 w-12 text-muted-foreground mx-auto mb-4" />
          <h3 className="text-lg font-semibold mb-2">No File Selected</h3>
          <p className="text-muted-foreground">
            Select a file from the explorer to view its contents
          </p>
        </CardContent>
      </Card>
    );
  }

  return (
    <Card className={`h-full flex flex-col ${isFullscreen ? 'fixed inset-4 z-50' : ''}`}>
      <CardHeader className="pb-3">
        <div className="flex items-center justify-between">
          <CardTitle className="flex items-center gap-2 truncate">
            <FileText className="h-5 w-5 flex-shrink-0" />
            <span className="truncate">{fileName}</span>
            <div className="flex gap-1 flex-shrink-0">
              <Badge 
                variant="secondary" 
                className={`text-white ${getLanguageBadgeColor(language)}`}
              >
                {language.toUpperCase()}
              </Badge>
              {fileSize && (
                <Badge variant="outline" className="text-xs">
                  {formatFileSize(fileSize)}
                </Badge>
              )}
            </div>
          </CardTitle>
          
          <div className="flex items-center gap-1">
            <Button
              size="icon"
              variant="ghost"
              onClick={() => setShowLineNumbers(!showLineNumbers)}
              data-testid="button-toggle-line-numbers"
            >
              <Eye className="h-4 w-4" />
            </Button>
            <Button
              size="icon"
              variant="ghost"
              onClick={copyToClipboard}
              data-testid="button-copy-content"
            >
              <Copy className="h-4 w-4" />
            </Button>
            <Button
              size="icon"
              variant="ghost"
              onClick={downloadFile}
              data-testid="button-download-file"
            >
              <Download className="h-4 w-4" />
            </Button>
            <Button
              size="icon"
              variant="ghost"
              onClick={() => setIsFullscreen(!isFullscreen)}
              data-testid="button-toggle-fullscreen"
            >
              {isFullscreen ? (
                <Minimize2 className="h-4 w-4" />
              ) : (
                <Maximize2 className="h-4 w-4" />
              )}
            </Button>
          </div>
        </div>
      </CardHeader>
      
      <CardContent className="flex-1 overflow-hidden p-0">
        <ScrollArea className="h-full">
          <div className="flex">
            {showLineNumbers && (
              <div className="p-4 border-r bg-muted/30 sticky left-0">
                {renderLineNumbers(content)}
              </div>
            )}
            <div className="flex-1 p-4">
              {renderContent(content)}
            </div>
          </div>
        </ScrollArea>
      </CardContent>
    </Card>
  );
}