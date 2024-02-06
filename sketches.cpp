#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <bitset>
using namespace std;

constexpr uint64_t c1 = 0xcc9e2d51ull;
constexpr uint64_t c2 = 0x1b873593ull;
constexpr uint64_t r1 = 15;
constexpr uint64_t r2 = 13;
constexpr uint64_t m = 5;
constexpr uint64_t n = 0xe6546b64ull;


uint64_t a1 = 97102761753631939;
    uint64_t a2 = 56842397421741207;
    uint64_t a3 = 61811894731001917;
    uint64_t a4 = 31415926535897931;
    uint64_t a5 = 98765432109876543;

    uint64_t b1 = 42506983374872291;
    uint64_t b2 = 18456721873196387;
    uint64_t b3 = 37217717671130671;
    uint64_t b4 = 27182818284590453;
    uint64_t b5 = 57548533468232177;

uint64_t murmur_hash(const std::string& key, uint64_t seed = 0) {
  uint64_t hash = seed;
  const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
  size_t len = key.length();
  for (size_t i = 0; i < len / 8; ++i) {
    uint64_t k = *reinterpret_cast<const uint64_t*>(data + i * 8);
    k *= c1;
    k = (k << r1) | (k >> (64 - r1));
    k *= c2;
    
    hash ^= k;
    hash = ((hash << r2) | (hash >> (64 - r2))) * m + n;
  }
  const uint8_t* tail = data + (len / 8) * 8;
  uint64_t k1 = 0;
  switch (len & 7) {
  case 7:
    k1 ^= static_cast<uint64_t>(tail[6]) << 48;
  case 6:
    k1 ^= static_cast<uint64_t>(tail[5]) << 40;
  case 5:
    k1 ^= static_cast<uint64_t>(tail[4]) << 32;
  case 4:
    k1 ^= static_cast<uint64_t>(tail[3]) << 24;
  case 3:
    k1 ^= static_cast<uint64_t>(tail[2]) << 16;
  case 2:
    k1 ^= static_cast<uint64_t>(tail[1]) << 8;
  case 1:
    k1 ^= static_cast<uint64_t>(tail[0]);
    k1 *= c1;
    k1 = (k1 << r1) | (k1 >> (64 - r1));
    k1 *= c2;
    hash ^= k1;
  }
  hash ^= len;
  hash ^= (hash >> 37);
  hash *= 0x85ebca6bull;
  hash ^= (hash >> 32);
  return hash;
}

uint64_t ax_b_string_func(uint64_t a, uint64_t b, string to_hash){
  
  uint64_t hash = b;
  for(char c : to_hash){
    hash = hash * a + static_cast<uint64_t>(c);
  }
  
  return (uint64_t) hash;
};

class Sketch {
  public:

    

  

    virtual void insert(string s) = 0;
    
    


    uint64_t hashvals[10] = {a1,a2,a3,a4,a5,b1,b2,b3,b4,b5}; //my hash values
    uint64_t applyH(int sw, std::string word, int hsh,int seed = 0){ //does the hash transformation with the given swith seeds and hash numbers like (a1,b1,a2,b2...)
     if(sw==1){
      return ax_b_string_func(hashvals[hsh-1],hashvals[hsh+4],word);}
    else{
      return murmur_hash(word,seed);
    }

    
  }

};


class MembershipSketch: public Sketch{
  public:
    virtual bool isMember(string s) = 0;
};




class Cardinality_Sketch:public Sketch{
public:
  virtual double estimate(string s) = 0;
};


int findindex(uint64_t number, int m) {
    if (m <= 1) return 0;  // if m is less or equal to 1 return 0

    // calculate the msb numbers to extract
    int first = floor(std::log2(m));
    
    // mask to get the msb bits
    uint64_t mask = (first == 64) ? ~0ULL : ((1ULL << first) - 1);
    // shift numbers to the right to get the integer answer
    uint64_t result = (number >> (64 - first)) & mask;

    return result;  // return
}




int findc0s(uint64_t myint, int m){
  
  

  int out = 0;
  int bitsToCheck = 64 - floor(log2(m)); // calculate number of bis to check

  while (bitsToCheck-- > 0 && (myint & 1) == 0) { // while there are bits 
    myint >>= 1; // shift the number to right
    out++; // increment
  }
  
  return 1+out;//return
}




