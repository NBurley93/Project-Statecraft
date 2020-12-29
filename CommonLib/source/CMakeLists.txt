file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cc" "*.hh")
target_sources(RitualCommonLib PRIVATE
    ${sources}
)