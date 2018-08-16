//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "main_controller.h"
#include "message_queue.h"
#include "messages.h"
#include "common.h"
#include "utility/stdc_memory.h"
#include <ensemble_chorus.h>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_utf8.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <sys/stat.h>

Main_Controller::Main_Controller(Message_Queue &mq_in, Message_Queue &mq_out)
    : mq_in_(mq_in), mq_out_(mq_out),
      msg_buffer_(Messages::allocate_buffer()),
      parameter_value_(new float[ensemble_chorus_parameter_count()])
{
}

void Main_Controller::request_parameters()
{
    Messages::RequestNotifyParameters msg;
    mq_out_.write_message_retrying(msg, std::chrono::milliseconds(1));
}

void Main_Controller::handle_messages()
{
    Message_Queue &mq_in = mq_in_;

    auto &msg = *reinterpret_cast<Basic_Message *>(msg_buffer_.get());
    while (mq_in.read_message(msg))
        process_message(msg);
}

void Main_Controller::set_parameter_callback(void (*callback)(unsigned, float, void *), void *userdata)
{
    parameter_callback_ = callback;
    parameter_userdata_ = userdata;
}

void Main_Controller::set_modulation_callback(void (*callback)(const float[6], const float[6], void *), void *userdata)
{
    modulation_callback_ = callback;
    modulation_userdata_ = userdata;
}

void Main_Controller::send_parameter(unsigned id, double value)
{
    Messages::RequestSetParameter msg;
    msg.id = id;
    msg.value = value;
    mq_out_.write_message_retrying(msg, std::chrono::milliseconds(1));
    parameter_value_[id] = value;
}

bool Main_Controller::load_parameters()
{
    Fl_Native_File_Chooser chooser(Fl_Native_File_Chooser::BROWSE_FILE);
    chooser.title("Load...");
    chooser.filter("Preset\t*.ecp");

    int ret = chooser.show();
    if (ret != 0)
        return ret == 1;

    const char *filename = chooser.filename();
    FILE_u stream(fl_fopen(filename, "rb"));
    if (!stream)
        return false;

    struct stat st;
    if (fstat(fileno(stream.get()), &st) != 0)
        return false;

    size_t filesize = st.st_size;
    constexpr size_t filesize_max = 512 * 1024;
    if (filesize > filesize_max)
        return false;

    std::unique_ptr<char[]> filedata(new char[filesize]);
    if (fread(filedata.get(), 1, filesize, stream.get()) != filesize)
        return false;

    unsigned parameter_count = ensemble_chorus_parameter_count();
    std::unique_ptr<float[]> parameter(new float[parameter_count]);
    if (!ensemble_chorus_load_parameters(filedata.get(), filesize, parameter.get()))
        return false;

    for (unsigned i = 0; i < parameter_count; ++i)
        send_parameter(i, parameter[i]);

    request_parameters();
    return true;
}

bool Main_Controller::save_parameters()
{
    Fl_Native_File_Chooser chooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    chooser.title("Save...");
    chooser.filter("Preset\t*.ecp");
    chooser.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM|Fl_Native_File_Chooser::USE_FILTER_EXT);

    int ret = chooser.show();
    if (ret != 0)
        return ret == 1;

    size_t length;
    stdc_unique_ptr<char[]> text(ensemble_chorus_save_parameters(&length, parameter_value_.get()));
    if (!text)
        return false;

    const char *filename = chooser.filename();
    FILE_u stream(fl_fopen(filename, "wb"));
    if (!stream)
        return false;

    if (fwrite(text.get(), length, 1, stream.get()) != 1 || fflush(stream.get()) != 0) {
        fl_unlink(filename);
        return false;
    }

    return true;
}

void Main_Controller::process_message(const Basic_Message &msg)
{
    switch (msg.tag) {
    case Message_Tag::NotifyParameter: {
        const auto &body = reinterpret_cast<const Messages::NotifyParameter &>(msg);
        load_parameter(body.id, body.value);
        parameter_value_[body.id] = body.value;
        break;
    }
    case Message_Tag::NotifyModulation: {
        const auto &body = reinterpret_cast<const Messages::NotifyModulation &>(msg);
        if (modulation_callback_)
            modulation_callback_(body.slow, body.fast, modulation_userdata_);
        break;
    }
    default:
        assert(false);
    }
}

void Main_Controller::load_parameter(unsigned id, float value)
{
    if (parameter_callback_)
        parameter_callback_(id, value, parameter_userdata_);
}
