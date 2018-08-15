//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ensemble_chorus.h"
#include <pugixml.hpp>
#include <sstream>
#include <cstdlib>
#include <cstring>

bool ensemble_chorus_load_parameters(const char *data, size_t length, float *parameters)
{
    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i)
        parameters[i] = ensemble_chorus_parameter_default((ec_parameter)i);

    pugi::xml_document doc;
    if (!doc.load_buffer(data, length))
        return false;

    pugi::xml_node root = doc.document_element();
    unsigned parameters_loaded = 0;

    for (pugi::xml_node p_node : root.children("parameter")) {
        const char *name = p_node.attribute("name").value();
        ec_parameter id = ensemble_chorus_parameter_by_name(name);
        if (id == (ec_parameter)-1)
            continue;
        const char *text = p_node.text().as_string();
        size_t textlen = std::strlen(text);
        //
        auto is_whitespace = [](char c) -> bool
            { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; };
        while (is_whitespace(text[0]))
            { ++text; --textlen; }
        while (textlen > 0 && is_whitespace(text[textlen - 1]))
            --textlen;
        std::string textbuf(text, text + textlen);
        text = textbuf.c_str();
        //
        float value;
        unsigned size;
        if (std::sscanf(text, "%f%n", &value, &size) != 1 || textlen != size)
            continue;
        parameters[id] = value;
        ++parameters_loaded;
    }

    return parameters_loaded > 0;
}

char *ensemble_chorus_save_parameters(size_t *length, const float *parameters)
{
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("preset");
    for (unsigned i = 0, n = ensemble_chorus_parameter_count(); i < n; ++i) {
        pugi::xml_node p_node = root.append_child("parameter");
        p_node.append_attribute("name")
            .set_value(ensemble_chorus_parameter_name((ec_parameter)i));
        char text[64];
        sprintf(text, "%g", parameters[i]);
        p_node.append_child(pugi::node_pcdata).set_value(text);
    }

    std::ostringstream stream(std::ios::binary);
    pugi::xml_writer_stream writer(stream);
    doc.save(writer);

    std::string result = stream.str();
    size_t result_size = result.size();

    char *result_cstr = (char *)std::malloc(result_size + 1);
    if (!result_cstr)
        throw std::bad_alloc();
    std::memcpy(result_cstr, result.data(), result_size);
    result_cstr[result_size] = '\0';

    *length = result_size;
    return result_cstr;
}
