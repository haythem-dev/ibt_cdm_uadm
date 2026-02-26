# Overview

This is a full-stack web application built with a React frontend and Express.js backend. The project uses TypeScript throughout and implements a modern development stack with Vite for frontend bundling, Drizzle ORM for database operations, and shadcn/ui for the component library. The application appears to be a starter template or boilerplate with basic user management functionality and a clean, professional UI.

# User Preferences

Preferred communication style: Simple, everyday language.

# System Architecture

## Frontend Architecture
- **Framework**: React 18 with TypeScript
- **Build Tool**: Vite for fast development and optimized production builds
- **Routing**: Wouter for lightweight client-side routing
- **State Management**: TanStack Query (React Query) for server state management
- **UI Components**: shadcn/ui component library built on Radix UI primitives
- **Styling**: Tailwind CSS with CSS variables for theming
- **Form Handling**: React Hook Form with Zod validation via Hookform Resolvers

## Backend Architecture
- **Runtime**: Node.js with Express.js framework
- **Language**: TypeScript with ESM modules
- **Database ORM**: Drizzle ORM with PostgreSQL dialect
- **Session Management**: Connect-pg-simple for PostgreSQL session storage
- **Development**: Hot reload with tsx for TypeScript execution

## Data Storage
- **Database**: PostgreSQL (configured via Neon serverless driver)
- **ORM**: Drizzle ORM for type-safe database operations
- **Schema**: Centralized schema definition in `shared/schema.ts`
- **Migrations**: Drizzle Kit for database migrations in `./migrations` directory
- **Session Storage**: PostgreSQL-based session storage for user sessions

## Development Environment
- **Monorepo Structure**: Client and server code in separate directories with shared types
- **Development Server**: Vite dev server with Express API proxy
- **Hot Reload**: Full-stack hot reload during development
- **TypeScript**: Strict type checking across frontend, backend, and shared code
- **Path Aliases**: Configured for clean imports (`@/`, `@shared/`)

## Authentication & User Management
- **User Schema**: Basic user model with username, password, and UUID primary key
- **Storage Interface**: Abstracted storage layer with in-memory implementation for development
- **Password Handling**: Prepared for secure password hashing (implementation pending)
- **Session Management**: Cookie-based sessions with PostgreSQL storage

## API Architecture
- **RESTful Design**: Express routes prefixed with `/api`
- **Request Logging**: Comprehensive request/response logging with timing
- **Error Handling**: Centralized error handling middleware
- **Type Safety**: Shared TypeScript types between frontend and backend

# External Dependencies

## Database & ORM
- **@neondatabase/serverless**: Serverless PostgreSQL driver for Neon database
- **drizzle-orm**: Type-safe ORM for database operations
- **drizzle-kit**: Database migration and introspection tools
- **connect-pg-simple**: PostgreSQL session store for Express sessions

## Frontend Libraries
- **@tanstack/react-query**: Server state management and caching
- **wouter**: Lightweight React router
- **@hookform/resolvers**: Form validation resolvers for React Hook Form
- **date-fns**: Date manipulation utilities

## UI Components & Styling
- **@radix-ui/***: Comprehensive set of unstyled, accessible UI primitives
- **tailwindcss**: Utility-first CSS framework
- **class-variance-authority**: Type-safe variant styling
- **clsx & tailwind-merge**: Utility functions for conditional CSS classes

## Development Tools
- **vite**: Fast build tool and development server
- **@vitejs/plugin-react**: React support for Vite
- **tsx**: TypeScript execution engine for Node.js
- **esbuild**: Fast JavaScript bundler for production builds
- **@replit/vite-plugin-runtime-error-modal**: Development error overlay
- **@replit/vite-plugin-cartographer**: Replit-specific development tooling

## Utility Libraries
- **nanoid**: URL-safe unique ID generator
- **cmdk**: Command palette component
- **embla-carousel-react**: Carousel/slider component
- **lucide-react**: Icon library with React components