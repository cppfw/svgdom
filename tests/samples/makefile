include prorab.mk

$(eval $(call prorab-config, ../../config))

this_name := parsesvg

this_srcs += main.cpp

this_ldlibs += -lsvgdom -lpapki -lutki
this_ldflags += -L$(d)../../src/out/$(c)

this_no_install := true

$(eval $(prorab-build-app))

this_dirs := $(subst /, ,$(d))
this_test := $(word $(words $(this_dirs)),$(this_dirs))

this_svgs := $(patsubst $(d)%,%,$(shell ls $(d)testdata/*.svg))

define this_rules
test:: $(prorab_this_name)
$(.RECIPEPREFIX)@myci-running-test.sh $(this_test)
$(.RECIPEPREFIX)$(a)cp $(d)../../src/out/$(c)/*.dll $(d)out/$(c) || true
$(.RECIPEPREFIX)$(a)for f in $(this_svgs); do \
	    echo "checking file $$$$f" && \
	    (cd $(d) && LD_LIBRARY_PATH=../../src/out/$(c) DYLD_LIBRARY_PATH=$$$$LD_LIBRARY_PATH ./out/$(c)/parsesvg $$$$f ) && \
	    cmp $(d)out.svg $(d)$$$$f.cmp; \
	    if [ $$$$? -ne 0 ]; then myci-error.sh "parsed SVG did not match etalon sample"; exit 1; fi; \
		rm -f $(d)out.svg; \
	done
$(.RECIPEPREFIX)@myci-passed.sh
endef
$(eval $(this_rules))

# add dependency on libsvgdom
$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libsvgdom$(dot_so))

$(eval $(call prorab-include, ../../src/makefile))
