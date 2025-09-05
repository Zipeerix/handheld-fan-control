#!/bin/sh
set -e

echo "Container's IP address: `awk 'END{print $1}' /etc/hosts`"

cd /backend

# TODO: Here do conan build . --build=missing & copy binary to /backend/out