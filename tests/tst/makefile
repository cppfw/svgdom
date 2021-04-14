include prorab.mk
include prorab-test.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs := $(call prorab-src-dir, .)

ifeq ($(os), windows)
else
    # this_ldflags += -rdynamic
endif

this_ldflags += -L $(d)../../src/out/$(c)
this_ldlibs += -ltst -lutki -lsvgdom

$(eval $(prorab-build-app))

$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libsvgdom$(dot_so))

this_test_cmd := mkdir -p out/$(c); cp /mingw64/bin/libtst.dll out/$(c); ls out/$(c); echo "" | $(prorab_this_name) --jobs=$(prorab_nproc) --junit-out=junit.xml
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c)

$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
