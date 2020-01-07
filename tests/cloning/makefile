include prorab.mk

this_name := tests

this_cxxflags += -Wall
this_cxxflags += -Werror
this_cxxflags += -DDEBUG
this_cxxflags += -fstrict-aliasing #strict aliasing!!!
this_cxxflags += -std=c++11
this_cxxflags += -g

this_srcs += main.cpp

this_ldlibs += -lsvgdom -lpapki -lstdc++
this_ldflags += -L$(d)../../src

ifeq ($(os), linux)
    this_cxxflags += -fPIC
    this_ldlibs +=
else ifeq ($(os), macosx)
    this_cxxflags += -stdlib=libc++ #this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
else ifeq ($(os),windows)
endif

this_no_install := true

$(eval $(prorab-build-app))

this_dirs := $(subst /, ,$(d))
this_test := $(word $(words $(this_dirs)),$(this_dirs))

define this_rules
test:: $(prorab_this_name)
	@myci-running-test.sh $(this_test)
	$(prorab_echo)cp $(d)../../src/*.dll $(d) || true
	$(prorab_echo)LD_LIBRARY_PATH=$(d)../../src DYLD_LIBRARY_PATH=$$$$LD_LIBRARY_PATH $(d)./tests; \
		if [ $$$$? -ne 0 ]; then myci-error.sh "test failed"; exit 1; fi
	@myci-passed.sh
endef
$(eval $(this_rules))




#add dependency on libsvgdom
$(prorab_this_name): $(abspath $(d)../../src/libsvgdom$(soext))

$(eval $(call prorab-include, ../../src/makefile))
