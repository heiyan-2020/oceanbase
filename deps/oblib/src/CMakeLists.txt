add_library(oblib_base_base INTERFACE)
add_library(oblib_base_base_base INTERFACE)

get_property(EASY_INCLUDE_DIRS GLOBAL PROPERTY "EASY_INCLUDE_DIRS" )


target_include_directories(
  oblib_base_base_base INTERFACE
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/deps/oblib/src/lib/oracleclient
  ${CMAKE_SOURCE_DIR}/deps/easy/src
  ${CMAKE_SOURCE_DIR}/deps/oblib/src
  ${CMAKE_SOURCE_DIR}/deps/oblib/src/common
  ${CMAKE_SOURCE_DIR}/deps/easy/src/include
  ${CMAKE_SOURCE_DIR}/deps/ussl-hook
  ${CMAKE_SOURCE_DIR}/deps/ussl-hook/loop
  ${CMAKE_SOURCE_DIR}/src/objit/include
  ${CMAKE_SOURCE_DIR}/src/objit/src
  ${DEP_DIR}/include
  ${DEP_DIR}/include/libxml2
  ${DEP_DIR}/include/mariadb
  ${DEP_3RD_DIR}/usr/local/include
  ${DEVTOOLS_DIR}
  ${DEP_3RD_DIR}/usr/local/include
  ${DEP_3RD_DIR}/usr/include/
  ${DEP_3RD_DIR}/usr/local/include
  ${DEP_DIR}/include/apr-1/
  ${DEP_DIR}/include/icu/common
  ${USSL_INCLUDE_DIRS}
  )

if (OB_USE_CLANG)
  # The following clang warnings should be fixed later: -Wno-unused-variable -Wno-invalid-offsetof
  target_compile_options(oblib_base_base_base
    INTERFACE
    -fno-strict-aliasing -fno-omit-frame-pointer ${MARCH_CFLAGS} ${MTUNE_CFLAGS}
    -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D_NO_EXCEPTION -DTSI_STATIC_SUM
    -DOCI_LINK_RUNTIME
    -Wall -Wextra -Wformat -Werror
    -Wno-deprecated -Wno-address-of-packed-member -Wno-sign-compare -Wno-tautological-compare
    -Wno-psabi -Wno-c99-designator -Wno-int-in-bool-context -Wno-sizeof-array-div
    -Wno-implicit-const-int-float-conversion -Wno-fortify-source -Wno-non-c-typedef-for-linkage
    -Wno-uninitialized-const-reference -Wno-unused-function -Wno-atomic-alignment
    -Wno-tautological-constant-out-of-range-compare -Wno-unused-parameter
    -Wno-string-plus-int -Wno-misleading-indentation -Wno-unused-private-field
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-overloaded-virtual -Wno-reserved-user-defined-literal
    -Wno-self-assign -Wno-inconsistent-missing-override -Wno-mismatched-tags -Wno-unused-variable
    -Wno-invalid-offsetof -Wno-unevaluated-expression -Wno-constant-logical-operand>
    $<$<COMPILE_LANGUAGE:C>:-Wno-unused-parameter>)
  if (ENABLE_SANITY)
    target_compile_options(oblib_base_base
      INTERFACE
      "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-Xclang -load -Xclang ${CMAKE_CURRENT_SOURCE_DIR}/lib/alloc/libsanity_${ARCHITECTURE}.so>"
      -fno-builtin-memset
      -fno-builtin-bzero
      -fno-builtin-memcpy
      -fno-builtin-memmove
      -fno-builtin-memcmp
      -fno-builtin-strlen
      -fno-builtin-strnlen
      -fno-builtin-strcpy
      -fno-builtin-strncpy
      -fno-builtin-strcmp
      -fno-builtin-strncmp
      -fno-builtin-strcasecmp
      -fno-builtin-strncasecmp
      -fno-builtin-vsprintf
      -fno-builtin-vsnprintf
      -fno-builtin-sprintf
      -fno-builtin-snprintf
    )
  endif()
