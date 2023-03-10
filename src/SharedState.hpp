#ifndef SHARED_STATE_HPP
#define SHARED_STATE_HPP

#include <atomic>
#include <condition_variable>
#include <future>

struct Song {
  std::string Name;
  int Duration = 0;

  Song() = default;
  Song(const std::string& name, int dur):
    Name(name), Duration(dur) {}
  Song(std::string&& name, int dur):
    Name(name), Duration(dur) {}
  bool operator==(const Song&);
 // Song& operator=(const Song&);
};

class SharedState {
 private:
  //bool ready = false;
  std::atomic<bool> ready;
  Song song;
  int time;
  std::mutex Mutex;
  std::condition_variable notifier;
 public:
  //std::future<void> waitForTheEnd;
  
  SharedState() = default;
  SharedState(Song&& s, int time);
  SharedState(const Song& s, int time);
  void Update(const Song& s, int t);

  std::pair<Song, int> getState();
  std::mutex& getMutex();
  std::condition_variable& getNotifier();
  bool Ready();
  void StopPlaying();
};

#endif
