import { z } from "zod";

// Project Configuration Schema
export const projectConfigSchema = z.object({
  id: z.string().default("new-project"),
  name: z.string().min(1, "Project name is required"),
  environment: z.enum(["development", "staging", "production"]).default("development"),
  status: z.enum(["initializing", "ready", "error"]).default("ready"),
  createdAt: z.date().default(() => new Date()),
  updatedAt: z.date().default(() => new Date()),
});

export const insertProjectConfigSchema = projectConfigSchema.omit({
  id: true,
  createdAt: true,
  updatedAt: true,
});

export type ProjectConfig = z.infer<typeof projectConfigSchema>;
export type InsertProjectConfig = z.infer<typeof insertProjectConfigSchema>;

// Development Tool Schema
export const developmentToolSchema = z.object({
  id: z.string(),
  name: z.string(),
  icon: z.string(),
  status: z.enum(["ready", "configuring", "error"]).default("ready"),
  version: z.string().optional(),
});

export type DevelopmentTool = z.infer<typeof developmentToolSchema>;

// Future Feature Schema
export const futureFeatureSchema = z.object({
  id: z.string(),
  name: z.string(),
  description: z.string(),
  status: z.enum(["planned", "in-progress", "completed"]).default("planned"),
  priority: z.enum(["low", "medium", "high"]).default("medium"),
});

export type FutureFeature = z.infer<typeof futureFeatureSchema>;
