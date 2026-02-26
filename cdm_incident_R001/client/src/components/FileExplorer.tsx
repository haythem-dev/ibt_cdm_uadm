import { useState } from 'react';
import { ChevronRight, ChevronDown, File, Folder, Search } from 'lucide-react';
import { Card } from '@/components/ui/card';
import { Input } from '@/components/ui/input';
import { Button } from '@/components/ui/button';
import { Badge } from '@/components/ui/badge';

interface FileNode {
  id: string;
  name: string;
  type: 'file' | 'folder';
  size?: number;
  children?: FileNode[];
  path: string;
}

interface FileExplorerProps {
  files: FileNode[];
  onFileSelect?: (file: FileNode) => void;
  selectedFile?: string;
}

export default function FileExplorer({ files, onFileSelect, selectedFile }: FileExplorerProps) {
  const [expandedFolders, setExpandedFolders] = useState<Set<string>>(new Set());
  const [searchTerm, setSearchTerm] = useState('');

  const toggleFolder = (folderId: string) => {
    const newExpanded = new Set(expandedFolders);
    if (newExpanded.has(folderId)) {
      newExpanded.delete(folderId);
    } else {
      newExpanded.add(folderId);
    }
    setExpandedFolders(newExpanded);
  };

  const getFileIcon = (type: string, name: string) => {
    if (type === 'folder') return <Folder className="h-4 w-4 text-blue-500" />;
    
    const ext = name.split('.').pop()?.toLowerCase();
    const iconClass = "h-4 w-4";
    
    switch (ext) {
      case 'js':
      case 'jsx':
      case 'ts':
      case 'tsx':
        return <File className={`${iconClass} text-yellow-500`} />;
      case 'json':
        return <File className={`${iconClass} text-green-500`} />;
      case 'md':
        return <File className={`${iconClass} text-blue-400`} />;
      case 'css':
      case 'scss':
        return <File className={`${iconClass} text-pink-500`} />;
      case 'html':
        return <File className={`${iconClass} text-orange-500`} />;
      default:
        return <File className={`${iconClass} text-muted-foreground`} />;
    }
  };

  const formatFileSize = (bytes?: number) => {
    if (!bytes) return '';
    const sizes = ['B', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(1024));
    return Math.round(bytes / Math.pow(1024, i) * 100) / 100 + ' ' + sizes[i];
  };

  const filterFiles = (files: FileNode[], term: string): FileNode[] => {
    if (!term) return files;
    
    return files.reduce((acc: FileNode[], file) => {
      if (file.name.toLowerCase().includes(term.toLowerCase())) {
        acc.push(file);
      } else if (file.type === 'folder' && file.children) {
        const filteredChildren = filterFiles(file.children, term);
        if (filteredChildren.length > 0) {
          acc.push({ ...file, children: filteredChildren });
        }
      }
      return acc;
    }, []);
  };

  const renderFileNode = (node: FileNode, level: number = 0) => {
    const isExpanded = expandedFolders.has(node.id);
    const isSelected = selectedFile === node.id;
    
    return (
      <div key={node.id}>
        <div
          className={`flex items-center gap-2 px-2 py-1 text-sm cursor-pointer hover-elevate rounded-md transition-colors ${
            isSelected ? 'bg-primary/10 text-primary' : 'hover:bg-muted/50'
          }`}
          style={{ paddingLeft: `${level * 16 + 8}px` }}
          onClick={() => {
            if (node.type === 'folder') {
              toggleFolder(node.id);
            } else {
              onFileSelect?.(node);
              console.log('File selected:', node.path);
            }
          }}
          data-testid={`file-item-${node.id}`}
        >
          {node.type === 'folder' && (
            <Button size="icon" variant="ghost" className="h-4 w-4 p-0">
              {isExpanded ? (
                <ChevronDown className="h-3 w-3" />
              ) : (
                <ChevronRight className="h-3 w-3" />
              )}
            </Button>
          )}
          {node.type === 'file' && <div className="w-4" />}
          
          {getFileIcon(node.type, node.name)}
          
          <span className="flex-1 truncate">{node.name}</span>
          
          {node.type === 'file' && node.size && (
            <Badge variant="secondary" className="text-xs">
              {formatFileSize(node.size)}
            </Badge>
          )}
        </div>
        
        {node.type === 'folder' && isExpanded && node.children && (
          <div>
            {node.children.map(child => renderFileNode(child, level + 1))}
          </div>
        )}
      </div>
    );
  };

  const filteredFiles = filterFiles(files, searchTerm);

  return (
    <Card className="h-full flex flex-col">
      <div className="p-4 border-b">
        <h3 className="font-semibold mb-3">File Explorer</h3>
        <div className="relative">
          <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 h-4 w-4 text-muted-foreground" />
          <Input
            placeholder="Search files..."
            value={searchTerm}
            onChange={(e) => setSearchTerm(e.target.value)}
            className="pl-9"
            data-testid="input-file-search"
          />
        </div>
      </div>
      
      <div className="flex-1 overflow-auto p-2">
        {filteredFiles.length === 0 ? (
          <div className="text-center py-8 text-muted-foreground">
            {searchTerm ? 'No files match your search' : 'No files available'}
          </div>
        ) : (
          <div className="space-y-1">
            {filteredFiles.map(file => renderFileNode(file))}
          </div>
        )}
      </div>
    </Card>
  );
}