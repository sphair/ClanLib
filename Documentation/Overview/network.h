
/*! \page Network Network Module Overview

                    <h2>Sockets</h2>

                    <ul>
                        <li>clan::SocketName - Address and port for an IP end point</li>
                        <li>clan::TCPListen - Listens for incoming TCP connections</li>
                        <li>clan::TCPConnection - A socket TCP connection</li>
                        <li>clan::UDPSocket - A UDP socket</li>
                        <li>clan::DNSResolver, clan::DNSPacket - Low-level DNS resolver</li>
                    </ul>

                    <h2>Web</h2>

                    <ul>
                        <li>clan::WebRequest, clan::WebResponse - Sending HTTP requests and receiving responses</li>
                        <li>clan::HTTPServer, clan::HTTPServerConnection - Simple HTTP server</li>
                        <li>clan::HTTPRequestHandler, clan::HTTPRequestHandlerProvider - Interface for handling HTTP server requests</li>
                    </ul>

                    <h2>NetGame</h2>

                    <ul>
                        <li>clan::NetGameClient - Client main class</li>
                        <li>clan::NetGameServer - Server main class</li>
                        <li>clan::NetGameConnection - Represents a connection between a client and the server</li>
                        <li>clan::NetGameEvent, clan::NetGameEventValue - Class for sending or receiving a NetGame network event</li>
                        <li>clan::NetGameEventDispatcher_vX - Helper template classes for calling functions based on an event name</li>
                    </ul>

                    <h2>TLS</h2>

                    <ul>
                        <li>clan::TLSConnection - TLS over a TCPConnection</li>
                    </ul>

*/
