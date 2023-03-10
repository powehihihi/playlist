#include "App.hpp"
#include "playlist.hpp"
#include <curses.h>
#include <mutex>
#include <thread>

App::App(std::list<Song>&& List, int loops): 
  state(std::make_shared<SharedState>(*List.begin(), 0)),
  ui(),
  playlist(std::move(List), state, loops) {
  }
  
void App::Run() {
  std::thread keysThread(&App::HandleKeys, this);
  auto& Mutex = state->getMutex();
  auto& notifier = state->getNotifier();
  while (!state->Ready()) {
    auto newstate = state->getState();
    ui.Update(newstate.first, newstate.second);
    std::unique_lock<std::mutex> lk(Mutex);
    notifier.wait(lk);
    lk.unlock();
  }
  keysThread.join();
}

void App::HandleKeys() {
  while (!state->Ready()) {
    int key = getch();
    switch(key) {
      case 'h':
        playlist.Prev();
        break;
      case 'l':
        playlist.Next();
        break;
      case ' ':
        playlist.Pause();
        break;
      default:
        playlist.Play();
    }
  }
}
