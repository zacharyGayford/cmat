# ----------------------------
# Makefile Options
# ----------------------------

NAME = CMAT
ICON = icon.png
DESCRIPTION = "complex matrix equation solver"
COMPRESSED = YES
HAS_PRINTF = NO

CFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
