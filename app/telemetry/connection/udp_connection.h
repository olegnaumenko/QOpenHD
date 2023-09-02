#ifndef MUDPLINK_H
#define MUDPLINK_H

#include "../util/mavlink_include.h"

#include <thread>
#include <memory.h>
#include <atomic>
#include <sstream>
#include <optional>
#include <mutex>


/**
 * @brief Mavlink udp link - follows the semi-established pattern
 * of using udp, listening but sending responses to whoever supplied
 * us with data
 */
class UDPConnection
{
public:
    typedef std::function<void(mavlink_message_t msg)> MAV_MSG_CB;
    UDPConnection(const std::string local_ip,const int local_port,MAV_MSG_CB cb);

    bool start();

    void stop();

    void send_message(const mavlink_message_t& msg);
private:
    void process_data(const uint8_t* data,int data_len);
    void process_mavlink_message(mavlink_message_t msg);

    std::unique_ptr<std::thread> m_receive_thread=nullptr;
    std::atomic<bool> m_keep_receiving=false;
    void loop_receive();
    const std::string m_local_ip;
    const int m_local_port;
    int m_socket_fd=-1;
    mavlink_status_t m_recv_status{};
    const MAV_MSG_CB m_cb;
private:
    struct Remote{
        std::string ip;
        int port;
        std::string to_string()const{
            std::stringstream ss;
            ss<<ip<<":"<<port;
            return ss.str();
        }
    };
    std::optional<Remote> get_current_remote();
    void set_remote(const std::string ip,int port);
    std::mutex m_remote_nutex;
    std::optional<Remote> m_curr_remote;
};

#endif // MUDPLINK_H