import 'delayed_event.dart';

/// Maintains a sequenced of `DelayedEvent`s
class EventSequence<T> {
  final List<DelayedEvent<T>> events;
  int _index;

  EventSequence(this.events) : _index = 0;

  /// Starts execute of all events in the sequence
  ///
  /// `onNext` is called whenever the next event becomes active
  void start(Function(T data) onNext) {
    if (_index >= events.length) {
      return;
    }

    events[_index++].start(
      onValid: onNext,
      onExpired: (_) => start(onNext),
    );
  }
}
