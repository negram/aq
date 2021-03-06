#ifndef AVROQ_AVRO_SCHEMANODE_H_
#define AVROQ_AVRO_SCHEMANODE_H_

#include <string>
#include <memory>
#include <type_traits>

namespace avro {
namespace node {

class Node {
public:
    Node(int number, const std::string &typeName, const std::string &itemName);
    virtual ~Node();

    template<class T>
    inline bool is() const {
        return dynamic_cast<const T*>(this) != nullptr;
    }

    template<class T>
    inline bool isOneOf() const {
        return is<T>();
    }


    template<class T, class... Args>
    auto isOneOf() const ->
         typename std::enable_if< (0 < sizeof...(Args)), bool>::type
    {
        return is<T>() || isOneOf<Args...>();
    }

    template<class T>
    inline T& as() {
        return dynamic_cast<T&>(*this);
    }

    template<class T>
    inline const T& as() const {
        return dynamic_cast<const T&>(*this);
    }

    const std::string &getTypeName() const;

    const std::string &getItemName() const;

    int getNumber() const;

private:
    int number;
    std::string typeName;
    std::string itemName;
};

}
}

#endif /* AVROQ_AVRO_SCHEMANODE_H_ */
