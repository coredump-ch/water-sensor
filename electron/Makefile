PARTICLE=particle
TARGET=electron
MINITERM=miniterm.py
DEV=/dev/ttyACM0
BAUD=9600

all: compile

compile:
	${PARTICLE} compile ${TARGET}

serial:
	${MINITERM} ${DEV} ${BAUD}

.PHONY: all compile serial
