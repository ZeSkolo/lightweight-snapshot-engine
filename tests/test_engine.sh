#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
S=build/test-store; rm -rf "$S"; mkdir -p build
printf 'version-one\n' > build/in1.txt
printf 'version-two\n' > build/in2.txt
bin/snapstore init "$S" 16
bin/snapstore write "$S" 3 build/in1.txt
bin/snapstore snapshot "$S" baseline
bin/snapstore write "$S" 3 build/in2.txt
bin/snapstore read "$S" 3 build/current.out
cmp build/in2.txt build/current.out
bin/snapstore rollback "$S" baseline
bin/snapstore read "$S" 3 build/rollback.out
cmp build/in1.txt build/rollback.out
bin/snapstore verify "$S"
bin/snapstore delete-snapshot "$S" baseline
[[ "$(bin/snapstore list "$S")" == *"Snapshots (0)"* ]]
echo "ALL TESTS PASSED"
