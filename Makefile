platform = unix

ifeq ($(platform),)
platform = unix
ifeq ($(shell uname -a),)
   platform = win
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   platform = osx
else ifneq ($(findstring win,$(shell uname -a)),)
   platform = win
endif
endif

ifeq ($(platform), unix)
   CC = gcc
   TARGET := libretro-simcp.so
   fpic := -fPIC
   SHARED := -shared -Wl,--version-script=libretro/link.T -Wl,--no-undefined -fPIC
else ifeq ($(platform), osx)
   TARGET := libretro.dylib
   fpic := -fPIC
   SHARED := -dynamiclib

# Classic Platforms ####################
# Platform affix = classic_<ISA>_<µARCH>

# (armv7 a7, hard point, neon based) ### 
# NESC, SNESC, C64 mini 
else ifeq ($(platform), classic_armv7_a7)
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	CFLAGS += -Ofast \
	-flto=4 -fwhole-program -fuse-linker-plugin \
	-fdata-sections -ffunction-sections -Wl,--gc-sections \
	-fno-stack-protector -fno-ident -fomit-frame-pointer \
	-falign-functions=1 -falign-jumps=1 -falign-loops=1 \
	-fno-unwind-tables -fno-asynchronous-unwind-tables -fno-unroll-loops \
	-fmerge-all-constants -fno-math-errno \
	-marm -march=armv7ve -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard #armv7-a for older GCC i.e. 4.8
	ASFLAGS += $(CFLAGS)
	HAVE_NEON = 1
	ARCH = arm
	BUILTIN_GPU = neon
	USE_DYNAREC = 1
#######################################

else
   CC = gcc
   TARGET := retro-simcp.dll
   SHARED := -shared -static-libgcc -static-libstdc++ -s -Wl,--version-script=libretro/link.T -Wl,--no-undefined
endif

ifeq ($(DEBUG), 1)
   CFLAGS += -O0 -g
else
   CFLAGS += -O3
endif

EMU = SimCoupe

HINCLUDES := -I./$(EMU) -I./$(EMU)/.. -I./$(EMU)/Base -I./$(EMU)/Retro -Ilibretro 

SIMCP_SRC_FILES =   \
$(EMU)/Base/ATA.o\
$(EMU)/Base/AVI.o\
$(EMU)/Base/Action.o\
$(EMU)/Base/AtaAdapter.o\
$(EMU)/Base/Atom.o\
$(EMU)/Base/AtomLite.o\
$(EMU)/Base/BlipBuffer.o\
$(EMU)/Base/BlueAlpha.o\
$(EMU)/Base/Breakpoint.o\
$(EMU)/Base/CPU.o\
$(EMU)/Base/Clock.o\
$(EMU)/Base/Debug.o\
$(EMU)/Base/Disassem.o\
$(EMU)/Base/Disk.o\
$(EMU)/Base/Drive.o\
$(EMU)/Base/Expr.o\
$(EMU)/Base/Font.o\
$(EMU)/Base/Frame.o\
$(EMU)/Base/GIF.o\
$(EMU)/Base/GUI.o\
$(EMU)/Base/GUIDlg.o\
$(EMU)/Base/GUIIcons.o\
$(EMU)/Base/HardDisk.o\
$(EMU)/Base/IO.o\
$(EMU)/Base/Joystick.o\
$(EMU)/Base/Keyboard.o\
$(EMU)/Base/Keyin.o\
$(EMU)/Base/Main.o\
$(EMU)/Base/Memory.o\
$(EMU)/Base/Mouse.o\
$(EMU)/Base/Options.o\
$(EMU)/Base/PNG.o\
$(EMU)/Base/Parallel.o\
$(EMU)/Base/Paula.o\
$(EMU)/Base/SAA1099.o\
$(EMU)/Base/SAMVox.o\
$(EMU)/Base/SDIDE.o\
$(EMU)/Base/SID.o\
$(EMU)/Base/Screen.o\
$(EMU)/Base/Sound.o\
$(EMU)/Base/Stream.o\
$(EMU)/Base/Tape.o\
$(EMU)/Base/Util.o\
$(EMU)/Base/Video.o\
$(EMU)/Base/WAV.o\
$(EMU)/Base/ioapi.o\
$(EMU)/Base/unzip.o\
$(EMU)/Retro/Audio.o\
$(EMU)/Retro/Floppy.o\
$(EMU)/Retro/IDEDisk.o\
$(EMU)/Retro/Input.o\
$(EMU)/Retro/MIDI.o\
$(EMU)/Retro/OSD.o\
$(EMU)/Retro/retro.o\
$(EMU)/Retro/UI.o
#$(EMU)/Retro/SDL12.o\
#$(EMU)/Retro/SDL_GL.o\


OBJECTS :=  $(SIMCP_SRC_FILES)\
	libretro/libretro-simcp.o libretro/simcp-mapper.o libretro/vkbd.o \
	libretro/graph.o libretro/diskutils.o libretro/fontmsx.o  

DEFINES += -DUSE_ZLIB -DLSB_FIRST -DNDEBUG -D__LITTLE_ENDIAN__
CFLAGS += $(DEFINES) -DRETRO=1 -O3 -funroll-loops  -fsigned-char  \
	-ffast-math -fomit-frame-pointer -finline-functions -s -fPIC

CXXFLAGS  +=	$(CFLAGS) 

CPPFLAGS += $(CFLAGS)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "** BUILDING $(TARGET) FOR PLATFORM $(platform) **"
	$(CXX) $(fpic) $(SHARED) $(INCLUDES) -o $@ $(OBJECTS) -lm -lz -lpthread
	@echo "** BUILD SUCCESSFUL! GG NO RE **"
    	
%.o: %.c
	$(CC) $(CFLAGS) $(HINCLUDES) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(HINCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean

