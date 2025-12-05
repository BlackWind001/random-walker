INCLUDE_DIR="./third_party/SDL3.framework/Headers"

LINK_DIR="./third_party"

FRAMEWORK="SDL3"

RPATH_ARGS="-Wl,-rpath,@loader_path/./third_party"

EXEC_PATH="./build"

g++ -std=c++17 -v -g -o $EXEC_PATH -I$INCLUDE_DIR -F$LINK_DIR -framework $FRAMEWORK $RPATH_ARGS main.cc

$EXEC_PATH