/* Copyright 2017 Andrei Petre */

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

#include "include/imdb.h"
#include "include/classes.h"
#include "include/bst.h"
#include "include/treap.h"

IMDb::IMDb() {
    // initialize what you need here.
    activity = new Treap<std::string, int>;
    directors = new Treap<std::string, int>;
}

IMDb::~IMDb() {
  delete activity;
  delete directors;
}

void IMDb::add_movie(std::string movie_name,
   std::string movie_id,
   int timestamp,  // unix timestamp when movie was launched
   std::vector<std::string> categories,
   std::string director_name,
   std::vector<std::string> actor_ids) {
     movie new_movie(movie_name, movie_id, timestamp, categories,
       director_name, actor_ids);
     movies.insertKey(new_movie);

     for(auto it : actor_ids) {
       actor *act = this -> actor_bs(it, 0, actors.size() - 1);
       if(act -> get_first() == INT_MAX) {
         act -> set_first(timestamp);
         act -> set_last(timestamp);
         this -> activity -> insert(activity, it, 0);
       } else {
         if(act -> get_first() > timestamp) {
           act -> set_first(timestamp);
           this -> activity -> erase(activity, it);
           this -> activity -> insert(activity, it, act -> get_last() - timestamp);
         } else if(act -> get_last() < timestamp) {
           act -> set_last(timestamp);
           this -> activity -> erase(activity, it);
           this -> activity -> insert(activity, it, timestamp - act -> get_first());
         }
       }
     }

     if(this -> directors -> find(director_name)) {

       int acts = directors -> get_priority(director_name);
       director old_director = d_info[director_name];
       for(auto it : actor_ids) {
         old_director.add_known_actors(it);
       }
       if(old_director.nr_known_actors() != acts) {
         d_info[director_name] = old_director;
         directors -> erase(directors, director_name);
         directors -> insert(directors, director_name, old_director.nr_known_actors());
       }

     } else {

       director new_director(director_name);
       for(auto it : actor_ids) {
         new_director.add_known_actors(it);
       }
       d_info[director_name] = new_director;
       directors -> insert(directors, director_name, actor_ids.size());

     }
     latest_movie lm(movie_id, timestamp);
     latest_movies.insert(lm);

     top_rating newrt(movie_id, 0);
     rtop.insert(newrt);

     // partners

     for (auto it = actor_ids.begin(); it != actor_ids.end(); ++it) {
       std::set<top_rating> aux;
       if (!partners[*it].empty())
          aux = partners[*it];

       for (auto itt = actor_ids.begin(); itt != actor_ids.end(); ++itt)
         if(*it != *itt) {
           auto index = find(aux.begin(), aux.end(), *itt);
           int newrate;
           if(index == aux.end()) {
             newrate = 1;
           } else {
             newrate = index->get_rate() + 1;
             aux.erase(index);
           }
           top_rating newp(*itt, newrate);
           aux.insert(newp);

           //for pairs

           top_pair newpair;
           if (*itt< *it) {
             newpair.set_act1(*itt);
             newpair.set_act2(*it);
             newpair.set_val(newrate - 1);
           } else {
             newpair.set_act1(*it);
             newpair.set_act2(*itt);
             newpair.set_val(newrate - 1);
           }

           if (pairs.find(newpair) != pairs.end()) {
              pairs.erase(newpair);
           }
           newpair.set_val(newrate);
           pairs.insert(newpair);
         }
         partners[*it] = aux;
     }
}

void IMDb::add_user(std::string user_id, std::string name) {
  user new_user(user_id, name);
  if(this -> users.empty()) {
    this -> users.push_back(new_user);
    return ;
  }
  int index = this -> users.size();
  if(user_id > this -> users[index - 1].get_user_id()) {
    this -> users.push_back(new_user);
  } else {
    for(auto it = this -> users.begin(); it != this -> users.end(); ++it) {
      if(it -> get_user_id() > user_id) {
        this -> users.insert(it, new_user);
        break;
      }
    }
  }
}

void IMDb::add_actor(std::string actor_id, std::string name) {

    actor new_actor(actor_id, name);
    if(actors.empty()) {
      actors.push_back(new_actor);
      return;
    }
    int index = actors.size();
    if(actor_id > actors[index - 1].get_actor_id()) {
      actors.push_back(new_actor);
    } else {
      for(auto it = actors.begin(); it != actors.end(); ++it) {
        if (it->get_actor_id() > actor_id) {
          actors.insert(it, new_actor);
          break;
        }
    }
  }
}

