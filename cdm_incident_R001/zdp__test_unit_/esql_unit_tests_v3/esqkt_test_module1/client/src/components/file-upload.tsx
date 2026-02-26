import { useRef, useState } from "react";
import { useMutation } from "@tanstack/react-query";
import { useToast } from "@/hooks/use-toast";
import { queryClient } from "@/lib/queryClient";
import type { Project } from "@shared/schema";

interface FileUploadProps {
  projectId: string;
  onUploadSuccess: (project: Project) => void;
}

export default function FileUpload({ projectId, onUploadSuccess }: FileUploadProps) {
  const fileInputRef = useRef<HTMLInputElement>(null);
  const [isDragOver, setIsDragOver] = useState(false);
  const { toast } = useToast();

  const uploadMutation = useMutation({
    mutationFn: async (file: File) => {
      const formData = new FormData();
      formData.append('package', file);
      
      const response = await fetch(`/api/projects/${projectId}/upload`, {
        method: 'POST',
        body: formData,
      });
      
      if (!response.ok) {
        const error = await response.json();
        throw new Error(error.message || 'Upload failed');
      }
      
      return response.json();
    },
    onSuccess: (data) => {
      onUploadSuccess(data.project);
      queryClient.invalidateQueries({ queryKey: ["/api/projects", projectId, "files"] });
      toast({
        title: "Package uploaded successfully",
        description: "Project initialized and ready for analysis",
      });
    },
    onError: (error: Error) => {
      toast({
        title: "Upload failed",
        description: error.message || "The file format is not supported or the file is corrupted",
        variant: "destructive",
      });
    }
  });

  const handleFileSelect = (files: FileList | null) => {
    if (files && files.length > 0) {
      const file = files[0];
      uploadMutation.mutate(file);
    }
  };

  const handleClick = () => {
    fileInputRef.current?.click();
  };

  const handleDragOver = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragOver(true);
  };

  const handleDragLeave = () => {
    setIsDragOver(false);
  };

  const handleDrop = (e: React.DragEvent) => {
    e.preventDefault();
    setIsDragOver(false);
    handleFileSelect(e.dataTransfer.files);
  };

  return (
    <div className="mb-8">
      <div 
        className={`upload-zone rounded-lg p-12 text-center cursor-pointer ${
          isDragOver ? 'dragover' : ''
        }`}
        onClick={handleClick}
        onDragOver={handleDragOver}
        onDragLeave={handleDragLeave}
        onDrop={handleDrop}
        data-testid="upload-zone"
      >
        <div className="max-w-md mx-auto">
          {uploadMutation.isPending ? (
            <>
              <i className="fas fa-spinner fa-spin text-6xl text-primary mb-6"></i>
              <h3 className="text-xl font-semibold mb-2">Processing package...</h3>
              <p className="text-muted-foreground mb-6">Extracting files and analyzing structure</p>
            </>
          ) : (
            <>
              <i className="fas fa-cloud-upload-alt text-6xl text-primary mb-6"></i>
              <h3 className="text-xl font-semibold mb-2" data-testid="heading-upload">
                Upload Package
              </h3>
              <p className="text-muted-foreground mb-6">
                Drag and drop your package file here, or click to browse
              </p>
            </>
          )}
          
          <div className="flex flex-wrap gap-2 justify-center mb-4">
            <span className="px-3 py-1 bg-accent rounded-full text-xs">.zip</span>
            <span className="px-3 py-1 bg-accent rounded-full text-xs">.tar.gz</span>
            <span className="px-3 py-1 bg-accent rounded-full text-xs">.tar</span>
            <span className="px-3 py-1 bg-accent rounded-full text-xs">.whl</span>
            <span className="px-3 py-1 bg-accent rounded-full text-xs">.tgz</span>
          </div>
          
          {!uploadMutation.isPending && (
            <button 
              className="bg-primary hover:bg-primary/90 text-primary-foreground px-6 py-2 rounded-md transition-colors"
              data-testid="button-choose-file"
            >
              Choose File
            </button>
          )}
          
          <input 
            ref={fileInputRef}
            type="file" 
            className="hidden" 
            accept=".zip,.tar.gz,.tar,.whl,.tgz"
            onChange={(e) => handleFileSelect(e.target.files)}
            data-testid="input-file"
          />
        </div>
      </div>
    </div>
  );
}
