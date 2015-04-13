//
//  int.cc
//  avroq
//
//  Created by Mikhail Galanin on 04/04/15.
//  Copyright (c) 2015 Mikhail Galanin. All rights reserved.
//

#include "int.h"

namespace avro {
namespace node {

Int::Int(int number, const std::string &name) : Node(number, "int", name) {
}

}
}