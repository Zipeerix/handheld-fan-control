#!/bin/sh
set -e

echo "Container's IP address: `awk 'END{print $1}' /etc/hosts`"

cd /backend

mkdir out
conan build . --build=missing
cp build/Debug/HandheldFanControlBackend out/HandheldFanControlBackend
