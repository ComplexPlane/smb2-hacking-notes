#!/bin/bash

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

echo "Checking dependencies"
command -v cargo >/dev/null 2>&1 || { echo >&2 "Could not find 'cargo' in PATH. See https://crates.io/ for more info."; exit 1; }

echo "Building and running void_builder"
exec cargo run --manifest-path="$DIR/void_builder/Cargo.toml"
