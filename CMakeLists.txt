cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PamplejuceVersion)
include(CPM)
include(PamplejuceMacOS)
include(JUCEDefaults)

# ==============================================================================
# PER-PLUGIN: change these for each new plugin
# ==============================================================================
set(PROJECT_NAME "dumumub-0000004")
set(PRODUCT_NAME "dumumub-0000004")
set(COMPANY_NAME "dumumub")
set(BUNDLE_ID "com.dumumub.${PROJECT_NAME}")
set(FORMATS Standalone AU VST3 AUv3)

# PLUGIN_MANUFACTURER_CODE: always "Dumu" for dumumub — do not change
set(PLUGIN_MANUFACTURER_CODE "Dumu")

# PLUGIN_CODE: 4 chars, first char MUST be uppercase
# !!! MUST be unique across every dumumub plugin — never reuse a code !!!
set(PLUGIN_CODE "Du04")

set(PLUGIN_IS_SYNTH        FALSE)
set(PLUGIN_NEEDS_MIDI_IN   FALSE)
set(PLUGIN_NEEDS_MIDI_OUT  FALSE)
set(PLUGIN_IS_MIDI_EFFECT  FALSE)
# ==============================================================================

set(CMAKE_POLICY_DEFAULT_CMP0141        NEW      CACHE STRING "" FORCE)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded CACHE STRING "" FORCE)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

add_subdirectory(JUCE)

juce_add_plugin("${PROJECT_NAME}"
    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
    COMPANY_NAME "${COMPANY_NAME}"
    BUNDLE_ID "${BUNDLE_ID}"
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE ${PLUGIN_MANUFACTURER_CODE}
    PLUGIN_CODE ${PLUGIN_CODE}
    FORMATS "${FORMATS}"
    PRODUCT_NAME "${PRODUCT_NAME}"
    IS_SYNTH ${PLUGIN_IS_SYNTH}
    NEEDS_MIDI_INPUT ${PLUGIN_NEEDS_MIDI_IN}
    NEEDS_MIDI_OUTPUT ${PLUGIN_NEEDS_MIDI_OUT}
    IS_MIDI_EFFECT ${PLUGIN_IS_MIDI_EFFECT})

add_library(SharedCode INTERFACE)

include(SharedCodeDefaults)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
target_sources(SharedCode INTERFACE ${SourceFiles})

# Allow <JuceHeader.h> and sibling includes to be found with angle brackets
target_include_directories(SharedCode INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/source")

include(Assets)
include(XcodePrettify)

target_compile_definitions(SharedCode
    INTERFACE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    # JUCE_DISPLAY_SPLASH_SCREEN=0  # uncomment for Indie/Pro JUCE license or GPL release
    CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    VERSION="${CURRENT_VERSION}"
    PRODUCT_NAME_WITHOUT_VERSION="${PROJECT_NAME}")

target_link_libraries(SharedCode
    INTERFACE
    Assets
    juce_audio_utils
    juce_audio_processors
    juce_dsp
    juce_gui_basics
    juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_link_libraries("${PROJECT_NAME}" PRIVATE SharedCode)

# PamplejuceIPP disabled — path-with-spaces causes link failures on macOS
# include(PamplejuceIPP)
include(Tests)
include(Benchmarks)
include(GitHubENV)
