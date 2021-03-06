
#ifndef __avroq__filehandler__
#define __avroq__filehandler__

#include <string>
#include <memory>
#include <stdexcept>

#include "stringbuffer.h"

namespace avro {

class FileException : public std::runtime_error {
public:
    FileException(const std::string &msg);
};

class FileHandle {
public:
    explicit FileHandle(const std::string &filename);
    FileHandle(const FileHandle &) = delete;
    FileHandle() = delete;

    ~FileHandle();

    const std::string &fileName() const;

    std::unique_ptr<StringBuffer> mmapFile();
private:
    int fd = -1;
    int fileLength = 0;
    std::string filename;
    const char *mmappedFile = nullptr;
};

}

#endif /* defined(__avroq__filehandler__) */
