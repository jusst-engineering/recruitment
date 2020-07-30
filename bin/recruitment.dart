import 'dart:io';
import 'dart:async';
import 'dart:convert';

import 'package:pedantic/pedantic.dart';
import 'package:recruitment/model/metadata.dart';
import 'package:recruitment/model/state.dart';
import 'package:recruitment/delayed_event.dart';
import 'package:recruitment/event_sequence.dart';

void handleMsg(msg) {
  // Don't care about incoming data
  print('< $msg');
}

Stream<String> eventGenerator() {
  var playbackSequence = EventSequence<State>(
    [
      DelayedEvent(
          before: Duration(seconds: 10),
          after: Duration(seconds: 8),
          data: State(
            playback: PlaybackState.inactive,
            metadata: MetaData(),
          )),
      DelayedEvent(
        after: Duration(minutes: 1, seconds: 10),
        data: State(
          playback: PlaybackState.playing,
          metadata: MetaData(
              title: 'Shoulders',
              artist: 'Sainvil',
              duration: Duration(minutes: 1, seconds: 10).inSeconds),
        ),
      ),
      DelayedEvent(
        after: Duration(minutes: 2, seconds: 5),
        data: State(
          playback: PlaybackState.playing,
          metadata: MetaData(
              title: 'Lose Yourself',
              artist: 'Eminem',
              duration: Duration(minutes: 2, seconds: 5).inSeconds),
        ),
      ),
      DelayedEvent(
        data: State(metadata: MetaData(), playback: PlaybackState.inactive),
      )
    ],
  );

  var positionSequence = EventSequence<State>([
    DelayedEvent(
      before: Duration(seconds: 18),
      data: State(playbackPosition: 0),
    ),
    for (int i = 0; i < 70; i++)
      DelayedEvent(
        before: Duration(seconds: 1),
        data: State(playbackPosition: i + 1),
      ),
    for (int i = 0; i < 125; i++)
      DelayedEvent(
        before: Duration(seconds: 1),
        data: State(playbackPosition: i + 1),
      ),
  ]);

  var systemSequence = EventSequence<State>(
    [
      DelayedEvent(
        after: Duration(seconds: 10),
        data: State(
          system: SystemState.booting,
          bluetooth: BluetoothState.inactive,
          volume: 50,
        ),
      ),
      DelayedEvent(
        after: Duration(seconds: 10),
        data: State(system: SystemState.ready),
      ),
      DelayedEvent(
        after: Duration(seconds: 15),
        data: State(bluetooth: BluetoothState.pairing),
      ),
      DelayedEvent(
        after: Duration(seconds: 7),
        data: State(
          system: SystemState.error,
          bluetooth: BluetoothState.inactive,
        ),
      ),
      DelayedEvent(
        after: Duration(minutes: 10),
        data: State(
          system: SystemState.ready,
        ),
      ),
    ],
  );

  var volumeSequence = EventSequence<State>([
    DelayedEvent(before: Duration(seconds: 25), data: State(volume: 51)),
    DelayedEvent(before: Duration(milliseconds: 300), data: State(volume: 52)),
    DelayedEvent(before: Duration(milliseconds: 300), data: State(volume: 53)),
    DelayedEvent(before: Duration(milliseconds: 300), data: State(volume: 54)),
    DelayedEvent(before: Duration(milliseconds: 300), data: State(volume: 55)),
  ]);

  StreamController<String> controller;

  void sendState(State state) {
    controller.add(json.encode(state));
  }

  void startSequence() {
    systemSequence.start(sendState);
    playbackSequence.start(sendState);
    positionSequence.start(sendState);
    volumeSequence.start(sendState);
  }

  void stopSequence() {
    // FIXME: Interrupt EventSequence?
  }

  controller = StreamController(
    onListen: startSequence,
    onPause: stopSequence,
    onResume: startSequence,
    onCancel: stopSequence,
  );

  return controller.stream;
}

void main(List<String> arguments) {
  print('Welcome to the jusst.challenge');

  runZoned(() async {
    var server = await HttpServer.bind('0.0.0.0', 8808);
    await for (var req in server) {
      if (req.uri.path == '/ws') {
        // Create HttpSocket from http request
        var socket = await WebSocketTransformer.upgrade(req);
        socket.listen(handleMsg);
        unawaited(socket.addStream(eventGenerator()));
      }
    }
  }, onError: (err) => print('Error occured: $err'));
}
