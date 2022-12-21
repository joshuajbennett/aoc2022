add_executable(day19part1)
target_sources(day19part1 PRIVATE part1.cc)

add_executable(day19part2)
target_sources(day19part2 PRIVATE part2.cc)

add_executable(day19tests
    testmain.cc
    part1test.cc)

target_link_libraries(day19tests
    gtest
)
