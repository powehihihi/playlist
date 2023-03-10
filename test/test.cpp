//#ifdef TEST
#include <bits/chrono.h>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include "../src/SharedState.hpp"
#include "../src/playlist.hpp"
#include <iostream>


#define DEFAULT_START(list, loopnum) \
  auto tmplist=list; \
  auto ptr = std::make_shared<SharedState>(); \
  Playlist playlist(std::move(tmplist),ptr, loopnum); \
  playlist.Play();

#define DEFAULT_END() \
  while (!ptr->Ready()){};

std::list<Song> list1 { Song{"347aidan - Demons and Monsters", 198}, 
                        Song{"88rising, BIBI, 347aidan - The Weekend", 168},
                        Song{"Red Velvet - Russian Roulette", 209}};

std::list<Song> list2 { Song{"short1", 3},
                        Song{"short2", 4},
                        Song{"short3", 1},
                        Song{"short4", 1} };

std::list<Song> emptylist;


auto Stopwatch(std::function<void(void)> f) {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  f();
  auto end = high_resolution_clock::now();
  return duration_cast<seconds>(end-start).count();
}

void test1() {
  DEFAULT_START(list2, 1);
  DEFAULT_END();
}

void test2() {
  DEFAULT_START(list2, 2);
  DEFAULT_END();
}

void test11() {
  DEFAULT_START(list2, 3);
  DEFAULT_END();
}
void test18() {
  DEFAULT_START(emptylist, 1);
  DEFAULT_END();
}

TEST_CASE("Duration") {
  
  REQUIRE( Stopwatch(test18) ==0);
  REQUIRE( Stopwatch(test1) == 3+4+1+1 );
  REQUIRE( Stopwatch(test2) == (3+4+1+1)*2 );
  REQUIRE( Stopwatch(test11) == (3+4+1+1)*3 );


}


void test3() {
  DEFAULT_START(list2, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  playlist.Pause();
  std::this_thread::sleep_for(std::chrono::seconds(6));
  playlist.Play();
  DEFAULT_END();
}
void test4() {
  DEFAULT_START(list2, 2);
  playlist.Play();
  std::this_thread::sleep_for(std::chrono::milliseconds(10500));
  playlist.Pause();
  std::this_thread::sleep_for(std::chrono::seconds(6));
  playlist.Play();
  DEFAULT_END();
}
auto test12() {
  DEFAULT_START(list2, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(7500));
  playlist.Pause();
  auto res12 = ptr->getState();
  playlist.Play();
  DEFAULT_END();
  return res12.first.Name;
}

TEST_CASE("Pause") {
  REQUIRE( Stopwatch(test3) == 3+4+1+1 + 6);
  REQUIRE( Stopwatch(test4) == (3+4+1+1)*2 + 6);
  REQUIRE( test12() == "short3");
}


void test5() {
  auto tmplist=list2;
  auto ptr = std::make_shared<SharedState>();
  Playlist playlist(std::move(tmplist), ptr, 1);
  playlist.Next();
  playlist.Play();
  DEFAULT_END();
}
void test6() {
  auto tmplist=list2;
  auto ptr = std::make_shared<SharedState>();
  Playlist playlist(std::move(tmplist), ptr, 2);
  playlist.Next();
  playlist.Next();
  playlist.Play();
  DEFAULT_END();
}
void test7() {
  auto tmplist=list2;
  auto ptr = std::make_shared<SharedState>();
  Playlist playlist(std::move(tmplist), ptr, 1);
  playlist.Next();
  playlist.Next();
  playlist.Next();
  playlist.Next();
  playlist.Play();
  DEFAULT_END();
}

auto test16() {
  DEFAULT_START(list2, 1);
  playlist.Next();
  playlist.Next();
  std::this_thread::sleep_for(std::chrono::milliseconds(5)); // ждём когда выполнится запрос....
                                                             // это необходимый костыль для некоторых тестов
                                                             // в приложении UI уведомляется при каждом апдейте
  auto res = ptr->getState().first.Name;
  DEFAULT_END();
  return res;
}

auto test17() {
  auto tmplist=list2;
  auto ptr = std::make_shared<SharedState>();
  Playlist playlist(std::move(tmplist), ptr, 1);
  playlist.Next();
  playlist.Next();
  std::this_thread::sleep_for(std::chrono::milliseconds(5)); // костыль
  auto res = ptr->getState().first.Name;
  DEFAULT_END();
  return res;
}

TEST_CASE("Next") {
  REQUIRE( Stopwatch(test5) == 3+4+1+1 - 3 );
  REQUIRE( Stopwatch(test6) == (3+4+1+1)*2 -3 -4 );
  REQUIRE( Stopwatch(test7) == 0 ); // skipped whole loop
  REQUIRE( test16() == "short3");
  REQUIRE( test17() == "short3");
}

void test8() {
  DEFAULT_START(list2, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(4200));
  playlist.Prev();
  DEFAULT_END();
}
auto test13() {
  DEFAULT_START(list2, 1);
  std::this_thread::sleep_for(std::chrono::seconds(4));
  playlist.Prev();
  std::this_thread::sleep_for(std::chrono::milliseconds(5)); // костыль конечно....
  auto res = ptr->getState().first.Name;
  DEFAULT_END();
  return res;
}

void test14() {
  DEFAULT_START(list2, 1);
  playlist.Prev();
  playlist.Prev();
  playlist.Prev();
  playlist.Prev();
  playlist.Prev();
  DEFAULT_END();
}
TEST_CASE("Prev") {
  REQUIRE( Stopwatch(test8) == 3+4+1+1 +1+3 );
  REQUIRE( test13() == "short1" );
  REQUIRE( Stopwatch(test14) == 3+4+1+1 );
}


void test9() {
  DEFAULT_START(list2, 1);
  playlist.AddSong({"longer", 7});
  DEFAULT_END();
}
void test10() {
  DEFAULT_START(list2, 2);
  playlist.AddSong({"longer", 7});
  DEFAULT_END();
}
auto test15() {
  DEFAULT_START(list2, 1);
  playlist.AddSong({"longer", 7});
  std::this_thread::sleep_for(std::chrono::seconds(3+4+1+1 +1));
  auto res = ptr->getState().first.Name;
  DEFAULT_END();
  return res;
}
TEST_CASE("New") {
  REQUIRE( Stopwatch(test9) == 3+4+1+1 +7 );
  REQUIRE( Stopwatch(test10) == (3+4+1+1 +7)*2 );
  REQUIRE( test15() == "longer" );
}
