import type { ProjectConfig, InsertProjectConfig, DevelopmentTool, FutureFeature } from "@shared/schema";

export interface IStorage {
  // Project Configuration
  getProjectConfig(): Promise<ProjectConfig>;
  updateProjectConfig(config: InsertProjectConfig): Promise<ProjectConfig>;
  
  // Development Tools
  getDevelopmentTools(): Promise<DevelopmentTool[]>;
  
  // Future Features
  getFutureFeatures(): Promise<FutureFeature[]>;
}

export class MemStorage implements IStorage {
  private projectConfig: ProjectConfig = {
    id: "new-project",
    name: "New Project",
    environment: "development",
    status: "ready",
    createdAt: new Date(),
    updatedAt: new Date(),
  };

  private developmentTools: DevelopmentTool[] = [
    {
      id: "react",
      name: "React",
      icon: "fab fa-react",
      status: "ready",
      version: "18.x",
    },
    {
      id: "javascript",
      name: "JavaScript",
      icon: "fab fa-js-square",
      status: "ready",
      version: "ES6+",
    },
    {
      id: "tailwind",
      name: "CSS Framework",
      icon: "fab fa-css3-alt",
      status: "ready",
      version: "Tailwind",
    },
  ];

  private futureFeatures: FutureFeature[] = [
    {
      id: "auth",
      name: "User Authentication",
      description: "Login, registration, and user management",
      status: "planned",
      priority: "high",
    },
    {
      id: "api",
      name: "API Integration",
      description: "External API connections and data fetching",
      status: "planned",
      priority: "medium",
    },
    {
      id: "database",
      name: "Database Schema",
      description: "Data models and persistence layer",
      status: "planned",
      priority: "medium",
    },
  ];

  async getProjectConfig(): Promise<ProjectConfig> {
    return this.projectConfig;
  }

  async updateProjectConfig(config: InsertProjectConfig): Promise<ProjectConfig> {
    this.projectConfig = {
      ...this.projectConfig,
      ...config,
      updatedAt: new Date(),
    };
    return this.projectConfig;
  }

  async getDevelopmentTools(): Promise<DevelopmentTool[]> {
    return this.developmentTools;
  }

  async getFutureFeatures(): Promise<FutureFeature[]> {
    return this.futureFeatures;
  }
}
