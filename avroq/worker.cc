#include <memory>
#include <iostream>

#include <avro/block.h>
#include <avro/blockdecoder.h>
#include <avro/eof.h>
#include <avro/finished.h>

#include <avro/codec/create.h>

#include <filter/filter.h>
#include <filter/equality_expression.h>
#include <filter/record_expression.h>

#include "fileemitor.h"

#include "worker.h"


Worker::Worker(FileEmitor &emitor) : emitor(emitor) {

}

void Worker::operator()() {

    avro::Block block;
    std::unique_ptr<avro::BlockDecoder> decoder;
    size_t fileId = -1;

    std::vector<uint8_t> storage;
    storage.resize(4 * 1024 * 1024);

    while (true) {

        try {

            auto task = emitor.getNextTask(decoder, fileId);

            if (!task) {
                break;
            }

            auto codec = avro::codec::createForHeader(*task->header);

            auto data = codec->decode(*task->buffer, storage);

            block.buffer.assignData(data);
            block.objectCount = task->objectCount;

            decoder->decodeAndDumpBlock(block);

        } catch (const avro::Eof &e) {
            ; // reading done
        } catch (const avro::Finished &e) {
        	emitor.finished();
            break;
        } catch(const std::runtime_error &e) {
        	emitor.finished();
            std::cerr << "Ooops! Something happened: " << e.what() << std::endl;
            break;
        }

    }
}
