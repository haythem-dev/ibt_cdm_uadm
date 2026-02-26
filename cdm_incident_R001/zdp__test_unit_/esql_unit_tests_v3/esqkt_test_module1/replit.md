# Package Analyzer

## Overview

Package Analyzer is a full-stack web application for analyzing software packages. The application allows users to upload package files (zip, tar, whl, etc.), configure analysis settings, and view the results. It features a modern React frontend with shadcn/ui components and an Express.js backend with PostgreSQL database integration.

## User Preferences

Preferred communication style: Simple, everyday language.

## System Architecture

### Frontend Architecture

The frontend is built with React and TypeScript, utilizing a component-based architecture:

- **Framework**: React with TypeScript for type safety
- **UI Components**: shadcn/ui component library built on Radix UI primitives
- **Styling**: Tailwind CSS with a dark theme design system
- **State Management**: TanStack Query (React Query) for server state management
- **Routing**: Wouter for lightweight client-side routing
- **Form Handling**: React Hook Form with Zod validation
- **Build Tool**: Vite for fast development and optimized builds

The frontend follows a modular structure with reusable UI components, custom hooks, and organized page components. The application uses a query-based data fetching pattern with proper error handling and loading states.

### Backend Architecture

The backend is an Express.js REST API server:

- **Framework**: Express.js with TypeScript
- **File Upload**: Multer middleware for handling package uploads with file type validation
- **Storage**: Abstracted storage interface with in-memory implementation (ready for database integration)
- **Development**: Vite integration for hot module replacement and development tooling
- **API Design**: RESTful endpoints for project management and file operations

The server implements middleware for request logging, error handling, and serves the built frontend in production. The storage layer uses an interface pattern that allows for easy swapping between implementations.

### Data Storage

The application uses PostgreSQL with Drizzle ORM:

- **Database**: PostgreSQL as the primary database
- **ORM**: Drizzle ORM for type-safe database operations and migrations
- **Connection**: Neon Database serverless connection (@neondatabase/serverless)
- **Schema**: Well-defined tables for users, projects, uploaded files, and project files with proper relationships
- **Migrations**: Drizzle Kit for database schema management and migrations

The database schema supports user management, project organization, file uploads, and hierarchical project file structures with foreign key relationships.

### Authentication and Authorization

Currently implements a simplified authentication approach:

- **User Management**: Basic user schema with username/password fields
- **Session Handling**: Ready for session-based authentication with connect-pg-simple for PostgreSQL session storage
- **Default User**: Uses "default-user" for development, ready for proper user authentication implementation

### Development and Build

The application uses a modern development setup:

- **Development**: Concurrent frontend (Vite) and backend (tsx) development servers
- **Production Build**: Vite builds the frontend, esbuild bundles the backend
- **Type Checking**: Shared TypeScript configuration across frontend, backend, and shared modules
- **Code Quality**: ESM modules throughout, strict TypeScript configuration

## External Dependencies

### Core Framework Dependencies
- **@neondatabase/serverless**: PostgreSQL serverless connection for database operations
- **drizzle-orm** and **drizzle-kit**: Type-safe ORM and migration tools
- **express**: Web framework for the REST API server
- **react** and **react-dom**: Frontend framework and rendering
- **@tanstack/react-query**: Server state management and data fetching

### UI and Styling
- **@radix-ui/***: Primitive components for accessible UI elements (dialogs, dropdowns, forms, etc.)
- **tailwindcss**: Utility-first CSS framework
- **class-variance-authority**: Component variant management
- **lucide-react**: Icon library

### Development Tools
- **vite**: Build tool and development server
- **tsx**: TypeScript execution for Node.js
- **typescript**: Type checking and compilation
- **@replit/vite-plugin-runtime-error-modal**: Replit-specific development tools

### File Handling and Validation
- **multer**: File upload middleware
- **zod**: Schema validation library
- **react-hook-form**: Form state management and validation

### Utilities
- **wouter**: Lightweight routing for React
- **date-fns**: Date manipulation utilities
- **clsx** and **tailwind-merge**: CSS class management
- **nanoid**: Unique ID generation

The application is designed to be deployed on Replit with environment-based configuration for database connections and proper separation of development and production concerns.