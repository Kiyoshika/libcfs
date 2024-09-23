add_library(cfs 
    directory.c
    path.c
    result.c
)
target_include_directories(cfs PUBLIC ${PROJECT_SOURCE_DIR}/include)
