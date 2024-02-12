option(ENABLE_ASAN "Enable AddressSanitizer" NO)

if(MSVC)
    if(ENABLE_ASAN)
        string(REPLACE "/RTC1" ""
            CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}"
        )
        string(REPLACE "/RTC1" ""
            CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}"
        )
        add_compile_options(
            /fsanitize=address
        )
    endif()
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    option(ENABLE_LSAN "Enable LeakSanitizer" NO)
    option(ENABLE_TSAN "Enable ThreadSanitizer" NO)
    option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" NO)

    if((ENABLE_ASAN AND ENABLE_TSAN) OR
        (ENABLE_LSAN AND ENABLE_TSAN))
        message(FATAL_ERROR
            "Invalid sanitizer combination:\n"
            "  ENABLE_ASAN:  ${ENABLE_ASAN}\n"
            "  ENABLE_LSAN:  ${ENABLE_LSAN}\n"
            "  ENABLE_TSAN:  ${ENABLE_TSAN}\n"
        )
    endif()

    add_compile_options(
        -fno-omit-frame-pointer
        $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>
        $<$<BOOL:${ENABLE_LSAN}>:-fsanitize=leak>
        $<$<BOOL:${ENABLE_TSAN}>:-fsanitize=thread>
        $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>
    )
    add_link_options(
        $<$<BOOL:${ENABLE_ASAN}>:-fsanitize=address>
        $<$<BOOL:${ENABLE_LSAN}>:-fsanitize=leak>
        $<$<BOOL:${ENABLE_TSAN}>:-fsanitize=thread>
        $<$<BOOL:${ENABLE_UBSAN}>:-fsanitize=undefined>
    )

    if(ENABLE_ASAN)
        set(ASAN_OPTIONS "ASAN_OPTIONS=detect_leaks=1:abort_on_error=0")
    endif()

    if(ENABLE_LSAN)
        set(LSAN_OPTIONS "LSAN_OPTIONS=suppressions=${CMAKE_SOURCE_DIR}/lsan.supp")
    endif()

    if(ENABLE_UBSAN)
        set(UBSAN_OPTIONS "UBSAN_OPTIONS=print_stacktrace=1")
    endif()

    string(CONCAT TEST_ENV "${TEST_ENV}" "${ASAN_OPTIONS};${LSAN_OPTIONS};${UBSAN_OPTIONS}")
endif()
