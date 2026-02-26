# Overview

This is a full-stack web application built with a modern TypeScript stack, featuring a React frontend with shadcn/ui components and an Express.js backend. The application uses PostgreSQL with Drizzle ORM for database operations and is configured for deployment on platforms like Replit. It includes comprehensive UI components, form handling, state management with React Query, and a well-structured development environment with Vite for fast development builds.

# User Preferences

Preferred communication style: Simple, everyday language.

# System Architecture

## Frontend Architecture
- **Framework**: React 18 with TypeScript and Vite for development
- **Routing**: Wouter for lightweight client-side routing
- **Styling**: Tailwind CSS with shadcn/ui component library using Radix UI primitives
- **State Management**: React Query (TanStack Query) for server state management
- **Form Handling**: React Hook Form with Zod validation through @hookform/resolvers
- **Build Tool**: Vite with TypeScript support and custom path aliases

## Backend Architecture
- **Server**: Express.js with TypeScript
- **Database**: PostgreSQL with Drizzle ORM for type-safe database operations
- **Session Management**: Express sessions with PostgreSQL store (connect-pg-simple)
- **Development**: TSX for TypeScript execution in development
- **Production**: esbuild for optimized server bundling

## Data Layer
- **ORM**: Drizzle ORM configured for PostgreSQL dialect
- **Schema**: Centralized schema definition in shared directory
- **Validation**: Drizzle-zod integration for runtime type validation
- **Storage Interface**: Abstracted storage layer with in-memory implementation for development and extensible interface for production database integration

## Authentication & Security
- Session-based authentication with PostgreSQL session store
- Password handling infrastructure in user schema
- Credentials included in API requests for session management

## Development Environment
- **Monorepo Structure**: Shared types and schemas between client and server
- **Path Aliases**: Configured for clean imports across client, server, and shared code
- **Hot Reload**: Vite development server with Replit-specific plugins
- **Database Migrations**: Drizzle Kit for schema migrations
- **Type Safety**: End-to-end TypeScript with shared type definitions

# External Dependencies

## Database
- **PostgreSQL**: Primary database using Neon serverless driver
- **Connection**: Environment variable-based connection string (DATABASE_URL)

## UI Components
- **shadcn/ui**: Pre-built component library with Radix UI primitives
- **Lucide React**: Icon library for consistent iconography
- **Tailwind CSS**: Utility-first CSS framework with custom design tokens

## Development Tools
- **Replit Integration**: Custom Vite plugins for Replit development environment
- **TypeScript**: Full type safety across frontend and backend
- **ESLint/Prettier**: Code formatting and linting (implied by TypeScript setup)

## Third-Party Services
- **Neon Database**: Serverless PostgreSQL hosting
- **Replit Platform**: Development and potential hosting environment with specialized tooling