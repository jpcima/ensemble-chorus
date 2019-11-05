#include "reader.h"
#include <boost/tokenizer.hpp>
#include <iostream>
#include <fstream>

typedef std::vector<std::string> TokenList;
static bool read_file_tokens(const char *filename, TokenList &tokens);
static Layout read_tokens_layout(TokenList::iterator &tok_it, TokenList::iterator tok_end);

Layout read_file_layout(const char *filename)
{
    std::vector<std::string> tokens;
    if (!read_file_tokens(filename, tokens))
        throw std::runtime_error("Cannot read fluid design file.");

    TokenList::iterator tok_it = tokens.begin();
    TokenList::iterator tok_end = tokens.end();
    return read_tokens_layout(tok_it, tok_end);
}

static std::string consume_next_token(TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    if (tok_it == tok_end)
        throw file_format_error("Premature end of tokens");
    return *tok_it++;
}

static bool try_consume_next_token(const char *text, TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    if (tok_it == tok_end)
        return false;

    if (*tok_it != text)
        return false;

    ++tok_it;
    return true;
}

static void ensure_next_token(const char *text, TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    std::string tok = consume_next_token(tok_it, tok_end);
    if (tok != text)
        throw file_format_error("Unexpected token: " + tok);
}

static std::string consume_enclosed_string(TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    ensure_next_token("{", tok_it, tok_end);
    unsigned depth = 1;

    std::string text;
    for (;;) {
        std::string part = consume_next_token(tok_it, tok_end);
        if (part == "}") {
            if (--depth == 0)
                return text;
        }
        else if (part == "{")
            ++depth;
        if (!text.empty())
            text.push_back(' ');
        text.append(part);
    }

    return text;
}

static std::string consume_any_string(TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    if (tok_it != tok_end && *tok_it == "{")
        return consume_enclosed_string(tok_it, tok_end);
    else
        return consume_next_token(tok_it, tok_end);
}

static int consume_int_token(TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    std::string text = consume_next_token(tok_it, tok_end);
    return std::stoi(text);
}

static int consume_real_token(TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    std::string text = consume_next_token(tok_it, tok_end);
    return std::stod(text);
}

static void consume_image_properties(LayoutImage &image, TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    for (bool have = true; have;) {
        if (try_consume_next_token("xywh", tok_it, tok_end)) {
            ensure_next_token("{", tok_it, tok_end);
            image.x = consume_int_token(tok_it, tok_end);
            image.y = consume_int_token(tok_it, tok_end);
            image.w = consume_int_token(tok_it, tok_end);
            image.h = consume_int_token(tok_it, tok_end);
            ensure_next_token("}", tok_it, tok_end);
        }
        else
            have = false;
    }
}

static void consume_layout_item_properties(LayoutItem &item, TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    ensure_next_token("{", tok_it, tok_end);
    for (bool have = true; have;) {
        if (try_consume_next_token("open", tok_it, tok_end))
            ; // skip
        else if (try_consume_next_token("selected", tok_it, tok_end))
            ; // skip
        else if (try_consume_next_token("label", tok_it, tok_end))
            item.label = consume_any_string(tok_it, tok_end);
        else if (try_consume_next_token("xywh", tok_it, tok_end)) {
            ensure_next_token("{", tok_it, tok_end);
            item.x = consume_int_token(tok_it, tok_end);
            item.y = consume_int_token(tok_it, tok_end);
            item.w = consume_int_token(tok_it, tok_end);
            item.h = consume_int_token(tok_it, tok_end);
            ensure_next_token("}", tok_it, tok_end);
        }
        else if (try_consume_next_token("box", tok_it, tok_end))
            item.box = consume_next_token(tok_it, tok_end);
        else if (try_consume_next_token("down_box", tok_it, tok_end))
            item.down_box = consume_next_token(tok_it, tok_end);
        else if (try_consume_next_token("labelfont", tok_it, tok_end))
            item.labelfont = consume_int_token(tok_it, tok_end);
        else if (try_consume_next_token("labelsize", tok_it, tok_end))
            item.labelsize = consume_int_token(tok_it, tok_end);
        else if (try_consume_next_token("labeltype", tok_it, tok_end))
            item.labeltype = consume_any_string(tok_it, tok_end);
        else if (try_consume_next_token("align", tok_it, tok_end))
            item.align = consume_int_token(tok_it, tok_end);
        else if (try_consume_next_token("type", tok_it, tok_end))
            item.type = consume_any_string(tok_it, tok_end);
        else if (try_consume_next_token("callback", tok_it, tok_end))
            item.callback = consume_any_string(tok_it, tok_end);
        else if (try_consume_next_token("class", tok_it, tok_end))
            item.classname = consume_any_string(tok_it, tok_end);
        else if (try_consume_next_token("minimum", tok_it, tok_end))
            item.minimum = consume_real_token(tok_it, tok_end);
        else if (try_consume_next_token("maximum", tok_it, tok_end))
            item.maximum = consume_real_token(tok_it, tok_end);
        else if (try_consume_next_token("step", tok_it, tok_end))
            item.step = consume_real_token(tok_it, tok_end);
        else if (try_consume_next_token("image", tok_it, tok_end)) {
            item.image.filepath = consume_any_string(tok_it, tok_end);
            consume_image_properties(item.image, tok_it, tok_end);
        }
        else if (try_consume_next_token("visible", tok_it, tok_end))
            /* skip */;
        else
            have = false;
    }
    ensure_next_token("}", tok_it, tok_end);
}

