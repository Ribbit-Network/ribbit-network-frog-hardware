#!/bin/bash
# Run all the checks we care about. Minor trick though:
# run them all even if one fails, then exit with the highest exit code.
# This is so we can always see errors from all the checks.

set -euo pipefail

code=0
check() {
    local new_code
    echo ">> $@"
    set +e
    "$@"
    new_code=$?
    set -e
    if [[ $new_code -gt $code ]]; then
        code=$new_code
    fi
}

check isort --check .
check black --check .
check find . -name '*.py' -exec pylint '{}' +
check mypy .

# This is where something like `check pytest` would go

exit $code
