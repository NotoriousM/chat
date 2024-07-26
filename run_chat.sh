cd "$(dirname "$0")"

make

if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

if [ $# -ne 2 ]; then
    echo "Usage: $0 <IPv4 address> <port>"
    exit 1
fi

./chat "$1" "$2"
