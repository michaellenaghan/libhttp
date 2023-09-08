# 
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

#This makefile is used to test the other Makefiles

TOP = ..
TEST_OUT = test_install

include $(TOP)/resources/Makefile.in-os

all: test

test: buildoptions buildlibs buildinstall
test: buildexamples threaded

ifeq ($(TARGET_OS),OSX)
test: dmg
endif

test: clean
	@echo PASSED

buildexamples:
	@echo "================"
	$(MAKE) -C $(TOP)/examples/embedded_c clean all
	$(MAKE) -C $(TOP)/examples/embedded_c clean
	@echo "================"
	$(MAKE) -C $(TOP)/examples/chat clean all
	$(MAKE) -C $(TOP)/examples/chat clean
	@echo "================"
	$(MAKE) -C $(TOP)/examples/hello clean all
	$(MAKE) -C $(TOP)/examples/hello clean
	@echo "================"
	$(MAKE) -C $(TOP)/examples/post clean all
	$(MAKE) -C $(TOP)/examples/post clean
	@echo "================"
	$(MAKE) -C $(TOP)/examples/upload clean all
	$(MAKE) -C $(TOP)/examples/upload clean
	@echo "================"
	$(MAKE) -C $(TOP)/examples/websocket clean all
	$(MAKE) -C $(TOP)/examples/websocket clean

buildoptions:
	@echo "================"
	$(MAKE) -C $(TOP) clean build

threaded:
	@echo "================"
	$(MAKE) -j 8 -C $(TOP) clean
	$(MAKE) -j 8 -C $(TOP) build

buildinstall:
	@echo "================"
	$(MAKE) -C $(TOP) clean install PREFIX=$(TEST_OUT)

buildlibs:
	@echo "================"
	$(MAKE) -C $(TOP) clean lib
	@echo "================"
	$(MAKE) -C $(TOP) clean slib

clean:
	@echo "================"
	$(MAKE) -C $(TOP) clean
	rm -rf $(TOP)/$(TEST_OUT)

.PHONY: all buildoptions buildinstall clean os linux
