/* Copyright 2017 Andrei Petre */

#ifndef SD_TEMA3_INCLUDE_CLASSES_H_
#define SD_TEMA3_INCLUDE_CLASSES_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <climits>

class movie {
 public:
   movie() {}

   movie(std::string movie_name,
      std::string movie_id,
      int timestamp,
      std::vector<std::string> categories,
      std::string director_name,
      std::vector<std::string> actor_ids) {
        this -> movie_id = movie_id;
        this -> movie_name = movie_name;
        this -> timestamp = timestamp;
        this -> director_name = director_name;
        this -> categories = categories;
        this -> actor_ids = actor_ids;
        this -> rate_sum = 0;
        this -> nr_rates = 0;
   }

   ~movie(){}

   std::string get_movie_id() {
     return movie_id;
   }

   std::string get_movie_name() {
     return movie_name;
   }

   int get_timestamp() {
     return timestamp;
   }

   std::string get_director_name() {
     return director_name;
   }

   std::vector<std::string> get_categories() {
     return categories;
   }

   std::vector<std::string> get_actor_ids() {
     return actor_ids;
   }

   int get_rate_sum() {
     return rate_sum;
   }

   int get_nr_rates() {
     return nr_rates;
   }

   void modify_rate(int x) {
     rate_sum += x;
   }

   void modify_nr_rates(int x) {
     nr_rates += x;
   }

   void add_rate(std::string user_id, int rating) {
     user_rates[user_id] = rating;
   }

   int get_user_rate(std::string user_id) {
     return user_rates[user_id];
   }

 private:
   std::string movie_id;
   std::string movie_name;
   int timestamp;
   std::string director_name;
   std::vector<std::string> categories;
   std::vector<std::string> actor_ids;
   int rate_sum;
   int nr_rates;
   std::unordered_map<std::string, int> user_rates;

};

class user {
 public:
   user(std::string user_id, std::string name) {
     this -> user_id = user_id;
     this -> name = name;
   }

   ~user(){}

   std::string get_user_id() {
     return user_id;
   }

 private:
   std::string user_id;
   std::string name;

};

class actor {
 public:
   actor(std::string actor_id, std::string name) {
     this -> actor_id = actor_id;
     this -> name = name;
     first = INT_MAX;
     last = 0;
   }

   ~actor(){}

   std::string get_actor_id() {
     return actor_id;
   }

   int get_first() {
     return first;
   }

   int get_last() {
     return last;
   }

   void set_first(int x) {
     this -> first = x;
   }

   void set_last(int x) {
     this -> last = x;
   }

 private:
   std::string actor_id;
   std::string name;
   int first, last;

};

class director {
  public:
    director(){}

    director(std::string director_name) {
      this -> director_name = director_name;
    }

    ~director(){}

    void add_known_actors(std::string actor_id) {
      known_actors[actor_id] = true;
    }

    int nr_known_actors() {
      return known_actors.size();
    }

  private:
    std::string director_name;
    std::unordered_map<std::string, bool> known_actors;

};

class latest_movie{
public:
  const bool operator < ( const latest_movie &r ) const{
        return ( timestamp< r.timestamp)
               ||(( timestamp== r.timestamp) && ( movie_id > r.movie_id));
    }
  latest_movie(std::string movie_id, int timestamp) {
    this->movie_id = movie_id;
    this->timestamp = timestamp;
  }
  int get_t() const {
    return timestamp;
  }
  std::string get_mid() const {
    return movie_id;
  }
private:
  int timestamp;
  std::string movie_id;
};

class top_rating{
public:
  const bool operator < ( const top_rating &r ) const{
        return ( rate < r.rate)
               ||(( rate== r.rate) && ( id > r.id));
    }

    top_rating(const std::string& strKey = "", const int& strData = 1)
      : id(strKey),
        rate(strData) {}

    bool operator==(const top_rating& rhs) const
    {
      return id == rhs.id;
    }
    top_rating(std::string id, double rate) {
      this->id = id;
      this->rate = rate;
    }
    int get_rate() const{
      return rate;
    }

    std::string get_id() const {
      return id;
    }

private:
  int rate;
  std::string id;
};

class year_rating{
public:
    const bool operator < (const year_rating &r) const {
        return (rating < r.rating) || ((rating == r.rating) && (year > r.year));
    }

    year_rating(const std::string& strKey = "", const int& strData = 1)
      : year(strKey),
        rating(strData) {}

    bool operator==(const year_rating &r) const
    {
      return year == r.year;
    }

    year_rating(std::string key, double value) {
        year = key;
        rating = value;
    }

    year_rating operator = (const year_rating &r) {
        this -> year = r.year;
        this -> rating = r.rating;
        this -> m_rate = r.m_rate;
    }

    double get_rating() const{
        return rating;
    }

    void set_rating(double r) {
        rating = r;
    }

    std::string get_year() const {
        return year;
    }

    int get_nr_movies() const {
        return m_rate.size();
    }

    void add_movie(std::string movie_id) {
        m_rate[movie_id] = 1;
    }

    void remove_movie(std::string movie_id) {
        m_rate.erase(movie_id);
    }

private:
    std::string year;
    double rating;
    std::unordered_map<std::string, bool> m_rate;
};

class top_pair {
  public:
    bool operator==(const top_pair& rhs) const {
      return (act1 == rhs.act1 && act2 == rhs.act2);
    }

    const bool operator < ( const top_pair &r ) const {
          return ( value < r.value)
                 ||(( value == r.value) && ( act1 > r.act1))  || ((value == r.value) && (act1 == r.act1) && (act2 > r.act2) );
    }

  /*  top_rating(const std::string& one = "", const std::string& two = "", const int& strData = 1)
      : act1(one),
        act2(two),
        rate(strData) {}
*/
    top_pair() {}
    top_pair(std::string one, std::string two, double val) {
        this->act1 = one;
        this->act2 = two;
        this->value = val;
    }
    std::string get_one() const {
      return act1;
    }

    std::string get_two() const {
      return act2;
    }

    int get_val() const {
      return value;
    }

    void set_act1(std::string act) {
      act1 = act;
    }

    void set_act2(std::string act) {
      act2 = act;
    }

    void set_val(int val) {
      value = val;
    }

  private:
    int value;
    std::string act1, act2;
};


#endif  // SD_TEMA3_INCLUDE_CLASSES_H_
