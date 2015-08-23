find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
find_program(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)
find_program(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(L10N
  REQUIRED_VARS GETTEXT_XGETTEXT_EXECUTABLE GETTEXT_MSGMERGE_EXECUTABLE GETTEXT_MSGFMT_EXECUTABLE
)

include(CMakeParseArguments)

function(GETTEXT_XGETTEXT domain)
  set(options "")
  set(oneValueArgs "")
  set(multiValueArgs FILES)

  cmake_parse_arguments(parsed "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(potFile "${CMAKE_CURRENT_SOURCE_DIR}/${domain}.pot")

  if (parsed_FILES)
    add_custom_command(
      OUTPUT "${potFile}"
      COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} --default-domain=${domain} --output=${potFile} --language=C --extract-all --sort-by-file ${parsed_FILES}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      DEPENDS ${parsed_FILES}
    )

    add_custom_target(potfile DEPENDS ${potFile})
  endif()

endfunction()


function(GETTEXT_MERGE potFile)

  set(options ALL)
  set(oneValueArgs "")
  set(multiValueArgs LANGUAGES)

  cmake_parse_arguments(parsed "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(poFiles)

  foreach(lang ${parsed_LANGUAGES})
    set(poFile "${CMAKE_CURRENT_SOURCE_DIR}/${lang}.po")

    add_custom_command(
      OUTPUT "${poFile}"
      COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --update --backup=none --quiet --sort-by-file ${poFile} ${potFile}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      DEPENDS ${potFile}
    )

    list(APPEND poFiles ${poFile})
  endforeach()

  if (parsed_ALL)
    add_custom_target(pofiles ALL DEPENDS ${poFiles})
  else()
    add_custom_target(pofiles DEPENDS ${poFiles})
  endif()

endfunction()

function(GETTEXT_FMT domain)

  set(options ALL)
  set(oneValueArgs INSTALL_DESTINATION)
  set(multiValueArgs LANGUAGES)

  cmake_parse_arguments(parsed "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(gmoFiles)

  foreach(lang ${parsed_LANGUAGES})
    set(poFile "${CMAKE_CURRENT_SOURCE_DIR}/${lang}.po")
    set(gmoFile "${CMAKE_CURRENT_BINARY_DIR}/${lang}.gmo")

    add_custom_command(
      OUTPUT "${gmoFile}"
      COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${gmoFile} ${poFile}
      DEPENDS ${poFile}
    )

    if(parsed_INSTALL_DESTINATION)
      install(FILES ${gmoFile} DESTINATION ${parsed_INSTALL_DESTINATION}/${lang}/LC_MESSAGES/ RENAME ${domain}.mo)
    endif()

    list(APPEND gmoFiles ${gmoFile})
  endforeach()

  if (parsed_ALL)
    add_custom_target(mofiles ALL DEPENDS ${gmoFiles})
  else()
    add_custom_target(mofiles DEPENDS ${gmoFiles})
  endif()

endfunction()
