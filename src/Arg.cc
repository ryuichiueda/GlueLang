// Copyright 2014 Ryuichi Ueda
// Released under the MIT License.
#include "Arg.h"
#include "Feeder.h"
#include "Environment.h"
using namespace std;

Arg::Arg(Feeder *f,Environment *env) : Element(f,env)
{
}

Arg::~Arg()
{
}
