include prorab.mk
include prorab-test.mk

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += main.cpp

this_ldlibs += -lsvgdom -lpapki -lutki
this_ldflags += -L$(d)../../src/out/$(c)

this_no_install := true

$(eval $(prorab-build-app))

# add dependency on libsvgdom
$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libsvgdom$(dot_so))

this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c)

$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
