#ifndef CPPJIEAB_JIEBA_H
#define CPPJIEAB_JIEBA_H

#include "QuerySegment.hpp"
#include "KeywordExtractor.hpp"

namespace cppjieba {

class Jieba {
 public:
  Jieba(const std::string& dict_path = "", 
        const std::string& model_path = "",
        const std::string& user_dict_path = "", 
        const std::string& idf_path = "", 
        const std::string& stop_word_path = "") 
    : dict_trie_(getPath(dict_path, "jieba.dict.utf8"), getPath(user_dict_path, "user.dict.utf8")),
      model_(getPath(model_path, "hmm_model.utf8")),
      mp_seg_(&dict_trie_),
      hmm_seg_(&model_),
      mix_seg_(&dict_trie_, &model_),
      full_seg_(&dict_trie_),
      query_seg_(&dict_trie_, &model_),
      extractor(&dict_trie_, &model_, 
                getPath(idf_path, "idf.utf8"), 
                getPath(stop_word_path, "stop_words.utf8")) {
  }
  ~Jieba() {
  }

  struct LocWord {
    std::string word;
    size_t begin;
    size_t end;
  }; // struct LocWord

  void Cut(const std::string& sentence, std::vector<std::string>& words, bool hmm = true) const {
    mix_seg_.Cut(sentence, words, hmm);
  }
  void Cut(const std::string& sentence, std::vector<Word>& words, bool hmm = true) const {
    mix_seg_.Cut(sentence, words, hmm);
  }
  void CutAll(const std::string& sentence, std::vector<std::string>& words) const {
    full_seg_.Cut(sentence, words);
  }
  void CutAll(const std::string& sentence, std::vector<Word>& words) const {
    full_seg_.Cut(sentence, words);
  }
  void CutForSearch(const std::string& sentence, std::vector<std::string>& words, bool hmm = true) const {
    query_seg_.Cut(sentence, words, hmm);
  }
  void CutForSearch(const std::string& sentence, std::vector<Word>& words, bool hmm = true) const {
    query_seg_.Cut(sentence, words, hmm);
  }
  void CutHMM(const std::string& sentence, std::vector<std::string>& words) const {
    hmm_seg_.Cut(sentence, words);
  }
  void CutHMM(const std::string& sentence, std::vector<Word>& words) const {
    hmm_seg_.Cut(sentence, words);
  }
  void CutSmall(const std::string& sentence, std::vector<std::string>& words, size_t max_word_len) const {
    mp_seg_.Cut(sentence, words, max_word_len);
  }
  void CutSmall(const std::string& sentence, std::vector<Word>& words, size_t max_word_len) const {
    mp_seg_.Cut(sentence, words, max_word_len);
  }
  
  void Tag(const std::string& sentence, std::vector<std::pair<std::string, std::string> >& words) const {
    mix_seg_.Tag(sentence, words);
  }
  std::string LookupTag(const std::string &str) const {
    return mix_seg_.LookupTag(str);
  }
  bool InsertUserWord(const std::string& word, const std::string& tag = UNKNOWN_TAG) {
    return dict_trie_.InsertUserWord(word, tag);
  }

  bool InsertUserWord(const std::string& word,int freq, const std::string& tag = UNKNOWN_TAG) {
    return dict_trie_.InsertUserWord(word,freq, tag);
  }

  bool DeleteUserWord(const std::string& word, const std::string& tag = UNKNOWN_TAG) {
    return dict_trie_.DeleteUserWord(word, tag);
  }
  
  bool Find(const std::string& word)
  {
    return dict_trie_.Find(word);
  }

  void ResetSeparators(const std::string& s) {
    //TODO
    mp_seg_.ResetSeparators(s);
    hmm_seg_.ResetSeparators(s);
    mix_seg_.ResetSeparators(s);
    full_seg_.ResetSeparators(s);
    query_seg_.ResetSeparators(s);
  }

  const DictTrie* GetDictTrie() const {
    return &dict_trie_;
  } 
  
  const HMMModel* GetHMMModel() const {
    return &model_;
  }

  void LoadUserDict(const std::vector<std::string>& buf)  {
    dict_trie_.LoadUserDict(buf);
  }

  void LoadUserDict(const std::set<std::string>& buf)  {
    dict_trie_.LoadUserDict(buf);
  }

  void LoadUserDict(const std::string& path)  {
    dict_trie_.LoadUserDict(path);
  }

 private:
  static std::string pathJoin(const std::string& dir, const std::string& filename) {
    if (dir.empty()) {
        return filename;
    }
    
    char last_char = dir[dir.length() - 1];
    if (last_char == '/' || last_char == '\\') {
        return dir + filename;
    } else {
        #ifdef _WIN32
        return dir + '\\' + filename;
        #else
        return dir + '/' + filename;
        #endif
    }
  }

  static std::string getCurrentDirectory() {
    std::string path(__FILE__);
    size_t pos = path.find_last_of("/\\");
    return (pos == std::string::npos) ? "" : path.substr(0, pos);
  }

  static std::string getPath(const std::string& path, const std::string& default_file) {
    if (path.empty()) {
      std::string current_dir = getCurrentDirectory();
      std::string parent_dir = current_dir.substr(0, current_dir.find_last_of("/\\"));
      std::string grandparent_dir = parent_dir.substr(0, parent_dir.find_last_of("/\\"));
      return pathJoin(pathJoin(grandparent_dir, "dict"), default_file);
    }
    return path;
  }

  DictTrie dict_trie_;
  HMMModel model_;
  
  // They share the same dict trie and model
  MPSegment mp_seg_;
  HMMSegment hmm_seg_;
  MixSegment mix_seg_;
  FullSegment full_seg_;
  QuerySegment query_seg_;

 public:
  KeywordExtractor extractor;
}; // class Jieba

} // namespace cppjieba

#endif // CPPJIEAB_JIEBA_H
