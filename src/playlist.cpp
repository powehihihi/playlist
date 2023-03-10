#include "playlist.hpp"
#include <chrono>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <iostream>


Playlist::Playlist(std::list<Song>&& tmpplaylist,
                   std::shared_ptr<SharedState> ptr, 
                   int loopNumber): playlist(tmpplaylist),
                                    curSong(playlist.begin()),
                                    state(ptr),
                                    t(&Playlist::EmulatePlay, this),
                                    loopNumber(loopNumber) {
//                                      state->waitForTheEnd = Promise.get_future();
                                      Song tmp = playlist.empty() ? Song("", 0): *playlist.begin();
                                      state->Update(tmp, 0);
                                    }


Playlist::~Playlist() {
  StopAndClosePlaylist();
}

void Playlist::StopAndClosePlaylist() {
  std::unique_lock<std::mutex> lk(protectData);
  Tasks.push(task::stop);
  lk.unlock();
  cv.notify_one();
  t.join();
}

void Playlist::Play() {
  std::unique_lock<std::mutex> lk(protectData);
  Tasks.push(task::play);
  lk.unlock();
  cv.notify_one();
}
void Playlist::Pause() {
  std::unique_lock<std::mutex> lk(protectData);
  Tasks.push(task::pause);
  lk.unlock();
  cv.notify_one();
}
void Playlist::Next() {
  std::unique_lock<std::mutex> lk(protectData);
  Tasks.push(task::next);
  lk.unlock();
  cv.notify_one();
}
void Playlist::Prev() {
  std::unique_lock<std::mutex> lk(protectData);
  Tasks.push(task::prev);
  lk.unlock();
  cv.notify_one();
}

void Playlist::EmulatePlay() {
  bool prev = false;
  bool isPause = true;
  //Promise.set_value_at_thread_exit();
  std::unique_lock<std::mutex> lk(protectData);
  while (loopNumber!=0) {
    //isPause = true; // well it could be an optional feature but code is already bloated...
    for (curSong=playlist.begin() ; curSong!=playlist.end(); ) {
      for (curTime=0; curTime<curSong->Duration; ) {
        // basic idea: we start "playing" (waiting for a second) only when unpaused and taskless
        if (isPause && Tasks.empty()) {
          cv.wait(lk, [&]() {
                return !Tasks.empty();
              });
        }
        else if (Tasks.empty()) {
          state->Update(*curSong, curTime);
          #ifdef TEST
          auto start = std::chrono::high_resolution_clock::now();
          #endif // TEST
          auto status = cv.wait_for(lk, std::chrono::seconds(1), [&]() {
                return !Tasks.empty() && Tasks.front()!=task::play;
              });
          if (status==false) {
            #ifdef TEST
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count() << std::endl;
            std::cout << curSong->Name << ' ' << curSong->Duration << ' ' << curTime << std::endl;
            #endif
            // music played for a second ==> increase time
            ++curTime;
            continue;
          }
            #ifdef TEST
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count() << std::endl;
            std::cout << "Interruption: " <<curSong->Name << ' ' << curSong->Duration << ' ' << curTime << std::endl;
            #endif
          
        }
        else {
          auto Task = Tasks.front();
          Tasks.pop();
          if (Task==task::pause) {
            isPause = true;
          }
          else {
            isPause = false;
            if (Task==task::next)
              break;                    // instanttly jump on next iteration of curSong for-loop
            if (Task==task::prev && curSong!=playlist.begin()) {
              prev = true;              // notice that curSong for-loop doesn't have an increment
              break;
            }
            if (Task==task::stop) {
              state->StopPlaying();
              return;
            }
          }
        }
      }
      if (prev) {                         // instead of increment in curSong for-loop
        prev=false;
        --curSong;
      }
      else
        ++curSong;
    }
    if (loopNumber>0)                   // can't use for-loop because loopNumber==-1 is infinity 
      --loopNumber;
  }
  state->StopPlaying();
}
void Playlist::SetLoopNumber(int x) {
  loopNumber = x;
}

void Playlist::AddSong(const Song& s) {
  std::unique_lock<std::mutex> lk(protectData);
  playlist.push_back(s);
}
void Playlist::AddSong(Song&& s) {
  std::unique_lock<std::mutex> lk(protectData);
  playlist.push_back(s);
  // to do smth else ?
}
