
#ifndef USER_H
#define USER_H
#include "RecommenderSystem.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "Movie.h"
#define FLAG (-10)
using std::unordered_map;
using std::vector;
using std::pair;
using std::string;
using std::shared_ptr;

class RecommenderSystem;
typedef shared_ptr<Movie> sp_movie; // define your smart pointer
typedef unordered_map<sp_movie, double, hash_func, equal_func> rank_map;
class RSUser {
 public:

  RSUser (const string &username, rank_map &rank_m,
          const std::shared_ptr<RecommenderSystem> &rs);
  /**
   * a getter for the user's name
   * @return the username
   */
  string get_name () const
  { return _username; }

  /**
   * function for adding a movie to the DB
   * @param movie a Movie object
   * @param features a vector of the movie's features
   * @param rate the user rate for this movie
   * @return true for success, false otherwise
   */
  void add_movie_to_rs (const std::string &name, int year,
                        const std::vector<double> &features,
                        double rate);

  /**
   * a getter for the ranks map
   * @return
   */

  rank_map get_ranks () const
  { return rank_m; }

  /**
   * returns a recommendation according to the movie's content
   * @return recommendation
   */
  sp_movie get_recommendation_by_content () const;

  /**
   * returns a recommendation according to the similarity recommendation
   * method
   * @param k the number of the most similar movies to calculate by
   * @return recommendation
   */
  sp_movie get_recommendation_by_cf (int k) const;

  /**
   * predicts the score for a given movie
   * @param name the name of the movie
   * @param year the year the movie was created
   * @param k the parameter which represents the
   * number of the most similar movies to predict the score by
   * @return predicted score for the given movie
   */
  double get_prediction_score_for_movie (const std::string &name,
                                         int year, int k) const;

  /**
   * output stream operator
   * @param os the output stream
   * @param user the user
   * @return predicted score for movie, 0 if not exist
   */
  friend std::ostream &operator<< (std::ostream &os, RSUser &user);
 private:
  string _username;
  rank_map rank_m; //<sp_movie, rank (double)>
  std::shared_ptr<RecommenderSystem> rs;
  friend vector<double> get_features (const sp_movie &movie, const RSUser &
  user);
  friend double get_rank (const sp_movie &movie, const RSUser &user);
};
/**
 * initialises all unseen movies' rank to (FLAG = -10 == NA)
 */
void nullify_rank_map (rank_map &rank_mp,
                       const std::shared_ptr<RecommenderSystem> &rec_sys);

#endif //USER_H
