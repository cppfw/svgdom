include prorab.mk

$(eval $(call prorab-config, ../config))

this_name := svgdom

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir,.)

this_ldlibs += -lcssdom -lpapki -lmikroxml -lutki -lstdc++ -lm

ifeq ($(os),linux)
    this_cxxflags += -fPIC # Since we are building shared library, we need Position-Independend Code
else ifeq ($(os),windows)
else ifeq ($(os),macosx)
    this_cxxflags += -stdlib=libc++ #this is needed to be able to use c++11 std lib
endif

$(eval $(prorab-build-lib))
