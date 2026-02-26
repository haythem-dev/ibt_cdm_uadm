# Overview

This is a full-stack web application built with React, Express.js, and TypeScript. It serves as a modern foundation project with a clean architecture that separates frontend and backend concerns. The application uses Drizzle ORM for database operations with PostgreSQL, features a responsive UI built with shadcn/ui components and Tailwind CSS, and implements a RESTful API structure. The project is designed to be easily extensible and deployable on platforms like Replit.

# User Preferences

Preferred communication style: Simple, everyday language.

# System Architecture

## Frontend Architecture
- **Framework**: React 18 with TypeScript for type safety and modern component patterns
- **Routing**: Wouter for lightweight client-side routing without the complexity of React Router
- **State Management**: TanStack Query for server state management and caching, providing efficient data fetching and synchronization
- **UI Framework**: shadcn/ui components built on Radix UI primitives for accessible, customizable components
- **Styling**: Tailwind CSS with CSS variables for theming, enabling consistent design system and dark mode support
- **Build Tool**: Vite for fast development server and optimized production builds

## Backend Architecture
- **Framework**: Express.js with TypeScript for robust REST API development
- **Database**: PostgreSQL with Drizzle ORM for type-safe database operations and schema management
- **Session Management**: PostgreSQL session store for persistent user sessions
- **Development**: Hot module replacement with custom error overlay for improved developer experience

## Project Structure
- **Monorepo Layout**: Client and server code in separate directories with shared schema definitions
- **Shared Types**: Common TypeScript definitions in `/shared` directory for consistent data models
- **Component Organization**: UI components separated by type (pages, components, ui) for maintainability
- **Path Aliases**: Configured for clean imports using `@/` for client code and `@shared/` for shared types

## Data Layer
- **ORM**: Drizzle ORM chosen for its TypeScript-first approach and excellent performance
- **Database**: PostgreSQL for robust relational data storage with UUID primary keys
- **Migrations**: Automated database schema management through Drizzle Kit
- **Connection**: Neon serverless PostgreSQL for scalable cloud database hosting

## Development Environment
- **Runtime**: Node.js with ES modules for modern JavaScript features
- **Type Checking**: Strict TypeScript configuration with comprehensive type coverage
- **Hot Reload**: Vite dev server with custom middleware integration for seamless development
- **Error Handling**: Custom error boundaries and development-time error overlays

# External Dependencies

## Database Services
- **Neon Database**: Serverless PostgreSQL hosting with connection pooling and automatic scaling
- **Connection Pooling**: Built-in connection management for efficient database resource usage

## UI Component Libraries
- **Radix UI**: Comprehensive set of accessible, unstyled UI primitives for building design systems
- **Lucide React**: Modern icon library with consistent styling and tree-shaking support
- **Embla Carousel**: Lightweight carousel component for interactive content display

## Development Tools
- **Replit Integration**: Custom Vite plugins for Replit development environment compatibility
- **PostCSS**: CSS processing with Tailwind CSS and Autoprefixer for cross-browser compatibility
- **ESBuild**: Fast JavaScript bundler for production builds with tree-shaking optimization

## Utility Libraries
- **date-fns**: Modern date utility library for date manipulation and formatting
- **clsx & tailwind-merge**: Conditional class name utilities for dynamic styling
- **nanoid**: Secure, URL-safe unique string ID generator for client-side operations