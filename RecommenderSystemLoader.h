
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"
using std::unique_ptr;
using std::make_unique;

class RecommenderSystemLoader {

 private:

 public:
  RecommenderSystemLoader () = delete;
  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movies
   * @return shared pointer to a RecommenderSystem which was
   * created with those movies
   */
  static unique_ptr<RecommenderSystem> create_rs_from_movies_file
      (const std::string &movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
