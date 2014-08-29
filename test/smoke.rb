#!/usr/bin/env ruby

require '../dir'

# ["", "/", "tmp", "/", "__test_dir__20140829-5628-avzoep", "/", "[abcdef]"]
p Dir::Glob.path_split '/tmp/__test_dir__20140829-5628-avzoep/[abcdef]'
p Dir::Glob.path_split '*'

Dir.chdir '/home/alex' do
  f = Dir.glob '**/example/**/*.m4'
  p f
  p f.size
end
