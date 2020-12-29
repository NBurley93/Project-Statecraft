file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cc" "*.hh")
target_sources(game_toolbox PRIVATE
    ${sources}
    app.rc
)