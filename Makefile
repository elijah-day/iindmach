obj_files = \
	src/iind/iind_controls.o \
	src/iind/iind_edit.o \
	src/iind/iind_gui.o \
	src/iind/iind_init.o \
	src/iind/iind_load.o \
	src/iind/iind_render.o \
	src/iind/iind_run.o \
	src/iind/iind_world.o \
	src/main.o

build_dir = build
target_dir = build/iindmach

OS ?= linux
CFLAGS = -w

ifeq ($(OS), win32)
	CC = i686-w64-mingw32-gcc
	LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	CFLAGS += \
		-w -Wl,-subsystem,windows -m32 -mwindows \
		-I external/SDL2-2.0.22/i686-w64-mingw32/include \
		-I external/SDL2_image-2.0.5/i686-w64-mingw32/include \
		-I external/SDL2_ttf-2.0.15/i686-w64-mingw32/include \
		-I external/SDL2-2.0.22/i686-w64-mingw32/include/SDL2 \
		-I external/SDL2_image-2.0.5/i686-w64-mingw32/include/SDL2 \
		-I external/SDL2_ttf-2.0.15/i686-w64-mingw32/include/SDL2 \
		-L external/SDL2-2.0.22/i686-w64-mingw32/lib \
		-L external/SDL2_image-2.0.5/i686-w64-mingw32/lib \
		-L external/SDL2_ttf-2.0.15/i686-w64-mingw32/lib
	TARGET = $(target_dir)/iindmach.exe
	run_method = wine64 iindmach.exe
	cp_bin_method = \
		cp external/SDL2-2.0.22/i686-w64-mingw32/bin/SDL2.dll \
		$(target_dir); \
		cp external/SDL2_image-2.0.5/i686-w64-mingw32/bin/SDL2_image.dll \
		$(target_dir); \
		cp external/SDL2_image-2.0.5/i686-w64-mingw32/bin/zlib1.dll \
		$(target_dir); \
		cp external/SDL2_image-2.0.5/i686-w64-mingw32/bin/libpng16-16.dll \
		$(target_dir); \
		cp external/SDL2_ttf-2.0.15/i686-w64-mingw32/bin/libfreetype-6.dll \
		$(target_dir); \
		cp external/SDL2_ttf-2.0.15/i686-w64-mingw32/bin/SDL2_ttf.dll \
		$(target_dir); \

else ifeq ($(OS), win64)
	CC = x86_64-w64-mingw32-gcc
	LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	CFLAGS += \
		-w -Wl,-subsystem,windows -m64 -mwindows \
		-I external/SDL2-2.0.22/x86_64-w64-mingw32/include \
		-I external/SDL2_image-2.0.5/x86_64-w64-mingw32/include \
		-I external/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include \
		-I external/SDL2-2.0.22/x86_64-w64-mingw32/include/SDL2 \
		-I external/SDL2_image-2.0.5/x86_64-w64-mingw32/include/SDL2 \
		-I external/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include/SDL2 \
		-L external/SDL2-2.0.22/x86_64-w64-mingw32/lib \
		-L external/SDL2_image-2.0.5/x86_64-w64-mingw32/lib \
		-L external/SDL2_ttf-2.0.15/x86_64-w64-mingw32/lib
	TARGET = $(target_dir)/iindmach.exe
	run_method = wine64 iindmach.exe
	cp_bin_method = \
		cp external/SDL2-2.0.22/x86_64-w64-mingw32/bin/SDL2.dll \
		$(target_dir); \
		cp external/SDL2_image-2.0.5/x86_64-w64-mingw32/bin/SDL2_image.dll \
		$(target_dir); \
		cp external/SDL2_image-2.0.5/x86_64-w64-mingw32/bin/zlib1.dll \
		$(target_dir); \
		cp external/SDL2_image-2.0.5/x86_64-w64-mingw32/bin/libpng16-16.dll \
		$(target_dir); \
		cp external/SDL2_ttf-2.0.15/x86_64-w64-mingw32/bin/libfreetype-6.dll \
		$(target_dir); \
		cp external/SDL2_ttf-2.0.15/x86_64-w64-mingw32/bin/SDL2_ttf.dll \
		$(target_dir); \

else ifeq ($(OS), linux)
	CC = gcc
	CFLAGS +=
	LIBS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf
	TARGET = $(target_dir)/iindmach
	run_method = ./iindmach

endif

all: build run

build: $(obj_files)
	mkdir -p $(target_dir)
	$(CC) $(obj_files) $(CFLAGS) $(LIBS) -o $(TARGET)
	$(cp_bin_method)
	make cp-data
	make cp-docs

cp-data:
	-rm -rf $(target_dir)/assets
	-rm -rf $(target_dir)/modules
	cp -r data/assets $(target_dir)
	cp -r data/modules $(target_dir)
	cp data/dialogue.list $(target_dir)

cp-docs:
	-rm -rf $(target_dir)/docs
	-rm $(target_dir)/README.md
	-rm $(target_dir)/LICENSE
	cp -r docs $(target_dir)
	cp README.md $(target_dir)
	cp LICENSE $(target_dir)

run:
	cd build/iindmach; $(run_method); cd ../../

clean:
	rm -f $(obj_files)

destroy: clean
	rm -rf $(build_dir)
