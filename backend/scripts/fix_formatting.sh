find src -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
find src -iname '*.h' -o -iname '*.tpp' | xargs clang-format -i
find test -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -i
find test -iname '*.h' -o -iname '*.tpp' | xargs clang-format -i
autopep8 -i conanfile.py
cmake-format -i CMakeLists.txt