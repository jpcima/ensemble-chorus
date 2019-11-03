#include "layout.h"
#include "reader.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Please indicate a fluid design file.\n";
        return 1;
    }

    Layout layout = read_file_layout(argv[1]);
    layout = flatten_layout(layout);

    std::cout << "namespace Widgets {\n";

    for (const LayoutItem &item : layout.items) {
        if (item.classname == "Function")
            continue;

        if (item.id.empty())
            continue;

        std::cout << "    DECL_IGNORABLE static Rect " << item.id << " = "
            "{" << item.x << ", " << item.y << ", " << item.w << ", " << item.h << "}"
            ";\n";
    }

    std::cout << "}\n";

    std::cout << "\n";

    std::cout << "namespace Labels {\n";

    for (const LayoutItem &item : layout.items) {
        if (item.id.empty() || item.label.empty())
            continue;

        std::cout << "    DECL_IGNORABLE static Label " << item.id << " = "
            "{\"" << item.label << "\", " << item.align << ", " <<
            "{" << item.x << ", " << item.y << ", " << item.w << ", " << item.h << "}"
            "};\n";
    }

    std::cout << "}\n";

    return 0;
}
