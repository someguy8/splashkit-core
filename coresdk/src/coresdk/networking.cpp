#include <sstream>
#include <cmath>
#include <iomanip>

#include "easylogging++.h"

#include "networking.h"
#include "network_driver.h"
#include "utility_functions.h"

namespace splashkit_lib
{
    static map<string, connection> connections;
    static map<string, server_socket> server_sockets;
    static vector<message> messages;
    static int UDP_PACKET_SIZE = 1024;

    server_socket create_server(const string &name, unsigned short int port, connection_type protocol)
    {
        sk_network_connection *con = nullptr;
        if (protocol == TCP)
        {
            sk_open_tcp_connection(nullptr, port);
        }
        else if (protocol == UDP)
        {
            sk_open_udp_connection(port);
        }
        else
        {
            LOG(ERROR) << "Unknown protocol " << protocol << " passed to create_server";
        }

        server_socket socket = new sk_server_data;
        if (con->_socket && (con->kind == TCP || con->kind == UDP))
        {
            socket->id = SERVER_SOCKET_PTR;
            socket->name = name;
            socket->socket = con;
            socket->port = port;
            socket->new_connections = 0;
            socket->protocol = protocol;

            server_sockets.insert({name, socket});
        }

        return socket;
    }

    server_socket create_server(const string &name, unsigned short int port)
    {
        return create_server(name, port, TCP);
    }

    server_socket server_named(const string &name)
    {
        if (server_sockets.find(name) != server_sockets.end())
        {
            return server_sockets[name];
        }

        LOG(WARNING) << "server_named found no server named '" << name << "'.";

        return nullptr;
    }

    bool close_server(server_socket svr)
    {
        // Ref cannot be null, therefore this always returns true
        //if (svr != null) return false;

        for(auto const connection : svr->connections)
        {
            if (!close_connection(connection)) return false;
        }

        server_sockets.erase(svr->name);

        delete svr;

        return true;
    }

    bool close_server(const string &name)
    {
        return close_server(server_sockets[name]);
    }

    void close_all_servers()
    {
        for(auto const &pair : server_sockets)
        {
            close_server(pair.second);
        }
    }

    bool server_has_new_connection(const string &name)
    {
        return server_has_new_connection(server_sockets[name]);
    }

    bool server_has_new_connection(server_socket server)
    {
        return server->new_connections > 0;
    }

    bool has_new_connections()
    {
        for(auto const &pair : server_sockets)
        {
            if (server_has_new_connection(pair.second))
            {
                return true;
            }
        }
        return false;
    }

    connection _create_connection(const string& name, connection_type protocol)
    {
        connection result = new sk_connection_data;
        result->id = CONNECTION_PTR;
        result->name = name;
        result->ip = 0;
        result->string_ip = "";
        result->port = 0;
        result->protocol = protocol;
        result->part_msg_data = "";
        result->open = true;
        result->socket->_socket = nullptr;
        result->socket->kind = UNKNOWN;
        return result;
    }

    bool _establish_connection(connection con, const string& host, unsigned short int port, connection_type protocol)
    {
        con->string_ip = host;
        con->port = port;
        con->protocol = protocol;

        if (protocol == TCP)
        {
            con->socket = sk_open_tcp_connection(host.c_str(), port);

            if (!con->socket->_socket)
            {
                return false;
            }

            con->ip = sk_network_address(con->socket);
        }
        else if (protocol == UDP)
        {
            con->socket = sk_open_udp_connection(port);
            con->ip = sk_network_address(con->socket);
        }
        else
        {
            LOG(ERROR) << "Invalid protocol passed to _establish_connection: " << protocol;
            return false;
        }

        return true;
    }

    connection open_connection(const string &host, unsigned short int port)
    {
        return open_connection(name_for_connection(host, port), host, port, TCP);
    }

    connection open_connection(const string &name, const string &host, unsigned short int port)
    {
        return open_connection(name, host, port, TCP);
    }

    connection open_connection(const string &name, const string &host, unsigned short int port, connection_type protocol)
    {
        connection con = _create_connection(name, protocol);

        if (_establish_connection(con, host, port, protocol))
        {
            connections.insert({name_for_connection(host, port), con});
        }
        else
        {
            delete con;
        }

        return con;
    }

    connection retrieve_connection(const string &name, int idx)
    {
        return retrieve_connection(server_sockets[name], idx);
    }

