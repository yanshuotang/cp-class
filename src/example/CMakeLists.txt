FIND_PACKAGE(THREAD_DB REQUIRED)
MARK_AS_ADVANCED(
THREAD_DB_INCLUDE_DIR
THREAD_DB_LIBRARY
)
MESSAGE(STATUS ${THREAD_DB_INCLUDE_DIR}, ${THREAD_DB_LIBRARY})
SET(DIR_SRCS
    main.c)
ADD_EXECUTABLE(example ${DIR_SRCS})
TARGET_LINK_LIBRARIES(example constant common cpclass endianswap utf8tools)

INSTALL(TARGETS example RUNTIME DESTINATION bin)

