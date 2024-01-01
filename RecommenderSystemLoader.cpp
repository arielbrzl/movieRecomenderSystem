#include <string>
#include <sstream>
#include "RecommenderSystemLoader.h"
#define PATH_ERROR "ERROR:this file can't open"
#define WRONG_NUM "ERROR: given feature not in correct range"
#define UPPER_BOUND 10
#define LOWER_BOUND 1
unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  unique_ptr<RecommenderSystem> rs_ptr = make_unique<RecommenderSystem> ();
  std::ifstream in_file (movies_file_path, std::ios::in);
  std::string line, name, temp;
  int year;
  vector<double> features;
  if (!in_file.is_open ())
    {
      throw std::runtime_error (PATH_ERROR);
    }
  while (std::getline (in_file, line))
    {
      int counter = 0;
      for (char i: line)
        {
          if (i == '-' || i == ' ')
            {
              if (counter == 0)
                { name = temp; }
              if (counter == 1)
                { year = stoi (temp); }
              if (counter > 1)
                {
                  double score = stod (temp);
                  if (score >= LOWER_BOUND && score <= UPPER_BOUND)
                    { features.push_back (score); }
                  else
                    { throw std::length_error (WRONG_NUM); }
                }
              counter++;
              temp = "";
            }
          else
            { temp += i; }
        }
      features.push_back (stod (temp));
      temp = "";
      rs_ptr->add_movie (name, year, features);
      size_t i = features.size ();
      for (size_t k = 0; k < i; k++)
        { features.pop_back (); }
    }
  in_file.close ();
  return rs_ptr;
}