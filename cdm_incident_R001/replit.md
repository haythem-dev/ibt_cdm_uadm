# Package Analysis Tool

## Overview

This is a full-stack web application designed to analyze package files and provide comprehensive insights about their structure, dependencies, and composition. The tool supports multiple package formats (ZIP, TAR, WHL, EGG) and provides an intuitive interface for developers and researchers to explore package contents. The application features a modern React frontend with shadcn/ui components and an Express.js backend, designed to handle file upload, extraction, and analysis workflows.

## User Preferences

Preferred communication style: Simple, everyday language.

## System Architecture

### Frontend Architecture
- **Framework**: React with TypeScript using Vite as the build tool
- **UI Library**: shadcn/ui components built on Radix UI primitives
- **Styling**: Tailwind CSS with custom design system supporting dark/light modes
- **State Management**: React hooks with TanStack Query for server state management
- **Routing**: Wouter for lightweight client-side routing
- **Component Structure**: Modular design with reusable UI components including file upload zones, code viewers, analysis dashboards, and file explorers

### Backend Architecture
- **Runtime**: Node.js with Express.js framework
- **Language**: TypeScript with ES modules
- **API Structure**: RESTful endpoints under `/api` prefix
- **Database Integration**: Configured for Drizzle ORM with PostgreSQL support
- **Session Management**: Express sessions with PostgreSQL session store (connect-pg-simple)
- **Development**: Hot module replacement with Vite integration for seamless development experience

### Data Storage Solutions
- **Primary Database**: PostgreSQL via Neon serverless database
- **ORM**: Drizzle ORM for type-safe database operations with Zod schema validation
- **Session Storage**: PostgreSQL-backed session store for user session persistence
- **File Storage**: Temporary file system storage for package extraction and analysis
- **Configuration**: Environment-based database URL configuration with migration support

### Authentication and Authorization
- **User Schema**: Basic user authentication with username/password fields
- **Session Management**: Express session middleware with PostgreSQL persistence
- **Password Security**: Configured for secure password handling (implementation pending)
- **User Operations**: CRUD operations for user management through storage interface

## External Dependencies

### UI and Styling Dependencies
- **@radix-ui/***: Complete suite of accessible, unstyled UI primitives for building the component library
- **tailwindcss**: Utility-first CSS framework with custom configuration for design system
- **class-variance-authority**: Utility for managing component variants and styling patterns
- **cmdk**: Command menu component for search and navigation functionality

### Development and Build Tools
- **vite**: Modern build tool with React plugin and runtime error overlay for development
- **typescript**: Static type checking with comprehensive tsconfig for client, server, and shared code
- **esbuild**: Fast bundler for production server builds
- **drizzle-kit**: Database migration and schema management tools

### Database and Backend Services
- **@neondatabase/serverless**: Serverless PostgreSQL database connection for cloud deployment
- **drizzle-orm**: Type-safe ORM with PostgreSQL dialect support
- **drizzle-zod**: Integration between Drizzle schemas and Zod validation

### Data Processing and Analysis
- **@tanstack/react-query**: Powerful data fetching and caching library for client-server synchronization
- **date-fns**: Modern date utility library for timestamp manipulation and formatting
- **@hookform/resolvers**: Form validation resolvers for React Hook Form integration

### Development Experience
- **@replit/vite-plugin-***: Replit-specific plugins for enhanced development experience and debugging
- **wouter**: Minimalist routing library for single-page application navigation
- **nanoid**: Secure URL-friendly unique string ID generator for session and request tracking