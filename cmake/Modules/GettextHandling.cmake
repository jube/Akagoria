function(GETTEXT_MERGE potFile)

  set(options ALL)
  set(oneValueArgs "")
  set(multiValueArgs LANGUAGES)

  CMAKE_PARSE_ARGUMENTS(parsed "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(poFiles)

  foreach(lang ${parsed_LANGUAGES})
    set(poFile "${CMAKE_CURRENT_SOURCE_DIR}/${lang}.po")

    add_custom_command(
       OUTPUT "${poFile}"
       COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --update --quiet --sort-by-file ${poFile} ${potFile}
       WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
       DEPENDS ${potFile}
    )

    list(APPEND poFiles ${poFile})
  endforeach()

  if (parse_ALL)
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

  if (parse_ALL)
    add_custom_target(mofiles ALL DEPENDS ${gmoFiles})
  else()
    add_custom_target(mofiles DEPENDS ${gmoFiles})
  endif()

endfunction()
