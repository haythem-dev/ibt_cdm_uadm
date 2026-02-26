# Replit.md

## Overview

This is a C++ application code analysis project focused on examining the tpldauftrag.cxx business day enhancement CPR implementation. The project includes a modern React-based web interface for displaying the analysis results of order archiving system modifications that prevent business blindness during emergency operations.

## User Preferences

Preferred communication style: Simple, everyday language.

## Recent Analysis

**Date:** August 14, 2025  
**Focus:** CPR Business Day Enhancement in tpldauftrag.cxx  
**Key Finding:** Successfully identified and analyzed intelligent time-based archiving logic that prevents business disruption during emergency archiving operations.

## System Architecture

### Frontend Architecture
- **Framework**: React 18 with TypeScript and Vite for fast development and building
- **Routing**: Wouter for lightweight client-side routing
- **UI Components**: shadcn/ui component library built on Radix UI primitives with Tailwind CSS for styling
- **State Management**: TanStack Query (React Query) for server state management and caching
- **Form Handling**: React Hook Form with Zod validation resolvers
- **Build System**: Vite with custom configuration for development and production builds

### Backend Architecture
- **Framework**: Express.js with TypeScript for the REST API server
- **Database ORM**: Drizzle ORM with PostgreSQL dialect for type-safe database operations
- **Storage Layer**: Abstracted storage interface (IStorage) with in-memory implementation for development
- **Session Management**: Built-in support for PostgreSQL session storage using connect-pg-simple
- **Development**: Hot module replacement and error overlay for improved developer experience

### Database Design
- **Primary Database**: PostgreSQL with Neon Database serverless driver
- **Schema Management**: Drizzle Kit for migrations and schema synchronization
- **Type Safety**: Fully typed database schema with Zod validation integration
- **Current Schema**: Users table with UUID primary keys, unique usernames, and password fields

### Styling and Design System
- **CSS Framework**: Tailwind CSS with custom configuration and CSS variables
- **Design Tokens**: Comprehensive color system with light/dark mode support
- **Typography**: Multiple font families (Open Sans, Georgia, Menlo) with proper font loading
- **Component Theming**: Consistent styling through shadcn/ui's design system with customizable radius and color schemes

### Development and Build Pipeline
- **Development Server**: Vite development server with Express.js proxy for API routes
- **Type Checking**: Comprehensive TypeScript configuration with path aliases
- **Code Quality**: ESBuild for production bundling with Node.js compatibility
- **Asset Management**: Static asset handling with proper build output structure

### Project Structure
- **Monorepo Layout**: Client, server, and shared code separation
- **Path Aliases**: Organized imports with `@/` for client components and `@shared/` for common code
- **Configuration**: Centralized configuration files for tools like Tailwind, TypeScript, and Drizzle

## External Dependencies

### Core Dependencies
- **@neondatabase/serverless**: PostgreSQL database driver optimized for serverless environments
- **drizzle-orm**: Type-safe ORM with PostgreSQL dialect support
- **express**: Web framework for the backend API server
- **@tanstack/react-query**: Server state management and caching library

### UI and Styling
- **@radix-ui/***: Comprehensive collection of unstyled, accessible UI primitives
- **tailwindcss**: Utility-first CSS framework for styling
- **class-variance-authority**: Tool for creating variant-based component APIs
- **lucide-react**: Icon library with consistent design language

### Development Tools
- **vite**: Fast build tool and development server
- **typescript**: Type system for JavaScript
- **@replit/vite-plugin-runtime-error-modal**: Development error overlay for Replit environment
- **drizzle-kit**: CLI tool for database migrations and schema management

### Form and Validation
- **react-hook-form**: Performant form library with minimal re-renders
- **@hookform/resolvers**: Validation resolvers for various schema libraries
- **zod**: TypeScript-first schema validation library
- **drizzle-zod**: Integration between Drizzle ORM and Zod for schema validation

### Additional Libraries
- **wouter**: Minimalist routing library for React applications
- **date-fns**: Modern JavaScript date utility library
- **embla-carousel-react**: Carousel component for React applications
- **cmdk**: Command menu component for keyboard-driven interfaces