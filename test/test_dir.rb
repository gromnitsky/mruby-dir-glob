def mruby?
  RUBY_ENGINE == 'mruby'
end

if mruby?
  testunit_class = MTest::Unit::TestCase
else
  require 'test/unit'
  require 'tmpdir'
  require 'fileutils'
  require 'pathname'

  testunit_class = Test::Unit::TestCase
end

require '../dir'
require '../dir_glob'

class TestDir < testunit_class

  def setup
    @verbose = $VERBOSE
    $VERBOSE = nil
    @root = '3e3b0b06-6485-4cf1-b1a5-99790b52253a'
    Dir.mkdir @root
    @nodir = File.join(@root, "dummy")
    for i in ?a..?z
      if i.unpack('C*').first % 2 == 0
        File.open(File.join(@root, i), 'a')
      else
        Dir.mkdir(File.join(@root, i))
      end
    end
  end

  def teardown
    $VERBOSE = @verbose
    system "rm -rf #{@root}" if File.directory?(@root)
  end

  def test_glob
    assert_equal((%w(. ..) + (?a..?z).to_a).map{|f| File.join(@root, f) },
                 Dir.glob(File.join(@root, "*"), File::FNM_DOTMATCH).sort)
    assert_equal([@root] + (?a..?z).map {|f| File.join(@root, f) }.sort,
                 Dir.glob([@root, File.join(@root, "*")]).sort)
    # FIXME
#    assert_equal([@root] + (?a..?z).map {|f| File.join(@root, f) }.sort,
#                 Dir.glob(@root + "\0\0\0" + File.join(@root, "*")).sort)

    dirs = ((?a..?z).each_with_index.select {|file, idx|
         idx % 2 == 0
       }.map {|f| File.join(File.join(@root, f.first), "") }.sort)
    assert_equal(dirs, Dir.glob(File.join(@root, "*/")).sort)

    File.open(File.join(@root, "{}"), 'a')
    assert_equal(%w({} a).map{|f| File.join(@root, f) },
                 Dir.glob(File.join(@root, '{\{\},a}')))
    assert_equal([], Dir.glob(File.join(@root, '[')))
    assert_equal([], Dir.glob(File.join(@root, '[a-\\')))

    assert_equal([File.join(@root, "a")], Dir.glob(File.join(@root, 'a\\')))

    assert_equal((?a..?f).map {|f| File.join(@root, f) }.sort,
                 Dir.glob(File.join(@root, '[abcdef]')).sort)
  end

  def test_glob_recursive
    bug6977 = '[ruby-core:47418]'
    Dir.chdir(@root) do
      system "mkdir -p a/b/c/d/e/f"
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/b/c/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/?/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/**/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/**/d/e/f"), bug6977)

      bug8283 = '[ruby-core:54387] [Bug #8283]'
      dirs = ["a/.x", "a/b/.y"]
      system "mkdir -p #{dirs.join ' '}"
      dirs.map {|dir| open("#{dir}/z", "w") {}}
      assert_equal([], Dir.glob("a/**/z").sort, bug8283)
      assert_equal(["a/.x/z"], Dir.glob("a/**/.x/z"), bug8283)
      assert_equal(["a/.x/z"], Dir.glob("a/.x/**/z"), bug8283)
      assert_equal(["a/b/.y/z"], Dir.glob("a/**/.y/z"), bug8283)
    end
  end

end

MTest::Unit.new.run if mruby?
