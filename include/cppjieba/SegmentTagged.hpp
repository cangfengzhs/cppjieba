#ifndef CPPJIEBA_SEGMENTTAGGED_H
#define CPPJIEBA_SEGMENTTAGGED_H

#include "SegmentBase.hpp"

namespace cppjieba {

class SegmentTagged : public SegmentBase{
 public:
  SegmentTagged() {
  }
  virtual ~SegmentTagged() {
  }

  virtual bool Tag(const std::string& src, std::vector<pair<std::string, std::string> >& res) const = 0;

  virtual const DictTrie* GetDictTrie() const = 0;

}; // class SegmentTagged

} // cppjieba

#endif
