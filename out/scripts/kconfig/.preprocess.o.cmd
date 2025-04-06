cmd_scripts/kconfig/preprocess.o := /home/aryan/toolchains/neutron-clang/bin/clang -Wp,-MMD,scripts/kconfig/.preprocess.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11     -I ./scripts/kconfig -c -o scripts/kconfig/preprocess.o ../scripts/kconfig/preprocess.c

source_scripts/kconfig/preprocess.o := ../scripts/kconfig/preprocess.c

deps_scripts/kconfig/preprocess.o := \
  ../scripts/kconfig/list.h \
  ../scripts/kconfig/lkc.h \
    $(wildcard include/config/prefix) \
  ../scripts/kconfig/expr.h \
  ../scripts/kconfig/lkc_proto.h \

scripts/kconfig/preprocess.o: $(deps_scripts/kconfig/preprocess.o)

$(deps_scripts/kconfig/preprocess.o):
