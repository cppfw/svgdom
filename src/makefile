include prorab.mk
include prorab-license.mk
include prorab-clang-format.mk
include prorab-install-dbgsrc.mk

$(eval $(call prorab-try-simple-include, $(CONANBUILDINFO_DIR)conanbuildinfo.mak))

$(eval $(call prorab-config, ../config))

this_name := svgdom

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir,.)

this_cxxflags += $(addprefix -I,$(CONAN_INCLUDE_DIRS))
this_cxxflags_test += $(addprefix -I,$(CONAN_INCLUDE_DIRS))
this_ldflags += $(addprefix -L,$(CONAN_LIB_DIRS))

this_ldlibs += -lcssom -lpapki -lmikroxml -lutki -lstdc++ -lm

$(eval $(prorab-build-lib))

$(eval $(prorab-clang-format))

this_src_dir := $(this_name)
this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-install-dbgsrc))
