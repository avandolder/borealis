add_requires("raylib ~4", "entt ~3.10.1", "tmx", "zlib", "libxml2")

set_defaultmode("debug")
set_languages("c17", "cxx20")

target("borealis")
  set_kind("binary")
  add_files("src/*.cpp", "src/*.mpp")
  add_packages("raylib", "entt", "tmx", "zlib", "libxml2")
  add_cxflags("-Wall", "-Wextra", "-Werror", "-pedantic")

  add_defines("DEBUG")
  set_symbols("debug")
  set_optimize("none")
