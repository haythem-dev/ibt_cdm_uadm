import { useState } from 'react';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Badge } from '@/components/ui/badge';
import { Button } from '@/components/ui/button';
import { Progress } from '@/components/ui/progress';
import { 
  FileText, 
  Archive, 
  AlertTriangle, 
  CheckCircle, 
  Info,
  Download,
  Share
} from 'lucide-react';

interface AnalysisResult {
  packageName: string;
  version?: string;
  size: number;
  fileCount: number;
  folderCount: number;
  vulnerabilities: number;
  dependencies: string[];
  description?: string;
  author?: string;
  license?: string;
}

interface AnalysisDashboardProps {
  analysisResult?: AnalysisResult;
  isAnalyzing?: boolean;
}

export default function AnalysisDashboard({ 
  analysisResult, 
  isAnalyzing = false 
}: AnalysisDashboardProps) {
  const [activeTab, setActiveTab] = useState('overview');

  const formatFileSize = (bytes: number) => {
    const sizes = ['B', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(1024));
    return Math.round(bytes / Math.pow(1024, i) * 100) / 100 + ' ' + sizes[i];
  };

  const getSecurityStatus = (vulnerabilities: number) => {
    if (vulnerabilities === 0) return { status: 'Safe', color: 'text-green-600', icon: CheckCircle };
    if (vulnerabilities <= 5) return { status: 'Warning', color: 'text-yellow-600', icon: AlertTriangle };
    return { status: 'High Risk', color: 'text-red-600', icon: AlertTriangle };
  };

  if (isAnalyzing) {
    return (
      <Card className="h-full">
        <CardHeader>
          <CardTitle className="flex items-center gap-2">
            <Archive className="h-5 w-5 animate-pulse" />
            Analyzing Package...
          </CardTitle>
        </CardHeader>
        <CardContent className="space-y-4">
          <div className="space-y-2">
            <div className="flex justify-between text-sm">
              <span>Extracting files...</span>
              <span>78%</span>
            </div>
            <Progress value={78} className="h-2" />
          </div>
          <div className="space-y-3">
            <div className="flex items-center gap-2 text-sm text-muted-foreground">
              <div className="w-2 h-2 bg-primary rounded-full animate-pulse" />
              Scanning file structure
            </div>
            <div className="flex items-center gap-2 text-sm text-muted-foreground">
              <div className="w-2 h-2 bg-muted rounded-full" />
              Analyzing dependencies
            </div>
            <div className="flex items-center gap-2 text-sm text-muted-foreground">
              <div className="w-2 h-2 bg-muted rounded-full" />
              Security scan
            </div>
          </div>
        </CardContent>
      </Card>
    );
  }

  if (!analysisResult) {
    return (
      <Card className="h-full flex items-center justify-center">
        <CardContent className="text-center">
          <Archive className="h-12 w-12 text-muted-foreground mx-auto mb-4" />
          <h3 className="text-lg font-semibold mb-2">No Package Loaded</h3>
          <p className="text-muted-foreground">
            Upload a package to see detailed analysis results
          </p>
        </CardContent>
      </Card>
    );
  }

  const securityStatus = getSecurityStatus(analysisResult.vulnerabilities);
  const SecurityIcon = securityStatus.icon;

  return (
    <Card className="h-full">
      <CardHeader className="pb-3">
        <div className="flex items-center justify-between">
          <CardTitle className="flex items-center gap-2">
            <Archive className="h-5 w-5" />
            Package Analysis
          </CardTitle>
          <div className="flex gap-2">
            <Button size="sm" variant="outline" data-testid="button-export-results">
              <Download className="h-4 w-4 mr-2" />
              Export
            </Button>
            <Button size="sm" variant="outline" data-testid="button-share-results">
              <Share className="h-4 w-4 mr-2" />
              Share
            </Button>
          </div>
        </div>
      </CardHeader>
      
      <CardContent className="flex-1 overflow-auto">
        <Tabs value={activeTab} onValueChange={setActiveTab}>
          <TabsList className="grid w-full grid-cols-3">
            <TabsTrigger value="overview" data-testid="tab-overview">Overview</TabsTrigger>
            <TabsTrigger value="dependencies" data-testid="tab-dependencies">Dependencies</TabsTrigger>
            <TabsTrigger value="security" data-testid="tab-security">Security</TabsTrigger>
          </TabsList>

          <TabsContent value="overview" className="space-y-4">
            <div className="grid grid-cols-2 gap-4">
              <Card>
                <CardContent className="p-4">
                  <div className="flex items-center gap-2 mb-2">
                    <FileText className="h-4 w-4 text-primary" />
                    <span className="font-medium">Package Info</span>
                  </div>
                  <div className="space-y-1 text-sm">
                    <div><span className="text-muted-foreground">Name:</span> {analysisResult.packageName}</div>
                    {analysisResult.version && (
                      <div><span className="text-muted-foreground">Version:</span> {analysisResult.version}</div>
                    )}
                    {analysisResult.author && (
                      <div><span className="text-muted-foreground">Author:</span> {analysisResult.author}</div>
                    )}
                    {analysisResult.license && (
                      <div><span className="text-muted-foreground">License:</span> {analysisResult.license}</div>
                    )}
                  </div>
                </CardContent>
              </Card>

              <Card>
                <CardContent className="p-4">
                  <div className="flex items-center gap-2 mb-2">
                    <Archive className="h-4 w-4 text-primary" />
                    <span className="font-medium">Structure</span>
                  </div>
                  <div className="space-y-1 text-sm">
                    <div><span className="text-muted-foreground">Size:</span> {formatFileSize(analysisResult.size)}</div>
                    <div><span className="text-muted-foreground">Files:</span> {analysisResult.fileCount}</div>
                    <div><span className="text-muted-foreground">Folders:</span> {analysisResult.folderCount}</div>
                  </div>
                </CardContent>
              </Card>
            </div>

            {analysisResult.description && (
              <Card>
                <CardContent className="p-4">
                  <div className="flex items-center gap-2 mb-2">
                    <Info className="h-4 w-4 text-primary" />
                    <span className="font-medium">Description</span>
                  </div>
                  <p className="text-sm text-muted-foreground">{analysisResult.description}</p>
                </CardContent>
              </Card>
            )}
          </TabsContent>

          <TabsContent value="dependencies" className="space-y-4">
            <Card>
              <CardContent className="p-4">
                <div className="flex items-center gap-2 mb-3">
                  <span className="font-medium">Dependencies ({analysisResult.dependencies.length})</span>
                </div>
                {analysisResult.dependencies.length === 0 ? (
                  <p className="text-sm text-muted-foreground">No dependencies found</p>
                ) : (
                  <div className="space-y-2">
                    {analysisResult.dependencies.map((dep, index) => (
                      <div key={index} className="flex items-center justify-between p-2 border rounded-md">
                        <span className="text-sm font-mono">{dep}</span>
                        <Badge variant="secondary">dependency</Badge>
                      </div>
                    ))}
                  </div>
                )}
              </CardContent>
            </Card>
          </TabsContent>

          <TabsContent value="security" className="space-y-4">
            <Card>
              <CardContent className="p-4">
                <div className="flex items-center gap-2 mb-3">
                  <SecurityIcon className={`h-4 w-4 ${securityStatus.color}`} />
                  <span className="font-medium">Security Status</span>
                  <Badge 
                    variant={analysisResult.vulnerabilities === 0 ? "secondary" : "destructive"}
                    className="ml-auto"
                  >
                    {securityStatus.status}
                  </Badge>
                </div>
                <div className="space-y-2">
                  <div className="flex justify-between text-sm">
                    <span>Vulnerabilities found:</span>
                    <span className={analysisResult.vulnerabilities > 0 ? 'text-red-600' : 'text-green-600'}>
                      {analysisResult.vulnerabilities}
                    </span>
                  </div>
                  {analysisResult.vulnerabilities > 0 && (
                    <div className="p-3 bg-destructive/10 border border-destructive/20 rounded-md">
                      <p className="text-sm text-destructive">
                        Security issues detected. Review dependencies and update to secure versions.
                      </p>
                    </div>
                  )}
                  {analysisResult.vulnerabilities === 0 && (
                    <div className="p-3 bg-green-50 dark:bg-green-900/20 border border-green-200 dark:border-green-800 rounded-md">
                      <p className="text-sm text-green-700 dark:text-green-400">
                        No known security vulnerabilities found.
                      </p>
                    </div>
                  )}
                </div>
              </CardContent>
            </Card>
          </TabsContent>
        </Tabs>
      </CardContent>
    </Card>
  );
}