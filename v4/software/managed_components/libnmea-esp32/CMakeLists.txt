cmake_minimum_required(VERSION 3.5)

set(common "libnmea/src/nmea/nmea.c"
           "libnmea/src/nmea/parser_static.c"
           "libnmea/src/parsers/parse.c"
           )

# 'register_component' used instead of 'idf_component_register'
# for compatibility with IDF 3.3.
set(COMPONENT_ADD_INCLUDEDIRS "libnmea/src/nmea" "libnmea/src/parsers")
set(COMPONENT_SRCS ${common})
register_component()

set(parsers gpgga gpgll gprmc gpgsa gpgsv gptxt gpvtg)

foreach(parser ${parsers})
    # add source file
    set(src_file "libnmea/src/parsers/${parser}.c")
    target_sources(${COMPONENT_TARGET} PRIVATE ${src_file})

    # add some preprocessor definitions to rename the interface functions
    set(prefix nmea_${parser}_)
    set(defs "allocate_data=${prefix}allocate_data"
             "free_data=${prefix}free_data"
             "init=${prefix}init"
             "parse=${prefix}parse"
             "set_default=${prefix}set_default"
    )
    set_source_files_properties(${src_file} PROPERTIES COMPILE_DEFINITIONS "${defs}")

    # Enable the parser
    string(TOUPPER ${parser} parser_uppercase)
    target_compile_definitions(${COMPONENT_TARGET} PRIVATE "ENABLE_${parser_uppercase}=1")
endforeach()

list(LENGTH parsers parsers_count)
target_compile_definitions(${COMPONENT_TARGET} PRIVATE PARSER_COUNT=${parsers_count})
target_compile_options(${COMPONENT_TARGET} PRIVATE "-Wno-strict-prototypes")

if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_LESS "4.0")
    set_source_files_properties(libnmea/src/parsers/gpgsa.c PROPERTIES COMPILE_OPTIONS "-Wno-char-subscripts")
endif()
