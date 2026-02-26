import { Code, Smartphone, Puzzle, Rocket, Shield, Settings } from "lucide-react";
import { Button } from "@/components/ui/button";

export default function Home() {
  const features = [
    {
      icon: <Code className="text-blue-600 text-xl" />,
      title: "Clean Code Structure",
      description: "Organized file structure with clear separation of concerns, making it easy to maintain and scale your application.",
      bgColor: "bg-blue-100"
    },
    {
      icon: <Smartphone className="text-green-600 text-xl" />,
      title: "Responsive Design",
      description: "Mobile-first approach with responsive components that work seamlessly across all device sizes.",
      bgColor: "bg-green-100"
    },
    {
      icon: <Puzzle className="text-purple-600 text-xl" />,
      title: "Modular Components",
      description: "Reusable components designed for easy customization and extension as your project grows.",
      bgColor: "bg-purple-100"
    },
    {
      icon: <Rocket className="text-orange-600 text-xl" />,
      title: "Performance Optimized",
      description: "Built with performance in mind, using modern best practices for fast loading and smooth interactions.",
      bgColor: "bg-orange-100"
    },
    {
      icon: <Shield className="text-red-600 text-xl" />,
      title: "Secure Foundation",
      description: "Security best practices implemented from the ground up to protect your application and users.",
      bgColor: "bg-red-100"
    },
    {
      icon: <Settings className="text-teal-600 text-xl" />,
      title: "Easy Configuration",
      description: "Simple configuration options that allow you to customize the project to fit your specific needs.",
      bgColor: "bg-teal-100"
    }
  ];

  const handleGetStarted = () => {
    console.log("Get Started clicked");
    // TODO: Implement get started functionality
  };

  const handleLearnMore = () => {
    console.log("Learn More clicked");
    // TODO: Implement learn more functionality
  };

  const handleStartBuilding = () => {
    console.log("Start Building clicked");
    // TODO: Implement start building functionality
  };

  return (
    <div className="min-h-screen bg-slate-50">
      {/* Hero Section */}
      <main className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="py-20">
          <div className="text-center">
            <h1 className="text-4xl sm:text-5xl lg:text-6xl font-bold text-slate-900 mb-6" data-testid="text-hero-title">
              Ready to Build
              <span className="block text-blue-600">Something Amazing</span>
            </h1>
            <p className="text-xl text-slate-600 max-w-3xl mx-auto mb-8" data-testid="text-hero-description">
              Your project foundation is set up and ready for development. Clean structure, organized codebase, and scalable architecture await your creative vision.
            </p>
            <div className="flex flex-col sm:flex-row gap-4 justify-center">
              <Button
                onClick={handleGetStarted}
                className="bg-blue-600 hover:bg-blue-700 text-white font-medium py-3 px-8 rounded-lg transition-colors"
                data-testid="button-get-started"
              >
                Get Started
              </Button>
              <Button
                variant="outline"
                onClick={handleLearnMore}
                className="border border-slate-300 hover:border-slate-400 text-slate-700 font-medium py-3 px-8 rounded-lg transition-colors"
                data-testid="button-learn-more"
              >
                Learn More
              </Button>
            </div>
          </div>
        </div>
      </main>

      {/* Features Section */}
      <section className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-16">
        <div className="text-center mb-16">
          <h2 className="text-3xl font-bold text-slate-900 mb-4" data-testid="text-features-title">
            Built for Development
          </h2>
          <p className="text-lg text-slate-600 max-w-2xl mx-auto" data-testid="text-features-description">
            Everything you need to start building your next project with confidence and speed.
          </p>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8">
          {features.map((feature, index) => (
            <div
              key={index}
              className="bg-white rounded-xl shadow-sm border border-slate-200 p-8 hover:shadow-md transition-shadow"
              data-testid={`card-feature-${index}`}
            >
              <div className={`w-12 h-12 ${feature.bgColor} rounded-lg flex items-center justify-center mb-6`}>
                {feature.icon}
              </div>
              <h3 className="text-xl font-semibold text-slate-900 mb-3" data-testid={`text-feature-title-${index}`}>
                {feature.title}
              </h3>
              <p className="text-slate-600" data-testid={`text-feature-description-${index}`}>
                {feature.description}
              </p>
            </div>
          ))}
        </div>
      </section>

      {/* CTA Section */}
      <section className="bg-slate-900 text-white">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-16">
          <div className="text-center">
            <h2 className="text-3xl font-bold mb-4" data-testid="text-cta-title">
              Ready to Start Building?
            </h2>
            <p className="text-slate-300 text-lg mb-8 max-w-2xl mx-auto" data-testid="text-cta-description">
              Your foundation is ready. Add your features, customize the design, and bring your vision to life.
            </p>
            <Button
              onClick={handleStartBuilding}
              className="bg-blue-600 hover:bg-blue-700 text-white font-medium py-3 px-8 rounded-lg transition-colors"
              data-testid="button-start-building"
            >
              Start Building
            </Button>
          </div>
        </div>
      </section>

      {/* Footer */}
      <footer className="bg-white border-t border-slate-200">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-12">
          <div className="grid grid-cols-1 md:grid-cols-4 gap-8">
            <div className="col-span-1 md:col-span-2">
              <h3 className="text-lg font-semibold text-slate-900 mb-4" data-testid="text-footer-title">
                Project Foundation
              </h3>
              <p className="text-slate-600 mb-4" data-testid="text-footer-description">
                A clean, scalable foundation for your next web application. Built with modern technologies and best practices.
              </p>
            </div>
            <div>
              <h4 className="text-sm font-semibold text-slate-900 uppercase tracking-wider mb-4" data-testid="text-footer-resources-title">
                Resources
              </h4>
              <ul className="space-y-2">
                <li>
                  <a href="#" className="text-slate-600 hover:text-slate-900 transition-colors" data-testid="link-documentation">
                    Documentation
                  </a>
                </li>
                <li>
                  <a href="#" className="text-slate-600 hover:text-slate-900 transition-colors" data-testid="link-examples">
                    Examples
                  </a>
                </li>
                <li>
                  <a href="#" className="text-slate-600 hover:text-slate-900 transition-colors" data-testid="link-support">
                    Support
                  </a>
                </li>
              </ul>
            </div>
            <div>
              <h4 className="text-sm font-semibold text-slate-900 uppercase tracking-wider mb-4" data-testid="text-footer-connect-title">
                Connect
              </h4>
              <ul className="space-y-2">
                <li>
                  <a href="#" className="text-slate-600 hover:text-slate-900 transition-colors" data-testid="link-github">
                    GitHub
                  </a>
                </li>
                <li>
                  <a href="#" className="text-slate-600 hover:text-slate-900 transition-colors" data-testid="link-twitter">
                    Twitter
                  </a>
                </li>
                <li>
                  <a href="#" className="text-slate-600 hover:text-slate-900 transition-colors" data-testid="link-discord">
                    Discord
                  </a>
                </li>
              </ul>
            </div>
          </div>
          <div className="border-t border-slate-200 mt-8 pt-8">
            <p className="text-center text-slate-600 text-sm" data-testid="text-footer-copyright">
              &copy; {new Date().getFullYear()} Project Foundation. Ready for your next big idea.
            </p>
          </div>
        </div>
      </footer>
    </div>
  );
}