static void set_default_alignment(LayoutItem &item)
{
    static const char *valuator_classnames[] = {
        "Fl_Adjuster",
        "Fl_Counter",
        "Fl_Dial",
        "Fl_Fill_Slider",
        "Fl_Hor_Fill_Slider",
        "Fl_Hor_Nice_Slider",
        "Fl_Hor_Slider",
        "Fl_Nice_Slider",
        "Fl_Roller",
        "Fl_Scrollbar",
        "Fl_Simple_Counter",
        "Fl_Slider",
        "Fl_Value_Input",
        "Fl_Value_Output",
        "Fl_Value_Slider",
    };

    for (const char *valuator_classname : valuator_classnames) {
        if (item.baseclassname == valuator_classname) {
            item.align = 2; // default alignment: bottom
            return;
        }
    }
}

static LayoutItem consume_layout_item(const std::string &classname, TokenList::iterator &tok_it, TokenList::iterator tok_end, bool anonymous = false)
{
    LayoutItem item;
    item.baseclassname = classname;
    item.classname = classname;
    if (!anonymous)
        item.id = consume_any_string(tok_it, tok_end);
    set_default_alignment(item);
    consume_layout_item_properties(item, tok_it, tok_end);
    if (tok_it != tok_end && *tok_it == "{") {
        consume_next_token(tok_it, tok_end);
        for (std::string text; (text = consume_next_token(tok_it, tok_end)) != "}";) {
            if (text == "decl") {
                consume_any_string(tok_it, tok_end);
                consume_any_string(tok_it, tok_end);
            }
            else if (text == "Function") {
                consume_any_string(tok_it, tok_end);
                consume_any_string(tok_it, tok_end);
                consume_any_string(tok_it, tok_end);
            }
            else
                item.items.push_back(consume_layout_item(text, tok_it, tok_end));
        }
    }
    return item;
}

static Layout read_tokens_layout(TokenList::iterator &tok_it, TokenList::iterator tok_end)
{
    Layout layout;

    std::string version_name;
    std::string header_name;
    std::string code_name;

    while (tok_it != tok_end) {
        std::string key = consume_next_token(tok_it, tok_end);

        if (key == "version")
            version_name = consume_next_token(tok_it, tok_end);
        else if (key == "header_name") {
            ensure_next_token("{", tok_it, tok_end);
            header_name = consume_next_token(tok_it, tok_end);
            ensure_next_token("}", tok_it, tok_end);
        }
        else if (key == "code_name") {
            ensure_next_token("{", tok_it, tok_end);
            code_name = consume_next_token(tok_it, tok_end);
            ensure_next_token("}", tok_it, tok_end);
        }
        else if (key == "decl") {
            consume_any_string(tok_it, tok_end);
            consume_any_string(tok_it, tok_end);
        }
        else if (key == "widget_class") {
            key = consume_next_token(tok_it, tok_end);
            layout.items.push_back(consume_layout_item(key, tok_it, tok_end, true));
        }
        else
            layout.items.push_back(consume_layout_item(key, tok_it, tok_end));
    }

    return layout;
}

static bool read_file_tokens(const char *filename, TokenList &tokens)
{
    std::ifstream stream(filename);
    std::string line;

    std::string text;
    while (std::getline(stream, line)) {
        if (!line.empty() && line[0] != '#') {
            text.append(line);
            text.push_back('\n');
        }
    }

    if (stream.bad())
        return false;

    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    tokenizer tok(text, boost::char_separator<char>(" \t\r\n", "{}"));
    for (tokenizer::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
        tokens.push_back(*tok_iter);

    return !stream.bad();
}
