#include <sstream>
#include <iostream>
#include <algorithm>
#include "parser.hh"

namespace AttributeParser 
{

TagMap parseTags(std::istream &in, uint16_t &numTags) {

  // Create empty tag map
  TagMap tags;
  
  //While there are lines left to process
  while (numTags) {
    // Extract line and decrement line count.
    std::string buf;
    std::getline(in, buf);
    --numTags;

    // If we are closing a tag, return the nested tags we've collected so far.
    if (std::string_view(buf).substr(0, 2) == "</") {
      return tags;
    }
    
    // Remove everything except string tokens. We should have tag attrName1 attrValue1 attrName2 attrValue2 ...
    buf.erase(std::remove(buf.begin(), buf.end(), '<'), buf.end());
    buf.erase(std::remove(buf.begin(), buf.end(), '\"'), buf.end());
    buf.erase(std::remove(buf.begin(), buf.end(), '='), buf.end());
    buf.erase(std::remove(buf.begin(), buf.end(), '>'), buf.end());

    // Now we can just stream out the tokens.
    std::stringstream bufStream{buf};
    std::string tagName;
    bufStream >> tagName;

    // If well formatted we go until we reach the last attribute token pair.
    AttrMap attributes;
    std::string attrName, attrValue;
    while (bufStream >> attrName >> attrValue) {
      attributes[attrName] = attrValue;
    }

    // This tag is the collection of attributes and either nested tags or nothing if we immediately have a closing tag.
    tags[tagName] = std::unique_ptr<Tag>(new Tag{attributes, parseTags(in, numTags)});
  }
  return tags;
}

Query parseQuery(std::istream &in) {
  std::string line_buffer;
  std::getline(in, line_buffer);
  std::string_view buffer_view{line_buffer};

  std::vector<std::string> tagNames;
  auto pos = buffer_view.find(".");
  while (pos != std::string::npos) {
    tagNames.emplace_back(buffer_view.substr(0, pos));
    buffer_view.remove_prefix(pos + 1);
    pos = buffer_view.find(".");
  }
  pos = buffer_view.find("~");
  tagNames.emplace_back(buffer_view.substr(0, pos));
  
  std::string attribute{buffer_view.substr(pos+1)};

  return {tagNames, attribute};
}

std::optional<std::string_view> getAttribute(Query const &query, TagMap const &tags) {
  TagMap const *pTags = &tags;
  Tag const *pCurTag = nullptr;
  for (auto tagName : query.tagNames) {
    auto it = pTags->find(tagName);
    if (it == tags.end()) {
        return {};
    }
    pCurTag = it->second.get();
    pTags = &pCurTag->tags;
  }

  auto it = pCurTag->attributes.find(query.attribute);
  if (it == pCurTag->attributes.end()) {
      return {};
  }
  return std::string_view{it->second};
}

void parseStream(std::istream &in) {

  std::string line_buffer;
  std::getline(in, line_buffer);
  std::stringstream line_stream(line_buffer);
  uint16_t numTags, numQueries;
  line_stream >> numTags >> numQueries;

  auto const tags = AttributeParser::parseTags(in, numTags);

  for (uint8_t ii = 0; ii < numQueries; ++ii) {
      auto const query = AttributeParser::parseQuery(in);
      auto const attribute = AttributeParser::getAttribute(query, tags);
      std::cout << attribute.value_or("Not Found!") << std::endl;
  }
}

}