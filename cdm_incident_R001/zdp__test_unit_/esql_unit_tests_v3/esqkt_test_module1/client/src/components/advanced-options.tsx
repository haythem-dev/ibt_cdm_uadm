import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Textarea } from "@/components/ui/textarea";
import { Collapsible, CollapsibleContent, CollapsibleTrigger } from "@/components/ui/collapsible";
import { ChevronDown } from "lucide-react";
import { useState } from "react";
import type { InsertProject } from "@shared/schema";

interface AdvancedOptionsProps {
  config: InsertProject;
  onChange: (config: InsertProject) => void;
}

export default function AdvancedOptions({ config, onChange }: AdvancedOptionsProps) {
  const [isOpen, setIsOpen] = useState(false);
  
  const updateConfig = (updates: Partial<InsertProject>) => {
    onChange({ ...config, ...updates });
  };

  return (
    <div className="bg-card border border-border rounded-lg p-6 mb-8">
      <Collapsible open={isOpen} onOpenChange={setIsOpen}>
        <CollapsibleTrigger className="flex items-center justify-between w-full text-left" data-testid="button-toggle-advanced">
          <h3 className="text-lg font-semibold">Advanced Options</h3>
          <ChevronDown 
            className={`h-4 w-4 transition-transform ${isOpen ? 'rotate-180' : ''}`}
          />
        </CollapsibleTrigger>
        
        <CollapsibleContent className="mt-4 space-y-4">
          <div>
            <Label htmlFor="custom-rules" className="block text-sm font-medium mb-2">
              Custom Analysis Rules
            </Label>
            <Textarea
              id="custom-rules"
              value={config.customRules || ""}
              onChange={(e) => updateConfig({ customRules: e.target.value || null })}
              placeholder="# Define custom rules in YAML format..."
              className="h-24 font-mono text-sm"
              data-testid="textarea-custom-rules"
            />
          </div>
          
          <div>
            <Label htmlFor="exclude-patterns" className="block text-sm font-medium mb-2">
              Exclude Patterns
            </Label>
            <Input
              id="exclude-patterns"
              type="text"
              value={config.excludePatterns || ""}
              onChange={(e) => updateConfig({ excludePatterns: e.target.value || null })}
              placeholder="*.pyc, node_modules/, .git/"
              className="font-mono text-sm"
              data-testid="input-exclude-patterns"
            />
          </div>
          
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
            <div>
              <Label htmlFor="memory-limit" className="block text-sm font-medium mb-2">
                Memory Limit (MB)
              </Label>
              <Input
                id="memory-limit"
                type="number"
                value={config.memoryLimit || "512"}
                onChange={(e) => updateConfig({ memoryLimit: e.target.value || "512" })}
                data-testid="input-memory-limit"
              />
            </div>
            
            <div>
              <Label htmlFor="timeout" className="block text-sm font-medium mb-2">
                Timeout (seconds)
              </Label>
              <Input
                id="timeout"
                type="number"
                value={config.timeout || "300"}
                onChange={(e) => updateConfig({ timeout: e.target.value || "300" })}
                data-testid="input-timeout"
              />
            </div>
          </div>
        </CollapsibleContent>
      </Collapsible>
    </div>
  );
}
