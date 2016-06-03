GCC = c++
WARNINGS = -Wall -Werror
OPTIONS = -m32 -pipe -msse -mfpmath=sse
FLAGS = -fvisibility=hidden -fvisibility-inlines-hidden -fno-exceptions -fno-threadsafe-statics
EXTRA_FLAGS = -fno-rtti
DEFINES = -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_snprintf=snprintf \
		  -D_vsnprintf=vsnprintf -DGNUC -D_LINUX -DPOSIX -DCOMPILER_GCC
INCLUDES = -Ihl2sdk-csgo/public -Ihl2sdk-csgo/public/engine -Ihl2sdk-csgo/public/mathlib \
		   -Ihl2sdk-csgo/public/tier0 -Ihl2sdk-csgo/public/tier1 -Ihl2sdk-csgo/public/game/server

PROTOBUF_FLAGS = $(WARNINGS) $(OPTIONS) $(FLAGS)
PLUGIN_FLAGS = $(WARNINGS) $(OPTIONS) $(FLAGS) $(EXTRA_FLAGS) $(DEFINES) $(INCLUDES)

all: affesay.so

affesay.so: affesay.o affesayplugin.o recipientfilters.o netmessages.pb.o cstrike15_usermessages.pb.o libtier0.so libvstdlib.so hl2sdk-csgo/lib/linux/tier1_i486.a hl2sdk-csgo/lib/linux/interfaces_i486.a
	$(GCC) $^ $(OPTIONS) -static-libgcc -lstdc++ $(shell pkg-config --cflags --libs protobuf) -shared -o affesay.so

affesay.o: affesay.cpp affesayplugin.h recipientfilters.h cstrike15_usermessages.pb.h cplayerinfo.h
	$(GCC) $< $(PLUGIN_FLAGS) -c -o $@

%.pb.o: %.pb.cc %.pb.h
	$(GCC) $< $(PROTOBUF_FLAGS) -c -o $@

%.o: %.cpp %.h
	$(GCC) $< $(PLUGIN_FLAGS) -c -o $@

cstrike15_usermessages.pb.h cstrike15_usermessages.pb.cc: cstrike15_usermessages.proto netmessages.pb.h
	protoc --proto_path=. --proto_path=/usr/include --cpp_out=. $<

%.pb.h %.pb.cc: %.proto
	protoc --proto_path=. --proto_path=/usr/include --cpp_out=. $<

libtier0.so: hl2sdk-csgo/lib/linux/libtier0.so
	cp $^ $@

libvstdlib.so: hl2sdk-csgo/lib/linux/libvstdlib.so
	cp $^ $@

netmessages.proto: hl2sdk-csgo/public/engine/protobuf/netmessages.proto
	cp $^ $@

cstrike15_usermessages.proto: hl2sdk-csgo/public/game/shared/csgo/protobuf/cstrike15_usermessages.proto
	cp $^ $@

clean:
	rm -rf *.o *.so *.pb.h *.pb.cc *.proto
