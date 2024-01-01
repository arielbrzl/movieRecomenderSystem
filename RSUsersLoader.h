
#ifndef USERFACTORY_H
#define USERFACTORY_H
#include <unordered_map>
#include "RSUser.h"

using std::unique_ptr;
using std::shared_ptr;
using std::vector;
class RecommenderSystem;
typedef std::unordered_map<sp_movie, double, hash_func, equal_func> rank_map;
class RSUsersLoader {
 private:

 public:
  RSUsersLoader () = delete;
  /**
   *
   * loads users by the given format with their movie's ranks
   * @param users_file_path a path to the file of the users and their movie
   * ranks
   * @param rs RecommendingSystem for the Users
   * @return vector of the users created according to the file
   */
  static vector<RSUser> create_users_from_file
      (const std::string &users_file_path, unique_ptr<RecommenderSystem> rs)
  noexcept (false);
};
/**
 * this function takes a line from the input file and creates an RSUser object,
 * and saves their rates
 * @param line = line of input file
 * @param shared_rs = shared_ptr of relevant RecommenderSystem
 * @param movie_vec vector of pairs of <movie name, movie year
 * @return RSUser
 */
RSUser next_user (string &line, const shared_ptr<RecommenderSystem>
&shared_rs, const vector<pair<string, int>> &movie_vec);
/**
 * this function creates a vector of <movie name, movie year> according to
 * their order at input file
 * @param line = the first line of input file
 * @param movie_vec = a pre-initialised empty vector
 */
void push_movie_to_vec (string &line, vector<pair<string, int>> &movie_vec);
/***
 * this function turns a string (containing NA or digits) to double
 * throws exceptions if the rating exceeds 1 to 10
 * @param temp string of NA or digits
 * @return double as NA represented by -10
 */
double get_rate (string &temp);

#endif //USERFACTORY_H