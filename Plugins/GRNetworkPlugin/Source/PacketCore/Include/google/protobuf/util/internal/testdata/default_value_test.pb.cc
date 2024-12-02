// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/util/internal/testdata/default_value_test.proto

#include "google/protobuf/util/internal/testdata/default_value_test.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace proto_util_converter {
namespace testing {
constexpr DefaultValueTest::DefaultValueTest(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : repeated_double_()
  , string_value_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , bytes_value_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , double_value_(0)
  , int64_value_(int64_t{0})
  , float_value_(0)
  , int32_value_(0)
  , uint64_value_(uint64_t{0u})
  , uint32_value_(0u)
  , bool_value_(false)
  , enum_value_(0)
{}
struct DefaultValueTestDefaultTypeInternal {
  constexpr DefaultValueTestDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~DefaultValueTestDefaultTypeInternal() {}
  union {
    DefaultValueTest _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT DefaultValueTestDefaultTypeInternal _DefaultValueTest_default_instance_;
}  // namespace testing
}  // namespace proto_util_converter
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto = nullptr;

const uint32_t TableStruct_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, double_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, repeated_double_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, float_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, int64_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, uint64_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, int32_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, uint32_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, bool_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, string_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, bytes_value_),
  PROTOBUF_FIELD_OFFSET(::proto_util_converter::testing::DefaultValueTest, enum_value_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::proto_util_converter::testing::DefaultValueTest)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::proto_util_converter::testing::_DefaultValueTest_default_instance_),
};

const char descriptor_table_protodef_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\?google/protobuf/util/internal/testdata"
  "/default_value_test.proto\022\034proto_util_co"
  "nverter.testing\"\377\002\n\020DefaultValueTest\022\024\n\014"
  "double_value\030\001 \001(\001\022\027\n\017repeated_double\030\002 "
  "\003(\001\022\023\n\013float_value\030\003 \001(\002\022\023\n\013int64_value\030"
  "\005 \001(\003\022\024\n\014uint64_value\030\007 \001(\004\022\023\n\013int32_val"
  "ue\030\t \001(\005\022\024\n\014uint32_value\030\013 \001(\r\022\022\n\nbool_v"
  "alue\030\r \001(\010\022\024\n\014string_value\030\017 \001(\t\022\027\n\013byte"
  "s_value\030\021 \001(\014B\002\010\001\022N\n\nenum_value\030\022 \001(\0162:."
  "proto_util_converter.testing.DefaultValu"
  "eTest.EnumDefault\">\n\013EnumDefault\022\016\n\nENUM"
  "_FIRST\020\000\022\017\n\013ENUM_SECOND\020\001\022\016\n\nENUM_THIRD\020"
  "\002b\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto = {
  false, false, 489, descriptor_table_protodef_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto, "google/protobuf/util/internal/testdata/default_value_test.proto", 
  &descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto::offsets,
  file_level_metadata_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto, file_level_enum_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto, file_level_service_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_getter() {
  return &descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto(&descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto);
namespace proto_util_converter {
namespace testing {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* DefaultValueTest_EnumDefault_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto);
  return file_level_enum_descriptors_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[0];
}
bool DefaultValueTest_EnumDefault_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))
constexpr DefaultValueTest_EnumDefault DefaultValueTest::ENUM_FIRST;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::ENUM_SECOND;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::ENUM_THIRD;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::EnumDefault_MIN;
constexpr DefaultValueTest_EnumDefault DefaultValueTest::EnumDefault_MAX;
constexpr int DefaultValueTest::EnumDefault_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || (_MSC_VER >= 1900 && _MSC_VER < 1912))

// ===================================================================

class DefaultValueTest::_Internal {
 public:
};

DefaultValueTest::DefaultValueTest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned),
  repeated_double_(arena) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:proto_util_converter.testing.DefaultValueTest)
}
DefaultValueTest::DefaultValueTest(const DefaultValueTest& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      repeated_double_(from.repeated_double_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  string_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    string_value_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_string_value().empty()) {
    string_value_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_string_value(), 
      GetArenaForAllocation());
  }
  bytes_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    bytes_value_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_bytes_value().empty()) {
    bytes_value_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_bytes_value(), 
      GetArenaForAllocation());
  }
  ::memcpy(&double_value_, &from.double_value_,
    static_cast<size_t>(reinterpret_cast<char*>(&enum_value_) -
    reinterpret_cast<char*>(&double_value_)) + sizeof(enum_value_));
  // @@protoc_insertion_point(copy_constructor:proto_util_converter.testing.DefaultValueTest)
}

