import { insertProjectConfigSchema } from "@shared/schema";
import type { IStorage } from "./storage";
import { MemStorage } from "./storage";
import type { Express, Request, Response } from "express";

export async function registerRoutes(app: Express) {
  const storage = new MemStorage();

  // Get project configuration
  app.get("/api/project", async (req: Request, res: Response) => {
    try {
      const config = await storage.getProjectConfig();
      res.json(config);
    } catch (error) {
      console.error("Error fetching project config:", error);
      res.status(500).json({ error: "Failed to fetch project configuration" });
    }
  });

  // Update project configuration
  app.patch("/api/project", async (req: Request, res: Response) => {
    try {
      const parseResult = insertProjectConfigSchema.safeParse(req.body);
      if (!parseResult.success) {
        return res.status(400).json({ error: "Invalid request data", details: parseResult.error });
      }
      
      const updatedConfig = await storage.updateProjectConfig(parseResult.data);
      res.json(updatedConfig);
    } catch (error) {
      console.error("Error updating project config:", error);
      res.status(500).json({ error: "Failed to update project configuration" });
    }
  });

  // Get development tools
  app.get("/api/tools", async (req: Request, res: Response) => {
    try {
      const tools = await storage.getDevelopmentTools();
      res.json(tools);
    } catch (error) {
      console.error("Error fetching development tools:", error);
      res.status(500).json({ error: "Failed to fetch development tools" });
    }
  });

  // Get future features
  app.get("/api/features", async (req: Request, res: Response) => {
    try {
      const features = await storage.getFutureFeatures();
      res.json(features);
    } catch (error) {
      console.error("Error fetching future features:", error);
      res.status(500).json({ error: "Failed to fetch future features" });
    }
  });

  return app;
}