void IMDb::add_rating(std::string user_id, std::string movie_id, int rating) {
  movie *mov = this -> movies.searchKey(movie_id);
  int garbage = mov->get_nr_rates();

  std::string old_rating = get_rating(movie_id);
  double d_rate;
  if(old_rating == "none") {
    d_rate = 0;
  } else {
    d_rate = std::stod(old_rating);
  }


  mov -> add_rate(user_id, rating);
  mov -> modify_rate(rating);
  mov -> modify_nr_rates(1);

  // caut in set movie, il sterg, si reinserez cu noul scor
  top_rating junk(movie_id, garbage);
  auto it = std::find(rtop.begin(), rtop.end(), junk);
  if(it != rtop.end()) {
    rtop.erase(it);
  }
  top_rating newrt(movie_id, mov->get_nr_rates());
  rtop.insert(newrt);

  std::vector<std::string> cat_list = mov -> get_categories();
  int year = mov -> get_timestamp();
  time_t startTime_t = (time_t) year;
  std::string startTime = asctime(localtime(&startTime_t));
  startTime = startTime.substr(startTime.find('\n') - 4, 4);
  year = std::stoi(startTime);

  for(auto it : cat_list) {
      auto iter = std::find(categ[it].begin(), categ[it].end(), std::to_string(year));

      if(iter != categ[it].end()) {

          double old_rate = iter -> get_rating();
          old_rate *= iter -> get_nr_movies();
          old_rate -= d_rate;
          old_rate += std::stod(get_rating(movie_id));

          year_rating yr2 = *iter;
          yr2.add_movie(movie_id);
          yr2.set_rating(old_rate / yr2.get_nr_movies());
          categ[it].erase(iter);
          categ[it].insert(yr2);

      } else {
          year_rating yr2(std::to_string(year), std::stod(get_rating(movie_id)));
          yr2.add_movie(movie_id);
          categ[it].insert(yr2);

      }
  }

}

void IMDb::update_rating(std::string user_id, std::string movie_id,
                         int rating) {
  movie* mov = movies.searchKey(movie_id);
  int past_rate = mov->get_user_rate(user_id);
  double garbage = double((double)mov->get_rate_sum()/(double)mov->get_nr_rates());

  std::string old_rating = get_rating(movie_id);
  double d_rate;
  if(old_rating == "none") {
    d_rate = 0;
  } else {
    d_rate = std::stod(old_rating);
  }

  mov->modify_rate(-past_rate);
  mov->modify_rate(rating);
  mov->add_rate(user_id, rating);

  std::vector<std::string> cat_list = mov -> get_categories();
  int year = mov -> get_timestamp();
  time_t startTime_t = (time_t) year;
  std::string startTime = asctime(localtime(&startTime_t));
  startTime = startTime.substr(startTime.find('\n') - 4, 4);
  year = std::stoi(startTime);

  for(auto it : cat_list) {
      auto iter = std::find(categ[it].begin(), categ[it].end(), std::to_string(year));
      if(iter != categ[it].end()) {

          double old_rate = iter -> get_rating();
          old_rate *= iter -> get_nr_movies();
          old_rate -= d_rate;
          old_rate += std::stod(get_rating(movie_id));

          year_rating yr2 = *iter;
          yr2.add_movie(movie_id);
          yr2.set_rating(old_rate / yr2.get_nr_movies());
          categ[it].erase(iter);
          categ[it].insert(yr2);

      }
  }
}

void IMDb::remove_rating(std::string user_id, std::string movie_id) {
  movie *mov = this -> movies.searchKey(movie_id);
  int garbage = mov->get_nr_rates();
  int past_rate = mov -> get_user_rate(user_id);

  std::string old_rating = get_rating(movie_id);
  double d_rate;
  if(old_rating == "none") {
    d_rate = 0;
  } else {
    d_rate = std::stod(old_rating);
  }

  mov -> add_rate(movie_id, 0);
  mov -> modify_rate(- past_rate);
  mov -> modify_nr_rates(-1);

  std::vector<std::string> cat_list = mov -> get_categories();
  int year = mov -> get_timestamp();
  time_t startTime_t = (time_t) year;
  std::string startTime = asctime(localtime(&startTime_t));
  startTime = startTime.substr(startTime.find('\n') - 4, 4);
  year = std::stoi(startTime);

  top_rating junk(movie_id, garbage);
  auto it = std::find(rtop.begin(), rtop.end(), junk);
  if(it != rtop.end()) {
    rtop.erase(it);
  }
  top_rating newrt(movie_id, mov->get_nr_rates());
    rtop.insert(newrt);

    for(auto it : cat_list) {
        auto iter = find(categ[it].begin(), categ[it].end(), std::to_string(year));
        if(iter != categ[it].end()) {
            double old_rate = iter -> get_rating();
            old_rate *= iter -> get_nr_movies();
            old_rate -= d_rate;
            year_rating yr2 = *iter;
            if(mov -> get_nr_rates() != 0) {
                std::string new_rating = get_rating(movie_id);
                double n_rate;
                if(new_rating == "none") {
                  n_rate = 0;
                } else {
                  n_rate = std::stod(new_rating);
                }
                old_rate += n_rate;
            } else {
                yr2.remove_movie(movie_id);
            }

            yr2.set_rating(old_rate / yr2.get_nr_movies());
            categ[it].erase(iter);
            if(yr2.get_nr_movies() != 0) {
                categ[it].insert(yr2);
            }

        }
    }

}

