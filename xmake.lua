add_rules("mode.debug", "mode.release")

target("pyrite")
  set_kind("static")
  add_files("src/*.cc")

target("test")
  set_kind("binary")
  add_files("test/main.cc")

target("testbyte")
  set_kind("binary")
  add_files("test/byte.cc")
  add_deps("pyrite")

target("testpackage")
  set_kind("binary")
  add_files("test/package.cc")
  add_deps("pyrite")
