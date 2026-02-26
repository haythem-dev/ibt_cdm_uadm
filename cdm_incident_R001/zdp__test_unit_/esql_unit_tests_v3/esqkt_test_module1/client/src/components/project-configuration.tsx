import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Checkbox } from "@/components/ui/checkbox";
import type { InsertProject } from "@shared/schema";

interface ProjectConfigurationProps {
  config: InsertProject;
  onChange: (config: InsertProject) => void;
}

export default function ProjectConfiguration({ config, onChange }: ProjectConfigurationProps) {
  const updateConfig = (updates: Partial<InsertProject>) => {
    onChange({ ...config, ...updates });
  };

  return (
    <div className="bg-card border border-border rounded-lg p-6 mb-8">
      <h3 className="text-lg font-semibold mb-4" data-testid="heading-project-config">
        Project Configuration
      </h3>
      
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        <div>
          <Label htmlFor="project-name" className="block text-sm font-medium mb-2">
            Project Name
          </Label>
          <Input
            id="project-name"
            type="text"
            value={config.name}
            onChange={(e) => updateConfig({ name: e.target.value })}
            placeholder="my-package-analysis"
            data-testid="input-project-name"
          />
        </div>
        
        <div>
          <Label htmlFor="package-type" className="block text-sm font-medium mb-2">
            Package Type
          </Label>
          <Select 
            value={config.packageType} 
            onValueChange={(value) => updateConfig({ packageType: value })}
          >
            <SelectTrigger data-testid="select-package-type">
              <SelectValue placeholder="Select package type" />
            </SelectTrigger>
            <SelectContent>
              <SelectItem value="auto-detect">Auto-detect</SelectItem>
              <SelectItem value="python">Python Package</SelectItem>
              <SelectItem value="nodejs">Node.js Package</SelectItem>
              <SelectItem value="java">Java Archive</SelectItem>
              <SelectItem value="generic">Generic Archive</SelectItem>
            </SelectContent>
          </Select>
        </div>
        
        <div>
          <Label htmlFor="analysis-depth" className="block text-sm font-medium mb-2">
            Analysis Depth
          </Label>
          <Select 
            value={config.analysisDepth} 
            onValueChange={(value) => updateConfig({ analysisDepth: value })}
          >
            <SelectTrigger data-testid="select-analysis-depth">
              <SelectValue placeholder="Select analysis depth" />
            </SelectTrigger>
            <SelectContent>
              <SelectItem value="surface">Surface Level</SelectItem>
              <SelectItem value="standard">Standard</SelectItem>
              <SelectItem value="deep">Deep Analysis</SelectItem>
            </SelectContent>
          </Select>
        </div>
        
        <div>
          <Label htmlFor="output-format" className="block text-sm font-medium mb-2">
            Output Format
          </Label>
          <Select 
            value={config.outputFormat} 
            onValueChange={(value) => updateConfig({ outputFormat: value })}
          >
            <SelectTrigger data-testid="select-output-format">
              <SelectValue placeholder="Select output format" />
            </SelectTrigger>
            <SelectContent>
              <SelectItem value="json">JSON Report</SelectItem>
              <SelectItem value="html">HTML Report</SelectItem>
              <SelectItem value="markdown">Markdown Summary</SelectItem>
              <SelectItem value="csv">CSV Data</SelectItem>
            </SelectContent>
          </Select>
        </div>
      </div>
      
      <div className="mt-6 space-y-4">
        <div className="flex items-center space-x-2">
          <Checkbox
            id="vulnerability-scan"
            checked={config.includeVulnerabilityScan}
            onCheckedChange={(checked) => 
              updateConfig({ includeVulnerabilityScan: Boolean(checked) })
            }
            data-testid="checkbox-vulnerability-scan"
          />
          <Label htmlFor="vulnerability-scan" className="text-sm">
            Include dependency vulnerability scan
          </Label>
        </div>
        
        <div className="flex items-center space-x-2">
          <Checkbox
            id="generate-scaffolding"
            checked={config.generateScaffolding}
            onCheckedChange={(checked) => 
              updateConfig({ generateScaffolding: Boolean(checked) })
            }
            data-testid="checkbox-generate-scaffolding"
          />
          <Label htmlFor="generate-scaffolding" className="text-sm">
            Generate project scaffolding templates
          </Label>
        </div>
      </div>
    </div>
  );
}