std::string IMDb::get_rating(std::string movie_id) {
  movie *mov = movies.searchKey(movie_id);
    int rate_sum = mov->get_rate_sum();
    int nr_rates = mov->get_nr_rates();
    if(nr_rates == 0)
    return "none";
    double rate = double(rate_sum)/double(nr_rates);
    rate *=1000;
    int rateint = floor(rate);
    if(rateint%10 >= 5) rateint+=10;
    rateint /= 10;
    rate = double(rateint)/100;
    std::string result = std::to_string(rate);
    if(result[1] == '.') result.resize(4);
    else result.resize(5);
    return result;
}

std::string IMDb::get_longest_career_actor() {
    if(this -> activity -> isNil()) {
      return "none";
    }
    return this -> activity -> peek();
}

std::string IMDb::get_most_influential_director() {
  if(this -> directors -> isNil()) {
    return "none";
  }
  return this -> directors -> peek();
}

std::string IMDb::get_best_year_for_category(std::string category) {
    if(categ[category].size() == 0) {
        return "none";
    } else {

        return categ[category].rbegin() -> get_year();
    }
}

std::string IMDb::get_2nd_degree_colleagues(std::string actor_id) {
    return "none";
}

std::string IMDb::get_top_k_most_recent_movies(int k) {
  if(latest_movies.empty())
    return "none";

  std::set<latest_movie>::reverse_iterator it = latest_movies.rbegin();
  std::string answer = it->get_mid();
  ++it;
  for(int i = 2; i <= k && it != latest_movies.rend(); ++i) {
    answer += " ";
    answer += it->get_mid();
    ++it;
  }
  return answer;
}

std::string IMDb::get_top_k_actor_pairs(int k) {
    if(pairs.empty())
      return "none";

    /*for(auto it : pairs) {
      std::cout << it.get_one() << " " << it.get_two() << "\n\n";
    }
    std::cout << " \n\n";
*/
    auto it = pairs.rbegin();
    std::string answer;
    answer += "(" + it->get_one() + " " + it->get_two();
    answer += " " + std::to_string(it->get_val()) + ")";
    ++it;
    for (int i = 2; i <= k && it != pairs.rend(); ++i) {
      answer += " (" + it->get_one() + " " + it->get_two();
      answer += " " + std::to_string(it->get_val()) + ")";
      ++it;
    }
    //answer += "-----";
    return answer;
}

std::string IMDb::get_top_k_partners_for_actor(int k, std::string actor_id) {
    std::set<top_rating> aux;
    aux = partners[actor_id];

    if(aux.empty()) return "none";

    //return "dc";
    std::set<top_rating>::reverse_iterator it = aux.rbegin();
    std::string answer = it->get_id();

    ++it;
    for(int i = 2; i <= k && it != aux.rend(); ++i) {
      answer += " ";
      answer += it->get_id();
      ++it;
    }
    return answer;
}

std::string IMDb::get_top_k_most_popular_movies(int k) {
    if(rtop.empty())
      return "none";

    std::set<top_rating>::reverse_iterator it = rtop.rbegin();
    std::string answer = it->get_id();

    ++it;
    for(int i = 2; i <= k && it != rtop.rend(); ++i) {
      answer += " ";
      answer += it->get_id();
      ++it;
    }
    return answer;

}

std::string IMDb::get_avg_rating_in_range(int start, int end) {
    return "none";
}

actor *IMDb::actor_bs(std::string actor_id, int left, int right) {
  if(left > right) {
    return nullptr;
  }
  int mij = (left + right) / 2;
  if(actors[mij].get_actor_id() == actor_id) {
    return &actors[mij];
  }
  if(actors[mij].get_actor_id() < actor_id) {
    return actor_bs(actor_id, mij + 1, right);
  } else {
    return actor_bs(actor_id, left, mij - 1);
  }
}
