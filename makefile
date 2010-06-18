# include pre-make file with a bunch of definitions
include ../pre.mk

# local CFLAGS, LIBS, and LDFLAGS
CFLAGS += --std=gnu99 -fpic -DPIC -D__LINUX__ -DX86_ASM

# list of object files to generate
OBJECTS = \
	controller.o \
  map.o \
	ppsp_input.o \

# build targets
all: ppsp_input.so

clean:
	rm -f *.o *.so

# build rules
.c.o:
	$(CXX) -o $@ $(CFLAGS) $(SDL_FLAGS) $(GTK_FLAGS) -c $<

ppsp_input.so: $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) $(ASM_LDFLAGS) $(PLUGIN_LDFLAGS) $(SDL_LIBS) -o $@
	$(STRIP) $@

controller.o: controller.c
	$(CXX) $(CFLAGS) $(SDL_FLAGS) -c -o $@ $<
map.o: map.c
	$(CXX) $(CFLAGS) $(SDL_FLAGS) -c -o $@ $<
ppsp_input.o: ppsp_input.c
	$(CXX) $(CFLAGS) $(SDL_FLAGS) -DMAINDEF -c -o $@ $<


