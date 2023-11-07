import 'package:json_annotation/json_annotation.dart';

part 'metadata.g.dart';

@JsonSerializable(includeIfNull: false)
class MetaData {
  final String? title;
  final String? artist;
  final String? coverArt;
  final int? duration;

  MetaData({
    this.title,
    this.artist,
    this.coverArt,
    this.duration,
  });

  factory MetaData.fromJson(Map<String, dynamic> json) =>
      _$MetaDataFromJson(json);
  Map<String, dynamic> toJson() => _$MetaDataToJson(this);
}
