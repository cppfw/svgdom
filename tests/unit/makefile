include prorab.mk
include prorab-test.mk

$(eval $(call prorab-config, ../../config))

this_no_install := true

this_name := tests

this_srcs := $(call prorab-src-dir, .)

this_ldflags += -L ../../src/out/$(c)
this_ldlibs += -ltst -lutki -lsvgdom -lpapki

this_cxxflags += -isystem ../../src

$(eval $(prorab-build-app))

$(eval $(call prorab-depend, $(prorab_this_name), ../../src/out/$(c)/libsvgdom$(dot_so)))

this_test_cmd := $(prorab_this_name) --jobs=$(prorab_nproc) --junit-out=out/$(c)/junit.xml
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c)

$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
