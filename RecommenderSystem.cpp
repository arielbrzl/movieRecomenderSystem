#include "RecommenderSystem.h"
#include "RSUser.h"
#define NOT_FOUND 0
#define FIRST 0
#define ZERO_DIVISION "ERROR: division by zero"
#define MOVIE_NOT_FOUND "ERROR: movie is not find at pool, cant tell features"
#define MISTAKE "ERROR: this exception means that the for loop is not good"
#define INVALID_VECTOR "ERROR: cant multiply vectors of different sizes"
using std::pair;
typedef std::unordered_map<sp_movie, double, hash_func, equal_func> rank_map;
RecommenderSystem::RecommenderSystem ()
{
  pool = {};
}
sp_movie RecommenderSystem::add_movie (const std::string &name, int year,
                                       const std::vector<double> &features)
{
  sp_movie new_sp = std::make_shared<Movie> (name, year);
  pool.insert ({new_sp, features});
  return new_sp;
}
sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie to_find = std::make_shared<Movie> (name, year);
  auto it = pool.find (to_find);
  if (it == pool.end ())
    {
      return {nullptr};
    }
  return it->first;

}
std::ostream &operator<< (std::ostream &os, RecommenderSystem &rec_sys)
{
  auto iter = rec_sys.pool.begin ();
  while (iter != rec_sys.pool.end ())
    {
      os << *(iter->first);
      ++iter;
    }
  return os;
}

double get_average_score (const rank_map &normed_rank)
{
  double counter = 0, sum = 0;
  for (const auto &iter: normed_rank)
    {
      if (iter.second != FLAG)
        {
          sum += iter.second;
          counter++;
        }
    }
  return sum / counter;
}
rank_map get_normed_rank_map (RSUser &user)
{

  double average_score = get_average_score (user.get_ranks ());
  rank_map normed_rank (HASH_START, sp_movie_hash, sp_movie_equal);
  for (const auto &iter: user.get_ranks ())
    {
      double current_rank = iter.second;
      if (current_rank != FLAG)
        {
          normed_rank.insert (pair<sp_movie, double> (iter.first, iter
                                              .second- average_score));
        }
      else
        {
          normed_rank.insert (pair<sp_movie, double>
                                  (iter.first, iter.second));
        }
    }
  return normed_rank;
}

vector<double> get_preference_vector (rank_map &normed_map, size_t
num_of_features, pool_type &pool)
{
  vector<double> preference_vector;
  preference_vector.reserve (num_of_features);
  for (size_t i = 0; i < num_of_features; i++)
    {
      preference_vector.push_back (FIRST);
    }
  //normed_map(sp_movie, normed_rank)
  for (auto &normed_map_iter: normed_map)
    {
      if (normed_map_iter.second != FLAG)
        {
          for (size_t i = 0; i < num_of_features; i++)
            {
              auto pool_iter = pool.begin ();
              //pool_iter = <sp_movie, vector<double>>
              for (; pool_iter != pool.end (); ++pool_iter)
                {
                  if (pool_iter->first == normed_map_iter.first)
                    { break; }
                }
              double feature = pool_iter->second[i];
              preference_vector[i] += (normed_map_iter.second) * feature;
            }

        }
    }
  return preference_vector;
}
vector<double> get_features (const sp_movie &movie, const RSUser &user)
{

  return user.rs->pool[movie];
}
double scalar_mult (const vector<double> &a, const vector<double> &b)
{
  if (a.size () != b.size ())
    {
      throw std::runtime_error (INVALID_VECTOR);
    }
  double sum = 0;
  for (size_t i = 0; i < a.size (); i++)
    {
      sum += a[i] * b[i];
    }
  return sum;
}

double get_similarity (const vector<double> &feats, const vector<double>
&prefs)
{
  double numerator = scalar_mult (feats, prefs);
  double denominator = std::sqrt (scalar_mult (feats, feats) * scalar_mult
      (prefs, prefs));
  if (denominator == 0)
    { throw std::runtime_error (ZERO_DIVISION); }
  return numerator / denominator;
}

double get_rank (const sp_movie &movie, const RSUser &user)
{

  auto iter = user.rank_m.find (movie);
  if (iter == user.rank_m.end ())
    {
      throw std::runtime_error (MISTAKE);
    }
  return iter->second;
}

