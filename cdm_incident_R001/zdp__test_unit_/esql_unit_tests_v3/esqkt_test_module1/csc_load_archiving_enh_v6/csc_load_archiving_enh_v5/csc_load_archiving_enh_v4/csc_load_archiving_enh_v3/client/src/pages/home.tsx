import { Button } from "@/components/ui/button";

export default function Home() {
  return (
    <div className="min-h-screen bg-slate-50">
      {/* Header */}
      <header className="bg-white shadow-sm border-b border-slate-200">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
          <div className="flex justify-between items-center h-16">
            <div className="flex items-center">
              <div className="flex-shrink-0">
                <h1 className="text-xl font-semibold text-slate-900">Project Starter</h1>
              </div>
            </div>
            <nav className="hidden md:block">
              <div className="ml-10 flex items-baseline space-x-4">
                <a href="#" className="text-slate-600 hover:text-slate-900 px-3 py-2 rounded-md text-sm font-medium transition-colors duration-200">Home</a>
                <a href="#" className="text-slate-600 hover:text-slate-900 px-3 py-2 rounded-md text-sm font-medium transition-colors duration-200">About</a>
                <a href="#" className="text-slate-600 hover:text-slate-900 px-3 py-2 rounded-md text-sm font-medium transition-colors duration-200">Contact</a>
              </div>
            </nav>
            <div className="md:hidden">
              <button type="button" className="text-slate-600 hover:text-slate-900 focus:outline-none focus:ring-2 focus:ring-primary-500">
                <svg className="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                  <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M4 6h16M4 12h16M4 18h16" />
                </svg>
              </button>
            </div>
          </div>
        </div>
      </header>

      {/* Hero Section */}
      <main className="flex-1">
        <section className="py-20 px-4 sm:px-6 lg:px-8">
          <div className="max-w-4xl mx-auto text-center">
            <h1 className="text-4xl md:text-6xl font-bold text-slate-900 mb-6">
              <span>Welcome to Your</span>{" "}
              <span className="text-primary-600">New Project</span>
            </h1>
            <p className="text-xl text-slate-600 mb-8 max-w-2xl mx-auto leading-relaxed">
              This is your clean slate. A minimal, well-structured foundation ready to be transformed into whatever amazing application you envision.
            </p>
            <div className="flex flex-col sm:flex-row gap-4 justify-center">
              <Button className="bg-primary-600 text-white px-8 py-3 rounded-lg font-medium hover:bg-primary-700 focus:outline-none focus:ring-2 focus:ring-primary-500 focus:ring-offset-2 transition-colors duration-200">
                Get Started
              </Button>
              <Button variant="outline" className="border border-slate-300 text-slate-700 px-8 py-3 rounded-lg font-medium hover:bg-slate-50 focus:outline-none focus:ring-2 focus:ring-primary-500 focus:ring-offset-2 transition-colors duration-200">
                Learn More
              </Button>
            </div>
          </div>
        </section>
      </main>

      {/* Feature Grid */}
      <section className="py-16 bg-white">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
          <div className="text-center mb-12">
            <h2 className="text-3xl font-bold text-slate-900 mb-4">Ready for Anything</h2>
            <p className="text-lg text-slate-600 max-w-2xl mx-auto">
              Built with modern standards and best practices, ready to scale with your ideas.
            </p>
          </div>
          <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-8">
            <div className="bg-slate-50 rounded-xl p-6 hover:shadow-lg transition-shadow duration-200">
              <div className="w-12 h-12 bg-primary-100 rounded-lg flex items-center justify-center mb-4">
                <svg className="w-6 h-6 text-primary-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M13 10V3L4 14h7v7l9-11h-7z"></path>
                </svg>
              </div>
              <h3 className="text-xl font-semibold text-slate-900 mb-2">Fast Setup</h3>
              <p className="text-slate-600">Get up and running quickly with a clean, organized codebase that follows modern conventions.</p>
            </div>
            
            <div className="bg-slate-50 rounded-xl p-6 hover:shadow-lg transition-shadow duration-200">
              <div className="w-12 h-12 bg-primary-100 rounded-lg flex items-center justify-center mb-4">
                <svg className="w-6 h-6 text-primary-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M4 5a1 1 0 011-1h14a1 1 0 011 1v2a1 1 0 01-1 1H5a1 1 0 01-1-1V5zM4 13a1 1 0 011-1h6a1 1 0 011 1v6a1 1 0 01-1 1H5a1 1 0 01-1-1v-6zM16 13a1 1 0 011-1h2a1 1 0 011 1v6a1 1 0 01-1 1h-2a1 1 0 01-1-1v-6z"></path>
                </svg>
              </div>
              <h3 className="text-xl font-semibold text-slate-900 mb-2">Flexible Structure</h3>
              <p className="text-slate-600">Modular architecture that adapts to your needs, whether building a simple site or complex application.</p>
            </div>
            
            <div className="bg-slate-50 rounded-xl p-6 hover:shadow-lg transition-shadow duration-200">
              <div className="w-12 h-12 bg-primary-100 rounded-lg flex items-center justify-center mb-4">
                <svg className="w-6 h-6 text-primary-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z"></path>
                </svg>
              </div>
              <h3 className="text-xl font-semibold text-slate-900 mb-2">Best Practices</h3>
              <p className="text-slate-600">Built with accessibility, performance, and maintainability in mind from day one.</p>
            </div>
          </div>
        </div>
      </section>

      {/* Status Section */}
      <section className="py-16 bg-slate-50">
        <div className="max-w-4xl mx-auto px-4 sm:px-6 lg:px-8 text-center">
          <div className="bg-white rounded-2xl shadow-sm border border-slate-200 p-8">
            <div className="inline-flex items-center px-4 py-2 rounded-full bg-green-100 text-green-800 text-sm font-medium mb-6">
              <svg className="w-4 h-4 mr-2" fill="currentColor" viewBox="0 0 20 20">
                <path fillRule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zm3.707-9.293a1 1 0 00-1.414-1.414L9 10.586 7.707 9.293a1 1 0 00-1.414 1.414l2 2a1 1 0 001.414 0l4-4z" clipRule="evenodd"></path>
              </svg>
              <span>Project Initialized</span>
            </div>
            <h2 className="text-2xl font-bold text-slate-900 mb-4">Ready for Development</h2>
            <p className="text-slate-600 mb-6">
              Your project foundation is set up and ready. Time to start building something amazing!
            </p>
            <div className="flex flex-col sm:flex-row gap-3 justify-center">
              <span className="text-sm text-slate-500">Next steps:</span>
              <div className="flex flex-wrap gap-2 justify-center">
                <span className="px-3 py-1 bg-slate-100 text-slate-700 rounded-full text-sm">Define requirements</span>
                <span className="px-3 py-1 bg-slate-100 text-slate-700 rounded-full text-sm">Add features</span>
                <span className="px-3 py-1 bg-slate-100 text-slate-700 rounded-full text-sm">Deploy</span>
              </div>
            </div>
          </div>
        </div>
      </section>

      {/* Footer */}
      <footer className="bg-white border-t border-slate-200">
        <div className="max-w-7xl mx-auto py-8 px-4 sm:px-6 lg:px-8">
          <div className="flex flex-col md:flex-row justify-between items-center">
            <div className="mb-4 md:mb-0">
              <p className="text-sm text-slate-600">
                © 2024 Project Starter. Ready for your vision.
              </p>
            </div>
            <div className="flex space-x-6">
              <a href="#" className="text-sm text-slate-600 hover:text-slate-900 transition-colors duration-200">Documentation</a>
              <a href="#" className="text-sm text-slate-600 hover:text-slate-900 transition-colors duration-200">Support</a>
              <a href="#" className="text-sm text-slate-600 hover:text-slate-900 transition-colors duration-200">GitHub</a>
            </div>
          </div>
        </div>
      </footer>
    </div>
  );
}
