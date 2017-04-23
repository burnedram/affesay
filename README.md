# THIS REPOSITORY IS NOT ACTIVE
The code inside is just really me testing out the possiblities with the SourceSDK, and you shouldn't think that you could use this plugin for anything.  
The original intention was to create support for showing colored messages in the players' chat, if you are looking for that functionality, please take a look at [ColorSay](https://github.com/burnedram/csgo-plugin-color-say).

# affesay
This plugin uses the AlliedModders CSGO SDK [mirror](https://github.com/alliedmodders/hl2sdk/tree/csgo)

# Compilation
##You need
* pkg-config
* protoc 2.5.0
* git submodule init && git submodule update

## protoc
A copy of the protobuf 2.5.0 source code is provided with the AlliedModders mirror, located in  
`hl2sdk-csgo/common/protobuf-2.5.0/src`  
But it seems to be missing some key compilation components (`autogen.sh`, `configure`),  
so download it from [source](https://github.com/google/protobuf/releases/tag/v2.5.0) instead

You can follow the official installation [guide](https://github.com/google/protobuf/tree/master/src#c-installation---unix), unless you're using a 64-bit distro, and want to run this 32-bit plugin on it. By default it will compile it as a 64-bit program and library, which can't be loaded into `srcds_linux`

To fix this run:
```
./configure --build=i686-pc-linux-gnu \
            --host=x86_64-pc-linux-gnu \
            "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"
```
instead of just `./configure`.  
Also, it will probably help to add `--prefix=/usr` as mentioned in the guide

## Actually compiling the plugin
`make`

# Installation
Copy the `affesay.so` into `${CSGO_DS_DIR}/csgo/addons` and create a [`affesay.vdf`](https://developer.valvesoftware.com/wiki/Server_plugins#Installing) in the same folder.
