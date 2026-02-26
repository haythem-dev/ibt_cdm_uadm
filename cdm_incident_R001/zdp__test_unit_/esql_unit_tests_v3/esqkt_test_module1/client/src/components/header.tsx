export default function Header() {
  return (
    <header className="border-b border-border bg-card">
      <div className="container mx-auto px-6 py-4">
        <div className="flex items-center justify-between">
          <div className="flex items-center space-x-4">
            <div className="flex items-center space-x-2">
              <div className="w-8 h-8 bg-primary rounded-lg flex items-center justify-center">
                <i className="fas fa-box text-primary-foreground text-sm"></i>
              </div>
              <h1 className="text-xl font-bold text-foreground" data-testid="heading-app-title">
                Package Analyzer
              </h1>
            </div>
            <span className="text-sm text-muted-foreground" data-testid="text-version">
              v1.0.0
            </span>
          </div>
          <div className="flex items-center space-x-4">
            <button 
              className="text-muted-foreground hover:text-foreground transition-colors"
              data-testid="button-settings"
            >
              <i className="fas fa-cog"></i>
            </button>
            <button 
              className="text-muted-foreground hover:text-foreground transition-colors"
              data-testid="button-help"
            >
              <i className="fas fa-question-circle"></i>
            </button>
          </div>
        </div>
      </div>
    </header>
  );
}
