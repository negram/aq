#ifndef __avroq__blockdecoder__
#define __avroq__blockdecoder__

#include <memory>
#include <unordered_map>

#include "dumper/tsvexpression.h"
#include "predicate/list.h"


namespace filter {
    class Filter;
    struct equality_expression;
}
namespace avro {

struct Block;
struct header;
class DeflatedBuffer;
class Limiter;

namespace dumper {
    class Tsv;
}
namespace node {
    class Node;
}
namespace predicate {
    class Predicate;
    class List;
}

class BlockDecoder {
    friend class SkipArray;
    friend class ApplyArray;
    friend class ApplyTsvAsJson;
    friend class SkipMap;
    using parse_func_t = std::function<int(DeflatedBuffer &)>;
    using dump_tsv_func_t = std::function<int(DeflatedBuffer &, dumper::Tsv &t)>;
public:
    using const_node_t = const std::unique_ptr<node::Node>;

    BlockDecoder(const struct header &header, Limiter &limit);

    void decodeAndDumpBlock(Block &block);
    void setFilter(std::unique_ptr<filter::Filter> flt);
    void setTsvFilterExpression(const dumper::TsvExpression &tsvFieldsList);
    void setDumpMethod(std::function<void(const std::string &)> dumpMethod);
    void setCountMethod(std::function<void(size_t)> coutMethod);
    void enableCountOnlyMode();
    void enableParseLoop();
    void outputAsJson(bool pretty);

private:
    const struct header &header;
    Limiter &limit;
    dumper::TsvExpression tsvFieldsList;
    std::unique_ptr<predicate::List> predicates;

    std::function<void(const std::string &)> dumpMethod;
    std::function<void(size_t)> coutMethod;
    bool countOnly = false;
    bool parseLoopEnabled = false;
    std::vector<parse_func_t> parseLoop;
    std::vector<dump_tsv_func_t> tsvDumpLoop;
    bool jsonMode = false;
    bool jsonPrettyMode = false;

    void decodeDocument(DeflatedBuffer &stream, const std::unique_ptr<node::Node> &schema);

    void dumpDocument(Block &block);

    template <class T>
    void dumpDocument(DeflatedBuffer &stream, const std::unique_ptr<node::Node> &schema, T &dumper);

    template <typename T>
    void skipOrApplyFilter(DeflatedBuffer &stream, const std::unique_ptr<node::Node> &schema);

    template <typename T>
    typename T::result_type convertFilterConstant(const filter::equality_expression* expr, const node::Node *filterNode) const;

    int compileFilteringParser(std::vector<parse_func_t> &parse_items, const std::unique_ptr<node::Node> &schema, int elementsToSkip = 1);

    template <typename SkipType, typename ApplyType, typename... Args>
    void skipOrApplyCompileFilter(std::vector<parse_func_t> &parse_items, const std::unique_ptr<node::Node> &schema, int ret, Args... args);

    int compileTsvExpression(std::vector<dump_tsv_func_t> &parse_items, const std::unique_ptr<node::Node> &schema, int elementsToSkip = 1);

    template <typename SkipType, typename ApplyType, typename... Args>
    void skipOrApplyTsvExpression(std::vector<dump_tsv_func_t> &parse_items, const std::unique_ptr<node::Node> &schema, int ret, Args... args);
};


}

#endif
