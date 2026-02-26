// Main application JavaScript for the starter project template

class StarterApp {
    constructor() {
        this.initialized = false;
        this.features = null;
        this.init();
    }

    /**
     * Initialize the application
     */
    init() {
        // Wait for DOM to be ready
        if (document.readyState === 'loading') {
            document.addEventListener('DOMContentLoaded', () => this.setup());
        } else {
            this.setup();
        }
    }

    /**
     * Setup the application after DOM is ready
     */
    setup() {
        try {
            // Initialize Feather icons
            if (typeof feather !== 'undefined') {
                feather.replace();
            }

            // Cache DOM elements
            this.cacheElements();

            // Bind event listeners
            this.bindEvents();

            // Mark as initialized
            this.initialized = true;

            console.log('Starter App initialized successfully');
        } catch (error) {
            console.error('Error initializing app:', error);
            this.showError('Failed to initialize application');
        }
    }

    /**
     * Cache frequently used DOM elements
     */
    cacheElements() {
        this.elements = {
            startBtn: document.getElementById('start-btn'),
            infoBtn: document.getElementById('info-btn'),
            features: document.getElementById('features'),
            body: document.body
        };

        // Validate required elements exist
        if (!this.elements.startBtn || !this.elements.infoBtn) {
            throw new Error('Required DOM elements not found');
        }
    }

    /**
     * Bind event listeners
     */
    bindEvents() {
        // Start button click handler
        this.elements.startBtn.addEventListener('click', (e) => {
            e.preventDefault();
            this.handleStart();
        });

        // Info button click handler
        this.elements.infoBtn.addEventListener('click', (e) => {
            e.preventDefault();
            this.handleInfo();
        });

        // Keyboard shortcuts
        document.addEventListener('keydown', (e) => {
            if (e.ctrlKey || e.metaKey) {
                switch (e.key) {
                    case 'Enter':
                        e.preventDefault();
                        this.handleStart();
                        break;
                    case 'i':
                        e.preventDefault();
                        this.handleInfo();
                        break;
                }
            }
        });

        // Window resize handler
        window.addEventListener('resize', this.debounce(() => {
            this.handleResize();
        }, 250));
    }

    /**
     * Handle start button click
     */
    handleStart() {
        try {
            this.showLoading(this.elements.startBtn);
            
            // Simulate initialization process
            setTimeout(() => {
                this.hideLoading(this.elements.startBtn);
                this.showMessage('Ready to begin! Configure your project requirements.', 'success');
                
                // Log user interaction
                this.logEvent('start_clicked');
            }, 1000);

        } catch (error) {
            console.error('Error handling start:', error);
            this.hideLoading(this.elements.startBtn);
            this.showError('Failed to start. Please try again.');
        }
    }

    /**
     * Handle info button click
     */
    handleInfo() {
        try {
            if (this.elements.features) {
                const isVisible = this.elements.features.style.display !== 'none';
                
                if (isVisible) {
                    this.hideFeatures();
                } else {
                    this.showFeatures();
                }
            }

            // Log user interaction
            this.logEvent('info_clicked');

        } catch (error) {
            console.error('Error handling info:', error);
            this.showError('Failed to show information.');
        }
    }

    /**
     * Show features section with animation
     */
    showFeatures() {
        if (!this.elements.features) return;

        this.elements.features.style.display = 'block';
        this.elements.features.classList.add('fade-in');
        
        // Update button text
        const icon = this.elements.infoBtn.querySelector('i');
        if (icon) {
            icon.setAttribute('data-feather', 'x');
            feather.replace();
        }

        // Scroll to features
        setTimeout(() => {
            this.elements.features.scrollIntoView({ 
                behavior: 'smooth', 
                block: 'start' 
            });
        }, 100);
    }

    /**
     * Hide features section
     */
    hideFeatures() {
        if (!this.elements.features) return;

        this.elements.features.style.display = 'none';
        this.elements.features.classList.remove('fade-in');
        
        // Reset button icon
        const icon = this.elements.infoBtn.querySelector('i');
        if (icon) {
            icon.setAttribute('data-feather', 'info');
            feather.replace();
        }
    }

    /**
     * Handle window resize
     */
    handleResize() {
        // Placeholder for responsive adjustments
        const width = window.innerWidth;
        
        if (width < 768) {
            this.elements.body.classList.add('mobile');
        } else {
            this.elements.body.classList.remove('mobile');
        }
    }

    /**
     * Show loading state on button
     */
    showLoading(button) {
        if (!button) return;
        
        button.classList.add('loading');
        button.disabled = true;
        
        const originalText = button.innerHTML;
        button.setAttribute('data-original-text', originalText);
        button.innerHTML = '<i data-feather="loader" class="me-2"></i>Loading...';
        
        feather.replace();
    }

    /**
     * Hide loading state on button
     */
    hideLoading(button) {
        if (!button) return;
        
        button.classList.remove('loading');
        button.disabled = false;
        
        const originalText = button.getAttribute('data-original-text');
        if (originalText) {
            button.innerHTML = originalText;
            button.removeAttribute('data-original-text');
            feather.replace();
        }
    }

    /**
     * Show success/info message
     */
    showMessage(message, type = 'info') {
        const alertClass = `alert-${type}`;
        const iconMap = {
            success: 'check-circle',
            info: 'info',
            warning: 'alert-triangle',
            danger: 'alert-circle'
        };

        const alert = document.createElement('div');
        alert.className = `alert ${alertClass} alert-dismissible fade show position-fixed`;
        alert.style.cssText = 'top: 20px; right: 20px; z-index: 1050; max-width: 400px;';
        
        alert.innerHTML = `
            <i data-feather="${iconMap[type] || 'info'}" class="me-2"></i>
            ${message}
            <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
        `;

        document.body.appendChild(alert);
        feather.replace();

        // Auto-dismiss after 5 seconds
        setTimeout(() => {
            if (alert.parentNode) {
                alert.remove();
            }
        }, 5000);
    }

    /**
     * Show error message
     */
    showError(message) {
        this.showMessage(message, 'danger');
    }

    /**
     * Log events for analytics (placeholder)
     */
    logEvent(eventName, data = {}) {
        const event = {
            name: eventName,
            timestamp: new Date().toISOString(),
            data: data,
            userAgent: navigator.userAgent
        };

        console.log('Event logged:', event);
        
        // In a real application, this would send to analytics service
        // Example: analytics.track(eventName, data);
    }

    /**
     * Debounce utility function
     */
    debounce(func, wait) {
        let timeout;
        return function executedFunction(...args) {
            const later = () => {
                clearTimeout(timeout);
                func(...args);
            };
            clearTimeout(timeout);
            timeout = setTimeout(later, wait);
        };
    }

    /**
     * Get application state
     */
    getState() {
        return {
            initialized: this.initialized,
            featuresVisible: this.elements.features ? 
                this.elements.features.style.display !== 'none' : false,
            timestamp: new Date().toISOString()
        };
    }
}

// Initialize the application
const app = new StarterApp();

// Export for potential use by other scripts
if (typeof module !== 'undefined' && module.exports) {
    module.exports = StarterApp;
}
