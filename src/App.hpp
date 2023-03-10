#include "SharedState.hpp"
#include <memory>
#ifndef APP_HPP
#include "UI.hpp"
#include "playlist.hpp"

class App {
 private:
  std::shared_ptr<SharedState> state;
  UI ui;
  Playlist playlist;
 public:
  App() = delete;
  App(std::list<Song>&&, int loops=-1);
  void Run();
  void HandleKeys();

};
#endif // !APP_HPP
