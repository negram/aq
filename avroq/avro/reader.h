//
//  avroreader.h
//  avroq
//
//  Created by Mikhail Galanin on 12/12/14.
//  Copyright (c) 2014 Mikhail Galanin. All rights reserved.
//

#ifndef __avroq__avroreader__
#define __avroq__avroreader__

#include <string>
#include <cstdint>
#include <map>

#include <boost/iostreams/filtering_stream.hpp>

namespace avro {

class SchemaNode;

struct header {
    std::map<std::string, std::string> metadata;
    std::unique_ptr<SchemaNode> schema;
    char sync[16] = {0}; // TODO sync length to a constant
};

class Reader {
public:

    class Eof {};

    Reader(const std::string & filename);
    ~Reader();

    header readHeader();

    void readBlock(const header &header);

    bool eof();
private:

    class Private;
    Private *d = nullptr;

    int64_t readLong(std::istream &input);
    std::string readString(std::istream &input);
    float readFloat(std::istream &input);
    double readDouble(std::istream &input);
    bool readBoolean(std::istream &input);

    void dumpShema(const std::unique_ptr<SchemaNode> &schema, int level = 0) const;
    void decodeBlock(boost::iostreams::filtering_istream &stream, const std::unique_ptr<SchemaNode> &schema, int level = 0);
};

}

#endif /* defined(__avroq__avroreader__) */
