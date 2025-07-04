#include "gtest/gtest.h"
#include "cppjieba/PreFilter.hpp"
#include "limonp/StringUtil.hpp"

using namespace cppjieba;

TEST(PreFilterTest, Test1) {
  std::unordered_set<Rune> symbol;
  symbol.insert(65292u); // "，"
  symbol.insert(12290u); // "。"
  std::string expected;
  std::string res;

  {
    std::string s = "你好，美丽的，世界";
    PreFilter filter(symbol, s);
    expected = "你好/，/美丽的/，/世界";
    ASSERT_TRUE(filter.HasNext());
    std::vector<std::string> words;
    while (filter.HasNext()) {
      PreFilter::Range range;
      range = filter.Next();
      words.push_back(GetStringFromRunes(s, range.begin, range.end - 1));
    }
    res = limonp::Join(words.begin(), words.end(), "/");
    ASSERT_EQ(res, expected);
  }

  {
    std::string s = "我来自北京邮电大学。。。学号123456，用AK47";
    PreFilter filter(symbol, s);
    expected = "我来自北京邮电大学/。/。/。/学号123456/，/用AK47";
    ASSERT_TRUE(filter.HasNext());
    std::vector<std::string> words;
    while (filter.HasNext()) {
      PreFilter::Range range;
      range = filter.Next();
      words.push_back(GetStringFromRunes(s, range.begin, range.end - 1));
    }
    res = limonp::Join(words.begin(), words.end(), "/");
    ASSERT_EQ(res, expected);
  }
}
