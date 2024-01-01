
#ifndef MOVIE_H
#define MOVIE_H
#include <map>
#include <unordered_map>
#include <fstream>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <cmath>
#define HASH_START 17
using std::array;
using std::vector;
using std::string;
using std::pair;
using std::map;
using std::shared_ptr;

class Movie;

typedef std::shared_ptr<Movie> sp_movie; // define your smart pointer

/**
 * those declarations and typedefs are given to you and should be used in
 * the exercise
 */
typedef std::size_t (*hash_func) (const sp_movie &movie);
typedef bool (*equal_func) (const sp_movie &m1, const sp_movie &m2);
std::size_t sp_movie_hash (const sp_movie &movie);
bool sp_movie_equal (const sp_movie &m1, const sp_movie &m2);

class Movie {
 public:
  /**
   * constructor
   * @param name: name of movie
   * @param year: year it was made
   */
  Movie (std::string name, int year) : _name (std::move (name)),
                                       _year (year)
  {};

/**
   * returns the name of the movie
   * @return const ref to name of movie
   */
  std::string get_name () const
  { return _name; }

  /**
   * returns the year the movie was made
   * @return year movie was made
   */
  int get_year () const
  { return _year; }

  /**
   * operator< for two movies
   * @param rhs: right hand side
   * @param lhs: left hand side
   * @return returns true if (rhs.year) < lhs.year or
   * (rhs.year == lhs.year & rhs.name == lhs.name) else return false
   */

  bool operator< (const Movie &rhs) const;

  /**
   * operator<< for movie
   * @param os ostream to output info with
   * @param movie movie to output
   */

  friend std::ostream &operator<< (std::ostream &, Movie &movie);
 private:
  std::string _name;
  int _year;
};

#endif //MOVIE_H
