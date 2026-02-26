import { useQuery } from "@tanstack/react-query";
import type { Project, ProjectFile } from "@shared/schema";

interface SidebarProps {
  currentProject: Project | null;
}

export default function Sidebar({ currentProject }: SidebarProps) {
  const { data: projectFiles = [] } = useQuery({
    queryKey: ["/api/projects", currentProject?.id, "files"],
    enabled: !!currentProject,
    select: (data: ProjectFile[]) => data,
  });

  const hasFiles = projectFiles.length > 0;

  return (
    <aside className="w-80 bg-card border-r border-border flex flex-col">
      <div className="p-6 border-b border-border">
        <h2 className="text-lg font-semibold mb-4" data-testid="heading-project-structure">
          Project Structure
        </h2>
        
        {!hasFiles ? (
          <div className="text-center py-8" data-testid="empty-state-project">
            <i className="fas fa-folder-open text-4xl text-muted-foreground mb-4"></i>
            <p className="text-muted-foreground mb-2">No project initialized</p>
            <p className="text-sm text-muted-foreground">Upload a package to get started</p>
          </div>
        ) : (
          <div className="space-y-1" data-testid="project-tree">
            {projectFiles.map((file) => (
              <div 
                key={file.id}
                className={`flex items-center space-x-2 py-1 px-2 hover:bg-accent rounded-sm cursor-pointer ${
                  file.parentId ? 'ml-4' : ''
                }`}
                data-testid={`file-item-${file.name}`}
              >
                <i className={`fas ${
                  file.type === 'folder' ? 'fa-folder text-primary' : 'fa-file-code text-muted-foreground'
                } text-xs w-4`}></i>
                <span className="text-sm font-mono">{file.name}</span>
              </div>
            ))}
          </div>
        )}
      </div>

      <div className="p-6 border-b border-border">
        <h3 className="text-sm font-semibold mb-3" data-testid="heading-quick-actions">
          Quick Actions
        </h3>
        <div className="space-y-2">
          <button 
            className="w-full text-left px-3 py-2 text-sm bg-secondary hover:bg-accent rounded-md transition-colors flex items-center space-x-2"
            disabled={!hasFiles}
            data-testid="button-run-analysis"
          >
            <i className="fas fa-play text-xs"></i>
            <span>Run Analysis</span>
          </button>
          <button 
            className="w-full text-left px-3 py-2 text-sm bg-secondary hover:bg-accent rounded-md transition-colors flex items-center space-x-2"
            disabled={!hasFiles}
            data-testid="button-export-results"
          >
            <i className="fas fa-download text-xs"></i>
            <span>Export Results</span>
          </button>
          <button 
            className="w-full text-left px-3 py-2 text-sm bg-secondary hover:bg-accent rounded-md transition-colors flex items-center space-x-2"
            disabled={!hasFiles}
            data-testid="button-clear-project"
          >
            <i className="fas fa-trash text-xs"></i>
            <span>Clear Project</span>
          </button>
        </div>
      </div>

      <div className="p-6 flex-1">
        <h3 className="text-sm font-semibold mb-3" data-testid="heading-analysis-status">
          Analysis Status
        </h3>
        <div className="bg-muted rounded-lg p-4">
          <div className="flex items-center space-x-2 mb-2">
            <div className={`w-2 h-2 rounded-full ${
              currentProject?.status === 'completed' ? 'bg-green-500' :
              currentProject?.status === 'processing' ? 'bg-yellow-500' :
              currentProject?.status === 'failed' ? 'bg-red-500' :
              'bg-muted-foreground'
            }`}></div>
            <span className="text-sm text-muted-foreground" data-testid="text-status">
              {currentProject?.status === 'completed' ? 'Completed' :
               currentProject?.status === 'processing' ? 'Processing' :
               currentProject?.status === 'failed' ? 'Failed' :
               'Ready'}
            </span>
          </div>
          <p className="text-xs text-muted-foreground" data-testid="text-status-description">
            {currentProject?.status === 'completed' ? 'Package analysis complete' :
             currentProject?.status === 'processing' ? 'Analyzing package structure' :
             currentProject?.status === 'failed' ? 'Analysis failed, please try again' :
             'Waiting for package upload'}
          </p>
        </div>
      </div>
    </aside>
  );
}