inline void DefaultValueTest::SharedCtor() {
string_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  string_value_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
bytes_value_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  bytes_value_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&double_value_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&enum_value_) -
    reinterpret_cast<char*>(&double_value_)) + sizeof(enum_value_));
}

DefaultValueTest::~DefaultValueTest() {
  // @@protoc_insertion_point(destructor:proto_util_converter.testing.DefaultValueTest)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void DefaultValueTest::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  string_value_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  bytes_value_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void DefaultValueTest::ArenaDtor(void* object) {
  DefaultValueTest* _this = reinterpret_cast< DefaultValueTest* >(object);
  (void)_this;
}
void DefaultValueTest::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void DefaultValueTest::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void DefaultValueTest::Clear() {
// @@protoc_insertion_point(message_clear_start:proto_util_converter.testing.DefaultValueTest)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  repeated_double_.Clear();
  string_value_.ClearToEmpty();
  bytes_value_.ClearToEmpty();
  ::memset(&double_value_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&enum_value_) -
      reinterpret_cast<char*>(&double_value_)) + sizeof(enum_value_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* DefaultValueTest::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // double double_value = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 9)) {
          double_value_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      // repeated double repeated_double = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedDoubleParser(_internal_mutable_repeated_double(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<uint8_t>(tag) == 17) {
          _internal_add_repeated_double(::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr));
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      // float float_value = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 29)) {
          float_value_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else
          goto handle_unusual;
        continue;
      // int64 int64_value = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 40)) {
          int64_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint64 uint64_value = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 56)) {
          uint64_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 int32_value = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 72)) {
          int32_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint32 uint32_value = 11;
      case 11:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 88)) {
          uint32_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bool bool_value = 13;
      case 13:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 104)) {
          bool_value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string string_value = 15;
      case 15:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 122)) {
          auto str = _internal_mutable_string_value();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "proto_util_converter.testing.DefaultValueTest.string_value"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes bytes_value = 17 [ctype = CORD];
      case 17:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 138)) {
          auto str = _internal_mutable_bytes_value();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
      case 18:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 144)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_enum_value(static_cast<::proto_util_converter::testing::DefaultValueTest_EnumDefault>(val));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* DefaultValueTest::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto_util_converter.testing.DefaultValueTest)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // double double_value = 1;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_double_value = this->_internal_double_value();
  uint64_t raw_double_value;
  memcpy(&raw_double_value, &tmp_double_value, sizeof(tmp_double_value));
  if (raw_double_value != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(1, this->_internal_double_value(), target);
  }

  // repeated double repeated_double = 2;
  if (this->_internal_repeated_double_size() > 0) {
    target = stream->WriteFixedPacked(2, _internal_repeated_double(), target);
  }

  // float float_value = 3;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_float_value = this->_internal_float_value();
  uint32_t raw_float_value;
  memcpy(&raw_float_value, &tmp_float_value, sizeof(tmp_float_value));
  if (raw_float_value != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(3, this->_internal_float_value(), target);
  }

  // int64 int64_value = 5;
  if (this->_internal_int64_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(5, this->_internal_int64_value(), target);
  }

  // uint64 uint64_value = 7;
  if (this->_internal_uint64_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(7, this->_internal_uint64_value(), target);
  }

  // int32 int32_value = 9;
  if (this->_internal_int32_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(9, this->_internal_int32_value(), target);
  }

  // uint32 uint32_value = 11;
  if (this->_internal_uint32_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(11, this->_internal_uint32_value(), target);
  }

  // bool bool_value = 13;
  if (this->_internal_bool_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(13, this->_internal_bool_value(), target);
  }

  // string string_value = 15;
  if (!this->_internal_string_value().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_string_value().data(), static_cast<int>(this->_internal_string_value().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "proto_util_converter.testing.DefaultValueTest.string_value");
    target = stream->WriteStringMaybeAliased(
        15, this->_internal_string_value(), target);
  }

  // bytes bytes_value = 17 [ctype = CORD];
  if (!this->_internal_bytes_value().empty()) {
    target = stream->WriteBytesMaybeAliased(
        17, this->_internal_bytes_value(), target);
  }

  // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
  if (this->_internal_enum_value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      18, this->_internal_enum_value(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto_util_converter.testing.DefaultValueTest)
  return target;
}

