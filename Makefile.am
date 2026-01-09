AM_CPPFLAGS = -I $(srcdir)/include

AM_CFLAGS = -O3 -g0 -Wall

ARFLAGS = cr

lib_LIBRARIES = lib/libllist.a
lib_libllist_a_SOURCES = src/llist.c include/llist.h

bin_PROGRAMS = bin/test-llist
bin_test_llist_SOURCES = src/test-llist.c
bin_test_llist_LDADD = lib/libllist.a

include_HEADERS = include/llist.h

EXTRA_DIST = windows acdoxygen.m4 amdoxygen.am doxygen.llist.cfg llist.pc .gitignore

super-clean: clean distclean
	@find . -name 'Makefile.in' -delete
	@find . -name 'config.h.in' -delete
	@find . -name '*.tar' -delete
	@find . -name '*.tar.*' -delete
	@find . -name '*~' -delete
	@find . -name '*.obj' -delete
	@find . -name '*.lib' -delete
	@find . -name '*.exe' -delete
	@rm -rf autom4te.cache
	@rm -f aclocal.m4
	@rm -f configure
	@rm -rf src/.deps
	@rm -f texput.log

include amdoxygen.am

MOSTLYCLEANFILES = $(DX_CLEANFILES)

EXTRA_DIST += $(DX_CONFIG)

install-data-hook:
	@if test -d $(DX_HTML_DIR); \
	 then \
	   echo installing $(DX_HTML_DIR) to $(prefix)/share/doc/llist; \
	   mkdir -p $(prefix)/share/doc/llist; \
	   cp -r $(DX_HTML_DIR) $(prefix)/share/doc/llist; \
	 fi
	@echo installing llist.pc
	@mkdir -p $(prefix)/share/pkgconfig
	@cp llist.pc $(prefix)/share/pkgconfig/llist.pc

uninstall-hook:
	@rm -rf $(prefix)/share/doc/llist
	@rm -rf $(prefix)/share/pkgconfig/llist.pc

code-stats:
	@echo -n " total lines of source code: "; wc -l src/*.[ch] include/*.[ch] | tail -1 | sed -e 's/ total//' -e 's/^  *//'
	@echo -n "actual lines of source code: "; (gcc -fpreprocessed -dD -E src/*.[ch] include/*.[ch] | sed -e '/^$$/d' -e '/^# [0-9]/d' | wc -l)

