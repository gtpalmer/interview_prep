#include <unordered_map>
#include <variant>
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <istream>
#include <utility>
#include <vector>

namespace AttributeParser 
{

struct Tag;

using AttrMap = std::unordered_map<std::string, std::string>;
using TagMap = std::unordered_map<std::string, std::unique_ptr<const Tag>>;

struct Tag {
  AttrMap attributes;
  TagMap tags;
};

struct Query {
  std::vector<std::string> tagNames;
  std::string attribute;
};

TagMap parseTags(std::istream &in, uint16_t &numTags);

Query parseQuery(std::istream &in);

std::optional<std::string_view> getAttribute(Query const &query, TagMap const &tags);

void parseStream(std::istream &in);

}
