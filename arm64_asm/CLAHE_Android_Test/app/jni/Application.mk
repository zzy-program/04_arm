APP_PLATFORM	:= android-16
APP_MODULES	+= clahe
APP_OPTIM	:= release
APP_CFLAGS	+= -Ofast -mcpu=cortex-a8 -mfloat-abi=softfp -mthumb -mfpu=neon -march=armv7-a -fvisibility=hidden -fstack-protector-all -ftree-vectorize -fopt-info-vec-optimized
APP_ABI		:=  armeabi-v7a
APP_CPPFLAGS    += -std=c++11
APP_STL 	:= gnustl_static
