#ifndef GAMERULES_HPP
#define GAMERULES_HPP

///@file

///\brief
/// Class GameRules
///\details
/// Class used by MainController
class GameRules {
 private:
  int game_time = 0;
  int max_lives = 100;

 public:
  ///\brief
  ///getGameTime
  ///\details
  /// return game_time
  int getGameTime() { return game_time; }

  ///\brief
  /// setGameTime
  ///\details
  /// set game_time with parameter time
  void setGameTime(int time) { game_time = time; }

  ///\brief
  /// getMaxLives
  ///\details
  /// return max_lives
  int getMaxLives() { return max_lives; }

  ///\brief
  /// setMaxLives
  ///\details
  /// set max_lives with parameter lives
  void setMaxLives(int lives) { max_lives = lives; }
};

#endif  // GAMERULES_HPP