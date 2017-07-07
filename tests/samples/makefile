include prorab.mk

this_name := parsesvg

this_cxxflags += -Wall
this_cxxflags += -Wno-format #no warnings about format
this_cxxflags += -DDEBUG
this_cxxflags += -fstrict-aliasing #strict aliasing!!!
this_cxxflags += -std=c++11
this_cxxflags += -g

this_srcs += main.cpp

this_ldlibs += -lsvgdom -lpapki -lstdc++
this_ldflags += -L$(d)../../src

ifeq ($(os), linux)
    this_ldlibs += -lpugixml
else ifeq ($(os), macosx)
    this_cxxflags += -stdlib=libc++ #this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++ `pkg-config --libs pugixml`
else ifeq ($(os),windows)
    this_cxxflags += -I/usr/include -I/usr/local/include
    this_ldflags += -L/usr/lib -L/usr/local/lib
    this_ldlibs += -lpugixml
endif


$(eval $(prorab-build-app))


ifeq ($(os),windows)
    this_test_cmd := $(prorab_echo)(cd $(d); cp ../../src/libsvgdom.dll .; $$^)
else
    this_test_cmd := $(prorab_echo)(cd $(d); LD_LIBRARY_PATH=../../src $$^)
endif




define this_rules
test:: $(prorab_this_name)
	@echo running $$^...
	$(this_test_cmd)
endef
$(eval $(this_rules))




#add dependency on libsvgdom
$(prorab_this_name): $(abspath $(d)../../src/libsvgdom$(soext))

$(eval $(call prorab-include,$(d)../../src/makefile))
