#ifndef DgDJIFRProtoParserUtil_hpp
#define DgDJIFRProtoParserUtil_hpp

#include "iostream"

#ifdef __cplusplus
extern "C"
{
#endif


int easy_flight_parser_to_file(const char *dji_txt_path, const char *sdk_key_c, const char *json_path);

//int easy_flight_parser_char(const char *file_path_c, const char *sdk_key_c, char* output_c);
//
//int dg_flight_parser(const std::string &file_path, const std::string &sdk_key, std::string &output);

// int easy_test_link_lib();

#ifdef __cplusplus
}
#endif

#endif /* DgDJIFRProtoParserUtil_hpp */

