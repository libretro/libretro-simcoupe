LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_DIR := ../../src

LOCAL_MODULE    := retro-fmsx

ifeq ($(TARGET_ARCH),arm)
LOCAL_CFLAGS += -DANDROID_ARM
endif

ifeq ($(TARGET_ARCH),x86)
LOCAL_CFLAGS +=  -DANDROID_X86
endif

ifeq ($(TARGET_ARCH),mips)
LOCAL_CFLAGS += -DANDROID_MIPS -D__mips__ -D__MIPSEL__
endif

EMU = ../SimCoupe

SIMCP_SRC_FILES =   \
$(EMU)/Base/ATA.cpp\
$(EMU)/Base/AVI.cpp\
$(EMU)/Base/Action.cpp\
$(EMU)/Base/AtaAdapter.cpp\
$(EMU)/Base/Atom.cpp\
$(EMU)/Base/AtomLite.cpp\
$(EMU)/Base/BlipBuffer.cpp\
$(EMU)/Base/BlueAlpha.cpp\
$(EMU)/Base/Breakpoint.cpp\
$(EMU)/Base/CPU.cpp\
$(EMU)/Base/Clock.cpp\
$(EMU)/Base/Debug.cpp\
$(EMU)/Base/Disassem.cpp\
$(EMU)/Base/Disk.cpp\
$(EMU)/Base/Drive.cpp\
$(EMU)/Base/Expr.cpp\
$(EMU)/Base/Font.cpp\
$(EMU)/Base/Frame.cpp\
$(EMU)/Base/GIF.cpp\
$(EMU)/Base/GUI.cpp\
$(EMU)/Base/GUIDlg.cpp\
$(EMU)/Base/GUIIcons.cpp\
$(EMU)/Base/HardDisk.cpp\
$(EMU)/Base/IO.cpp\
$(EMU)/Base/Joystick.cpp\
$(EMU)/Base/Keyboard.cpp\
$(EMU)/Base/Keyin.cpp\
$(EMU)/Base/Main.cpp\
$(EMU)/Base/Memory.cpp\
$(EMU)/Base/Mouse.cpp\
$(EMU)/Base/Options.cpp\
$(EMU)/Base/PNG.cpp\
$(EMU)/Base/Parallel.cpp\
$(EMU)/Base/Paula.cpp\
$(EMU)/Base/SAA1099.cpp\
$(EMU)/Base/SAMVox.cpp\
$(EMU)/Base/SDIDE.cpp\
$(EMU)/Base/SID.cpp\
$(EMU)/Base/Screen.cpp\
$(EMU)/Base/Sound.cpp\
$(EMU)/Base/Stream.cpp\
$(EMU)/Base/Tape.cpp\
$(EMU)/Base/Util.cpp\
$(EMU)/Base/Video.cpp\
$(EMU)/Base/WAV.cpp\
$(EMU)/Base/ioapi.c\
$(EMU)/Base/unzip.c\
$(EMU)/Retro/Audio.cpp\
$(EMU)/Retro/Floppy.cpp\
$(EMU)/Retro/IDEDisk.cpp\
$(EMU)/Retro/Input.cpp\
$(EMU)/Retro/MIDI.cpp\
$(EMU)/Retro/OSD.cpp\
$(EMU)/Retro/retro.cpp\
$(EMU)/Retro/UI.cpp


BUILD_APP =  $(SIMCP_SRC_FILES) 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(EMU) $(LOCAL_PATH)/$(EMU)/.. $(LOCAL_PATH)/$(EMU)/Base\
	$(LOCAL_PATH)/$(EMU)/Retro $(LOCAL_PATH)/../libretro  


OBJECTS := $(BUILD_APP)../libretro/libretro-simcp.c ../libretro/simcp-mapper.c ../libretro/vkbd.c \
	../libretro/graph.c ../libretro/diskutils.c ../libretro/fontmsx.c  



DEFINES +=  -DUSE_ZLIB -DLSB_FIRST -DNDEBUG -D__LITTLE_ENDIAN__ 

LOCAL_SRC_FILES    += $(OBJECTS)

LOCAL_CFLAGS +=   $(DEFINES) -DRETRO=1 -DAND -D_ANDROID_ \
		  -DRETRO=1 -O3 -funroll-loops  -fsigned-char  \
	-ffast-math -fomit-frame-pointer -finline-functions -s -fPIC

LOCAL_CXXFLAGS  +=	$(CFLAGS) 
LOCAL_CPPFLAGS += $(CFLAGS)

LOCAL_LDLIBS    := -shared -lz -Wl,--version-script=../libretro/link.T 

include $(BUILD_SHARED_LIBRARY)
