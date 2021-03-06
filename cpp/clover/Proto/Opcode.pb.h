// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Opcode.proto

#ifndef PROTOBUF_Opcode_2eproto__INCLUDED
#define PROTOBUF_Opcode_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace pb {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Opcode_2eproto();
void protobuf_AssignDesc_Opcode_2eproto();
void protobuf_ShutdownFile_Opcode_2eproto();


enum Opcode {
  SMSG_UNKONW_ERROR = 1,
  CMSG_ENTER_ROOM = 2,
  SMSG_ENTER_ROOM = 3,
  CMSG_USER_MESSAGE = 4,
  SMSG_USER_MESSAGE = 5,
  CMSG_LEAVE_ROOM = 6,
  SMSG_LEAVE_ROOM = 7
};
bool Opcode_IsValid(int value);
const Opcode Opcode_MIN = SMSG_UNKONW_ERROR;
const Opcode Opcode_MAX = SMSG_LEAVE_ROOM;
const int Opcode_ARRAYSIZE = Opcode_MAX + 1;

const ::google::protobuf::EnumDescriptor* Opcode_descriptor();
inline const ::std::string& Opcode_Name(Opcode value) {
  return ::google::protobuf::internal::NameOfEnum(
    Opcode_descriptor(), value);
}
inline bool Opcode_Parse(
    const ::std::string& name, Opcode* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Opcode>(
    Opcode_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::pb::Opcode>() {
  return ::pb::Opcode_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Opcode_2eproto__INCLUDED
