import { Card } from "@/components/ui/card";
import { Badge } from "@/components/ui/badge";
import { Button } from "@/components/ui/button";
import { 
  Code, 
  Rocket, 
  Monitor, 
  Server, 
  Database, 
  CheckCircle, 
  Clock, 
  Circle,
  Plus,
  Route,
  Plug,
  Settings,
  FolderOpen,
  File
} from "lucide-react";

export default function Home() {
  const handleQuickAction = (action: string) => {
    console.log('Quick action clicked:', action);
  };

  return (
    <div className="bg-gray-50 min-h-screen">
      {/* Header */}
      <header className="bg-white border-b border-gray-200">
        <div className="max-w-6xl mx-auto px-4 sm:px-6 lg:px-8">
          <div className="flex justify-between items-center py-4">
            <div className="flex items-center space-x-3">
              <div className="w-8 h-8 bg-primary rounded-lg flex items-center justify-center">
                <Code className="text-white w-4 h-4" />
              </div>
              <h1 className="text-xl font-semibold text-gray-900">New Project</h1>
            </div>
            <div className="flex items-center space-x-2">
              <Badge className="bg-success hover:bg-success/90 text-white">
                <CheckCircle className="w-3 h-3 mr-1" />
                Ready
              </Badge>
            </div>
          </div>
        </div>
      </header>

      <main className="max-w-6xl mx-auto px-4 sm:px-6 lg:px-8 py-12">
        {/* Welcome Section */}
        <div className="text-center mb-16">
          <div className="inline-flex items-center justify-center w-16 h-16 bg-success rounded-full mb-6">
            <Rocket className="text-white w-8 h-8" />
          </div>
          <h1 className="text-4xl font-bold text-gray-900 mb-4">Project Setup Complete!</h1>
          <p className="text-xl text-gray-600 max-w-2xl mx-auto">
            Your development environment is ready. Start building amazing features with this clean, extensible foundation.
          </p>
        </div>

        {/* Status Grid */}
        <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-6 mb-12">
          {/* Frontend Status */}
          <Card className="p-6">
            <div className="flex items-center justify-between mb-4">
              <div className="flex items-center space-x-3">
                <div className="w-10 h-10 bg-blue-100 rounded-lg flex items-center justify-center">
                  <Monitor className="text-blue-600 w-5 h-5" />
                </div>
                <h3 className="font-semibold text-gray-900">Frontend</h3>
              </div>
              <Badge className="bg-success hover:bg-success/90 text-white text-xs">
                <Circle className="w-2 h-2 mr-1 fill-current" />
                Active
              </Badge>
            </div>
            <p className="text-sm text-gray-600 mb-3">React development server running</p>
            <div className="text-xs text-gray-500">
              <div>Port: <span className="font-mono">5000</span></div>
              <div>Status: <span className="text-success font-medium">Connected</span></div>
            </div>
          </Card>

          {/* Backend Status */}
          <Card className="p-6">
            <div className="flex items-center justify-between mb-4">
              <div className="flex items-center space-x-3">
                <div className="w-10 h-10 bg-green-100 rounded-lg flex items-center justify-center">
                  <Server className="text-green-600 w-5 h-5" />
                </div>
                <h3 className="font-semibold text-gray-900">Backend</h3>
              </div>
              <Badge className="bg-success hover:bg-success/90 text-white text-xs">
                <Circle className="w-2 h-2 mr-1 fill-current" />
                Active
              </Badge>
            </div>
            <p className="text-sm text-gray-600 mb-3">API server initialized</p>
            <div className="text-xs text-gray-500">
              <div>Port: <span className="font-mono">5000</span></div>
              <div>Status: <span className="text-success font-medium">Running</span></div>
            </div>
          </Card>

          {/* Database Status */}
          <Card className="p-6">
            <div className="flex items-center justify-between mb-4">
              <div className="flex items-center space-x-3">
                <div className="w-10 h-10 bg-purple-100 rounded-lg flex items-center justify-center">
                  <Database className="text-purple-600 w-5 h-5" />
                </div>
                <h3 className="font-semibold text-gray-900">Database</h3>
              </div>
              <Badge variant="secondary" className="text-xs">
                <Clock className="w-2 h-2 mr-1" />
                Pending
              </Badge>
            </div>
            <p className="text-sm text-gray-600 mb-3">Ready for configuration</p>
            <div className="text-xs text-gray-500">
              <div>Config: <span className="font-mono">Not set</span></div>
              <div>Status: <span className="text-gray-500 font-medium">Configure later</span></div>
            </div>
          </Card>
        </div>

        {/* Project Structure */}
        <Card className="p-8 mb-12">
          <h2 className="text-2xl font-semibold text-gray-900 mb-6">Project Structure</h2>
          <div className="grid lg:grid-cols-2 gap-8">
            {/* File Tree */}
            <div>
              <h3 className="text-lg font-medium text-gray-900 mb-4">Directory Structure</h3>
              <div className="bg-gray-50 rounded-lg p-4 font-mono text-sm">
                <div className="space-y-1 text-gray-700">
                  <div><FolderOpen className="inline w-4 h-4 text-yellow-500 mr-2" />project-root/</div>
                  <div className="ml-4"><FolderOpen className="inline w-4 h-4 text-yellow-500 mr-2" />client/</div>
                  <div className="ml-8"><FolderOpen className="inline w-4 h-4 text-yellow-500 mr-2" />src/</div>
                  <div className="ml-12"><File className="inline w-4 h-4 text-blue-500 mr-2" />App.tsx</div>
                  <div className="ml-12"><File className="inline w-4 h-4 text-blue-500 mr-2" />main.tsx</div>
                  <div className="ml-8"><File className="inline w-4 h-4 text-gray-500 mr-2" />index.html</div>
                  <div className="ml-4"><FolderOpen className="inline w-4 h-4 text-yellow-500 mr-2" />server/</div>
                  <div className="ml-8"><File className="inline w-4 h-4 text-green-500 mr-2" />index.ts</div>
                  <div className="ml-8"><File className="inline w-4 h-4 text-green-500 mr-2" />routes.ts</div>
                  <div><File className="inline w-4 h-4 text-gray-500 mr-2" />package.json</div>
                </div>
              </div>
            </div>

            {/* Technologies */}
            <div>
              <h3 className="text-lg font-medium text-gray-900 mb-4">Tech Stack</h3>
              <div className="space-y-3">
                <div className="flex items-center justify-between p-3 bg-gray-50 rounded-lg">
                  <div className="flex items-center space-x-3">
                    <div className="w-6 h-6 bg-blue-500 rounded flex items-center justify-center">
                      <span className="text-white text-xs font-bold">R</span>
                    </div>
                    <span className="font-medium">React</span>
                  </div>
                  <span className="text-sm text-gray-500">Frontend Framework</span>
                </div>
                <div className="flex items-center justify-between p-3 bg-gray-50 rounded-lg">
                  <div className="flex items-center space-x-3">
                    <div className="w-6 h-6 bg-green-500 rounded flex items-center justify-center">
                      <span className="text-white text-xs font-bold">N</span>
                    </div>
                    <span className="font-medium">Node.js</span>
                  </div>
                  <span className="text-sm text-gray-500">Backend Runtime</span>
                </div>
                <div className="flex items-center justify-between p-3 bg-gray-50 rounded-lg">
                  <div className="flex items-center space-x-3">
                    <div className="w-6 h-6 bg-teal-500 rounded flex items-center justify-center">
                      <span className="text-white text-xs font-bold">T</span>
                    </div>
                    <span className="font-medium">Tailwind CSS</span>
                  </div>
                  <span className="text-sm text-gray-500">Styling Framework</span>
                </div>
              </div>
            </div>
          </div>
        </Card>

        {/* Quick Actions */}
        <Card className="p-8">
          <h2 className="text-2xl font-semibold text-gray-900 mb-6">Quick Actions</h2>
          <div className="grid sm:grid-cols-2 lg:grid-cols-4 gap-4">
            <Button 
              variant="outline" 
              className="flex flex-col items-center p-6 h-auto bg-gray-50 hover:bg-gray-100 border-gray-200 hover:border-gray-300"
              onClick={() => handleQuickAction('Add Component')}
              data-testid="button-add-component"
            >
              <div className="w-12 h-12 bg-primary rounded-lg flex items-center justify-center mb-3">
                <Plus className="text-white w-5 h-5" />
              </div>
              <h3 className="font-medium text-gray-900 mb-1">Add Component</h3>
              <p className="text-sm text-gray-500 text-center">Create new UI components</p>
            </Button>

            <Button 
              variant="outline" 
              className="flex flex-col items-center p-6 h-auto bg-gray-50 hover:bg-gray-100 border-gray-200 hover:border-gray-300"
              onClick={() => handleQuickAction('Add Route')}
              data-testid="button-add-route"
            >
              <div className="w-12 h-12 bg-green-500 rounded-lg flex items-center justify-center mb-3">
                <Route className="text-white w-5 h-5" />
              </div>
              <h3 className="font-medium text-gray-900 mb-1">Add Route</h3>
              <p className="text-sm text-gray-500 text-center">Setup new page routes</p>
            </Button>

            <Button 
              variant="outline" 
              className="flex flex-col items-center p-6 h-auto bg-gray-50 hover:bg-gray-100 border-gray-200 hover:border-gray-300"
              onClick={() => handleQuickAction('Add API')}
              data-testid="button-add-api"
            >
              <div className="w-12 h-12 bg-purple-500 rounded-lg flex items-center justify-center mb-3">
                <Plug className="text-white w-5 h-5" />
              </div>
              <h3 className="font-medium text-gray-900 mb-1">Add API</h3>
              <p className="text-sm text-gray-500 text-center">Create API endpoints</p>
            </Button>

            <Button 
              variant="outline" 
              className="flex flex-col items-center p-6 h-auto bg-gray-50 hover:bg-gray-100 border-gray-200 hover:border-gray-300"
              onClick={() => handleQuickAction('Configure')}
              data-testid="button-configure"
            >
              <div className="w-12 h-12 bg-orange-500 rounded-lg flex items-center justify-center mb-3">
                <Settings className="text-white w-5 h-5" />
              </div>
              <h3 className="font-medium text-gray-900 mb-1">Configure</h3>
              <p className="text-sm text-gray-500 text-center">Project settings</p>
            </Button>
          </div>
        </Card>
      </main>

      {/* Footer */}
      <footer className="border-t border-gray-200 bg-white mt-16">
        <div className="max-w-6xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
          <div className="flex flex-col sm:flex-row justify-between items-center">
            <div className="flex items-center space-x-4 mb-4 sm:mb-0">
              <span className="text-sm text-gray-500">Development Server</span>
              <div className="flex items-center space-x-1">
                <div className="w-2 h-2 bg-success rounded-full animate-pulse"></div>
                <span className="text-sm text-success font-medium">Online</span>
              </div>
            </div>
            <div className="flex items-center space-x-6 text-sm text-gray-500">
              <span>localhost:5000</span>
              <span>•</span>
              <span>Last updated: Just now</span>
            </div>
          </div>
        </div>
      </footer>
    </div>
  );
}
