import 'package:json_annotation/json_annotation.dart';

import 'metadata.dart';

part 'state.g.dart';

/// List of core sytem states.
enum SystemState {
  /// The system is currently booting.
  booting,

  /// The system is curently installing an update.
  updating,

  /// The system reached an error state.
  error,

  /// The system is ready for user interaction.
  ready,
}

/// List of audio playback states.
enum PlaybackState {
  /// The system is inactive and not playing any audio.
  inactive,

  /// The system has an active audio stream, which is currently paused, so that
  /// no actual audio data is played out.
  paused,

  /// The system is currently playing an audio stream.
  ///
  /// Check [State.metadata] to obtain more detailed information in this state.
  playing,
}

/// List of bluetooth connection states
enum BluetoothState {
  /// Bluetooth is inactive and no connection exists.
  inactive,

  /// Bluetooth is in pairing mode, so that devices can connect.
  pairing,

  /// A bluetooth connection to another device is established.
  connected,
}

/// System state update message.
///
/// The message receiver must aggregate the system state to keep track of a
/// coherent state. Any field is nullable in the messages. Whenever a field has
/// a value of `null`, the system must keep the last non-`null` value in the
/// aggreated state.
@JsonSerializable(explicitToJson: true, includeIfNull: false)
class State {
  final SystemState? system;

  /// The base syste state.
  final PlaybackState? playback;

  /// Current audio volume given in percent.
  final int? volume;

  /// Current bluetooth connection state.
  final BluetoothState? bluetooth;

  /// Current playback metadata.
  final MetaData? metadata;

  /// Current playback position.
  final int? playbackPosition;

  /// Create a [State] instance with the given fields initialized.
  State({
    this.system,
    this.playback,
    this.volume,
    this.bluetooth,
    this.metadata,
    this.playbackPosition,
  });

  factory State.fromJson(Map<String, dynamic> json) => _$StateFromJson(json);
  Map<String, dynamic> toJson() => _$StateToJson(this);
}
