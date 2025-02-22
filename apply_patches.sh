#!/bin/bash

# Navigate to the submodule directory
cd genpoly-fpg || exit 1

# Store if patch was already applied
PATCH_NEEDED=true
if grep -q "cleanup()" includes/settings.h; then
    echo "Patch already applied, skipping..."
    PATCH_NEEDED=false
fi

if [ "$PATCH_NEEDED" = true ]; then
    # Apply the patch
    patch -p1 < ../patches/modifications.patch

    # Check if patch was successful
    if [ $? -ne 0 ]; then
        echo "Failed to apply patch"
        exit 1
    fi

    echo "Patch applied successfully"
fi

# Store the git commit we're on
COMMIT=$(git rev-parse HEAD)

# If this is being run during build completion
if [ "$1" = "cleanup" ]; then
    echo "Cleaning up patched files..."
    # Reset any changes in the submodule
    git reset --hard $COMMIT
    git clean -fdx
    echo "Cleanup complete"
fi