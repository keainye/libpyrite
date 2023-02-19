#include <../header/client.h>
client::client(const char *Ip, int port)
{
    WSADATA data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        std::cout << "WSAStartup error";
    }

    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(port);
    addrServer.sin_addr.S_un.S_addr = inet_addr(Ip);

    SOCKET socketClient = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketClient == INVALID_SOCKET)
    {
        std::cout << "socket create error";
    }
    
    this->connection = socketClient;
    this->server = addrServer;
    this->sequence = 0;
}

bool client::AddRouter(std::string identifier, std::function<std::string(std::string)> controller)
{
    if (identifier.find("prt-") != 0)
    {
        return false;
    }

    this->router[identifier] = controller;
    return true;
}

int client::getSequence()
{
    this->sequence += 1;
    return this->sequence - 1;
}

client::~client()
{
    closesocket(this->connection);
    WSACleanup();
}

std::string Promise(std::string identifer, std::string body)
{
    return "";
}

int client::Tell(std::string identifer, std::string body)
{
    PrtPackage P(this->session, identifer, -1, body);
    std::string strP = P.ToBytes();
    const char *sendingMessage = (char *)strP.data();
    if (sizeof(sendingMessage) > MAX_TRANSMIT_SIZE)
    {
        std::cout << "content overflowed" << std::endl;
        return -1;
    }

    send(this->connection, sendingMessage, sizeof(sendingMessage), 0);
}

void client::processAck(PrtPackage p)
{
}

void client::process(std::string raw)
{
    PrtPackage req(raw);
    if (req.Identifier == "invalid")
    {
        return;
    }

    this->session = req.Session;

    if (req.Identifier == "prt-ack")
    {
        this->processAck(req);
    }

    if (this->router.find(req.Identifier) == this->router.end())
    {
        return;
    }

    std::function<std::string(std::string)> f = this->router[req.Identifier];
    std::string resp = f(req.Body);
    const char *sendingMessage = (char *)resp.data();
    send(this->connection, sendingMessage, sizeof(sendingMessage), 0);
}

int client::Start()
{
    if (connect(this->connection, (struct sockaddr *)&this->server,
                sizeof(this->server)) == INVALID_SOCKET)
    {
        std::cout << "connect error";
        system("pause");
        return 1;
    }

    while (1)
    {
        char recvBuff[MAX_TRANSMIT_SIZE];
        memset(recvBuff, 0, sizeof(recvBuff));

        if (recv(this->connection, recvBuff, sizeof(recvBuff), 0) <= 0)
        {
            std::cout << "recv error";
            break;
        }
        std::string raw(recvBuff);
        std::thread p(client::process, this, raw);
        p.detach();
    }
}