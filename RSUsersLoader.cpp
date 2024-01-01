#include "RSUsersLoader.h"
#define PATH_ERROR "ERROR:this file can't open"
#define INVALID_RANK "ERROR: rank must be between 1 to 10"
#define MOVIE_NOT_FOUND "ERROR: movie is not find at pool, cant tell features"
#define MISTAKE "this exception means that the for loop is not good"
#define FLAG (-10)
#define UPPER_BOUND 10
#define LOWER_BOUND 1
using std::string;

void push_movie_to_vec (string &line, vector<pair<string, int>> &movie_vec)
{
  string name, temp;
  int year;
  for (char i: line)
    {
      if (temp == " ")
        {
          temp = "";
        }
      if (i == '-')
        {
          name = temp;
          temp = "";
        }
      if (i == ' ')
        {
          year = stoi (temp);
          temp = "";
          movie_vec.emplace_back (name, year);
        }
      if (temp != " " && i != ' ' && i != '-')
        { temp += i; }
    }
  year = stoi (temp);
  movie_vec.emplace_back (name, year);
}

vector<double> get_features (const pair<string, int> &movie_pair,
                             const shared_ptr<RecommenderSystem> &shared_rs)
{
  sp_movie to_find = shared_rs->get_movie
      (movie_pair.first, movie_pair.second);
  if (to_find == nullptr)
    {
      throw std::runtime_error (MOVIE_NOT_FOUND);
    }
  auto it = shared_rs->pool.begin ();
  for (; it != shared_rs->pool.end (); ++it)
    {
      if (it->first == to_find)
        {
          return it->second;
        }
    }
  throw std::runtime_error (MISTAKE); //shouldnt happen
}

double get_rate (string &temp)
{
  if (temp == "NA" || temp == "NA\r")
    { return FLAG; }
  else
    {
      double rank = std::stod (temp);
      if (rank < LOWER_BOUND || rank > UPPER_BOUND)
        { throw std::length_error (INVALID_RANK); }
      return rank;
    }
}

RSUser next_user (string &line, const shared_ptr<RecommenderSystem>
&shared_rs,
                  const vector<pair<string, int>> &movie_vec)
{
  string user_name, temp;
  double rate = 0;
  size_t movie_counter = 0, i = 0;
  for (; i < line.size (); i++)
    {
      if (line[i] == ' ')
        {
          user_name = temp;
          temp = "";
          break;
        }
      else
        {
          temp += line[i];
        }
    }
  rank_map user_map (HASH_START, sp_movie_hash, sp_movie_equal);
  RSUser new_user = RSUser (user_name, user_map, shared_rs);
  i++;
  for (; i < line.size (); i++)
    {
      if (temp == " ")
        { temp = ""; }
      if (line[i] == ' ')
        {
          rate = get_rate (temp);
          pair<string, int> movie_pair = movie_vec[movie_counter];
          vector<double> features = get_features (movie_pair, shared_rs);
          new_user.add_movie_to_rs (movie_pair.first, movie_pair.second,
                                    features, rate);
          movie_counter++;
          temp = "";
        }
      else
        { temp += line[i]; }
    }
  rate = get_rate (temp);
  pair<string, int> movie_pair = movie_vec[movie_counter];
  vector<double> features = get_features (movie_pair, shared_rs);
  new_user.add_movie_to_rs (movie_pair.first, movie_pair.second,
                            features, rate);
  return new_user;
}

vector<RSUser> RSUsersLoader::create_users_from_file
    (const std::string &users_file_path, unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
  shared_ptr<RecommenderSystem> shared_rs = std::move (rs);
  vector<RSUser> user_vec;
  vector<pair<string, int>> movie_vec;
  string line;
  int counter = 0;
  std::ifstream in_file (users_file_path, std::ios::in);
  if (!in_file.is_open ())
    {
      throw std::runtime_error (PATH_ERROR);
    }
  while (std::getline (in_file, line))
    {
      if (counter == 0)
        {
          push_movie_to_vec (line, movie_vec);
          counter++;
        }
      else
        {
          user_vec.push_back (next_user (line, shared_rs, movie_vec));
        }
    }
  in_file.close ();
  return user_vec;
}