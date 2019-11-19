# gpSP makefile
# Gilead Kutnick - Exophase
# GP2X port(ion) - Z

# Global definitions
PREFIX  = /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr
CC        = mipsel-linux-gcc
STRIP     = mipsel-linux-strip
SOUND_DRIVERS = sdl
OBJS      = main.o cpu.o memory.o video.o input.o sound.o gui.o \
	cheats.o zip.o cpu_threaded.o mips_stub.o mygui.o sha1.o
BIN       = gpsp.dge 

# Platform specific definitions 

EXTRA_OPT = -Ofast -DRS97 -mplt -mips32r2 -DZAURUS -fdata-sections -ffunction-sections  -fprofile-use \
            -mno-relax-pic-calls -mlong32 -mlocal-sdata -mno-check-zero-division -mfp32 -mgp32 -mno-embedded-data \
            -fno-pic -mno-interlink-compressed -mno-mt -mno-micromips -mno-interlink-mips16 -fdata-sections \
            -ffunction-sections -fno-math-errno -funsafe-math-optimizations -fassociative-math \
            -ffinite-math-only -fsingle-precision-constant -fsection-anchors -mno-check-zero-division



COMMON_OPT = -g
COMMON_OPT += -O0 -falign-functions=2 -falign-labels -falign-jumps -fomit-frame-pointer


VPATH      += ..
CFLAGS     += -DZAURUS $(EXTRA_OPT)
# NOTE: -funroll-loops will slow down compiling considerably
CFLAGS     += -march=mips32r2 -G0
CFLAGS     += $(COMMON_OPT)

ASFLAGS    += $(COMMON_OPT)
ASFLAGS    += -march=mips32r2

INCLUDES   = -I${PREFIX}/include -I${PREFIX}/include/SDL2
LIBS       = -lSDL -lSDL_gfx -lpthread -lz -lm

# Compilation:

.SUFFIXES: .c

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(ASFLAGS) $(INCLUDES) -c -o $@ $<

all: $(BIN)

$(BIN) : $(OBJS)
	$(CC) $(COMMON_OPT) $(OBJS) $(LIBS) -o $(BIN)  

clean:
	rm -f *.o *.u *.z $(BIN) 

strip:
	$(STRIP) $(BIN)

release: $(BIN) strip	