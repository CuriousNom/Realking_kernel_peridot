cmd_scripts/dtc/libfdt/fdt_strerror.o := /home/aryan/toolchains/neutron-clang/bin/clang -Wp,-MMD,scripts/dtc/libfdt/.fdt_strerror.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ../scripts/dtc/libfdt -DNO_YAML  -I ./scripts/dtc -c -o scripts/dtc/libfdt/fdt_strerror.o ../scripts/dtc/libfdt/fdt_strerror.c

source_scripts/dtc/libfdt/fdt_strerror.o := ../scripts/dtc/libfdt/fdt_strerror.c

deps_scripts/dtc/libfdt/fdt_strerror.o := \
  ../scripts/dtc/libfdt/libfdt_env.h \
  ../scripts/dtc/libfdt/fdt.h \
  ../scripts/dtc/libfdt/libfdt.h \
  ../scripts/dtc/libfdt/libfdt_internal.h \

scripts/dtc/libfdt/fdt_strerror.o: $(deps_scripts/dtc/libfdt/fdt_strerror.o)

$(deps_scripts/dtc/libfdt/fdt_strerror.o):
