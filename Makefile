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

CC = gcc
CFLAGS = -Wall -Werror -W -pedantic-errors -w -g
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
TARGET = $(target_dir)/iindmach

all: build run

build: $(obj_files)
	-mkdir -p $(build_dir)
	-mkdir -p $(target_dir)
	$(CC) $(obj_files) $(CFLAGS) $(LIBS) -o $(TARGET)
	rm -rf $(target_dir)/assets
	rm -rf $(target_dir)/modules
	cp -r data/assets $(target_dir)
	cp -r data/modules $(target_dir)
	cp data/dialogue.list $(target_dir)

clean:
	rm -f $(obj_files)

run:
	cd build/iindmach; ./iindmach; cd ../../

destroy:
	rm -rf $(target_dir)
