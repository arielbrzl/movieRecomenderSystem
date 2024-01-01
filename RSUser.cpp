
// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"
#define NOT_FOUND 0

// implement your cpp code here
void nullify_rank_map (rank_map &rank_mp,
                       const std::shared_ptr<RecommenderSystem> &rec_sys)
{
  for (auto &movie: rec_sys->pool)
    {
      if (rank_mp.find (movie.first) == rank_mp.end ())
        {
          rank_mp[movie.first] = FLAG;
        }
    }
}

RSUser::RSUser (const string &username, rank_map &rank_mp,
                const std::shared_ptr<RecommenderSystem> &rec_sys)
{
  _username = username;
  nullify_rank_map (rank_mp, rec_sys);
  this->rank_m = rank_mp;
  this->rs = rec_sys;

}
void RSUser::add_movie_to_rs
    (const std::string &name, int year,
     const std::vector<double> &features, double rate)
{
  if (rs->get_movie (name, year) == nullptr)
    {
      rs->add_movie (name, year, features);
    }
  rank_m[rs->get_movie (name, year)] = rate;
}
sp_movie RSUser::get_recommendation_by_content () const
{
  return rs->recommend_by_content ((RSUser &) *this);
}
sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return rs->recommend_by_cf (*this, k);
}
double RSUser::get_prediction_score_for_movie
    (const string &name, int year, int k) const
{
  sp_movie movie = rs->get_movie (name, year);
  if (movie == nullptr)
    { return NOT_FOUND; }

  double pred_score = rs->predict_movie_score (*this, movie, k);
  return pred_score;
}

std::ostream &operator<< (std::ostream &os, RSUser &user)
{
  os << "name: " << user._username << std::endl;
  os << *(user.rs) << std::endl;
  return os;
}

