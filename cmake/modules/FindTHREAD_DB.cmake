#
# configuration to find DB_CXX library
#

# Include dir
find_path(THREAD_DB_INCLUDE_DIR
  NAMES thread_db.h
  PATHS /usr/include/ /usr/local/include/
)

# Finally the library itself
find_library(THREAD_DB_LIBRARY
  NAMES  thread_db
  PATHS /usr/lib/ /usr/local/lib/
)