class HyperLogLog:public Cardinality_Sketch{
  public:
  

    void insert(string s){}
    
    virtual double estimate(string s){
      
      double myint =0;//init myint
      
      for(int i=0;i<b;i++){
        
        myint+= pow(2,-int(buckets[i]));} //sum the probabilities of the numbers from buckets

      return b*(b/myint);//return
    }

    double estimateCardinality(){
      double am;
      if      (b ==16){am = 0.673;} // the values given in the pdf
      else if(b==32){am = 0.698;}
      else if (b==64){am=0.709;}
      else{am = 0.7212/(1+1.079/b);}
      return am* estimate("a"); //return
    }

    void add(string word){
      uint64_t hash = applyH(_switch,word,1);//call the hash function
      
      int zero = findc0s(hash,b);//call to find cons 0's +1
      
      int ind = findindex(hash,b);//call to find bucket index
      
      
      if(buckets[ind]<zero){buckets[ind]=zero;}//if it is smaller than my value replace it
      
    }


  




 
  int b; // number of buckets
  uint64_t* buckets; // bit array
  int _switch; // select between hash functions
  HyperLogLog(int b, int _switch):b(b),_switch(_switch),buckets(new uint64_t[b]){
    for (int i = 0; i<b; i++){buckets[i]=0;//init the array}
    
}}
~HyperLogLog(){delete [] buckets;}
};


class BloomFilter: public MembershipSketch{
  public:

    uint64_t* hasha = new uint64_t[5];//hash values
    int seedvals[5] = {7,1,6,9,14}; //seed values

    void insert(string str){}

  void gethash (string str){   //runs the hash functions to fill the 5 hash values to be used in bloomfilter
        for(int i =0; i<5;i++){
        hasha[i] = applyH(_switch,str,i+1,seedvals[i]);}
        
    }

    bool isMember(string str){

      gethash(str);
      
        for(int i = 0;i<5;i++){
        uint64_t new_n = hasha[i] % size ; // modulo to round the number back to our scale
        uint64_t bucket = new_n / 64 ;  //Bucket index
        uint64_t index = new_n%64 ; //index in bucket
        

        uint64_t add =pow(2, 64-index); // find the number to check if it is in our bloomfilter
        
        if((filter[bucket]&add) == add){}else{return false;}}//if it is now return false
        return true;
            

      


      
    }
     

     

    int get_size(){return size;}
    int get_k(){return k;}
    int k; // number of hash functions
    int size; // size in bits
    int _switch; // select between hash functions
    uint64_t* filter ;

    void add(string str){
      
       
      gethash(str);
       
      for(int i = 0;i<5;i++){
        uint64_t new_n = hasha[i] % size ; // modulo to round the number back to our scale
        uint64_t bucket = new_n / 64 ;  //Bucket index
        uint64_t index = new_n%64 ; //index in bucket
        
        uint64_t add =pow(2, 64-index);//value to add(or operation)
        
           filter[bucket] = filter[bucket] | add;//insert the 1 to its index
      }
    }

    BloomFilter(int k, int size, int _switch):k(k), size(size), _switch(_switch),filter(new uint64_t[size/64]){
      for (int i = 0; i<size/64;i++){
        filter[i] = 0;
      }
    //initilize the filters values to 0
    };
    ~BloomFilter(){delete [] filter;}

};


void evaluate_bloom(vector<string> words, BloomFilter *bloom, int i){

  int true_positives = 0;
  int false_positives = 0;
  
  for(int i = 0; i < 100; i++){
    true_positives += (int)bloom->isMember(words[i]);
  }

  for(int i = 0; i < 100; i++){
    false_positives += (int)bloom->isMember(words[words.size() - i - 1]);
  }  
  
  cout << "size: " << bloom->get_size() << " k: " << bloom->get_k() << " Inserted: " << i << " TP: " << true_positives << " FP: " << false_positives << endl;
}


class KMinVal: public Cardinality_Sketch{
public:

    int K; // number of values to keep
  uint64_t* val; // array
  int _switch; // select between hash functions
  KMinVal(int K, int _switch):K(K),_switch(_switch),val(new uint64_t[K]){
    for (int i = 0; i<K; i++){val[i]=-1;}//init values to -1
  }
  ~KMinVal(){delete [] val;}


   
  
