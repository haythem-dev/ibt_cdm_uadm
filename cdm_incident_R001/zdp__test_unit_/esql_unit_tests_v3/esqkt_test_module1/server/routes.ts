import type { Express } from "express";
import { createServer, type Server } from "http";
import multer from "multer";
import path from "path";
import fs from "fs";
import { storage } from "./storage";
import { insertProjectSchema, insertUploadedFileSchema, insertProjectFileSchema } from "@shared/schema";
import { z } from "zod";

const upload = multer({
  dest: "uploads/",
  limits: {
    fileSize: 50 * 1024 * 1024, // 50MB limit
  },
  fileFilter: (req, file, cb) => {
    const allowedTypes = ['.zip', '.tar', '.gz', '.tgz', '.whl'];
    const ext = path.extname(file.originalname).toLowerCase();
    const isGzTar = file.originalname.toLowerCase().endsWith('.tar.gz');
    
    if (allowedTypes.includes(ext) || isGzTar) {
      cb(null, true);
    } else {
      cb(new Error('Only package files are allowed (.zip, .tar.gz, .tar, .whl, .tgz)'));
    }
  }
});

export async function registerRoutes(app: Express): Promise<Server> {
  // Create a new project
  app.post("/api/projects", async (req, res) => {
    try {
      const projectData = insertProjectSchema.parse(req.body);
      const project = await storage.createProject({
        ...projectData,
        userId: req.body.userId || "default-user" // In a real app, get from session
      });
      res.json(project);
    } catch (error) {
      if (error instanceof z.ZodError) {
        res.status(400).json({ message: "Invalid project data", errors: error.errors });
      } else {
        res.status(500).json({ message: "Failed to create project" });
      }
    }
  });

  // Get all projects
  app.get("/api/projects", async (req, res) => {
    try {
      const userId = req.query.userId as string || "default-user";
      const projects = await storage.getProjectsByUserId(userId);
      res.json(projects);
    } catch (error) {
      res.status(500).json({ message: "Failed to fetch projects" });
    }
  });

  // Get a specific project
  app.get("/api/projects/:id", async (req, res) => {
    try {
      const project = await storage.getProject(req.params.id);
      if (!project) {
        return res.status(404).json({ message: "Project not found" });
      }
      res.json(project);
    } catch (error) {
      res.status(500).json({ message: "Failed to fetch project" });
    }
  });

  // Upload file to project
  app.post("/api/projects/:id/upload", upload.single('package'), async (req, res) => {
    try {
      if (!req.file) {
        return res.status(400).json({ message: "No file uploaded" });
      }

      const projectId = req.params.id;
      const project = await storage.getProject(projectId);
      if (!project) {
        // Clean up uploaded file
        fs.unlinkSync(req.file.path);
        return res.status(404).json({ message: "Project not found" });
      }

      // Save file info to storage
      const uploadedFile = await storage.createUploadedFile({
        projectId,
        filename: req.file.filename,
        originalName: req.file.originalname,
        mimetype: req.file.mimetype,
        size: req.file.size.toString(),
        path: req.file.path
      });

      // Update project status
      await storage.updateProject(projectId, { status: "processing" });

      // TODO: In a real implementation, this would trigger package extraction and analysis
      // For now, we'll create some mock project files to simulate extraction
      await simulatePackageExtraction(projectId);

      // Update project status to completed
      await storage.updateProject(projectId, { status: "completed" });

      res.json({ 
        message: "File uploaded successfully", 
        file: uploadedFile,
        project: await storage.getProject(projectId)
      });
    } catch (error) {
      if (req.file) {
        // Clean up uploaded file on error
        fs.unlinkSync(req.file.path);
      }
      res.status(500).json({ message: "File upload failed", error: error.message });
    }
  });

  // Get project files (extracted structure)
  app.get("/api/projects/:id/files", async (req, res) => {
    try {
      const projectFiles = await storage.getProjectFilesByProjectId(req.params.id);
      res.json(projectFiles);
    } catch (error) {
      res.status(500).json({ message: "Failed to fetch project files" });
    }
  });

  // Get uploaded files for a project
  app.get("/api/projects/:id/uploads", async (req, res) => {
    try {
      const uploadedFiles = await storage.getUploadedFilesByProjectId(req.params.id);
      res.json(uploadedFiles);
    } catch (error) {
      res.status(500).json({ message: "Failed to fetch uploaded files" });
    }
  });

  const httpServer = createServer(app);
  return httpServer;
}

// Simulate package extraction - in a real implementation this would actually extract the archive
async function simulatePackageExtraction(projectId: string) {
  const mockFiles = [
    { name: "src", path: "src/", type: "folder", size: null, parentId: null },
    { name: "main.py", path: "src/main.py", type: "file", size: "1234", parentId: null },
    { name: "utils.py", path: "src/utils.py", type: "file", size: "567", parentId: null },
    { name: "requirements.txt", path: "requirements.txt", type: "file", size: "89", parentId: null },
    { name: "README.md", path: "README.md", type: "file", size: "2048", parentId: null }
  ];

  for (const file of mockFiles) {
    await storage.createProjectFile({
      projectId,
      ...file
    });
  }
}