    connection retrieve_connection(server_socket server, int idx)
    {
        return server->connections.size() > idx ? server->connections[idx] : nullptr;
    }

    void close_all_connections()
    {
        for(auto const &pair : connections)
        {
            close_connection(pair.second);
        }
    }

    bool close_connection(connection a_connection) {
        //clear_messages(a_connection);
        //shut_connection(a_connection);
        // remove connection from connections map
        // remove connection from servers map
        return false;
    }

    bool close_connection(const string &name)
    {
        return close_connection(connections[name]);
    }

    int connection_count(const string &name)
    {
        return connection_count(server_sockets[name]);
    }

    int connection_count(server_socket server) {
        return server->connections.size();
    }

    unsigned int connection_ip(const string &name)
    {
        return connection_ip(connections[name]);
    }

    unsigned int connection_ip(connection a_connection) {
        return a_connection->ip;
    }

    connection connection_named(const string &name) {
        return connection();
    }

    bool is_connection_open(connection con) {
        return con->open;
    }

    bool is_connection_open(const string &name) {
        return is_connection_open(connections[name]);
    }

    bool accept_new_connection(server_socket server)
    {
        if (INVALID_PTR(server, SERVER_SOCKET_PTR))
        {
            LOG(WARNING) << "Invalid server_socket passed to accept_new_connection";
            return false;
        }
        
        server->new_connections = 0;

        network_connection con = sk_accept_connection(server->socket);

        if (con->_socket && (con->kind == TCP))
        {
            int ip = sk_network_address(con);
            int port = sk_get_network_port(con);

            connection client = _create_connection(server->name + "->" + name_for_connection(ipv4_to_str(ip), port), TCP);
            client->ip = ip;
            client->port = port;
            client->socket = con;

            server->connections.push_back(client);
            server->new_connections = 1;

            return true;
        }

        return false;
    }

    unsigned short int connection_port(connection a_connection) {
        return a_connection->port;
    }

    unsigned short int connection_port(const string &name)
    {
        return connection_port(connections[name]);
    }

    connection last_connection(server_socket server) {
        if (server->connections.empty())
        {
            return nullptr;
        }
        return server->connections[server->connections.size() - 1];
    }

    connection last_connection(const string &name) {
        return last_connection(server_sockets[name]);
    }

    void reconnect(const string &name)
    {
        reconnect(connections[name]);
    }


    void reconnect(connection a_connection)
    {
        string host = a_connection->string_ip;
        unsigned short port = a_connection->port;

        sk_close_connection(a_connection->socket);
        //a_connection.open = establish_connection(, host, port, a_connection.protocol)
    }

    connection message_connection(message msg)
    {
        return msg->connection;
    }

    void broadcast_message(const string &a_msg)
    {
        for(auto const& tcp_server: server_sockets)
        {
            broadcast_message(a_msg, tcp_server.second);
        }
        for (auto const& udp_connection: connections)
        {
            //broadcast_message(a_msg, udp_connection.second.);
        }
    }

    void broadcast_message(const string &a_msg, const string &name)
    {
        broadcast_message(a_msg, server_sockets[name]);
    }

    void broadcast_message(const string &a_msg, server_socket svr)
    {
        for (auto const& tcp_connection: svr->connections)
        {
            //broadcast_message(a_msg, tcp_connection);
        }
    }

    void check_network_activity()
    {

    }

    void clear_messages(server_socket svr)
    {
        // TODO delete all messages
    }

    void clear_messages(connection a_connection)
    {
        // TODO delete all messages
    }

    void clear_messages(const string &name)
    {
        if (server_sockets.count(name))
        {
            clear_messages(server_sockets[name]);
        }
        else if (connections.count(name))
        {
            clear_messages(connections[name]);
        }
    }

    void free_message(sk_message msg)
    {
        // TODO Find swingame equivalent
    }

    bool has_messages()
    {
        for(auto const& tcp_server: server_sockets)
        {
            if (has_messages(tcp_server.second))
            {
                return true;
            }
        }
        for (auto const& udp_connection: connections)
        {
            /*
            if (has_messages(udp_connection))
            {
                return true;
            }
             */
        }
        return false;
    }

    bool has_messages(connection con)
    {
        return con->messages.empty();
    }

    bool has_messages(server_socket svr)
    {
        return svr->messages.empty();
    }

    bool has_messages(const string &name)
    {
        return connections.count(name) ? connections[name]->messages.empty()
                                       : server_sockets[name]->messages.empty();
    }

