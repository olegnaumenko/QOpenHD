#include "udp_connection.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <qdebug.h>

UDPConnection::UDPConnection(const std::string local_ip,const int local_port,MAV_MSG_CB cb)
    :m_local_ip(local_ip),m_local_port(local_port),m_cb(cb)
{

}


bool UDPConnection::start()
{
    m_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (m_socket_fd < 0) {
        qDebug()<<"Cannot create socket"<<strerror(errno);
        return false;
    }

    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, m_local_ip.c_str(), &(addr.sin_addr));
    addr.sin_port = htons(m_local_port);

    // TODO needed ?
    // Without setting reuse, this might block infinite on some platforms
    /*int enable = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        LogWarn()<<"Cannot set socket reuse"; // warn,but continue anyway.
    }*/
    if (bind(m_socket_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        qDebug()<<"Cannot bind port "<<strerror(errno);
        return false;
    }
    m_keep_receiving=true;
    m_receive_thread=std::make_unique<std::thread>(&UDPConnection::loop_receive,this);
    return true;
}

void UDPConnection::stop()
{
    m_keep_receiving=false;
    if(m_receive_thread){
        m_receive_thread->join();
    }
    m_receive_thread=nullptr;
}

void UDPConnection::send_message(const mavlink_message_t &msg)
{
    auto opt_remote=get_current_remote();
    if(opt_remote.has_value()){
        const Remote& remote=opt_remote.value();
        struct sockaddr_in dest_addr {};
        dest_addr.sin_family = AF_INET;

        inet_pton(AF_INET, remote.ip.c_str(), &dest_addr.sin_addr.s_addr);
        dest_addr.sin_port = htons(remote.port);

        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t buffer_len = mavlink_msg_to_send_buffer(buffer, &msg);

        const auto send_len = sendto(
            m_socket_fd,
            reinterpret_cast<char*>(buffer),
            buffer_len,
            0,
            reinterpret_cast<const sockaddr*>(&dest_addr),
            sizeof(dest_addr));

        if (send_len != buffer_len) {
            qDebug()<<"Cannot send data to "<<remote.to_string().c_str();
        }
    }
}

void UDPConnection::process_data(const uint8_t *data, int data_len)
{
    for (int i = 0; i < data_len; i++) {
        mavlink_message_t msg;
        uint8_t res = mavlink_parse_char(0, (uint8_t)data[i], &msg, &m_recv_status);
        if (res) {
            process_mavlink_message(msg);
        }
    }
}

void UDPConnection::process_mavlink_message(mavlink_message_t message)
{
    // TODO
}

void UDPConnection::loop_receive()
{
    // Enough for MTU 1500 bytes.
    uint8_t buffer[2048];

    while (m_keep_receiving) {
        struct sockaddr_in src_addr = {};
        socklen_t src_addr_len = sizeof(src_addr);
        const auto recv_len = recvfrom(
            m_socket_fd,
            buffer,
            sizeof(buffer),
            0,
            reinterpret_cast<struct sockaddr*>(&src_addr),
            &src_addr_len);

        if (recv_len == 0) {
            // This can happen when shutdown is called on the socket,
            // therefore we check _should_exit again.
            continue;
        }

        if (recv_len < 0) {
            // This happens on destruction when close(_socket_fd) is called,
            // therefore be quiet.
            // LogErr() << "recvfrom error: " << GET_ERROR(errno);
            continue;
        }
        const std::string remote_ip=inet_ntoa(src_addr.sin_addr);
        const int remote_port=ntohs(src_addr.sin_port);
        set_remote(remote_ip,remote_port);
        process_data(buffer,recv_len);
    }
}

std::optional<UDPConnection::Remote> UDPConnection::get_current_remote()
{
    std::lock_guard<std::mutex> lock(m_remote_nutex);
    if(m_curr_remote.has_value()){
        return m_curr_remote.value();
    }
    return std::nullopt;
}


void UDPConnection::set_remote(const std::string ip, int port)
{
    std::lock_guard<std::mutex> lock(m_remote_nutex);
    if(m_curr_remote.has_value()){
        auto& remote=m_curr_remote.value();
        if(remote.ip!=ip || remote.port != port){
            auto new_remote=Remote{ip,port};
            qDebug()<<"Remote chnged from "<<remote.to_string().c_str()<<" to "<<new_remote.to_string().c_str();
            m_curr_remote=remote;
        }
    }else{
        auto new_remote=Remote{ip,port};
        qDebug()<<"Got remote "<<new_remote.to_string().c_str();
        m_curr_remote=new_remote;
    }
}

