DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR=$(dirname "$(dirname "${DIR}")")

mkdir -p build

alias make_clean="rm build/* -r"

alias make="cmake -S . -B build; cmake --build build"