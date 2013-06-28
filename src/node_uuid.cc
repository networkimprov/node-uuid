#include <node.h>
#include <v8.h>
#include <uuid/uuid.h>

using namespace v8;
using namespace node;

Handle<Value> GenerateStringLower(const Arguments& args) {
  HandleScope scope;
  uuid_t aId;
  char aIdStr[37];
  uuid_generate(aId);
  uuid_unparse_lower(aId, aIdStr);
  return scope.Close(String::New(aIdStr));
}


void init(Handle<Object> exports) {
 exports->Set(String::NewSymbol("generate_string_lower"), FunctionTemplate::New(GenerateStringLower)->GetFunction());
}

NODE_MODULE(node_uuid, init);
