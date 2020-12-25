file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cc" "*.hh")
target_sources(game_runtime PRIVATE
    ${sources}
)
target_precompile_headers(game_runtime PRIVATE pch.hh)