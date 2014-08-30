assert('File.fnmatch') do
  File.respond_to? :fnmatch
end
