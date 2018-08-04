//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "main_controller.h"
#include "message_queue.h"
#include "messages.h"
#include <ensemble_chorus.h>
#include <cstdio>
#include <cassert>

Main_Controller::Main_Controller(Message_Queue &mq_in, Message_Queue &mq_out)
    : mq_in_(mq_in), mq_out_(mq_out),
      msg_buffer_(Messages::allocate_buffer())
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
}

void Main_Controller::process_message(const Basic_Message &msg)
{
    switch (msg.tag) {
    case Message_Tag::NotifyParameter: {
        const auto &body = reinterpret_cast<const Messages::NotifyParameter &>(msg);
        load_parameter(body.id, body.value);
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