    int message_count(connection a_connection)
    {
        return a_connection->messages.size();
    }

    int message_count(const string &name)
    {
        return connections.count(name) ? connections[name]->messages.size()
                                       : server_sockets[name]->messages.size();
    }

    int message_count(server_socket svr)
    {
        return svr->messages.size();
    }

    string message_data(sk_message msg)
    {
        return msg.data;
    }

    string message_host(sk_message msg)
    {
        return msg.host;
    }

    unsigned short int message_port(sk_message msg)
    {
        return msg.port;
    }

    connection_type message_protocol(sk_message msg)
    {
        return msg.protocol;
    }

    message read_message(connection a_connection)
    {
        return new sk_message;
    }

    message read_message(const string &name)
    {
        return new sk_message;
    }

    message read_message(server_socket svr)
    {
        return new sk_message();
    }

    string read_message_data(connection a_connection)
    {
        return std::__cxx11::string();
    }

    string read_message_data(server_socket svr)
    {
        return std::__cxx11::string();
    }

    string read_message_data(const string &name)
    {
        return std::__cxx11::string();
    }

    bool send_message_to(const string &a_msg, connection a_connection)
    {
        return false;
    }

    bool send_message_to(const string &a_msg, const string &name)
    {
        return false;
    }

    string name_for_connection(const string host, const unsigned int port)
    {
        stringstream str;
        str << host << ":" << port;
        return str.str();
    }

    string hex_str_to_ipv4(const string &a_hex)
    {
        stringstream ipv4_string;
        ipv4_string << hex_to_dec_string(a_hex.substr(2,2));
        ipv4_string << "." << hex_to_dec_string(a_hex.substr(4,2));
        ipv4_string << "."<< hex_to_dec_string(a_hex.substr(6,2));
        ipv4_string << "." << hex_to_dec_string(a_hex.substr(8,2));
        return ipv4_string.str();
    }

    string hex_to_dec_string(const string &a_hex)
    {
        int dec = 0;
        for (int i = 0; i < a_hex.length(); i++)
        {
            int c_val = 0;
            if (a_hex[i] - '0' < 10)
            {
                c_val = a_hex[i] - '0';
            }
            else if (a_hex[i] - 'A' < 6)
            {
                c_val = (a_hex[i] - 'A') + 10;
            }
            dec += c_val * pow(16, (a_hex.length() - i - 1));
        }
        return to_string(dec);
    }

    string dec_to_hex(unsigned int a_dec)
    {
        uint32_t dec = (uint32_t) a_dec;
        stringstream hex_string;
        hex_string << "0x" << uppercase << hex << dec;
        return hex_string.str();
    }

    unsigned int ipv4_to_dec(const string &a_ip)
    {
        string::size_type lastpos = 0;
        unsigned int result = 0;
        for(unsigned int i = 0; i < 4; i++)
        {
            string::size_type pos = a_ip.find('.', lastpos);
            string token = pos == -1 ? a_ip.substr(lastpos) : a_ip.substr(lastpos, pos - lastpos);

            result += (token == "" || (lastpos == 0 && i > 0) ? 0 : stoi(token) << (3 - i) * 8);
            lastpos = pos + 1;
        }

        return result;
    }

    string ipv4_to_hex(const string& a_ip)
    {
        string::size_type lastpos = 0;
        stringstream hex_string;
        hex_string << "0x";
        for(unsigned int i = 0; i < 4; i++)
        {
            string::size_type pos = a_ip.find('.', lastpos);
            string token = pos == -1 ? a_ip.substr(lastpos) : a_ip.substr(lastpos, pos - lastpos);

            hex_string << setw(2) << setfill('0') << uppercase << hex
            << (token == "" || (lastpos == 0 && i > 0) ? 0 : stoi(token));

            lastpos = pos + 1;
        }

        return hex_string.str();
    }

    string ipv4_to_str(unsigned int ip)
    {
        uint32_t ipaddr = (uint32_t) ip;
        stringstream ip_string;
        ip_string << ((ipaddr >> 24) & 0xFF) << ".";
        ip_string << ((ipaddr >> 16) & 0xFF) << ".";
        ip_string << ((ipaddr >> 8) & 0xFF) << ".";
        ip_string << (ipaddr & 0xFF);
        return ip_string.str();
    }
    
    string my_ip()
    {
        // TODO implement ip address resolution. Should return ip address of connected network if one exists.
        return "127.0.0.1";
    }
}