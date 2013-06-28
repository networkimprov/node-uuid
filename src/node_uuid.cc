#include <node.h>
#include <v8.h>
#include <uuid/uuid.h>
#include <node_buffer.h>
#include <cstring>

using namespace v8;
using namespace node;

Handle<Value> UuidToBuffer(uuid_t uu) {
  return Buffer::New((const char *)uu, 16)->handle_;
}

bool IsValidUuid(Handle<Value> uu) {
  return (Buffer::HasInstance(uu) && Buffer::Length(uu) == 16);
}

void BufferToUuid(Handle<Value> src, uuid_t &dst) {
  memcpy(dst, Buffer::Data(src), 16);
}


Handle<Value> GenerateStringLower(const Arguments& args) {
  if (args.Length() != 0)
    return ThrowException(Exception::TypeError(String::New("arguments are ()")));
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
