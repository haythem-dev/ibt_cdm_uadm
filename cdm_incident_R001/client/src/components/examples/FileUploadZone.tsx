import FileUploadZone from '../FileUploadZone';

export default function FileUploadZoneExample() {
  const handleFileSelect = (file: File) => {
    console.log('File selected in example:', file.name);
  };

  return (
    <div className="p-4 max-w-md">
      <FileUploadZone 
        onFileSelect={handleFileSelect}
        maxFileSize={10}
        acceptedTypes={['.zip', '.tar', '.tar.gz', '.rar', '.7z']}
      />
    </div>
  );
}