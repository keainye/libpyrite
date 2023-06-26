add_rules("mode.debug", "mode.release")

target("pyrite")
  set_kind("static")
  add_files("src/*.cc")