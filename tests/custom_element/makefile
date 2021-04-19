include prorab.mk
include prorab-test.mk
include myci.mk

$(eval $(call myci-try-include, $(CONANBUILDINFO_DIR)conanbuildinfo.mak))

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += main.cpp

this_ldlibs += -lsvgdom -lpapki -lutki -lcssom
this_ldflags += -L$(d)../../src/out/$(c)

this_cxxflags += $(addprefix -I,$(CONAN_INCLUDE_DIRS))
this_ldflags += $(addprefix -L,$(CONAN_LIB_DIRS))

this_no_install := true

$(eval $(prorab-build-app))

# add dependency on libsvgdom
$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libsvgdom$(dot_so))

this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c) $(CONAN_LIB_DIRS)
$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
