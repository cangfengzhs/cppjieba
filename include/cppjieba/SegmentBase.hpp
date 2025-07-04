#ifndef CPPJIEBA_SEGMENTBASE_H
#define CPPJIEBA_SEGMENTBASE_H

#include "limonp/Logging.hpp"
#include "PreFilter.hpp"
#include <cassert>


namespace cppjieba {

const char* const SPECIAL_SEPARATORS = " \t\n\xEF\xBC\x8C\xE3\x80\x82";

class SegmentBase {
 public:
  SegmentBase() {
    XCHECK(ResetSeparators(SPECIAL_SEPARATORS));
  }
  virtual ~SegmentBase() {
  }

  virtual void Cut(const std::string& sentence, std::vector<std::string>& words) const = 0;

  bool ResetSeparators(const std::string& s) {
    symbols_.clear();
    RuneStrArray runes;
    if (!DecodeUTF8RunesInString(s, runes)) {
      XLOG(ERROR) << "UTF-8 decode failed for separators: " << s;
      return false;
    }
    for (size_t i = 0; i < runes.size(); i++) {
      if (!symbols_.insert(runes[i].rune).second) {
        XLOG(ERROR) << s.substr(runes[i].offset, runes[i].len) << " already exists";
        return false;
      }
    }
    return true;
  }
 protected:
  std::unordered_set<Rune> symbols_;
}; // class SegmentBase

} // cppjieba

#endif
