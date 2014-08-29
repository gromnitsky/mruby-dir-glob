MRuby::Gem::Specification.new('mruby-dir-glob') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Alexander Gromnitsky'
  spec.version = '0.0.1'
  spec.description = 'File.fnmatch() & Dir.glob()'
  spec.homepage = 'https://github.com/gromnitsky/mruby-dir-glob'

  spec.add_dependency 'mruby-require'
  spec.add_dependency 'mruby-dir'
  spec.add_dependency 'mruby-file-stat'
  spec.add_dependency 'mruby-io'
  spec.add_dependency 'mruby-errno'

  # how to specify a 'general' regexp engine?
#  spec.add_dependency 'mruby-regexp-pcre'
end
