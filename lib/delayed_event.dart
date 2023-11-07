import 'dart:async';

/// Generates an event with surrounding delays
class DelayedEvent<T> {
  /// The delay to wait before the event becomes valid
  final Duration before;

  /// The delay to wait after the event becomes invlaid
  final Duration after;
  final T data;

  Timer? _timer;

  DelayedEvent({
    this.before = Duration.zero,
    this.after = Duration.zero,
    required this.data,
  });

  /// Start the DelayedEvent
  ///
  /// Calls `onExpired` after the specified `delay`
  void start({
    Function(T data)? onValid,
    Function(T data)? onExpired,
  }) {
    if (_timer != null) return;
    _timer = Timer(before, () {
      if (onValid != null) {
        onValid(data);
      }
      _timer = Timer(after, () {
        _timer = null;
        if (onExpired != null) {
          onExpired(data);
        }
      });
    });
  }
}
