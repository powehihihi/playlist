#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <condition_variable>
#include <memory>
#include <thread>
#include <mutex>
#include <list>
#include <iterator>
#include <chrono>
#include <queue>
#include <future>

#include "SharedState.hpp"

class Playlist {
 private:
  enum class task {
    pause,
    play,
    next,
    prev,
    stop
  };
  // Data
  std::list<Song> playlist;
  std::list<Song>::iterator curSong;
  int curTime = 0;
  std::queue<task> Tasks;
  // Shared state (for UI/tests/server/IO etc)
  std::shared_ptr<SharedState> state;

  // Concurrency
  std::thread t;                // background thread for emulating music
  std::mutex protectData;
  std::condition_variable cv;   // t gets notified for every signal
  
 // std::promise<void> Promise;   // future from Promise notifies when the playing is over

  int loopNumber = -1;          // -1 is infinite looping
  
 public:
  Playlist() = delete;
  Playlist(std::list<Song>&& playlist,
           std::shared_ptr<SharedState> ptr, 
           int loopNum=-1);

  ~Playlist();                  // joins thread

  // interface 
  void Play();
  void Pause();
  void AddSong(const Song& s);
  void AddSong(Song&& s);
  void Next();
  void Prev();

  // other
  void SetLoopNumber(int x);
  //std::future<void> getFutureToWaitForTheEnd();
 private:
  void StopAndClosePlaylist(); // called by ~Playlist
  void EmulatePlay();
};

#endif
