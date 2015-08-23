set(AKAGORIA_LANGUAGES
  fr
)

file(GLOB YML_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  "../data/*.yml"
)

gettext_xgettext("akagoria"
  FILES ${YML_FILES}
)

gettext_merge("akagoria.pot"
  ALL
  LANGUAGES ${AKAGORIA_LANGUAGES}
)

gettext_fmt("akagoria"
  ALL
  LANGUAGES ${AKAGORIA_LANGUAGES}
  INSTALL_DESTINATION "${CMAKE_INSTALL_LOCALEDIR}"
)
