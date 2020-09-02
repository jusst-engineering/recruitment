# recruitment

This package provides a WebSocket server on which a series of json encoded
events is sent out. These events describe the behavior of a virtual
mediaplayer device.

## Building

Make sure to have `dart` installed on your system. Then run
```sh
pub get
pub run build_runner build
```

## Running

Start the server process by running
```sh
dart bin/recruitment.dart
```

## Usage

Once running a WebSocket is exposed on `http://0.0.0.0:8808/ws`. As soon as a
user subscribes to the socket a pre-defined sequence of events is started and
sent out to the WebSocket.

# [License](License)

See file LICENSE for licensing information.

Created from templates made available by Stagehand under a BSD-style
[license](https://github.com/dart-lang/stagehand/blob/master/LICENSE).


## Challenge Solution Building and Running 

dart bin/recruitment.dart
git clone --recursive-submodules https://github.com/dajanto/recruitment
cd recruitment
cd boost
./sh booststrap.sh
./b2
cd ..
g++ -pthread -I ~/Downloads/recruitment/websocketpp/ -I ~/Downloads/recruitment/boost/ -I ~/Downloads/recruitment/json/include/ Solution/echo_client.cpp -o foobar
./foobar
