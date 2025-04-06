cmd_scripts/dtc/data.o := /home/aryan/toolchains/neutron-clang/bin/clang -Wp,-MMD,scripts/dtc/.data.o.d -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu11   -I ../scripts/dtc/libfdt -DNO_YAML  -I ./scripts/dtc -c -o scripts/dtc/data.o ../scripts/dtc/data.c

source_scripts/dtc/data.o := ../scripts/dtc/data.c

deps_scripts/dtc/data.o := \
  ../scripts/dtc/dtc.h \
  ../scripts/dtc/libfdt/libfdt_env.h \
  ../scripts/dtc/libfdt/fdt.h \
  ../scripts/dtc/util.h \

scripts/dtc/data.o: $(deps_scripts/dtc/data.o)

$(deps_scripts/dtc/data.o):