else()
  target_compile_options(oblib_base_base_base
    INTERFACE
    -fno-strict-aliasing -fno-omit-frame-pointer ${MARCH_CFLAGS} ${MTUNE_CFLAGS}
    -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D_NO_EXCEPTION
    -DOCI_LINK_RUNTIME
    -Wall -Wextra -Wformat -Werror
    -Wno-deprecated
    -Wno-sign-compare
    -Wno-psabi
    -Wno-attributes
    -Wno-nonnull-compare
    -Wno-parentheses
    -Wno-implicit-fallthrough
    -Wno-switch
    -Wno-address-of-packed-member
    -Wno-ignored-qualifiers
    -Wno-format
    -Wno-type-limits
    -Wno-format-truncation
    -Wno-int-in-bool-context
    -Wno-cast-function-type
    -Wno-unused-function
    -Wno-overflow
    -Wno-unused-but-set-variable
    -Wno-memset-elt-size
    $<$<COMPILE_LANGUAGE:CXX>:
    -Wno-literal-suffix
    -Wno-invalid-offsetof
    -Wno-unused-variable
    -Wno-deprecated-copy
    -Wno-class-memaccess
    -Wno-unused-parameter
    -Wno-unused-but-set-variable
    -Wno-pragmas
    -Wno-maybe-uninitialized
    -Wno-subobject-linkage>
    $<$<COMPILE_LANGUAGE:C>:
    -Wno-unused-parameter>
    )
endif()

target_compile_features(oblib_base_base INTERFACE cxx_std_11)

set(LGPL_DEPS "-L${DEP_DIR}/lib/mariadb -lmariadb")
if (OB_STATIC_LINK_LGPL_DEPS)
  set(LGPL_DEPS "-L${DEP_DIR}/lib/mariadb -l:libmariadbclient.a")
endif()
target_link_libraries(oblib_base_base_base
  INTERFACE
  oss
  easy
  ${DEP_DIR}/lib/libisal.a
  ${DEP_DIR}/lib/libssl.a
  ${DEP_DIR}/lib/libcrypto.a
  ${DEP_DIR}/lib/libs2.a
  ${LGPL_DEPS}
  $<$<STREQUAL:"${ARCHITECTURE}","x86_64">:${DEP_DIR}/lib/libunwind.a>
  ${DEP_DIR}/lib/libz.a
  ${DEP_DIR}/lib/libs2.a
  ${DEP_DIR}/lib/libicui18n.a
  ${DEP_DIR}/lib/libicustubdata.a
  ${DEP_DIR}/lib/libicuuc.a
  -L${DEP_DIR}/var/usr/lib64
  -L${DEP_DIR}/var/usr/lib
  -L${DEP_3RD_DIR}/usr/lib
  -L${DEP_3RD_DIR}/usr/lib64
  -laio -lpthread -lcurl -ldl -lrt
  ${ARCH_LDFLAGS}
 )
target_link_libraries(oblib_base_base INTERFACE oblib_base_base_base)

add_library(oblib_base INTERFACE)
target_compile_options(oblib_base INTERFACE -D_GLIBCXX_USE_CXX11_ABI=0)
target_link_libraries(oblib_base INTERFACE oblib_base_base)

function(oblib_add_library target)
  message(STATUS "oblib_add_library ${target}")
  add_library(${target} OBJECT ${ARGN})
  list(APPEND oblib_object_libraries ${target})
  set(oblib_object_libraries
    "${oblib_object_libraries}" CACHE INTERNAL oblib_object_libraries)
endfunction()

function(oblib_add_extra_objects)
  list(APPEND oblib_extra_objects ${ARGN})
  set(oblib_extra_objects "${oblib_extra_objects}" CACHE INTERNAL oblib_extra_objects)
endfunction()

set(oblib_object_libraries "" CACHE INTERNAL oblib_object_libraries)
set(oblib_extra_objects "" CACHE INTERNAL oblib_extra_objects)

add_subdirectory(lib)
add_subdirectory(common)
add_subdirectory(rpc)

# Define static library which contains previous object libraries.
message(STATUS "oblib_extra_objects ${oblib_extra_objects}")
set_source_files_properties(${oblib_extra_objects} PROPERTIES GENERATED TRUE)
add_library(oblib STATIC ${oblib_extra_objects})
target_link_libraries(oblib PUBLIC oblib_base ${oblib_object_libraries})

if (TSI_STATIC_SUM)
  target_compile_definitions(oblib_base_base INTERFACE TSI_STATIC_SUM=1)
endif()
