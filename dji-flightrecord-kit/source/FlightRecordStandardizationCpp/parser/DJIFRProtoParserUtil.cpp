#include "DJIFRProtoParserUtil.hpp"
#include "DJIFRProtoParser.hpp"
#include <unistd.h>
#include <google/protobuf/util/json_util.h>
#include <fstream>

template<typename... Args>
static std::string str_format(const std::string &format, Args... args) {
    auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

    if (!buf)
        return std::string("");

    std::snprintf(buf.get(), size_buf, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size_buf - 1);
}

int easy_flight_parser_to_file(const char *dji_txt_path, const char *sdk_key_c, const char *json_path) {

    std::string file_path(dji_txt_path);
    std::string sdk_key(sdk_key_c);

    auto parser = std::make_shared<DJIFRProto::Standard::Parser>();
    auto result = parser->load(file_path);
    if (result != DJIFRProto::Standard::Success) {
        return (int) result;
    }
    result = parser->startRequestParser(
            sdk_key,
            [&parser, &json_path](DJIFR::standardization::ServerError error_code,
                                  const std::string &error_description) {
                std::string json_result;
                if (error_code == DJIFR::standardization::ServerError::Success) {
                    std::shared_ptr <DJIFRProto::Standard::SummaryInformation> info = nullptr;
                    parser->summaryInformation(&info);

                    std::string summary_proto_json_string = "";
                    std::string info_proto_json_string = "";

                    google::protobuf::util::JsonPrintOptions options;
                    options.add_whitespace = true;
                    options.always_print_primitive_fields = true;
                    options.preserve_proto_field_names = true;

                    if (!google::protobuf::util::MessageToJsonString(*info, &summary_proto_json_string, options).ok()) {
                        summary_proto_json_string = "{}";
                    }

                    std::shared_ptr <DJIFRProto::Standard::FrameTimeStates> frame_time_list;
                    parser->frame_time_states(&frame_time_list);
                    if (!google::protobuf::util::MessageToJsonString(*frame_time_list, &info_proto_json_string,
                                                                     options).ok()) {
                        info_proto_json_string = "{}";
                    }

                    json_result = str_format("{\"code\": %d,\n \"summary\": %s,\n \"info\": %s \n}",
                                             (int) error_code, summary_proto_json_string.c_str(),
                                             info_proto_json_string.c_str());

                } else {
                    json_result = str_format("{\"code\": %d,\n \"message\": \"%s\"\n}", (int) error_code,
                                             error_description.c_str());
                }

                std::fstream f;
                f.open(json_path, std::ios::out);
                f << json_result;
                f.close();
            });
    return (int) result;


}


