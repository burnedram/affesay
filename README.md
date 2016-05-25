# affesay
A CSGO plugin for displaying colored text. (CSay, but open source)

This plugin uses the AlliedModders CSGO SDK [mirror](https://githu mirrorb.com/alliedmodders/hl2sdk/tree/csgo)

# Compilation
##You need
* pkg-config
* protoc 2.5.0
* git submodule init && git submodule update

## protoc
A copy of the protobuf 2.5.0 source code is provided with the AlliedModders mirror, located in  
`hl2sdk-csgo/common/protobuf-2.5.0/src`

You can follow the official installation [guide](https://github.com/google/protobuf/tree/master/src#c-installation---unix), unless you're using a 64-bit distro, and want to run this 32-bit plugin on it. By default it will compile it as a 64-bit program and library, which can't be loaded into `srcds_linux`

To fix this run `CC="gcc -m32" CXX=g++ -m32" ./configure` instead of just `./configure`.  
Also, it will probably help to add `--prefix=/usr` as mentioned in the guide

## Actually compiling the plugin
`make`

# Installation
Copy the `affesay.so` into `${CSGO_DS_DIR}/csgo/addons` and create a [`affesay.vdf`](https://developer.valvesoftware.com/wiki/Server_plugins#Installing) in the same folder.
