#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
S=build/demo-store; rm -rf "$S"; mkdir -p build
printf 'CONFIG: stable-release\n' > build/stable.txt
printf 'CONFIG: experimental-change\n' > build/experimental.txt
echo '$ snapstore init demo-store 64'; bin/snapstore init "$S" 64
echo '$ snapstore write demo-store 7 stable.txt'; bin/snapstore write "$S" 7 build/stable.txt
echo '$ snapstore snapshot demo-store stable-v1'; bin/snapstore snapshot "$S" stable-v1
echo '$ snapstore write demo-store 7 experimental.txt'; bin/snapstore write "$S" 7 build/experimental.txt
echo '$ snapstore read demo-store 7 current.out'; bin/snapstore read "$S" 7 build/current.out; cat build/current.out
echo '$ snapstore rollback demo-store stable-v1'; bin/snapstore rollback "$S" stable-v1
echo '$ snapstore read demo-store 7 restored.out'; bin/snapstore read "$S" 7 build/restored.out; cat build/restored.out
echo '$ snapstore stats demo-store'; bin/snapstore stats "$S"
echo '$ snapstore verify demo-store'; bin/snapstore verify "$S"
