include(DownloadProject)
download_project(PROJ                mxml
                 GIT_REPOSITORY      https://github.com/widgetii/mxml.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${mxml_SOURCE_DIR} ${mxml_BINARY_DIR})

include_directories(${mxml_SOURCE_DIR})
