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

unsigned char* BufferToUuid(Handle<Value> src) {
  return (unsigned char*)Buffer::Data(src);
}

Handle<Value> Clear(const Arguments& args) {
  if (args.Length() != 1 || !IsValidUuid(args[0]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID)")));
  uuid_clear(BufferToUuid(args[0]));
  return Undefined();
}

Handle<Value> Compare(const Arguments& args) {
  if (args.Length() != 2 || !IsValidUuid(args[0]) || !IsValidUuid(args[1]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID, UUID)")));
  HandleScope aScope;
  return aScope.Close(Integer::New(uuid_compare(BufferToUuid(args[0]), BufferToUuid(args[1]))));
}

Handle<Value> Copy(const Arguments& args) {
  if (args.Length() != 1 || !IsValidUuid(args[0]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID)")));
  HandleScope aScope;
  uuid_t aId;
  uuid_copy(aId, BufferToUuid(args[0]));
  return aScope.Close(UuidToBuffer(aId));
}

Handle<Value> Generate(const Arguments& args) {
  if (args.Length() != 0)
    return ThrowException(Exception::TypeError(String::New("arguments are ()")));
  HandleScope aScope;
  uuid_t aId;
  uuid_generate(aId);
  return aScope.Close(UuidToBuffer(aId));
}

Handle<Value> GenerateRandom(const Arguments& args) {
  if (args.Length() != 0)
    return ThrowException(Exception::TypeError(String::New("arguments are ()")));
  HandleScope aScope;
  uuid_t aId;
  uuid_generate_random(aId);
  return aScope.Close(UuidToBuffer(aId));
}

Handle<Value> GenerateTime(const Arguments& args) {
  if (args.Length() != 0)
    return ThrowException(Exception::TypeError(String::New("arguments are ()")));
  HandleScope aScope;
  uuid_t aId;
  uuid_generate_time(aId);
  return aScope.Close(UuidToBuffer(aId));
}

Handle<Value> IsNull(const Arguments& args) {
  if (args.Length() != 1 || !IsValidUuid(args[0]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID)")));
  HandleScope aScope;
  return aScope.Close(Boolean::New(uuid_is_null(BufferToUuid(args[0])) == 1));
}

Handle<Value> Time(const Arguments& args) {
  if (args.Length() != 1 || !IsValidUuid(args[0]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID)")));
  HandleScope aScope;
  struct timeval aTime;
  uuid_time(BufferToUuid(args[0]), &aTime);
  return aScope.Close(Date::New((double)aTime.tv_sec * 1000 + (double) aTime.tv_usec / 1000));
}

Handle<Value> Parse(const Arguments& args) {
  if (args.Length() != 1 || !args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID string)")));
  HandleScope aScope;
  uuid_t aId;
  String::Utf8Value aStr(args[0]);
  if (aStr.length() != 36 || uuid_parse(*aStr, aId) != 0)
    return ThrowException(Exception::TypeError(String::New("UUID string invalid")));
  return aScope.Close(UuidToBuffer(aId));
}

enum CaseType { eDefaultCase, eLowerCase, eUpperCase };

Handle<Value> UnparseCase(Handle<Value> id, CaseType c) {
  HandleScope aScope;
  char aIdStr[37];
  switch (c) {
  case eLowerCase:
    uuid_unparse_lower(BufferToUuid(id), aIdStr);
    break;
  case eUpperCase:
    uuid_unparse_upper(BufferToUuid(id), aIdStr);
    break;
  default:
    uuid_unparse(BufferToUuid(id), aIdStr);
  }
  return aScope.Close(String::New(aIdStr));
}

Handle<Value> Unparse(const Arguments& args) {
  if (args.Length() != 1 || !IsValidUuid(args[0]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID)")));
  return UnparseCase(args[0], eDefaultCase);
}

Handle<Value> UnparseLower(const Arguments& args) {
  if (args.Length() != 1 || !IsValidUuid(args[0]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID)")));
  return UnparseCase(args[0], eLowerCase);
}

Handle<Value> UnparseUpper(const Arguments& args) {
  if (args.Length() != 1 || !IsValidUuid(args[0]))
    return ThrowException(Exception::TypeError(String::New("arguments are (UUID)")));
  return UnparseCase(args[0], eUpperCase);
}

Handle<Value> GenerateStringCase(CaseType c) {
  HandleScope aScope;
  uuid_t aId;
  char aIdStr[37];
  uuid_generate(aId);
  switch (c) {
  case eLowerCase:
    uuid_unparse_lower(aId, aIdStr);
    break;
  case eUpperCase:
    uuid_unparse_upper(aId, aIdStr);
    break;
  default:
    uuid_unparse(aId, aIdStr);
  }
  return aScope.Close(String::New(aIdStr));
}

Handle<Value> GenerateStringLower(const Arguments& args) {
  if (args.Length() != 0)
    return ThrowException(Exception::TypeError(String::New("arguments are ()")));
  return GenerateStringCase(eLowerCase);
}

Handle<Value> GenerateStringUpper(const Arguments& args) {
  if (args.Length() != 0)
    return ThrowException(Exception::TypeError(String::New("arguments are ()")));
  return GenerateStringCase(eUpperCase);
}


void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("clear"),                 FunctionTemplate::New(Clear)->GetFunction());
  exports->Set(String::NewSymbol("compare"),               FunctionTemplate::New(Compare)->GetFunction());
  exports->Set(String::NewSymbol("copy"),                  FunctionTemplate::New(Copy)->GetFunction());
  exports->Set(String::NewSymbol("generate"),              FunctionTemplate::New(Generate)->GetFunction());
  exports->Set(String::NewSymbol("generate_random"),       FunctionTemplate::New(GenerateRandom)->GetFunction());
  exports->Set(String::NewSymbol("generate_time"),         FunctionTemplate::New(GenerateTime)->GetFunction());
  exports->Set(String::NewSymbol("is_null"),               FunctionTemplate::New(IsNull)->GetFunction());
  exports->Set(String::NewSymbol("time"),                  FunctionTemplate::New(Time)->GetFunction());
  exports->Set(String::NewSymbol("parse"),                 FunctionTemplate::New(Parse)->GetFunction());
  exports->Set(String::NewSymbol("unparse"),               FunctionTemplate::New(Unparse)->GetFunction());
  exports->Set(String::NewSymbol("unparse_lower"),         FunctionTemplate::New(UnparseLower)->GetFunction());
  exports->Set(String::NewSymbol("unparse_upper"),         FunctionTemplate::New(UnparseUpper)->GetFunction());
  exports->Set(String::NewSymbol("generate_string_lower"), FunctionTemplate::New(GenerateStringLower)->GetFunction());
  exports->Set(String::NewSymbol("generate_string_upper"), FunctionTemplate::New(GenerateStringUpper)->GetFunction());
}

NODE_MODULE(node_uuid, init);