sp_movie RecommenderSystem::recommend_by_content (RSUser &user)
{
  rank_map normed_rank = get_normed_rank_map (user); //normed_map <sp
  sp_movie m = normed_rank.begin ()->first;
  size_t num_of_features = get_features (m, user).size ();
  vector<double> preference_vector = get_preference_vector (normed_rank,
                                              num_of_features, pool);
  vector<pair<sp_movie, double>> movie_similarity;

  for (auto &iter: normed_rank)
    {
      if (iter.second == FLAG)
        {
          vector<double> features = get_features (iter.first, user);
          movie_similarity.emplace_back (iter.first, get_similarity
              (features, preference_vector));
        }
    }
  pair<sp_movie, double> best = movie_similarity[FIRST];
  for (auto &i: movie_similarity)
    {
      if (i.second > best.second)
        {
          best = i;
        }
    }
  return best.first;
}
vector<pair<sp_movie, double>> get_k_closest_movies
    (const vector<double> &m_features, rank_map &watched_movies, int k,
     const RSUser &user)
{
  vector<pair<sp_movie, double>> all_movie_vec;
  vector<pair<sp_movie, double>> best_k_movies;
  for (const auto &another_m: watched_movies)
    {
      double similarity = get_similarity (get_features (another_m.first,
                                            user), m_features);
      all_movie_vec.emplace_back (another_m.first, similarity);
    }
  for (int i = 0; i < k; i++)
    {
      auto best = all_movie_vec[FIRST];
      size_t best_pos = 0, current = 0, worst_pos = 0;
      auto worst = all_movie_vec[FIRST];
      for (const auto &j: all_movie_vec)
        {
          if (j.second > best.second)
            {
              best = j;
              best_pos = current;
            }
          if (j.second < worst.second)
            {
              worst = j;
              worst_pos = current;
            }
          current++;
        }
      best_k_movies.push_back (best);
//      best.second = worst.second - 1; //make it unpickable
      all_movie_vec[best_pos].second = all_movie_vec[worst_pos].second - 1;
    }
  return best_k_movies;
}
double get_pred_rank (vector<pair<sp_movie, double>> &k_similar_movies,
                      const RSUser &user, const pair<sp_movie,
    vector<double>> &m)
{
  rank_map rank_m (user.get_ranks ());
  double denominator = 0, numerator = 0;
  for (auto &other_m: k_similar_movies)
    {
      double rank = get_rank (other_m.first, user);
      vector<double> other_m_features = get_features (other_m.first, user);
      double smj = get_similarity (other_m_features, m.second);
      numerator += smj * rank;
      denominator += smj;
    }
  if (denominator == 0)
    {
      throw std::runtime_error (ZERO_DIVISION);
    }
  return numerator / denominator;
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  rank_map rank_m (user.get_ranks ());
  rank_map watched_movies (HASH_START, sp_movie_hash, sp_movie_equal);
  pool_type unwatched_movies;
  vector<pair<sp_movie, double>> predicted_movie;
  for (const auto &iter: rank_m)
    {
      if (iter.second == FLAG)
        {
          vector<double> features = get_features (iter.first, user);
          unwatched_movies.insert ({iter.first, features});
        }
      else
        {
          watched_movies.insert ({iter.first, iter.second});
        }
    }
  for (const auto &m: unwatched_movies)
    {
      vector<pair<sp_movie, double>> k_similar_movies =
          get_k_closest_movies (m.second, watched_movies, k, user);
      double pred_rank = get_pred_rank (k_similar_movies, user, m);
      predicted_movie.emplace_back (m.first, pred_rank);
    }
  pair<sp_movie, double> best = predicted_movie[FIRST];
  for (auto &m: predicted_movie)
    {
      if (m.second > best.second)
        {
          best = m;
        }
    }
  return best.first;
}

double
RecommenderSystem::predict_movie_score (const RSUser &user, const sp_movie
&movie, int k)
{
  if (movie == nullptr)
    { return NOT_FOUND; }
  rank_map rank_m (user.get_ranks ());
  vector<double> m_features = get_features (movie, user);
  rank_map watched_movies (HASH_START, sp_movie_hash, sp_movie_equal);
  for (auto iter: rank_m) //rank_map =<sp_movie, rank) rank=10 <->NA
    {
      if (iter.second != FLAG)
        {
          watched_movies.insert (iter);
        }
    }
  vector<pair<sp_movie, double>> k_similar_movies =
      get_k_closest_movies (m_features, watched_movies, k, user);
  pair<sp_movie, vector<double>> movie_pair = {movie, m_features};
  return get_pred_rank (k_similar_movies, user, movie_pair);
}
