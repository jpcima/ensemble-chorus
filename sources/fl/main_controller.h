//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>
class Message_Queue;
struct Basic_Message;

class Main_Controller {
public:
    Main_Controller(Message_Queue &mq_in, Message_Queue &mq_out);

    void request_parameters();
    void handle_messages();

    void set_parameter_callback(void (*callback)(unsigned, float, void *), void *userdata);
    void set_modulation_callback(void (*callback)(const float[6], const float[6], void *), void *userdata);
    void send_parameter(unsigned id, double value);

private:
    Message_Queue &mq_in_;
    Message_Queue &mq_out_;
    std::unique_ptr<uint8_t[]> msg_buffer_;

    void (*parameter_callback_)(unsigned, float, void *) = nullptr;
    void *parameter_userdata_ = nullptr;

    void (*modulation_callback_)(const float[6], const float[6], void *) = nullptr;
    void *modulation_userdata_ = nullptr;

    void process_message(const Basic_Message &msg);
    void load_parameter(unsigned id, float value);
};