size_t DefaultValueTest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto_util_converter.testing.DefaultValueTest)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated double repeated_double = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->_internal_repeated_double_size());
    size_t data_size = 8UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<int32_t>(data_size));
    }
    total_size += data_size;
  }

  // string string_value = 15;
  if (!this->_internal_string_value().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_string_value());
  }

  // bytes bytes_value = 17 [ctype = CORD];
  if (!this->_internal_bytes_value().empty()) {
    total_size += 2 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_bytes_value());
  }

  // double double_value = 1;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_double_value = this->_internal_double_value();
  uint64_t raw_double_value;
  memcpy(&raw_double_value, &tmp_double_value, sizeof(tmp_double_value));
  if (raw_double_value != 0) {
    total_size += 1 + 8;
  }

  // int64 int64_value = 5;
  if (this->_internal_int64_value() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64SizePlusOne(this->_internal_int64_value());
  }

  // float float_value = 3;
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_float_value = this->_internal_float_value();
  uint32_t raw_float_value;
  memcpy(&raw_float_value, &tmp_float_value, sizeof(tmp_float_value));
  if (raw_float_value != 0) {
    total_size += 1 + 4;
  }

  // int32 int32_value = 9;
  if (this->_internal_int32_value() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32SizePlusOne(this->_internal_int32_value());
  }

  // uint64 uint64_value = 7;
  if (this->_internal_uint64_value() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64SizePlusOne(this->_internal_uint64_value());
  }

  // uint32 uint32_value = 11;
  if (this->_internal_uint32_value() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32SizePlusOne(this->_internal_uint32_value());
  }

  // bool bool_value = 13;
  if (this->_internal_bool_value() != 0) {
    total_size += 1 + 1;
  }

  // .proto_util_converter.testing.DefaultValueTest.EnumDefault enum_value = 18;
  if (this->_internal_enum_value() != 0) {
    total_size += 2 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_enum_value());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData DefaultValueTest::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    DefaultValueTest::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*DefaultValueTest::GetClassData() const { return &_class_data_; }

void DefaultValueTest::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<DefaultValueTest *>(to)->MergeFrom(
      static_cast<const DefaultValueTest &>(from));
}


void DefaultValueTest::MergeFrom(const DefaultValueTest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto_util_converter.testing.DefaultValueTest)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  repeated_double_.MergeFrom(from.repeated_double_);
  if (!from._internal_string_value().empty()) {
    _internal_set_string_value(from._internal_string_value());
  }
  if (!from._internal_bytes_value().empty()) {
    _internal_set_bytes_value(from._internal_bytes_value());
  }
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_double_value = from._internal_double_value();
  uint64_t raw_double_value;
  memcpy(&raw_double_value, &tmp_double_value, sizeof(tmp_double_value));
  if (raw_double_value != 0) {
    _internal_set_double_value(from._internal_double_value());
  }
  if (from._internal_int64_value() != 0) {
    _internal_set_int64_value(from._internal_int64_value());
  }
  static_assert(sizeof(uint32_t) == sizeof(float), "Code assumes uint32_t and float are the same size.");
  float tmp_float_value = from._internal_float_value();
  uint32_t raw_float_value;
  memcpy(&raw_float_value, &tmp_float_value, sizeof(tmp_float_value));
  if (raw_float_value != 0) {
    _internal_set_float_value(from._internal_float_value());
  }
  if (from._internal_int32_value() != 0) {
    _internal_set_int32_value(from._internal_int32_value());
  }
  if (from._internal_uint64_value() != 0) {
    _internal_set_uint64_value(from._internal_uint64_value());
  }
  if (from._internal_uint32_value() != 0) {
    _internal_set_uint32_value(from._internal_uint32_value());
  }
  if (from._internal_bool_value() != 0) {
    _internal_set_bool_value(from._internal_bool_value());
  }
  if (from._internal_enum_value() != 0) {
    _internal_set_enum_value(from._internal_enum_value());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void DefaultValueTest::CopyFrom(const DefaultValueTest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto_util_converter.testing.DefaultValueTest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DefaultValueTest::IsInitialized() const {
  return true;
}

void DefaultValueTest::InternalSwap(DefaultValueTest* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  repeated_double_.InternalSwap(&other->repeated_double_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &string_value_, lhs_arena,
      &other->string_value_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &bytes_value_, lhs_arena,
      &other->bytes_value_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(DefaultValueTest, enum_value_)
      + sizeof(DefaultValueTest::enum_value_)
      - PROTOBUF_FIELD_OFFSET(DefaultValueTest, double_value_)>(
          reinterpret_cast<char*>(&double_value_),
          reinterpret_cast<char*>(&other->double_value_));
}

::PROTOBUF_NAMESPACE_ID::Metadata DefaultValueTest::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_getter, &descriptor_table_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto_once,
      file_level_metadata_google_2fprotobuf_2futil_2finternal_2ftestdata_2fdefault_5fvalue_5ftest_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace testing
}  // namespace proto_util_converter
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto_util_converter::testing::DefaultValueTest* Arena::CreateMaybeMessage< ::proto_util_converter::testing::DefaultValueTest >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto_util_converter::testing::DefaultValueTest >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>