import { type User, type InsertUser, type Project, type InsertProject, type UploadedFile, type InsertUploadedFile, type ProjectFile, type InsertProjectFile } from "@shared/schema";
import { randomUUID } from "crypto";

export interface IStorage {
  // User operations
  getUser(id: string): Promise<User | undefined>;
  getUserByUsername(username: string): Promise<User | undefined>;
  createUser(user: InsertUser): Promise<User>;

  // Project operations
  getProject(id: string): Promise<Project | undefined>;
  getProjectsByUserId(userId: string): Promise<Project[]>;
  createProject(project: InsertProject): Promise<Project>;
  updateProject(id: string, updates: Partial<Project>): Promise<Project | undefined>;

  // File operations
  createUploadedFile(file: InsertUploadedFile): Promise<UploadedFile>;
  getUploadedFilesByProjectId(projectId: string): Promise<UploadedFile[]>;

  // Project file operations
  createProjectFile(file: InsertProjectFile): Promise<ProjectFile>;
  getProjectFilesByProjectId(projectId: string): Promise<ProjectFile[]>;
}

export class MemStorage implements IStorage {
  private users: Map<string, User>;
  private projects: Map<string, Project>;
  private uploadedFiles: Map<string, UploadedFile>;
  private projectFiles: Map<string, ProjectFile>;

  constructor() {
    this.users = new Map();
    this.projects = new Map();
    this.uploadedFiles = new Map();
    this.projectFiles = new Map();
  }

  async getUser(id: string): Promise<User | undefined> {
    return this.users.get(id);
  }

  async getUserByUsername(username: string): Promise<User | undefined> {
    return Array.from(this.users.values()).find(
      (user) => user.username === username,
    );
  }

  async createUser(insertUser: InsertUser): Promise<User> {
    const id = randomUUID();
    const user: User = { ...insertUser, id };
    this.users.set(id, user);
    return user;
  }

  async getProject(id: string): Promise<Project | undefined> {
    return this.projects.get(id);
  }

  async getProjectsByUserId(userId: string): Promise<Project[]> {
    return Array.from(this.projects.values()).filter(
      (project) => project.userId === userId,
    );
  }

  async createProject(insertProject: InsertProject): Promise<Project> {
    const id = randomUUID();
    const now = new Date();
    const project: Project = { 
      ...insertProject, 
      id, 
      status: "created",
      createdAt: now,
      updatedAt: now
    };
    this.projects.set(id, project);
    return project;
  }

  async updateProject(id: string, updates: Partial<Project>): Promise<Project | undefined> {
    const project = this.projects.get(id);
    if (!project) return undefined;
    
    const updatedProject = { ...project, ...updates, updatedAt: new Date() };
    this.projects.set(id, updatedProject);
    return updatedProject;
  }

  async createUploadedFile(insertFile: InsertUploadedFile): Promise<UploadedFile> {
    const id = randomUUID();
    const file: UploadedFile = { 
      ...insertFile, 
      id, 
      uploadedAt: new Date() 
    };
    this.uploadedFiles.set(id, file);
    return file;
  }

  async getUploadedFilesByProjectId(projectId: string): Promise<UploadedFile[]> {
    return Array.from(this.uploadedFiles.values()).filter(
      (file) => file.projectId === projectId,
    );
  }

  async createProjectFile(insertFile: InsertProjectFile): Promise<ProjectFile> {
    const id = randomUUID();
    const file: ProjectFile = { 
      ...insertFile, 
      id, 
      extractedAt: new Date() 
    };
    this.projectFiles.set(id, file);
    return file;
  }

  async getProjectFilesByProjectId(projectId: string): Promise<ProjectFile[]> {
    return Array.from(this.projectFiles.values()).filter(
      (file) => file.projectId === projectId,
    );
  }
}

export const storage = new MemStorage();
