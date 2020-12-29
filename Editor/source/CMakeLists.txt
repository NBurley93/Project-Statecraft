file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cc" "*.hh")
target_sources(game_editor PRIVATE
    ${sources}
    app.rc
)