require 'test/unit'

require 'tmpdir'
require 'fileutils'
require 'pathname'

require '../dir'

class TestDir < Test::Unit::TestCase

  def setup
    @verbose = $VERBOSE
    $VERBOSE = nil
    @root = Pathname.new(Dir.mktmpdir('__test_dir__')).realpath.to_s
    @nodir = File.join(@root, "dummy")
    for i in ?a..?z
      if i.ord % 2 == 0
        FileUtils.touch(File.join(@root, i))
      else
        FileUtils.mkdir(File.join(@root, i))
      end
    end
  end

  def teardown
    $VERBOSE = @verbose
    FileUtils.remove_entry_secure @root if File.directory?(@root)
  end

  def test_glob
    assert_equal((%w(. ..) + (?a..?z).to_a).map{|f| File.join(@root, f) },
                 Dir.glob(File.join(@root, "*"), File::FNM_DOTMATCH).sort)
    assert_equal([@root] + (?a..?z).map {|f| File.join(@root, f) }.sort,
                 Dir.glob([@root, File.join(@root, "*")]).sort)
    # FIXME
#    assert_equal([@root] + (?a..?z).map {|f| File.join(@root, f) }.sort,
#                 Dir.glob(@root + "\0\0\0" + File.join(@root, "*")).sort)

    assert_equal((?a..?z).step(2).map {|f| File.join(File.join(@root, f), "") }.sort,
                 Dir.glob(File.join(@root, "*/")).sort)

    FileUtils.touch(File.join(@root, "{}"))
    assert_equal(%w({} a).map{|f| File.join(@root, f) },
                 Dir.glob(File.join(@root, '{\{\},a}')))
    assert_equal([], Dir.glob(File.join(@root, '[')))
    assert_equal([], Dir.glob(File.join(@root, '[a-\\')))

    assert_equal([File.join(@root, "a")], Dir.glob(File.join(@root, 'a\\')))

    assert_equal((?a..?f).map {|f| File.join(@root, f) }.sort,
                 Dir.glob(File.join(@root, '[abcdef]')).sort)

    assert_raises(ArgumentError) { Dir.glob('foo[ba/r]baz') }
  end

  def test_glob_recursive
    bug6977 = '[ruby-core:47418]'
    Dir.chdir(@root) do
      FileUtils.mkdir_p("a/b/c/d/e/f")
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/b/c/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/?/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/**/d/e/f"), bug6977)
      assert_equal(["a/b/c/d/e/f"], Dir.glob("a/**/c/**/d/e/f"), bug6977)

      bug8283 = '[ruby-core:54387] [Bug #8283]'
      dirs = ["a/.x", "a/b/.y"]
      FileUtils.mkdir_p(dirs)
      dirs.map {|dir| open("#{dir}/z", "w") {}}
      assert_equal([], Dir.glob("a/**/z").sort, bug8283)
      assert_equal(["a/.x/z"], Dir.glob("a/**/.x/z"), bug8283)
      assert_equal(["a/.x/z"], Dir.glob("a/.x/**/z"), bug8283)
      assert_equal(["a/b/.y/z"], Dir.glob("a/**/.y/z"), bug8283)
    end
  end

end
