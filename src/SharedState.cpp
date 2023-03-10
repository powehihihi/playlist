#include "SharedState.hpp"
#include <mutex>
#include <utility>

bool Song::operator==(const Song& s) {
  return Duration==s.Duration && Name==s.Name;
}
void SharedState::Update(const Song& s, int t) {
  std::unique_lock<std::mutex> lk(Mutex);
  if (song==s && time==t)
    return;
  song = s;
  time = t;
  lk.unlock();
  notifier.notify_all();
}

bool SharedState::Ready() {
  return ready.load();
}
void SharedState::StopPlaying() {
  ready.store(true);
  notifier.notify_one();
}

std::condition_variable& SharedState::getNotifier() {
  return notifier;
}
std::pair<Song, int> SharedState::getState() {
  std::unique_lock<std::mutex> lk(Mutex);
  return std::make_pair(song, time);
}
std::mutex& SharedState::getMutex() {
  return Mutex;
}


SharedState::SharedState(Song&& s, int time):
  song(s), time(time) {}
SharedState::SharedState(const Song& s, int time):
  song(s), time(time) {}
