import { useQuery, useMutation, useQueryClient } from "@tanstack/react-query";
import { useState } from "react";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Badge } from "@/components/ui/badge";
import { Alert, AlertDescription } from "@/components/ui/alert";
import { useToast } from "@/hooks/use-toast";
import { Settings, HelpCircle, Box, CheckCircle, Clock, Plus, PlusCircle, Plug, Database, Rocket, ShieldCheck, Bug, AlertTriangle, Code, ChevronRight } from "lucide-react";
import { apiRequest } from "@/lib/queryClient";
import type { ProjectConfig, DevelopmentTool, FutureFeature } from "@shared/schema";

export default function Home() {
  const { toast } = useToast();
  const queryClient = useQueryClient();
  const [configFormData, setConfigFormData] = useState<{
    name: string;
    environment: "development" | "staging" | "production";
  }>({
    name: "",
    environment: "development"
  });

  // Fetch project configuration
  const { data: projectConfig, isLoading: configLoading, error: configError } = useQuery<ProjectConfig>({
    queryKey: ["/api/project"],
  });

  // Fetch development tools
  const { data: developmentTools, isLoading: toolsLoading } = useQuery<DevelopmentTool[]>({
    queryKey: ["/api/tools"],
  });

  // Fetch future features
  const { data: futureFeatures, isLoading: featuresLoading } = useQuery<FutureFeature[]>({
    queryKey: ["/api/features"],
  });

  // Update project configuration mutation
  const updateProjectMutation = useMutation({
    mutationFn: async (updates: { name: string; environment: "development" | "staging" | "production" }) => {
      return apiRequest("PATCH", "/api/project", updates);
    },
    onSuccess: () => {
      queryClient.invalidateQueries({ queryKey: ["/api/project"] });
      toast({
        title: "Success",
        description: "Project configuration updated successfully",
      });
    },
    onError: (error) => {
      console.error("Failed to update project:", error);
      toast({
        title: "Error",
        description: "Failed to update project configuration",
        variant: "destructive",
      });
    },
  });

  // Initialize form when project config loads
  if (projectConfig && configFormData.name === "") {
    setConfigFormData({
      name: projectConfig.name,
      environment: projectConfig.environment,
    });
  }

  const handleConfigSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    if (!configFormData.name.trim()) {
      toast({
        title: "Validation Error",
        description: "Project name is required",
        variant: "destructive",
      });
      return;
    }
    updateProjectMutation.mutate(configFormData);
  };

  const handleQuickAction = (action: string) => {
    toast({
      title: "Feature Coming Soon",
      description: `${action} functionality will be implemented in future updates`,
    });
  };

  if (configError) {
    return (
      <div className="min-h-screen bg-gray-50 dark:bg-gray-900 flex items-center justify-center">
        <Alert variant="destructive" className="max-w-md">
          <AlertTriangle className="h-4 w-4" />
          <AlertDescription>
            Failed to load project configuration. Please check your connection and try again.
          </AlertDescription>
        </Alert>
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-gray-50 dark:bg-gray-900">
      {/* Header */}
      <header className="bg-white dark:bg-gray-900 shadow-sm border-b border-gray-200 dark:border-gray-800">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
          <div className="flex justify-between items-center h-16">
            <div className="flex items-center space-x-3">
              <div className="w-8 h-8 bg-primary rounded-lg flex items-center justify-center">
                <Box className="text-primary-foreground" size={16} />
              </div>
              <h1 className="text-xl font-semibold text-gray-900 dark:text-gray-100" data-testid="text-project-title">
                {configLoading ? "Loading..." : projectConfig?.name || "New Project"}
              </h1>
              <Badge variant="secondary" className="status-ready" data-testid="badge-project-status">
                Ready
              </Badge>
            </div>
            <div className="flex items-center space-x-4">
              <Button variant="ghost" size="sm" data-testid="button-settings">
                <Settings size={16} />
              </Button>
              <Button variant="ghost" size="sm" data-testid="button-help">
                <HelpCircle size={16} />
              </Button>
            </div>
          </div>
        </div>
      </header>

      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        {/* Project Status */}
        <div className="mb-8">
          <Card>
            <CardHeader>
              <div className="flex items-center justify-between">
                <CardTitle data-testid="text-project-status-title">Project Status</CardTitle>
                <div className="flex items-center space-x-2">
                  <CheckCircle className="text-success" size={20} />
                  <span className="text-sm font-medium text-success" data-testid="text-status-initialized">
                    Initialized
                  </span>
                </div>
              </div>
            </CardHeader>
            <CardContent>
              <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                <div className="flex items-center space-x-3 p-3 bg-success/5 rounded-lg" data-testid="card-dev-environment">
                  <CheckCircle className="text-success" size={20} />
                  <div>
                    <p className="text-sm font-medium text-gray-900 dark:text-gray-100">Development Environment</p>
                    <p className="text-xs text-gray-600 dark:text-gray-400">Ready for development</p>
                  </div>
                </div>
                
                <div className="flex items-center space-x-3 p-3 bg-success/5 rounded-lg" data-testid="card-project-structure">
                  <CheckCircle className="text-success" size={20} />
                  <div>
                    <p className="text-sm font-medium text-gray-900 dark:text-gray-100">Project Structure</p>
                    <p className="text-xs text-gray-600 dark:text-gray-400">Basic setup complete</p>
                  </div>
                </div>
                
                <div className="flex items-center space-x-3 p-3 bg-success/5 rounded-lg" data-testid="card-error-handling">
                  <CheckCircle className="text-success" size={20} />
                  <div>
                    <p className="text-sm font-medium text-gray-900 dark:text-gray-100">Error Handling</p>
                    <p className="text-xs text-gray-600 dark:text-gray-400">Basic handlers configured</p>
                  </div>
                </div>
              </div>
            </CardContent>
          </Card>
        </div>

        <div className="grid grid-cols-1 lg:grid-cols-3 gap-8">
          {/* Main Content */}
          <div className="lg:col-span-2">
            {/* Component Placeholders */}
            <Card className="mb-6">
              <CardHeader>
                <CardTitle data-testid="text-component-placeholders-title">Component Placeholders</CardTitle>
              </CardHeader>
              <CardContent className="space-y-4">
                {/* Main Component Area */}
                <div className="border-2 border-dashed border-gray-300 dark:border-gray-600 rounded-lg p-6 text-center" data-testid="placeholder-main-component">
                  <div className="w-12 h-12 mx-auto mb-3 text-gray-400 flex items-center justify-center">
                    <Plus size={48} />
                  </div>
                  <h4 className="text-sm font-medium text-gray-900 dark:text-gray-100 mb-2">Main Component Area</h4>
                  <p className="text-xs text-gray-600 dark:text-gray-400 mb-4">This area is ready for your primary application component</p>
                  <Button 
                    onClick={() => handleQuickAction("Add Component")}
                    data-testid="button-add-component"
                  >
                    <Plus className="mr-2" size={16} />
                    Add Component
                  </Button>
                </div>
                
                {/* Secondary Placeholders */}
                <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                  <div className="border-2 border-dashed border-gray-300 dark:border-gray-600 rounded-lg p-4 text-center" data-testid="placeholder-data-visualization">
                    <div className="w-8 h-8 mx-auto mb-2 text-gray-400 flex items-center justify-center">
                      <Code size={32} />
                    </div>
                    <h4 className="text-sm font-medium text-gray-900 dark:text-gray-100 mb-1">Data Visualization</h4>
                    <p className="text-xs text-gray-600 dark:text-gray-400">Charts and graphs placeholder</p>
                  </div>
                  <div className="border-2 border-dashed border-gray-300 dark:border-gray-600 rounded-lg p-4 text-center" data-testid="placeholder-list-component">
                    <div className="w-8 h-8 mx-auto mb-2 text-gray-400 flex items-center justify-center">
                      <Code size={32} />
                    </div>
                    <h4 className="text-sm font-medium text-gray-900 dark:text-gray-100 mb-1">List Component</h4>
                    <p className="text-xs text-gray-600 dark:text-gray-400">Dynamic list placeholder</p>
                  </div>
                </div>
              </CardContent>
            </Card>

            {/* Future Features */}
            <Card>
              <CardHeader>
                <CardTitle data-testid="text-future-features-title">Future Development Areas</CardTitle>
              </CardHeader>
              <CardContent>
                <div className="space-y-3">
                  {featuresLoading ? (
                    <div className="text-sm text-gray-600 dark:text-gray-400">Loading features...</div>
                  ) : (
                    futureFeatures?.map((feature) => (
                      <div key={feature.id} className="flex items-center space-x-3 p-3 bg-gray-50 dark:bg-gray-800 rounded-lg" data-testid={`card-feature-${feature.id}`}>
                        <Clock className="text-warning flex-shrink-0" size={16} />
                        <div className="flex-1">
                          <p className="text-sm font-medium text-gray-900 dark:text-gray-100">{feature.name}</p>
                          <p className="text-xs text-gray-600 dark:text-gray-400">{feature.description}</p>
                        </div>
                        <Badge variant="secondary" className="status-planned">
                          {feature.status.charAt(0).toUpperCase() + feature.status.slice(1)}
                        </Badge>
                      </div>
                    ))
                  )}
                </div>
              </CardContent>
            </Card>
          </div>

          {/* Sidebar */}
          <div className="space-y-6">
            {/* Development Tools */}
            <Card>
              <CardHeader>
                <CardTitle data-testid="text-dev-tools-title">Development Tools</CardTitle>
              </CardHeader>
              <CardContent>
                <div className="space-y-3">
                  {toolsLoading ? (
                    <div className="text-sm text-gray-600 dark:text-gray-400">Loading tools...</div>
                  ) : (
                    developmentTools?.map((tool) => (
                      <div key={tool.id} className="flex items-center justify-between p-3 bg-gray-50 dark:bg-gray-800 rounded-lg" data-testid={`card-tool-${tool.id}`}>
                        <div className="flex items-center space-x-3">
                          <Code className="text-primary" size={16} />
                          <span className="text-sm font-medium text-gray-900 dark:text-gray-100">{tool.name}</span>
                        </div>
                        <span className="text-xs text-gray-600 dark:text-gray-400">{tool.version}</span>
                      </div>
                    ))
                  )}
                </div>
              </CardContent>
            </Card>

            {/* Project Configuration */}
            <Card>
              <CardHeader>
                <CardTitle data-testid="text-configuration-title">Configuration</CardTitle>
              </CardHeader>
              <CardContent>
                <form onSubmit={handleConfigSubmit} className="space-y-4">
                  <div className="space-y-2">
                    <Label htmlFor="project-name">Project Name</Label>
                    <Input
                      id="project-name"
                      value={configFormData.name}
                      onChange={(e) => setConfigFormData(prev => ({ ...prev, name: e.target.value }))}
                      placeholder="Enter project name"
                      data-testid="input-project-name"
                    />
                  </div>
                  
                  <div className="space-y-2">
                    <Label htmlFor="environment">Environment</Label>
                    <Select value={configFormData.environment} onValueChange={(value: "development" | "staging" | "production") => 
                      setConfigFormData(prev => ({ ...prev, environment: value }))
                    }>
                      <SelectTrigger data-testid="select-environment">
                        <SelectValue placeholder="Select environment" />
                      </SelectTrigger>
                      <SelectContent>
                        <SelectItem value="development">Development</SelectItem>
                        <SelectItem value="staging">Staging</SelectItem>
                        <SelectItem value="production">Production</SelectItem>
                      </SelectContent>
                    </Select>
                  </div>
                  
                  <Button 
                    type="submit" 
                    className="w-full" 
                    disabled={updateProjectMutation.isPending}
                    data-testid="button-save-config"
                  >
                    {updateProjectMutation.isPending ? "Saving..." : (
                      <>
                        <Settings className="mr-2" size={16} />
                        Save Configuration
                      </>
                    )}
                  </Button>
                </form>
              </CardContent>
            </Card>

            {/* Error Handling Status */}
            <Card>
              <CardHeader>
                <CardTitle data-testid="text-error-handling-title">Error Handling</CardTitle>
              </CardHeader>
              <CardContent>
                <div className="space-y-3">
                  <div className="flex items-center space-x-3 p-3 bg-success/5 rounded-lg" data-testid="card-error-boundaries">
                    <ShieldCheck className="text-success" size={16} />
                    <div>
                      <p className="text-sm font-medium text-gray-900 dark:text-gray-100">Error Boundaries</p>
                      <p className="text-xs text-gray-600 dark:text-gray-400">Configured and ready</p>
                    </div>
                  </div>
                  
                  <div className="flex items-center space-x-3 p-3 bg-success/5 rounded-lg" data-testid="card-logging-system">
                    <Bug className="text-success" size={16} />
                    <div>
                      <p className="text-sm font-medium text-gray-900 dark:text-gray-100">Logging System</p>
                      <p className="text-xs text-gray-600 dark:text-gray-400">Basic setup complete</p>
                    </div>
                  </div>
                  
                  <div className="flex items-center space-x-3 p-3 bg-warning/5 rounded-lg" data-testid="card-custom-handlers">
                    <AlertTriangle className="text-warning" size={16} />
                    <div>
                      <p className="text-sm font-medium text-gray-900 dark:text-gray-100">Custom Handlers</p>
                      <p className="text-xs text-gray-600 dark:text-gray-400">Awaiting implementation</p>
                    </div>
                  </div>
                </div>
              </CardContent>
            </Card>
          </div>
        </div>

        {/* Quick Actions */}
        <Card className="mt-8">
          <CardHeader>
            <CardTitle data-testid="text-quick-actions-title">Quick Actions</CardTitle>
          </CardHeader>
          <CardContent>
            <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
              <Button
                variant="outline"
                className="flex flex-col items-center p-4 h-auto space-y-2"
                onClick={() => handleQuickAction("Generate Component")}
                data-testid="button-add-component-quick"
              >
                <PlusCircle className="text-primary" size={24} />
                <span className="text-sm font-medium">Add Component</span>
              </Button>
              
              <Button
                variant="outline"
                className="flex flex-col items-center p-4 h-auto space-y-2"
                onClick={() => handleQuickAction("Setup API")}
                data-testid="button-setup-api"
              >
                <Plug className="text-primary" size={24} />
                <span className="text-sm font-medium">Setup API</span>
              </Button>
              
              <Button
                variant="outline"
                className="flex flex-col items-center p-4 h-auto space-y-2"
                onClick={() => handleQuickAction("Configure Database")}
                data-testid="button-configure-db"
              >
                <Database className="text-primary" size={24} />
                <span className="text-sm font-medium">Configure DB</span>
              </Button>
              
              <Button
                variant="outline"
                className="flex flex-col items-center p-4 h-auto space-y-2"
                onClick={() => handleQuickAction("Deploy Project")}
                data-testid="button-deploy"
              >
                <Rocket className="text-primary" size={24} />
                <span className="text-sm font-medium">Deploy</span>
              </Button>
            </div>
          </CardContent>
        </Card>
      </main>

      {/* Footer */}
      <footer className="bg-white dark:bg-gray-900 border-t border-gray-200 dark:border-gray-800 mt-12">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-6">
          <div className="flex items-center justify-between">
            <p className="text-sm text-gray-600 dark:text-gray-400" data-testid="text-footer-status">
              Project initialized and ready for development
            </p>
            <div className="flex items-center space-x-4">
              <span className="flex items-center space-x-2 text-sm text-gray-600 dark:text-gray-400">
                <Code className="text-primary" size={16} />
                <span data-testid="text-ready-to-code">Ready to code</span>
              </span>
            </div>
          </div>
        </div>
      </footer>
    </div>
  );
}
