import { useState } from "react";
import Header from "@/components/header";
import Sidebar from "@/components/sidebar";
import FileUpload from "@/components/file-upload";
import ProjectConfiguration from "@/components/project-configuration";
import AdvancedOptions from "@/components/advanced-options";
import { Button } from "@/components/ui/button";
import { useQuery, useMutation } from "@tanstack/react-query";
import { apiRequest, queryClient } from "@/lib/queryClient";
import { useToast } from "@/hooks/use-toast";
import type { Project, InsertProject } from "@shared/schema";

export default function Home() {
  const [currentProject, setCurrentProject] = useState<Project | null>(null);
  const [projectConfig, setProjectConfig] = useState<InsertProject>({
    name: "my-package-analysis",
    packageType: "auto-detect",
    analysisDepth: "standard",
    outputFormat: "json",
    includeVulnerabilityScan: false,
    generateScaffolding: true,
    customRules: null,
    excludePatterns: null,
    memoryLimit: "512",
    timeout: "300"
  });
  
  const { toast } = useToast();

  const { data: projects = [] } = useQuery({
    queryKey: ["/api/projects"],
    select: (data: Project[]) => data,
  });

  const createProjectMutation = useMutation({
    mutationFn: async (config: InsertProject) => {
      const res = await apiRequest("POST", "/api/projects", {
        ...config,
        userId: "default-user"
      });
      return await res.json();
    },
    onSuccess: (project: Project) => {
      setCurrentProject(project);
      queryClient.invalidateQueries({ queryKey: ["/api/projects"] });
      toast({
        title: "Project created successfully",
        description: "Your project is ready for package upload.",
      });
    },
    onError: () => {
      toast({
        title: "Failed to create project",
        description: "Please try again later.",
        variant: "destructive",
      });
    }
  });

  const handleCreateProject = () => {
    createProjectMutation.mutate(projectConfig);
  };

  const handleResetConfiguration = () => {
    setProjectConfig({
      name: "my-package-analysis",
      packageType: "auto-detect",
      analysisDepth: "standard",
      outputFormat: "json",
      includeVulnerabilityScan: false,
      generateScaffolding: true,
      customRules: null,
      excludePatterns: null,
      memoryLimit: "512",
      timeout: "300"
    });
    setCurrentProject(null);
  };

  const canCreateProject = projectConfig.name.trim() !== "";

  return (
    <div className="min-h-screen bg-background dark">
      <Header />
      
      <div className="flex h-[calc(100vh-73px)]">
        <Sidebar currentProject={currentProject} />
        
        <main className="flex-1 bg-background overflow-auto">
          <div className="p-8">
            <div className="max-w-4xl mx-auto">
              <div className="mb-8">
                <h2 className="text-2xl font-bold mb-2" data-testid="heading-create-project">
                  Create New Project
                </h2>
                <p className="text-muted-foreground">
                  Upload a package to analyze its structure and dependencies. Supports Python, Node.js, and other common package formats.
                </p>
              </div>

              {currentProject && (
                <FileUpload 
                  projectId={currentProject.id} 
                  onUploadSuccess={(project) => setCurrentProject(project)}
                />
              )}

              <ProjectConfiguration 
                config={projectConfig}
                onChange={setProjectConfig}
              />

              <AdvancedOptions 
                config={projectConfig}
                onChange={setProjectConfig}
              />

              <div className="flex flex-col sm:flex-row gap-4 justify-end">
                <Button 
                  variant="secondary"
                  onClick={handleResetConfiguration}
                  data-testid="button-reset"
                >
                  Reset Configuration
                </Button>
                <Button 
                  variant="outline" 
                  disabled
                  data-testid="button-save-template"
                >
                  Save Template
                </Button>
                <Button 
                  onClick={handleCreateProject}
                  disabled={!canCreateProject || createProjectMutation.isPending}
                  data-testid="button-create-project"
                >
                  {createProjectMutation.isPending ? "Creating..." : "Create Project"}
                </Button>
              </div>
            </div>
          </div>
        </main>
      </div>
    </div>
  );
}
