import { useState, useCallback } from 'react';
import { Upload, File, X } from 'lucide-react';
import { Card } from '@/components/ui/card';
import { Button } from '@/components/ui/button';
import { Badge } from '@/components/ui/badge';

interface FileUploadZoneProps {
  onFileSelect?: (file: File) => void;
  maxFileSize?: number; // in MB
  acceptedTypes?: string[];
}

export default function FileUploadZone({ 
  onFileSelect, 
  maxFileSize = 10,
  acceptedTypes = ['.zip', '.tar', '.tar.gz', '.rar', '.7z']
}: FileUploadZoneProps) {
  const [dragActive, setDragActive] = useState(false);
  const [selectedFile, setSelectedFile] = useState<File | null>(null);
  const [uploadProgress, setUploadProgress] = useState(0);
  const [isUploading, setIsUploading] = useState(false);

  const handleDrag = useCallback((e: React.DragEvent) => {
    e.preventDefault();
    e.stopPropagation();
    if (e.type === "dragenter" || e.type === "dragover") {
      setDragActive(true);
    } else if (e.type === "dragleave") {
      setDragActive(false);
    }
  }, []);

  const handleDrop = useCallback((e: React.DragEvent) => {
    e.preventDefault();
    e.stopPropagation();
    setDragActive(false);
    
    const files = e.dataTransfer.files;
    if (files && files[0]) {
      handleFileSelect(files[0]);
    }
  }, []);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const files = e.target.files;
    if (files && files[0]) {
      handleFileSelect(files[0]);
    }
  };

  const handleFileSelect = (file: File) => {
    // Validate file size
    if (file.size > maxFileSize * 1024 * 1024) {
      console.log(`File too large. Maximum size is ${maxFileSize}MB`);
      return;
    }

    setSelectedFile(file);
    
    // Simulate upload progress
    setIsUploading(true);
    setUploadProgress(0);
    
    const interval = setInterval(() => {
      setUploadProgress(prev => {
        if (prev >= 100) {
          clearInterval(interval);
          setIsUploading(false);
          console.log('File upload completed:', file.name);
          onFileSelect?.(file);
          return 100;
        }
        return prev + 10;
      });
    }, 100);
  };

  const clearFile = () => {
    setSelectedFile(null);
    setUploadProgress(0);
    setIsUploading(false);
  };

  return (
    <Card className="p-6">
      <div className="space-y-4">
        <div className="text-center">
          <h3 className="text-lg font-semibold">Upload Package</h3>
          <p className="text-sm text-muted-foreground">
            Drag and drop your package file or click to browse
          </p>
        </div>

        {!selectedFile ? (
          <div
            className={`relative border-2 border-dashed rounded-lg p-8 text-center transition-colors ${
              dragActive 
                ? 'border-primary bg-primary/5' 
                : 'border-muted hover:border-primary/50 hover:bg-muted/50'
            }`}
            onDragEnter={handleDrag}
            onDragLeave={handleDrag}
            onDragOver={handleDrag}
            onDrop={handleDrop}
          >
            <Upload className="mx-auto h-12 w-12 text-muted-foreground mb-4" />
            <div className="space-y-2">
              <p className="text-sm text-muted-foreground">
                Drop your package here or{' '}
                <label className="text-primary hover:text-primary/80 cursor-pointer font-medium">
                  browse files
                  <input
                    type="file"
                    className="hidden"
                    accept={acceptedTypes.join(',')}
                    onChange={handleChange}
                    data-testid="input-file-upload"
                  />
                </label>
              </p>
              <div className="flex flex-wrap gap-1 justify-center">
                {acceptedTypes.map(type => (
                  <Badge key={type} variant="secondary" className="text-xs">
                    {type}
                  </Badge>
                ))}
              </div>
              <p className="text-xs text-muted-foreground">
                Maximum size: {maxFileSize}MB
              </p>
            </div>
          </div>
        ) : (
          <div className="space-y-4">
            <div className="flex items-center justify-between p-4 border rounded-lg">
              <div className="flex items-center gap-3">
                <File className="h-8 w-8 text-primary" />
                <div>
                  <p className="font-medium text-sm">{selectedFile.name}</p>
                  <p className="text-xs text-muted-foreground">
                    {(selectedFile.size / 1024 / 1024).toFixed(2)} MB
                  </p>
                </div>
              </div>
              <Button
                size="icon"
                variant="ghost"
                onClick={clearFile}
                disabled={isUploading}
                data-testid="button-clear-file"
              >
                <X className="h-4 w-4" />
              </Button>
            </div>

            {isUploading && (
              <div className="space-y-2">
                <div className="flex justify-between text-sm">
                  <span>Uploading...</span>
                  <span>{uploadProgress}%</span>
                </div>
                <div className="w-full bg-muted rounded-full h-2">
                  <div
                    className="bg-primary h-2 rounded-full transition-all duration-300"
                    style={{ width: `${uploadProgress}%` }}
                  />
                </div>
              </div>
            )}

            {!isUploading && uploadProgress === 100 && (
              <div className="text-center">
                <p className="text-sm text-green-600 dark:text-green-400 font-medium">
                  ✓ Upload completed successfully
                </p>
              </div>
            )}
          </div>
        )}
      </div>
    </Card>
  );
}