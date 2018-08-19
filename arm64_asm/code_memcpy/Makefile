TOOLCHAIN=/media/disk2/bin/android-21-arm64
SYSROOT=${TOOLCHAIN}/sysroot

CC=${TOOLCHAIN}/bin/aarch64-linux-android-gcc
CPP=${TOOLCHAIN}/bin/aarch64-linux-android-g++
AS=${TOOLCHAIN}/bin/aarch64-linux-android-as
STRIP=${TOOLCHAIN}/bin/aarch64-linux-android-strip
CFLAGS=-O3 -fPIE -pie -g3 --sysroot=${SYSROOT}

all: hello
hello.o: hello.c
	${CC} ${CFLAGS} -c -o $@ $<

memcpy.o: memcpy.S asm.h
	${CC} ${CFLAGS} -c -o $@ $<

memcpy_base.o: memcpy_base.S asm.h
	${CC} ${CFLAGS} -c -o $@ $<

memcpy_neon.o: memcpy_neon.S asm.h
	${CC} ${CFLAGS} -c -o $@ $<

hello: memcpy.o memcpy_base.o memcpy_neon.o hello.o
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -f *.o hello

.PHONY: run
run:
	adb push hello /data/data
	adb shell /data/data/hello