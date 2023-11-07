import 'package:json_annotation/json_annotation.dart';

import 'metadata.dart';

part 'state.g.dart';

enum SystemState { booting, updating, error, ready }

enum PlaybackState { inactive, paused, playing }

enum BluetoothState { inactive, pairing, connected }

@JsonSerializable(explicitToJson: true, includeIfNull: false)
class State {
  final SystemState? system;
  final PlaybackState? playback;
  final int? volume;
  final BluetoothState? bluetooth;
  final MetaData? metadata;
  final int? playbackPosition;

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
