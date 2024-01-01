
#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H

#include "RSUser.h"
#include "Movie.h"
#include <unordered_map>
using std::vector;
using std::unordered_map;
using std::pair;
using std::string;
struct my_movie_cmp {
    bool operator() (const sp_movie &a, const sp_movie &b) const
    { return (*a) < (*b); }
};
typedef std::map<sp_movie, vector<double>, my_movie_cmp> pool_type;
class RSUser;
typedef std::unordered_map<sp_movie, double, hash_func, equal_func> rank_map;

class RecommenderSystem {
 public:

  explicit RecommenderSystem ();
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie add_movie (const std::string &name, int year,
                      const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based
   * on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (RSUser &user);

  /**
   * a function that calculates the movie with highest predicted
   * score based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item
   * cf procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return predicted score for movie. 0 if not exists in RS
   */
  sp_movie get_movie (const std::string &name, int year) const;
  friend void nullify_rank_map (rank_map &rank_mp,
                                const std::shared_ptr<RecommenderSystem>
                                &rec_sys);

  friend std::ostream &operator<< (std::ostream &, RecommenderSystem &rec_sys);
 private:
  pool_type pool;
  friend vector<double> get_features (const pair<string, int> &movie_pair,
                                      const shared_ptr<RecommenderSystem>
                                      &shared_rs);
  friend vector<double> get_features (const sp_movie &movie, const RSUser &
  user);

};
/**
 * returns the average ranking of a user
 */
double get_average_score (const rank_map &normed_rank);
/**
 * returns a "normed" rank_map
 */
rank_map get_normed_rank_map (RSUser &user);
/**
 * @param normed_map
 * @param num_of_features size of the feature vector of a given rec_sys
 * @param pool - all movies and features
 * @return a preference vector for user
 */
vector<double> get_preference_vector (rank_map &normed_map, size_t
num_of_features, pool_type &pool);
/**
 * returns a movie feature
 * @param movie sp_movie
 * @param user - some user
 * @return feature vector
 */
vector<double> get_features (const sp_movie &movie, const RSUser &user);
/**
 * performs scalar multiplication for two given vectors of the same size
 */
double scalar_mult (const vector<double> &a, const vector<double> &b);
/**
 * returns similarity of two given vectors
 */
double get_similarity (const vector<double> &feats, const vector<double>
&prefs);
/**
 * returns rating of a user to a given movie
 * @param movie
 * @param user
 * @return
 */
/**
 * returns a user rank of a movie based on user ranks
 */
double get_rank (const sp_movie &movie, const RSUser &user);
/**
 * returns a vector of the k_closest movies to m
 * @param m_features  vector<double> o features of m
 * @param watched_movies map of movies watched and their scores
 * @param k number of movies to compare to
 * @param user
 */
vector<pair<sp_movie, double>> get_k_closest_movies
    (const vector<double> &m_features, rank_map &watched_movies, int k,
     const RSUser &user);
/**
 * calculates the predicted rank for a movie
 * @param k_similar_movies vector of the k most similar movies
 */
double get_pred_rank (vector<pair<sp_movie, double>> &k_similar_movies,
                      const RSUser &user, const pair<sp_movie,
    vector<double>> &m);

#endif //RECOMMENDERSYSTEM_H
