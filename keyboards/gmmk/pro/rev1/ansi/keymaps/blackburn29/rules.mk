CONSOLE_ENABLE = yes
NKRO_ENABLE = no           # USB Nkey Rollover
ENCODER_ENABLE = yes
RGB_MATRIX_ENABLE = yes
DEBOUNCE_TYPE = sym_defer_pk

SRC += effects/blink.c
SRC += effects/fade.c
SRC += rgb_matrix_effects.c
SRC += encoder.c
