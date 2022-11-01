#include <unordered_map>
#include <variant>
#include <string>

class Tag;

class Tag {
    public:
        using TagVariant = std::variant<Tag, std::string>;
    private:
        std::unordered_map<std::string, Tag::TagVariant> mTags;
};