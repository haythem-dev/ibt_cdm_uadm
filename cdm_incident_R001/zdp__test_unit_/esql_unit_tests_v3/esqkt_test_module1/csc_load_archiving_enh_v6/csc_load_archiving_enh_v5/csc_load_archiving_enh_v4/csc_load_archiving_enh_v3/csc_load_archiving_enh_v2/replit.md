# Overview

This project has evolved from a starter template to include database archiving documentation for an enterprise CPR (Change Process Request) implementation. The project maintains its web foundation while serving as a documentation platform for order archiving processes using Informix database systems.

## Recent Changes (August 13, 2025)

- **Added**: Comprehensive archiving verification guide for tpldauftrag process
- **Focus**: Database verification steps for kdauftrag to akdauftrag archiving
- **Database**: Informix-specific SQL queries for pre-launch verification
- **Business Logic**: Emergency vs normal run time handling for business day calculations
- **CPR Enhancement**: Successfully implemented business day logic with 6 AM cutoff
- **Bug Fixes**: Resolved SQL syntax corruption and sequence timing issues
- **Status**: Enhanced tpldauftr binary working with RC: 0, business day filters operational

# User Preferences

Preferred communication style: Simple, everyday language.

# System Architecture

## Frontend Architecture
- **Static Web Application**: Built with vanilla HTML, CSS, and JavaScript for maximum simplicity and flexibility
- **Component-Based Styling**: Uses CSS custom properties (CSS variables) for consistent theming and easy customization
- **Responsive Design**: Implements Bootstrap 5.3.0 grid system and responsive utilities for mobile-first design
- **Modular JavaScript**: Object-oriented approach with a main `StarterApp` class that handles initialization, DOM manipulation, and event management

## Design Patterns
- **Progressive Enhancement**: Core functionality works without JavaScript, enhanced features added via JS
- **Error Handling**: Centralized error handling in the JavaScript application class
- **DOM Caching**: Elements are cached to improve performance and reduce DOM queries
- **Event-Driven Architecture**: Uses event delegation and proper event binding for user interactions

## UI/UX Framework
- **Bootstrap Integration**: Leverages Bootstrap's utility classes and component system for rapid development
- **Icon System**: Feather Icons provide consistent, scalable iconography throughout the interface
- **Accessibility**: Built with semantic HTML and accessibility considerations in mind
- **CSS Architecture**: Custom CSS variables for theming, follows modern CSS practices with flexbox layouts

# External Dependencies

## Frontend Libraries
- **Bootstrap 5.3.0**: CSS framework for responsive design and UI components (loaded via CDN)
- **Feather Icons**: Lightweight icon library for consistent iconography (loaded via CDN)

## Development Dependencies
- **Standard Web Technologies**: HTML5, CSS3, ES6+ JavaScript
- **No Build Process**: Direct browser execution without compilation or bundling tools

## Browser Compatibility
- **Modern Browsers**: Targets current versions of Chrome, Firefox, Safari, and Edge
- **Progressive Enhancement**: Graceful degradation for older browsers where possible