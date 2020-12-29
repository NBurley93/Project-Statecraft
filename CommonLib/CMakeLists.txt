add_subdirectory(source)
target_precompile_headers(RitualCommonLib PRIVATE pch.hh)
target_sources(RitualCommonLib PRIVATE pch.cc)