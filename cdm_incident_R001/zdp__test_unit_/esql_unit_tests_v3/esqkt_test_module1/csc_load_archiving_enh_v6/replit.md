# Overview

This project contains two main components:

1. **Full-Stack Web Application**: A modern React + TypeScript frontend with Express backend for project management and configuration tools
2. **Enhanced C++ Pharmaceutical Archiving System**: Enhanced `tpldauftr.cxx` with comprehensive logging for date, hour, and weekend logic using `cpr_log` function

## Recent Changes (August 17, 2025)
- ✓ Successfully enhanced C++ archiving system with detailed date/time/weekend logging
- ✓ Added comprehensive `cpr_log` function integration throughout business day calculations
- ✓ Enhanced weekend rollback logic with step-by-step logging (Sunday→Friday, Saturday→Friday)
- ✓ Added process initialization, execution, and completion summary logging
- ✓ Created detailed documentation of all logging enhancements
- ✓ **CRITICAL BUG FIX**: Fixed order of execution - business day cutoff now calculated BEFORE SQL preparation
- ✓ Moved `calculate_business_day_cutoff()` before `PrepareAll()` to ensure correct filtering

# User Preferences

Preferred communication style: Simple, everyday language.

# System Architecture

## Frontend Architecture
- **Framework**: React 18 with TypeScript for type-safe component development
- **Build Tool**: Vite for fast development server and optimized production builds
- **Routing**: Wouter for lightweight client-side routing without heavy dependencies
- **State Management**: TanStack Query (React Query) for server state management and caching
- **UI Components**: shadcn/ui component library built on Radix UI primitives
- **Styling**: Tailwind CSS with CSS variables for consistent theming and design tokens
- **Form Handling**: React Hook Form with Zod validation resolvers for type-safe forms

## Backend Architecture
- **Runtime**: Node.js with Express.js framework using TypeScript
- **API Design**: RESTful API with `/api` prefix for all endpoints
- **Request Handling**: Express middleware for JSON parsing, request logging, and error handling
- **Session Management**: Built-in support for PostgreSQL-based sessions using connect-pg-simple
- **Development**: Hot module replacement with tsx for TypeScript execution

## Data Storage Solutions
- **Database**: PostgreSQL with Drizzle ORM for type-safe database operations
- **Schema Management**: Drizzle Kit for database migrations and schema synchronization
- **Connection**: Neon Database serverless driver for PostgreSQL connectivity
- **Storage Interface**: Abstracted IStorage interface with in-memory implementation for development
- **Type Safety**: Fully typed schema definitions with Zod validation integration

## Authentication and Authorization
- **User Model**: UUID-based primary keys with unique usernames and password fields
- **Session Storage**: PostgreSQL session persistence for user authentication state
- **Password Security**: Infrastructure prepared for secure password hashing implementation
- **Storage Abstraction**: Clean separation between storage interface and implementation

# External Dependencies

## Database Services
- **Neon Database**: Serverless PostgreSQL hosting with connection pooling
- **Drizzle ORM**: Type-safe database toolkit with PostgreSQL dialect support
- **connect-pg-simple**: PostgreSQL session store for Express sessions

## UI and Styling
- **Radix UI**: Headless component primitives for accessible UI components
- **shadcn/ui**: Pre-built component library with consistent design patterns
- **Tailwind CSS**: Utility-first CSS framework with custom configuration
- **Lucide React**: Icon library for consistent iconography

## Development Tools
- **Vite**: Modern build tool with fast HMR and optimized bundling
- **TypeScript**: Static type checking across frontend, backend, and shared code
- **ESBuild**: Fast JavaScript bundler for production builds
- **Replit Integration**: Development environment plugins for runtime error handling

## API and State Management
- **TanStack Query**: Server state management with caching and synchronization
- **Wouter**: Minimalist routing library for client-side navigation
- **React Hook Form**: Performant form library with minimal re-renders
- **Zod**: TypeScript-first schema validation for forms and API data