  void insert(string s){}
  virtual double estimate(string a)
{
     

    double maxinlist = 0;
        for (int i = 0; i < K; i++) {
            if ((val[i] != UINT64_MAX) && (val[i] > maxinlist)) {
                maxinlist = val[i];
            }
        }
    

    return (K-1)*((pow(2,64)-1)/maxinlist);
    }







  double estimateCardinality(){ 
    
      
      
    return estimate("a");
  }
  



  void add(string word) {
    uint64_t hash = applyH(_switch, word, 2);
    int insertPos = -1;

    for (int i = 0; i < K; i++) {
        if (val[i] == hash) {
            return; // hash exists, do not add
        }
        if ((val[i] == UINT64_MAX || val[i] > hash) && insertPos == -1) {
            insertPos = i; // the position to add the hash
        }
    }

    if (insertPos != -1) {
        if (val[K - 1] != UINT64_MAX && hash >= val[K - 1]) {
            return;
        }

        for (int i = K - 2; i >= insertPos; i--) {
            val[i + 1] = val[i];
        }
        val[insertPos] = hash; // insert the hash
    }
}
};




  


  

 


  
    


int main(int argc, char** argv){

  string fname;
  fname = argv[1];
  
  ifstream file(fname);
  if(!file.is_open()){
    cout << "Error opening file: " << fname << endl;
    return 1;
  }

  vector<string> words;
  string word;

  //Reading each line into vector of strings
  while(std::getline(file, word)){
    words.push_back(word);
  }

  cout << "#######################################################################################" << endl;
  cout << "HyperLogLog Using ax+b hash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    HyperLogLog loglog(an, 1);
    for(int i = 0; i < words.size(); i++){
      loglog.add(words[i]);
    }
    
    cout << "buckets: " << an << " HyperLogLog estimate: " << loglog.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "HyperLogLog Using MurMurHash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    HyperLogLog loglog(an, 2);
    for(int i = 0; i < words.size(); i++){
      loglog.add(words[i]);
    }
    
    cout << "buckets: " << an << " HyperLogLog estimate: " << loglog.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "KMinVal Using ax + b hash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    KMinVal kminval(an, 1);
    for(int i = 0; i < words.size(); i++){
      kminval.add(words[i]);
    }
    cout << "buckets: " << an << " KMinVal estimate: " << kminval.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "KMinVal Using MurMurHash" << endl;
  for(int an = 2; an <= 8192; an *= 2){
    KMinVal kminval(an, 2);
    for(int i = 0; i < words.size(); i++){
      kminval.add(words[i]);
    }
    cout << "buckets: " << an << " KMinVal estimate: " << kminval.estimateCardinality() << endl;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "BloomFilter using ax + b hash" << endl;
  for(int i = 64; i <= 131072; i *= 2){
    BloomFilter *bloom = new BloomFilter(5, i, 1); //Note that we are using 5 hashes with ax + b
    for(int i = 0; i < words.size() - 100; i++){ //We do not add last 100 words to the bloom filter to be able to check if they return as false positives
      bloom->add(words[i]);
      if(i == 100){
	evaluate_bloom(words, bloom, i);}
      if(i == 500)
	evaluate_bloom(words, bloom, i);
      if(i == 1000)
	evaluate_bloom(words, bloom, i);
      if(i == 100000)
	evaluate_bloom(words, bloom, i);
      if(i == 1000000)
	evaluate_bloom(words, bloom, i);
    }
    delete bloom;
  }
  cout << "#######################################################################################" << endl;

  cout << "#######################################################################################" << endl;
  cout << "BloomFilter using MurMurHash" << endl;
  for(int i = 64; i <= 131072; i *= 2){
    BloomFilter *bloom = new BloomFilter(5, i, 2); //Note that we are using 5 hashes with ax + b
    for(int i = 0; i < words.size() - 100; i++){ //We do not add last 100 words to the bloom filter to be able to check if they return as false positives
      bloom->add(words[i]);
      if(i == 100)
	evaluate_bloom(words, bloom, i);
      if(i == 500)
	evaluate_bloom(words, bloom, i);
      if(i == 1000)
	evaluate_bloom(words, bloom, i);
      if(i == 100000)
	evaluate_bloom(words, bloom, i);
      if(i == 1000000)
	evaluate_bloom(words, bloom, i);
    }
    delete bloom;
  }
  cout << "#######################################################################################" << endl;

    
}